#include "main_window.hpp"

#include <imgui.h>
#include <imgui_neo_sequencer.h>

namespace vsa::ui {

namespace {
const char* time_unit_options[] = { "Day", "Month", "Year", "Age" };
std::size_t convert_time_to_days(const std::int32_t value, const std::int32_t unit_index)
{
    switch (unit_index) {
        case 0: return value;
        case 1: return value * 30;
        case 2: return value * 365;
        case 3: return value * 365 * 10;
        default: return 0;
    }
}

static float population_getter(void* data, int idx) {
    auto* vec = (std::vector<size_t>*)data;
    return (float)(*vec)[idx];
}

}

MainWindow::MainWindow() {}

MainWindow::~MainWindow() {}

void MainWindow::update()
{
    auto now = std::chrono::steady_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::seconds>(now - m_play_prev_sec);
    if (m_is_playing && !m_is_paused && diff.count() >= 1) {
        m_current_day += m_play_multiplier;
        m_play_prev_sec = now;
        if (m_current_day > m_end_day) {
            m_is_playing = false;
            m_current_day = m_end_day;
        }
    }
}

void MainWindow::render_menu()
{
    if (ImGui::BeginMenu("Simulation")) {
        if (ImGui::MenuItem("Load")) {}
        if (!m_simulation) {
            ImGui::BeginDisabled();
            if (ImGui::MenuItem("Save")) {}
            ImGui::EndDisabled();
        } else {
            if (ImGui::MenuItem("Save")) {}
        }

        ImGui::EndMenu();
    }
}

void MainWindow::render()
{
    ImGuiIO& io = ImGui::GetIO();

    // Get the width and height of the OS window
    float windowWidth = io.DisplaySize.x;
    float windowHeight = io.DisplaySize.y;

    ImGui::SetNextWindowPos({ 0, 0 });
    ImGui::SetNextWindowSize({ windowWidth, windowHeight });

    ImGui::Begin(
      "Simulation",
      nullptr,
      ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoBringToFrontOnFocus); // Create a window called "Hello, world!" and append into it.

    bool disabled = false;
    if (!m_simulation) {
        disabled = true;
        ImGui::BeginDisabled();
    }

    ImGui::Text("This is some useful text."); // Display some text (you can use a format strings too)

    ImGui::Spacing();

    if (!disabled) {
        {
            ImGui::Text("1 second equals 1");
            ImGui::SameLine();
            ImGui::Combo("Time unit", &m_play_multiplier_index, time_unit_options, IM_ARRAYSIZE(time_unit_options));
            m_play_multiplier = convert_time_to_days(1, m_play_multiplier_index);
        }
        {
            if (m_is_playing && !m_is_paused) {
                if (ImGui::Button("Pause")) { m_is_paused = true; }
            } else {
                if (ImGui::Button("Play")) {
                    m_is_playing = true;
                    m_play_prev_sec = std::chrono::steady_clock::now();
                    if (!m_is_paused) {
                        m_current_day = m_start_day;
                    }
                    m_is_paused = false;
                }
            }
            if (m_is_playing) {
                ImGui::SameLine();
                if (ImGui::Button("Stop")) {
                    m_is_playing = false;
                    m_current_day = m_start_day;
                }
            }
        }
        //
        auto population = m_simulation->get_data().get_points();

        ImGui::SameLine();
        ImGui::Text("Day: %d", m_current_day);
        ImGui::PlotHistogram("Population", population_getter, (void*)&population, population.size(), 0, NULL, 0.0f, 60.0f, ImVec2(0, 100.0f));

        if (ImGui::BeginNeoSequencer("Sequencer", &m_current_day, &m_start_day, &m_end_day)) {
            // Timeline code here
            ImGui::EndNeoSequencer();
        }
    }

    if (disabled) { ImGui::EndDisabled(); }

    ImGui::End();
}

void MainWindow::on_simulation(std::size_t id, std::shared_ptr<sim::Simulation> simulation)
{
    m_simulation = std::move(simulation);
    m_start_day = 0;

    auto size = m_simulation->get_data().get_points().size();
    m_end_day = size > 1 ? size - 1 : 1;
}

} // ui