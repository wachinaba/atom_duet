
#include <stdint.h>
#include <vector>
#include <functional>
#include <memory>

#include "quadui.hpp"

// physical io
namespace QuadUI {

  Input::Input() :
    press_duration_(0),
    state_(STATE_RELEASED)
  {}

  Input::Input(uint32_t press_duration, uint8_t state) :
    press_duration_(press_duration),
    state_(state)
  {}

  PhysicalButton::PhysicalButton(uint8_t pin, bool pullup=false) :
    Input(),
    pin_(pin),
    pullup_(pullup)
  {
    if (pullup) {
      pinMode(pin_, INPUT_PULLUP);
    } else {
      pinMode(pin_, INPUT);
    }
  }

  void PhysicalButton::update() {
    bool pressed = digitalRead(pin_) == LOW;
    pressed = pullup_ ? !pressed : pressed;

    if (pressed) {
      // reset duration when triggered
      if (!(state_ & MASK_PRESS)) {
        state_ = STATE_PRESSED_TRIGER;
        last_pressed_time_ = millis();
      } else {
        state_ = STATE_PRESSED;
      }
      
      press_duration_ = millis() - last_pressed_time_;

    } else {
      if (state_ & MASK_PRESS) {
        state_ = STATE_RELEASED_TRIGER;
      } else {
        state_ = STATE_RELEASED;
      }
    }
    
  }

  Tile::Tile() :
    controls_()
  {}

  Tile::Tile(const std::vector<std::shared_ptr<Control>>& controls) :
    controls_(controls)
  {}

  void Tile::update(const Input& input) {
    
  }

  TileManager::TileManager(std::shared_ptr<Tile> tile, PhysicalButton button) :
    tile_(tile),
    button_(button)
  {}

  void TileManager::update() {
    button_.update();
    Input input = button_.get_input();
    tile_->update(input);
  }


  void TileManager::draw(LovyanGFX& gfx) {
    tile_->draw(gfx);
  }

}