#include <Arduino.h>

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
#include "quadui_variants.hpp"
#include "app.hpp"



#if 1

BLEMIDI_CREATE_INSTANCE("atom_duet", MIDI)

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

QuadUI::PhysicalButton button1(41, false);

QuadUI::App app;

void setup() {

  Serial.begin(9600);
  
  // hw init
  pixels.begin();
  pixels.clear();
  pixels.setPixelColor(0, pixels.Color(255, 0, 0));
  pixels.show();
  lcd.init();
  lcd.setColorDepth(16);
  lcd.setRotation(1);
  app.init(&lcd, &pixels);
  MIDI.begin();
  Serial.println("setup done");
  delay(1000);

}

int count = 0;

void loop() {
  Serial.println("loop");
  app.update();
  app.draw(&lcd);
  delay(10);
}

#else
void setup() {};
void loop() {};
#endif