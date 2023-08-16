#include <LovyanGFX.hpp>

#include <stdint.h>
#include <vector>
#include <functional>
#include <memory>
#include <string>

#ifndef QUADUI_HPP
#define QUADUI_HPP

#include <LGFX_AUTODETECT.hpp>

// physical io
namespace QuadUI {

  class PhysicalButton;
  class Input;

  class TileManager;
  class Tile;
  class Control;

  class Input {
    public:
      static const uint8_t STATE_RELEASED = 0b00;
      static const uint8_t STATE_PRESSED_TRIGER = 0b01;
      static const uint8_t STATE_PRESSED = 0b11;
      static const uint8_t STATE_RELEASED_TRIGER = 0b10;
      static const uint8_t MASK_PRESS = 0b01;
      static const uint8_t MASK_TRIGGER = 0b10;

    public:
      Input();
      explicit Input(uint32_t press_duration);
      Input(uint32_t press_duration, uint8_t state);

      bool is_pressed() const {
        return state_ & MASK_PRESS;
      }
      bool is_released() const {
        return !(state_ & MASK_PRESS);
      }
      bool is_pressed_trigger() const {
        return state_ == STATE_PRESSED_TRIGER;
      }
      bool is_released_trigger() const {
        return state_ == STATE_RELEASED_TRIGER;
      }
      bool is_long_pressed(uint32_t duration) const {
        return press_duration_ > duration && is_pressed();
      }
      bool is_long_released_trigger(uint32_t duration) const {
        return press_duration_ > duration && is_released_trigger();
      }
      bool is_short_pressed(uint32_t duration) const {
        return press_duration_ <= duration && is_pressed();
      }
      bool is_short_released_trigger(uint32_t duration) const {
        return press_duration_ <= duration && is_released_trigger();
      }

    public:
      uint32_t press_duration_;
      uint8_t state_;
  };

  class PhysicalButton : public Input {
    public:
      explicit PhysicalButton(uint8_t pin, bool pullup);
      void update();
      Input get_input() {
        return Input(press_duration_, state_);
      }

    protected:
      uint8_t pin_;
      bool pullup_;
      uint32_t last_pressed_time_;
  };

  class Control {
    public:
      Control();
      virtual void update(const Input& input) = 0;
      virtual void draw(LGFX_Device* gfx) = 0;
      virtual void set_focus(bool is_focused) {
        is_focused_ = is_focused;
      }
      virtual void queue_render() {
        is_render_queued_ = true;
      }

    protected:
      bool is_focused_;
      bool is_render_queued_;
  };

  class Tile {
    public:
      Tile();
      Tile(const std::vector<std::shared_ptr<Control>>& controls, uint32_t duration_threshold);
      virtual ~Tile();
      virtual void update(const Input& input);
      virtual void draw(LGFX_Device* gfx);
      virtual void queue_render_all() {
        for (auto& control : controls_) {
          control->queue_render();
        }
      }

    protected:
      std::vector<std::shared_ptr<Control>> controls_;
      uint8_t focus_index_;
      uint32_t duration_threshold_;
      
  };

  class TileManager {
    public:
      TileManager(std::shared_ptr<Tile> tile, PhysicalButton button);
      virtual void update();
      virtual void draw(LGFX_Device* gfx);

    protected:
      std::shared_ptr<TileManager> parent_;
      std::shared_ptr<Tile> tile_;
      PhysicalButton button_;
  };
}

#endif // QUADUI_HPP