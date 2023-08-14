
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

  Control::Control() :
    is_focused_(false)
  {}

  Tile::Tile() :
    duration_threshold_(300),
    focus_index_(0),
    controls_()
  {}

  Tile::Tile(const std::vector<std::shared_ptr<Control>>& controls, uint32_t duration_threshold=(uint32_t)300) :
    controls_(controls),
    duration_threshold_(duration_threshold)
  {}

  void Tile::update(const Input& input) {
    Input input_for_focus(input);

    // check tile has at least one control
    if (controls_.size() == 0) {
      // debug
      Serial.println("Tile::update: no control");
      return;
    }

    if(input.is_short_pressed(duration_threshold_)) {
      input_for_focus.state_ = Input::STATE_RELEASED;
    }

    if(input.is_short_released_trigger(duration_threshold_)) {
      controls_[focus_index_]->set_focus(false);
      focus_index_ = (focus_index_ + 1) % controls_.size();
      controls_[focus_index_]->set_focus(true);
      input_for_focus.state_ = Input::STATE_RELEASED;
    }

    // update other controls
    for (uint8_t i = 0; i < controls_.size(); i++) {
      if (i != focus_index_) {
        controls_[i]->update(Input());
      } else {
        controls_[i]->update(input_for_focus);
      }
    }
  }

  void Tile::draw(LGFX_Device* gfx) {
    for (uint8_t i = 0; i < controls_.size(); i++) {
      controls_[i]->draw(gfx);
    }
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


  void TileManager::draw(LGFX_Device* gfx) {
    tile_->draw(gfx);
  }

}