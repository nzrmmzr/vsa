#include "village_app.hpp"

#include <tools/logger.hpp>
#include <ui/configuration_window.hpp>
#include <ui/main_window.hpp>
#include <ui/renderer.hpp>
#include <ui/test_window.hpp>

namespace vsa {

VillageApp::VillageApp()
  : m_renderer(std::make_shared<ui::Renderer>("Village simulator"))
  , m_controller(std::make_shared<sim::SimulationController>())
{
}

VillageApp::~VillageApp() = default;

bool VillageApp::init()
{
    VSA_LOG_INFO("app", "Initialization started.");
    m_controller->init();
    m_renderer->init();

    auto main_window = std::make_shared<ui::MainWindow>();
    m_controller->register_callback_listener(main_window);

    auto start_simulation = [this](const sim::SimulationConfig& conf) { m_controller->simulate(conf); };
    const auto conf_window = std::make_shared<ui::ConfigurationWindow>(start_simulation);
    m_controller->register_callback_listener(conf_window);

    m_renderer->register_window(main_window);
    m_renderer->register_window(conf_window);
    //m_renderer->register_window(std::make_shared<ui::TestWindow>());
    VSA_LOG_INFO("app", "Initialization finished.");
    return true;
}

bool VillageApp::run()
{
    VSA_LOG_INFO("app", "Application started.");
    m_controller->run();
    m_renderer->run_this_thread();
    m_controller->stop();
    VSA_LOG_INFO("app", "Application stopped.");
    return true;
}

bool VillageApp::stop()
{
    VSA_LOG_INFO("app", "Application stop requested.");
    m_renderer->stop();
    m_controller->stop();
    VSA_LOG_INFO("app", "Application stop processed.");
    return true;
}

bool VillageApp::deinit()
{
    VSA_LOG_INFO("app", "Deinitialization started.");
    m_renderer->deinit();
    m_controller->deinit();
    VSA_LOG_INFO("app", "Deinitialization finished.");
    return true;
}

}