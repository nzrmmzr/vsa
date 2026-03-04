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

float get_population(void* data, int idx)
{
    auto& sim = *reinterpret_cast<sim::Simulation*>(data);

    return sim.get_data().get_points()[idx].m_population;
}

float get_avg_age(void* data, int idx)
{
    auto& sim = *reinterpret_cast<sim::Simulation*>(data);

    return sim.get_data().get_points()[idx].m_avg_age_years;
}

float get_males_females(void* data, int idx)
{
    auto& sim = *reinterpret_cast<sim::Simulation*>(data);

    return 1.0 * sim.get_data().get_points()[idx].m_males / (sim.get_data().get_points()[idx].m_males + sim.get_data().get_points()[idx].m_females);
}

float get_born_deceased(void* data, int idx)
{
    auto& sim = *reinterpret_cast<sim::Simulation*>(data);
    float total = (sim.get_data().get_points()[idx].m_number_of_born + sim.get_data().get_points()[idx].m_number_of_deceased);
    return total > 0 ? (1.0f * sim.get_data().get_points()[idx].m_number_of_born / total) : 0.0f;
}

float get_males(void* data, int idx)
{
    auto& sim = *reinterpret_cast<sim::Simulation*>(data);

    return sim.get_data().get_points()[idx].m_males;
}

float get_females(void* data, int idx)
{
    auto& sim = *reinterpret_cast<sim::Simulation*>(data);

    return sim.get_data().get_points()[idx].m_females;
}

float get_couples(void* data, int idx)
{
    auto& sim = *reinterpret_cast<sim::Simulation*>(data);

    return sim.get_data().get_points()[idx].m_couples_count;
}

float get_avg_children_count(void* data, int idx)
{
    auto& sim = *reinterpret_cast<sim::Simulation*>(data);

    return sim.get_data().get_points()[idx].m_avg_children_count;
}

float get_avg_children_count_unique(void* data, int idx)
{
    auto& sim = *reinterpret_cast<sim::Simulation*>(data);

    return sim.get_data().get_points()[idx].m_avg_children_count_unique;
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
        ImGui::SameLine();
        ImGui::Text("Day: %d", m_current_day);

        if (ImGui::TreeNode("Current day data")) {
            const sim::SimulationDataPoint& current_p = m_simulation->get_data().get_points()[m_current_day];
            ImGui::Text("Day %d. (Year: %d)", m_current_day, m_current_day / 365);
            ImGui::Text("Population: %d", current_p.m_population);
            ImGui::Text("Average age: %d years", current_p.m_avg_age_years);
            ImGui::Text("Males/Females: %d/%d", current_p.m_males, current_p.m_females);
            ImGui::ProgressBar(1.0 * current_p.m_males / (current_p.m_males + current_p.m_females));
            ImGui::Text("Couples count: %d", current_p.m_couples_count);
            ImGui::Text("Average children count: %.2f", current_p.m_avg_children_count);
            ImGui::Text("Average children count unique: %.2f", current_p.m_avg_children_count_unique);
            ImGui::Text("Born / Deceased: %d / %d (Population: %+d)",
                current_p.m_number_of_born,
                current_p.m_number_of_deceased,
                static_cast<int>(current_p.m_number_of_born) - static_cast<int>(current_p.m_number_of_deceased));

            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Global data")) {
            const auto& global = m_simulation->get_data().get_global();
            ImGui::Text("Average child creating age (years): %d", global.m_avg_first_child_age);
            ImGui::Text("Number of residents dead before 18: %d", global.m_underage_deceased_count);
            ImGui::Text("Number of residents dead after 100: %d", global.m_elders_deceased_count);
            ImGui::Text("The eldest resident of all times was: %d years old.", global.m_the_eldest_resident_age);
            ImGui::TreePop();
        }

        if (ImGui::TreeNode("Global data by time")) {
            ImGui::PushItemWidth(windowWidth - 140);
            ImGui::PlotHistogram("Population", &get_population, m_simulation.get(), m_simulation->get_data().get_points().size(), 0, nullptr, FLT_MAX, FLT_MAX, ImVec2(0.0 , 160));
            ImGui::PlotHistogram("Average age", &get_avg_age, m_simulation.get(), m_simulation->get_data().get_points().size(), 0, nullptr, FLT_MAX, FLT_MAX, ImVec2(0.0 , 160));
            ImGui::PlotHistogram("Males", &get_males, m_simulation.get(), m_simulation->get_data().get_points().size(), 0, nullptr, FLT_MAX, FLT_MAX, ImVec2(0.0 , 160));
            ImGui::PlotHistogram("Females", &get_females, m_simulation.get(), m_simulation->get_data().get_points().size(), 0, nullptr, FLT_MAX, FLT_MAX, ImVec2(0.0 , 160));
            ImGui::PlotLines("Males/Females", &get_males_females, m_simulation.get(), m_simulation->get_data().get_points().size(), 0, nullptr, FLT_MAX, FLT_MAX, ImVec2(0.0 , 160));
            ImGui::PlotHistogram("Couples count", &get_couples, m_simulation.get(), m_simulation->get_data().get_points().size(), 0, nullptr, FLT_MAX, FLT_MAX, ImVec2(0.0 , 160));
            ImGui::PlotHistogram("Average children count", &get_avg_children_count, m_simulation.get(), m_simulation->get_data().get_points().size(), 0, nullptr, FLT_MAX, FLT_MAX, ImVec2(0.0 , 160));
            ImGui::PlotHistogram("Average children count (unique)", &get_avg_children_count_unique, m_simulation.get(), m_simulation->get_data().get_points().size(), 0, nullptr, FLT_MAX, FLT_MAX, ImVec2(0.0 , 160));
            ImGui::PlotLines("Born/ Deceased", &get_born_deceased, m_simulation.get(), m_simulation->get_data().get_points().size(), 0, nullptr, FLT_MAX, FLT_MAX, ImVec2(0.0 , 160));
            ImGui::PopItemWidth();
            ImGui::TreePop();
        }

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