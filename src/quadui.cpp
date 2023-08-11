#include <Arduino.h>

#include <stdint.h>
#include <vector>
#include <functional>
#include <memory>

// physical io
class PhysicalButton;

// ui processing
class Screen;
class Tile;
class Button;

// ui visual
class TileVisual;
class ButtonVisual;

class PhysicalButton{
  public:
    PhysicalButton(uint8_t pin, bool invert);
    void update();
    bool is_pressed() {return is_pressed_;};
  
  private:
    bool is_pressed_;
    uint8_t pin_;
    bool invert_;
    uint16_t press_duration_;
    bool previous_state_;
};

PhysicalButton::PhysicalButton(uint8_t pin, bool invert=true) : 
  is_pressed_(false),
  pin_(pin),
  invert_(invert),
  press_duration_(0),
  previous_state_(false)
{
  pinMode(pin_, INPUT);
}

void PhysicalButton::update() {
  // update state
  previous_state_ = is_pressed_;
  is_pressed_ = digitalRead(pin_);
  if(invert_) {
    is_pressed_ = !is_pressed_;
  }

  // update duration
  if(is_pressed_) {
    press_duration_++;
  } else {
    press_duration_ = 0;
  }
}


class Screen {
  public:
    Screen();
    void process();

  private:
    std::shared_ptr<Tile> current_tile_;

};

void Screen::process() {
    
}

class Tile{
  public:
    Tile(const std::shared_ptr<Screen>& screen);
    uint8_t get_focus_id() {return button_focus_id_;};

  private:
    std::shared_ptr<Screen> screen_;
    std::vector<std::shared_ptr<Button>> buttons_;
    uint8_t button_focus_id_;

};

class Button {
  public:
    Button(const std::shared_ptr<Tile>& tile, uint8_t focus_id, ButtonVisual visual);

  private:
    std::shared_ptr<Tile> tile;
    uint8_t focus_id_;
    uint8_t press_duration_;
    ButtonVisual visual_;

    bool is_focused();
};

bool Button::is_focused() {
  if(tile->get_focus_id() == focus_id_) return true;
  return false;
}

class ButtonVisual {
  public:
    ButtonVisual(const std::shared_ptr<Button>& button, uint8_t px, uint8_t py, uint8_t sx, uint8_t sy);
    void redraw();

  private:
    std::shared_ptr<Button> button_;
    void draw_focused_button();
    void draw_button();
};

void ButtonVisual::redraw() {
  // redraw button
  // if focused, draw outer border

  // draw contents

}