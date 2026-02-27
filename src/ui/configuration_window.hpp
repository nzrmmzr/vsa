#pragma once

#include <functional>

#include <simulation/simulation_config.hpp>
#include <simulation/simulation_controller.hpp>
#include <ui/window.hpp>

namespace vsa::ui {

class ConfigurationWindow
  : public Window
  , public sim::SimulationController::CallbackListener
{
public:
    explicit ConfigurationWindow(std::function<void(const sim::SimulationConfig&)> start_simulation);
    ~ConfigurationWindow() override;

    void render_menu() override;
    void render() override;

private:
    void on_simulation(std::size_t id, std::shared_ptr<sim::Simulation> simulation) override
    {
        m_is_simulation_running = false;
        m_is_visible = false;
    }

private:
    std::function<void(const sim::SimulationConfig&)> m_start_simulation;

    bool m_is_visible = true;
    bool m_is_simulation_running = false;

    std::int32_t m_max_duration_value = 1;
    std::int32_t m_max_duration_option_index = 3;
    sim::SimulationConfig m_config;


};

} // ui