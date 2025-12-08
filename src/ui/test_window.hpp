#pragma once

#include <ui/window.hpp>

namespace vsa::ui {

class TestWindow : public Window
{
public:
    explicit TestWindow();
    ~TestWindow() override;

    void render() override;

private:
    bool m_show_demo_window = true;
    bool m_show_another_window = false;
};

} // ui
