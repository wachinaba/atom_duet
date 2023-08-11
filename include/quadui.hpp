#include <LovyanGFX.hpp>

#include <stdint.h>
#include <vector>
#include <functional>
#include <memory>

// physical io
namespace QuadUI {

  class PhysicalButton;
  class Input;

  class TileManager;
  class Tile;
  class Control;

  class Input {
    public:
      const uint8_t STATE_RELEASED = 0b00;
      const uint8_t STATE_PRESSED_TRIGER = 0b01;
      const uint8_t STATE_PRESSED = 0b11;
      const uint8_t STATE_RELEASED_TRIGER = 0b10;
      const uint8_t MASK_PRESS = 0b01;
      const uint8_t MASK_TRIGGER = 0b10;

    public:
      Input();
      explicit Input(uint32_t press_duration);
      Input(uint32_t press_duration, uint8_t state);

      bool is_pressed() {
        return state_ & MASK_PRESS;
      }
      bool is_released() {
        return !(state_ & MASK_PRESS);
      }
      bool is_pressed_trigger() {
        return state_ == STATE_PRESSED_TRIGER;
      }
      bool is_released_trigger() {
        return state_ == STATE_RELEASED_TRIGER;
      }
      bool is_long_pressed(uint32_t duration) {
        return press_duration_ > duration && is_pressed();
      }
      bool is_long_released_trigger(uint32_t duration) {
        return press_duration_ > duration && is_released_trigger();
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
      virtual void draw(LovyanGFX& gfx) = 0;

    protected:
      bool is_focused_;
  };

  class Tile {
    public:
      Tile();
      explicit Tile(const std::vector<std::shared_ptr<Control>>& controls);
      virtual void update(const Input& input);
      virtual void draw(LovyanGFX& gfx);

    protected:
      std::vector<std::shared_ptr<Control>> controls_;
      uint8_t focus_index_;
      
  };

  class TileManager {
    public:
      TileManager(std::shared_ptr<Tile> tile, PhysicalButton button);
      void update();
      void draw(LovyanGFX& gfx);

    protected:
      std::shared_ptr<Tile> tile_;
      PhysicalButton button_;
  };
}