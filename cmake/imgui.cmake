include(FetchContent)

find_package(OpenGL)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG        7b6aead9fb88b3623e3b3725ebb42670cbe4c579 # v3.4
)

FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG        9a5d5c45f54b1301ea471622eddede70384243af # v1.92.4
)

FetchContent_MakeAvailable(glfw imgui)

add_library(imgui
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR} ${OPENGL_INCLUDE_DIR})
target_link_libraries(imgui PUBLIC glfw OpenGL::GL)

add_library(imgui_sequencer
        ${CMAKE_SOURCE_DIR}/libs/sequencer/imgui_neo_internal.cpp
        ${CMAKE_SOURCE_DIR}/libs/sequencer/imgui_neo_sequencer.cpp
)

target_include_directories(imgui_sequencer PUBLIC ${CMAKE_SOURCE_DIR}/libs/sequencer/)
target_link_libraries(imgui_sequencer PUBLIC imgui)

target_link_libraries(imgui PUBLIC imgui_sequencer)
