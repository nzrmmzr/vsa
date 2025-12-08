#pragma once

#include <ui/renderer.hpp>
#include <simulation/simulation_controller.hpp>

namespace vsa {

class VillageApp
{
public:
    explicit VillageApp();
    ~VillageApp();

    bool init();
    bool run();
    bool stop();
    bool deinit();
private:
    std::shared_ptr<ui::Renderer> m_renderer;
    std::shared_ptr<sim::SimulationController> m_controller;
};

}
