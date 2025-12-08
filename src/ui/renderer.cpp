#include "renderer.hpp"

#include <GLFW/glfw3.h>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <tools/logger.hpp>

namespace vsa::ui {

namespace {

void glfw_error_callback(int error, const char* description)
{
    VSA_LOG_ERROR("renderer", "GLFW Error {}: {}", error, description);
}

const char* set_window_hints()
{
    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100 (WebGL 1.0)
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(IMGUI_IMPL_OPENGL_ES3)
    // GL ES 3.0 + GLSL 300 es (WebGL 2.0)
    const char* glsl_version = "#version 300 es";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif
    return glsl_version;
}

}

Renderer::Renderer(const std::string& window_name, std::size_t window_width, std::size_t window_heigh)
  : m_window_name(window_name)
  , m_window_width(window_width)
  , m_window_heigh(window_heigh)
{
}

Renderer::~Renderer()
{
    stop();
}

bool Renderer::init(bool is_dark)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        VSA_LOG_ERROR("renderer", "GLFW initialization failed.");
        return false;
    }

    const char* glsl_version = set_window_hints();

    // Valid on GLFW 3.3+ only
    float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());

    m_window_real_size = { m_window_width * main_scale, m_window_heigh * main_scale };

    m_window = glfwCreateWindow(m_window_real_size.x, m_window_real_size.y, m_window_name.c_str(), nullptr, nullptr);

    if (m_window == nullptr) {
        VSA_LOG_ERROR("renderer", "Window creation failed.");
        return false;
    }

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    // Setup Dear ImGui style
    if (is_dark) {
        ImGui::StyleColorsDark();
    } else {
        ImGui::StyleColorsLight();
    }

    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);
    style.FontScaleDpi = main_scale;

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
#ifdef __EMSCRIPTEN__
    ImGui_ImplGlfw_InstallEmscriptenCallbacks(window, "#canvas");
#endif
    ImGui_ImplOpenGL3_Init(glsl_version);

    VSA_LOG_INFO("renderer", "Renderer initialized.");
    return true;
}

bool Renderer::deinit()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_window);
    glfwTerminate();

    VSA_LOG_INFO("renderer", "Renderer deinitialized.");
    return true;
}

// bool Renderer::run()
// {
//     stop();
//     m_should_stop.store(false);
//
//     m_is_working.store(false);
//     m_is_error.store(false);
//
//     m_working_thread = std::thread(&Renderer::working_thread, this);
//
//     std::unique_lock lock(m_is_working_mtx);
//     m_is_working_cv.wait(
//       lock, [is_working = &m_is_working, is_error = &m_is_error]() { return is_working->load() || is_error->load();
//       });
//
//     VSA_LOG_INFO("renderer", "Rendering main loop inited.");
//     return true;
// }

void Renderer::run_this_thread() { working_thread(); }

bool Renderer::register_window(std::shared_ptr<Window> w)
{
    m_registered_windows.push_back(w);
    return true;
}

void Renderer::working_thread()
{
    {
        std::unique_lock lock(m_is_working_mtx);
        m_is_working.store(true);
        m_is_working_cv.notify_all();
    }

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    VSA_LOG_INFO("renderer", "Rendering main loop started.");

    while (!m_should_stop.load() && !glfwWindowShouldClose(m_window)) {
        glfwPollEvents();
        if (glfwGetWindowAttrib(m_window, GLFW_ICONIFIED) != 0) {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        for (auto& w : m_registered_windows) {
            if (w->is_visible()) { w->update(); }
        }

        if (ImGui::BeginMainMenuBar()) {
            for (const auto& w : m_registered_windows) {
                w->render_menu();
            }
            ImGui::EndMainMenuBar();
        }

        // RENDER HERE
        for (auto& w : m_registered_windows) {
            if (w->is_visible()) { w->render(); }
        }

        // Rendering
        ImGui::Render();

        int display_w, display_h;
        glfwGetFramebufferSize(m_window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(
          clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(m_window);
    }

    {
        std::unique_lock lock(m_is_working_mtx);
        m_is_working.store(false);
        m_is_working_cv.notify_all();
    }

    VSA_LOG_INFO("renderer", "Rendering main loop finished.");
}

} // vsa