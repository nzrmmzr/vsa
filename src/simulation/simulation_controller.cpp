#include "simulation_controller.hpp"

#include "village/entities_registry.hpp"

#include <mutex>
#include <random>
#include <utility>

#include <tools/logger.hpp>
#include <tools/random_engine.hpp>

#include <village/village.hpp>
#include <village/village_config.hpp>
#include <village/residents/resident_factory.hpp>


namespace vsa::sim {

SimulationController::SimulationController() {}

SimulationController::~SimulationController() {}

bool SimulationController::init()
{
    VSA_LOG_INFO("sim_ctrl", "Simulation Controller initialized.");
    return true;
}

bool SimulationController::deinit()
{
    VSA_LOG_INFO("sim_ctrl", "Simulation Controller deinitialized.");
    return true;
}

bool SimulationController::run()
{
    stop();
    m_should_stop.store(false);

    m_is_working.store(false);
    m_is_error.store(false);

    m_working_thread = std::thread(&SimulationController::working_thread, this);

    std::unique_lock lock(m_is_working_mtx);
    m_is_working_cv.wait(
        lock,
        [is_working = &m_is_working, is_error = &m_is_error]() { return is_working->load() || is_error->load(); });

    VSA_LOG_INFO("sim_ctrl", "Simulation Controller main loop inited.");
    return true;
}

void SimulationController::register_callback_listener(std::shared_ptr<CallbackListener> listener)
{
    std::lock_guard lg(m_listener_mtx);
    m_listeners.push_back(std::move(listener));
}

std::size_t SimulationController::simulate(const SimulationConfig& config)
{
    VSA_LOG_INFO("sim_ctrl", "New simulation initiated.");

    {
        std::lock_guard lg(m_simulations_queue_mtx);
        m_simulations_queue.push(config);
    }

    return 0;
}

void SimulationController::working_thread()
{
    {
        std::unique_lock lock(m_is_working_mtx);
        m_is_working.store(true);
        m_is_working_cv.notify_all();
    }

    VSA_LOG_INFO("sim_ctrl", "Simulation Controller main loop started.");

    while (!m_should_stop.load()) {
        std::unique_lock lock(m_simulations_queue_mtx);

        if (m_simulations_queue.empty()) { continue; }

        VSA_LOG_INFO("sim_ctrl", "New simulation request detected. Processing started.");

        SimulationConfig config = m_simulations_queue.front();
        m_simulations_queue.pop();

        lock.unlock();

        // Start simulation processing

        tools::RandomEngine::get_instance().set_seed(config.m_seed);
        village::VillageConfig::set_config(config);

        const auto& residents_ids = village::EntitiesRegistry::get_instance().get_residents_ids();

        std::vector<std::shared_ptr<village::Resident>> initial_residents(config.population.initial_population);
        for (auto& r : initial_residents) {
            r = village::ResidentFactory::create_resident(
                residents_ids[tools::RandomEngine::get_instance().get_random_uint(0, residents_ids.size() - 1)],
                tools::RandomEngine::get_instance().get_random_uint(0, 1),
                tools::RandomEngine::get_instance().get_random_uint(config.population.initial_min_age * 365,
                                                                    config.population.initial_max_age * 365));
        }

        village::Village v(initial_residents);

        std::vector<SimulationDataPoint> points;
        points.reserve(config.max_duration_days);

        std::size_t avg_time_sum = 0;
        std::size_t avg_time_count = 0;
        for (std::size_t day = 0; v.get_size() > 0 && day <= config.max_duration_days; ++day) {
            std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
            points.push_back(v.iterate());

            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

            avg_time_sum += std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            avg_time_count++;

            if (day % (config.max_duration_days / 10) == 0) {
                VSA_LOG_INFO("sim_ctrl", "Simulation {}% done. Count: {}. Avg iteration time: {} ms.", (day / (1.0 * config.max_duration_days)) * 100, avg_time_count, avg_time_sum / avg_time_count);
                avg_time_count = 0;
                avg_time_sum = 0;
            }
        }

        SimulationData data(std::move(points));

        auto simulation = std::make_shared<Simulation>(config, data);

        {
            std::lock_guard lg(m_listener_mtx);
            for (auto l : m_listeners) { l->on_simulation(0, simulation); }
        }

        VSA_LOG_INFO("sim_ctrl", "New simulation processing finished.");
    }

    VSA_LOG_INFO("sim_ctrl", "Simulation Controller main loop finished.");
}

}