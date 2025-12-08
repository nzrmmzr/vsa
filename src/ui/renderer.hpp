#pragma once

#include <memory>
#include <string>
#include <thread>
#include <vector>

#include <GLFW/glfw3.h>

#include <glm/vec2.hpp>

#include <ui/window.hpp>

namespace vsa::ui {

class Renderer
{
public:
    explicit Renderer(const std::string& window_name, std::size_t window_width = 1280, std::size_t window_heigh = 800);
    ~Renderer();

    bool init(bool is_dark = true);
    bool deinit();

    //bool run();
    void run_this_thread();

    void stop()
    {
        m_should_stop.store(true);
        if (m_working_thread.joinable()) { m_working_thread.join(); }
    }

    bool register_window(std::shared_ptr<Window> w);

    // TODO: Temp
    bool is_working() const { return m_is_working.load(); }

private:
    void working_thread();

private:
    std::string m_window_name;
    std::size_t m_window_width;
    std::size_t m_window_heigh;
    glm::i32vec2 m_window_real_size = { 0, 0 };

    GLFWwindow* m_window = nullptr;

    std::thread m_working_thread;
    std::atomic<bool> m_should_stop = false;
    std::condition_variable m_is_working_cv;
    std::mutex m_is_working_mtx;
    std::atomic<bool> m_is_working = false;
    std::atomic<bool> m_is_error = false;

    std::vector<std::shared_ptr<Window>> m_registered_windows;
};

}
