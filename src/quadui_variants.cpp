#include "quadui.hpp"
#include "quadui_variants.hpp"

namespace QuadUI {

  SingleActionButton::SingleActionButton(uint16_t pos_x, uint16_t pos_y, uint16_t width, uint16_t height, std::function<void()> action, const std::string& text) :
        Control(),
        callback_(action),
        pos_x_(pos_x),
        pos_y_(pos_y),
        width_(width),
        height_(height),
        text_(text)
  {};

  void SingleActionButton::draw(LGFX_Device* gfx) {
    // draw background
    uint32_t bg_color = UI_COLOR_CONTROL_BG;
    if (is_focused_) {
      bg_color = UI_COLOR_CONTROL_BG_FOCUS;
      is_render_queued_ = true;
    }
    if (input_.is_pressed()) {
      bg_color = UI_COLOR_CONTROL_BG_ENTER;
      is_render_queued_ = true;
    }

    if (!is_render_queued_) return;

    gfx->fillRect(pos_x_, pos_y_, width_, height_, bg_color);

    // draw text
    gfx->setTextColor(UI_COLOR_CONTROL_FG, is_focused_ ? UI_COLOR_CONTROL_BG_FOCUS : bg_color);
    gfx->setTextDatum(MC_DATUM);
    gfx->drawString(text_.c_str(), pos_x_ + width_ / 2, pos_y_ + height_ / 2);
    
    is_render_queued_ = false;
  }

  void SingleActionButton::update(const Input& input) {
    input_ = input;
    if (input.is_released_trigger()) {
      callback_();
    }
  }


}
  

