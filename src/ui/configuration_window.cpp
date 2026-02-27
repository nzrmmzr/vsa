#include "configuration_window.hpp"

#include "village/entities_registry.hpp"
#include "village/village.hpp"

#include <imgui.h>

#include <utility>

namespace vsa::ui {

namespace {
const char* time_unit_options[] = { "Days", "Months", "Years", "Ages" };
std::size_t convert_time_to_days(const std::int32_t value, const std::int32_t unit_index)
{
    switch (unit_index) {
        case 0: return value;
        case 1: return value * 30;
        case 2: return value * 365;
        case 3: return value * 365 * 100;
        default: return 0;
    }
}
}

ConfigurationWindow::ConfigurationWindow(std::function<void(const sim::SimulationConfig&)> start_simulation)
  : m_start_simulation(std::move(start_simulation))
{
    const auto& eri = village::EntitiesRegistry::get_instance();
    for (const auto& r : eri.get_residents()) {
        m_config.residents[r.first] = sim::SimulationConfig::Resident();
    }
}

ConfigurationWindow::~ConfigurationWindow() {}

void ConfigurationWindow::render_menu()
{
    if (ImGui::BeginMenu("Configuration")) {
        if (ImGui::MenuItem("Open Configuration Window", "", m_is_visible)) { m_is_visible = !m_is_visible; }
        ImGui::EndMenu();
    }
}

void ConfigurationWindow::render()
{
    if (!m_is_visible) { return; }
    ImGui::Begin("Configuration",
                 &m_is_visible,
                 ImGuiWindowFlags_NoCollapse      //|
                 /* ImGuiWindowFlags_NoResize*/); // Create a window called "Hello, world!" and append into it.

    ImGui::SeparatorText("Duration");

    ImGui::DragInt("Max duration", &m_max_duration_value, 1, 100000);

    ImGui::Combo("Time unit", &m_max_duration_option_index, time_unit_options, IM_ARRAYSIZE(time_unit_options));
    m_config.max_duration_days = convert_time_to_days(m_max_duration_value, m_max_duration_option_index);

    ImGui::SeparatorText("Population");
    ImGui::DragInt("Initial min age", &m_config.population.initial_min_age, 1, 0, 90);
    ImGui::DragInt("Initial max age", &m_config.population.initial_max_age, 1, 0, 90);
    if (ImGui::DragFloat("Percentage of women", &m_config.population.percentage_of_women, 0, 0, 100)) {
        m_config.population.percentage_of_men = 100 - m_config.population.percentage_of_women;
    }
    if (ImGui::DragFloat("Percentage of men", &m_config.population.percentage_of_men, 0, 0, 100)) {
        m_config.population.percentage_of_women = 100 - m_config.population.percentage_of_men;
    }
    ImGui::DragFloat("Death probability day increment", &m_config.population.death_probability_day_increment, 0, 0, 1);
    ImGui::DragFloat("The probability of creating a couple (per day)",
                     &m_config.population.couple_creation_probability_per_day,
                     0,
                     0,
                     1);
    ImGui::DragFloat("The probability of breaking a couple (per day)",
                     &m_config.population.couple_breaking_probability_per_day,
                     0,
                     0,
                     1);

    ImGui::DragFloat("The probability of creating a child by couple (per day)",
                     &m_config.population.child_creation_probability_per_day,
                     0,
                     0,
                     1);

    ImGui::SeparatorText("Residents");

    const auto& ress = village::EntitiesRegistry::get_instance().get_residents();
    for (auto& r : m_config.residents) {
        const auto& entity = ress.at(r.first);
        ImGui::Text("%s", entity.name.c_str());
        ImGui::DragFloat(("Initial percentage##" + entity.name).c_str(), &r.second.initial_percentage, 0, 0, 1);
        ImGui::DragFloat(("Become probability##" + entity.name).c_str(), &r.second.become_probability, 0, 0, 1);
    }

    ImGui::SeparatorText("Run");

    if (m_is_simulation_running) {
        ImGui::BeginDisabled();
        if (ImGui::Button("Start")) {}
        ImGui::EndDisabled();
    } else {
        if (ImGui::Button("Start")) {
            m_is_simulation_running = true;
            m_start_simulation(m_config);
            ImGui::OpenPopup("Simulation processing");
        }
    }

    if (ImGui::BeginPopupModal("Simulation processing", &m_is_simulation_running)) {
        ImGui::Text("Please wait, simulation is running...");
        ImGui::EndPopup();
    }

    ImGui::End();
}

} // ui