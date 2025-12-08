#pragma once

namespace vsa::ui {

class Window
{
public:
    virtual ~Window() = default;

    virtual bool is_visible() { return true; }
    virtual void render_menu() {}
    virtual void render() = 0;

    virtual void update() {}
};

} // ui
