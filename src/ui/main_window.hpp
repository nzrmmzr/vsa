#pragma once

#include "imgui_internal.h"

#include <memory>

#include <simulation/simulation_controller.hpp>
#include <ui/window.hpp>

namespace vsa::ui {

class MainWindow
  : public Window
  , public sim::SimulationController::CallbackListener
{
public:
    explicit MainWindow();
    ~MainWindow() override;

    void update() override;

    void render_menu() override;
    void render() override;

private:
    void on_simulation(std::size_t id, std::shared_ptr<sim::Simulation> simulation) override;

private:
    std::shared_ptr<sim::Simulation> m_simulation;

    // TIMELINE
    std::int32_t m_current_day = 0;
    std::int32_t m_start_day = 0;
    std::int32_t m_end_day = 1;

    // PLAY
    bool m_is_playing = false;
    bool m_is_paused = false;
    std::int32_t m_play_multiplier_index = 0;
    std::size_t m_play_multiplier = 1;
    std::chrono::steady_clock::time_point m_play_prev_sec;
};

} // ui
