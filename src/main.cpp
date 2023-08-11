#define LGFX_AUTODETECT
#define LGFX_USE_V1   

#include <LovyanGFX.hpp>
#include <LGFX_AUTODETECT.hpp>

static LGFX lcd;


#include <Adafruit_NeoPixel.h>
#define PIN 38
#define NUMPIXELS 29

#include <BLEMIDI_Transport.h>
#include <hardware/BLEMIDI_ESP32.h>

#include "quadui.hpp"

BLEMIDI_CREATE_INSTANCE("atom_duet", MIDI)

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

QuadUI::PhysicalButton button1(41, false);

void setup() {
  lcd.init();
  lcd.setColorDepth(16);
  lcd.fillScreen(lcd.color888(255,255,255));
}

int count = 0;

void loop() {
  button1.update();
  if(button1.is_pressed_trigger()) lcd.fillScreen(lcd.color888(255,255,255));
  lcd.drawNumber(button1.press_duration_, 10, 10);
  if(button1.is_long_released_trigger(1000)){
    lcd.drawString("long pressed!", 10, 10);
    delay(1000);
    lcd.fillScreen(lcd.color888(255,255,255));
  }
}