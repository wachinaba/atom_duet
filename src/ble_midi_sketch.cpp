/*
// ヘッダをincludeします。
#define LGFX_AUTODETECT // 自動認識 (D-duino-32 XS, PyBadge はパネルID読取りが出来ないため自動認識の対象から外れています)
#define LGFX_USE_V1   

#include <LovyanGFX.hpp>
#include <LGFX_AUTODETECT.hpp> // クラス"LGFX"を用意します
// #include <lgfx_user/LGFX_ESP32_sample.hpp> // またはユーザ自身が用意したLGFXクラスを準備します
static LGFX lcd; // LGFXのインスタンスを作成。


#include <Adafruit_NeoPixel.h>
#define PIN 38
#define NUMPIXELS 29

#include <BLEMIDI_Transport.h>

//#include <hardware/BLEMIDI_ESP32_NimBLE.h>
#include <hardware/BLEMIDI_ESP32.h>
//#include <hardware/BLEMIDI_nRF52.h>
//#include <hardware/BLEMIDI_ArduinoBLE.h>

BLEMIDI_CREATE_INSTANCE("atom_duet", MIDI)

unsigned long t0 = millis();
bool isConnected = false;

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// -----------------------------------------------------------------------------
// When BLE connected, LED will turn on (indication that connection was successful)
// When receiving a NoteOn, LED will go out, on NoteOff, light comes back on.
// This is an easy and conveniant way to show that the connection is alive and working. 
// -----------------------------------------------------------------------------
void setup()
{
  MIDI.begin();
  pixels.begin();
  pixels.clear();
  lcd.init();
  lcd.setColorDepth(16);
  lcd.fillScreen(lcd.color888(255,255,255));

  BLEMIDI.setHandleConnected([]() {
    isConnected = true;
    pixels.setPixelColor(0, pixels.Color(255, 255, 255));
    pixels.show();
    lcd.drawString("Connected!", 10, 10);
  });

  BLEMIDI.setHandleDisconnected([]() {
    isConnected = false;
    pixels.setPixelColor(0, pixels.Color(0, 0, 0));
    pixels.show();
  });

  MIDI.setHandleNoteOn([](byte channel, byte note, byte velocity) {
    pixels.setPixelColor(1, pixels.Color(255, note, velocity));
    pixels.show();
  });
  MIDI.setHandleNoteOff([](byte channel, byte note, byte velocity) {
    pixels.setPixelColor(1, pixels.Color(0, 0, 0));
    pixels.show();
  });
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void loop()
{
  MIDI.read();

}kk
*/