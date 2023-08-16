#include "quadui.hpp"

#ifndef QUADUI_VARIANTS_HPP
#define QUADUI_VARIANTS_HPP

#include <LGFX_AUTODETECT.hpp>


namespace QuadUI {

  static const uint32_t UI_COLOR_BG = LGFX::color888(0, 0, 0);
  static const uint32_t UI_COLOR_FG = LGFX::color888(255, 255, 255);
  static const uint32_t UI_COLOR_CONTROL_BG = LGFX::color888(25, 25, 100);
  static const uint32_t UI_COLOR_CONTROL_FG = LGFX::color888(255, 255, 255);
  static const uint32_t UI_COLOR_CONTROL_BG_FOCUS = LGFX::color888(0, 0, 200);
  static const uint32_t UI_COLOR_CONTROL_BG_ENTER = LGFX::color888(255, 165, 0);
  
  // todo: Add interface "Actionable", which has a callback function
  
  class SingleActionButton : public Control {
    public: 
      SingleActionButton(uint16_t pos_x, uint16_t pos_y, uint16_t width, uint16_t height, std::function<void()> action, const std::string& text);
      virtual void draw(LGFX_Device* gfx) override;
      virtual void update(const Input& input) override;
      virtual void set_focus(bool is_focused) override {
        is_focused_ = is_focused;
        is_render_queued_ = true;
      }
      virtual void set_callback(std::function<void()> action) {
        callback_ = action;
      }

    protected:
      uint16_t pos_x_;
      uint16_t pos_y_;
      uint16_t width_;
      uint16_t height_;
      std::function<void()> callback_;
      std::string text_;
      Input input_;
      uint32_t prev_press_duration_;
  };

}
  
#endif // QUADUI_VARIANTS_HPP