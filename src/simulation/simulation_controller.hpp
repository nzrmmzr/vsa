#pragma once

#include <atomic>
#include <memory>
#include <queue>
#include <thread>

#include <simulation/simulation.hpp>
#include <simulation/simulation_config.hpp>
#include <simulation/simulation_data.hpp>

namespace vsa::sim {

class SimulationController
{
public:
    class CallbackListener
    {
    public:
        virtual ~CallbackListener() = default;

        virtual void on_simulation(std::size_t id, std::shared_ptr<Simulation> simulation) = 0;
    };

    SimulationController();
    ~SimulationController();

    bool init();
    bool deinit();

    bool run();
    bool stop()
    {
        m_should_stop.store(true);
        if (m_working_thread.joinable()) { m_working_thread.join(); }
        return true;
    }

    void register_callback_listener(std::shared_ptr<CallbackListener> listener);

    std::size_t simulate(const SimulationConfig& config);

private:
    void working_thread();

private:
    std::thread m_working_thread;
    std::atomic<bool> m_should_stop = false;
    std::condition_variable m_is_working_cv;
    std::mutex m_is_working_mtx;
    std::atomic<bool> m_is_working = false;
    std::atomic<bool> m_is_error = false;

    std::vector<std::shared_ptr<CallbackListener>> m_listeners;
    std::mutex m_listener_mtx;

    std::queue<SimulationConfig> m_simulations_queue;
    std::mutex m_simulations_queue_mtx;
};

}
