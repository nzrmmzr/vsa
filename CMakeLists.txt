cmake_minimum_required(VERSION 3.30)
project(village)

set(CMAKE_CXX_STANDARD 23)

include(cmake/dependencies.cmake)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/village_app.cpp
        src/ui/renderer.cpp
        src/tools/logger.cpp
        src/ui/window.cpp
        src/ui/test_window.cpp
        src/simulation/simulation_controller.cpp
        src/simulation/simulation.cpp
        src/simulation/simulation_config.cpp
        src/simulation/simulation_data.cpp
        src/ui/main_window.cpp
        src/ui/configuration_window.cpp
        src/village/residents/resident_baker.cpp
        src/village/residents/resident_blacksmith.cpp
        src/village/tools/tool_sickle.cpp
        src/village/tools/tool_triphammer.cpp
        src/village/village.cpp
        src/village/storage.cpp
        src/village/materials/material_metal_ore.cpp
        src/village/materials/material_wheat.cpp
        src/village/residents/resident_miner.cpp
        src/village/dishes/dish_bread.cpp
        src/village/village_builder.cpp
        src/village/village_factory.cpp
        src/village/entities_registry.cpp
        src/village/entity_registrator.cpp
        src/village/residents/resident_herbalist.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE src/)

target_link_libraries(${PROJECT_NAME} PRIVATE imgui glm::glm json::json spdlog::spdlog)


