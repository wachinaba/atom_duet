#include "quadui.hpp"
#include "quadui_variants.hpp"

#ifndef QUADUIAPP_HPP
#define QUADUIAPP_HPP

#include <LGFX_AUTODETECT.hpp>

#include <Adafruit_NeoPixel.h>

#include <BLEMIDI_Transport.h>
#include <hardware/BLEMIDI_ESP32.h>

#include "midi_wrapper.hpp"

#ifndef MIDI
#define MIDI MIDI_NAMESPACE::MidiInterface<BLEMIDI_NAMESPACE::BLEMIDI_Transport<BLEMIDI_NAMESPACE::BLEMIDI_ESP32>, BLEMIDI_NAMESPACE::MySettings>
#endif

namespace QuadUI{

  struct AppConfig {
    uint8_t led_fixed_color_brightness=80;
    uint8_t led_fixed_color_contrast=220;
    uint8_t led_fixed_color_hue=120;
    uint8_t midi_channel=1;
    uint8_t midi_keytrack_start=51;
  };

  class StaticLightController {
    public:
      StaticLightController(Adafruit_NeoPixel* pixels, AppConfig* config);
      void update();
    protected:
      Adafruit_NeoPixel* pixels_;
      uint32_t last_update_time_;
      uint32_t update_interval_;
      AppConfig* config_;
  };

  class WavingLightController {
    public:
      WavingLightController(Adafruit_NeoPixel* pixels, AppConfig* config);
      void update();
    protected:
      Adafruit_NeoPixel* pixels_;
      uint32_t last_update_time_;
      uint32_t update_interval_;
      uint8_t wave_position_;
      AppConfig* config_;
  };

  class MidiKeyTrackLightController {
    public:
      MidiKeyTrackLightController(Adafruit_NeoPixel* pixels, MIDI* midi_interface, AppConfig* config);
      ~MidiKeyTrackLightController();

      void handle_note_on(midi::Channel channel, byte pitch, byte velocity);
      void handle_note_off(midi::Channel channel, byte pitch, byte velocity);

      void update();
    protected:
      Adafruit_NeoPixel* pixels_;
      MIDI* midi_interface_;
      uint32_t last_update_time_;
      uint32_t update_interval_;
      AppConfig* config_;
  };

  class App : public TileManager{
    public:
      App();
      virtual void update();
      virtual void draw(LGFX_Device* gfx);

      void init(LGFX_Device* lcd, Adafruit_NeoPixel* pixels, MIDI* midi_interface);

      // factories
      void set_static_light_home_tile();
      void set_static_light_color_config_tile();
      void set_waving_light_home_tile();

      LGFX_Device* get_lcd() {
        return lcd_;
      }
      Adafruit_NeoPixel* get_pixels() {
        return pixels_;
      }
      MIDI* get_midi_interface() {
        return midi_interface_;
      }

      AppConfig config_;

      protected:
      // io
      LGFX_Device* lcd_;
      Adafruit_NeoPixel* pixels_;
      MidiWrapper* midi_wrapper_;
  };

  class AppTile : public Tile {
    public:
      explicit AppTile(App* parent_ptr);
      virtual void update(const Input& input);
      void draw(LGFX_Device* gfx);
      virtual void custom_draw(LGFX_Device* gfx) = 0;
    protected:
      App* parent_ptr_;
      bool render_queued_;
  };

  class StaticLightHomeTile : public AppTile {
    public:
      explicit StaticLightHomeTile(App* parent_ptr);
      virtual void update(const Input& input);
      virtual void custom_draw(LGFX_Device* gfx);

    protected:
      StaticLightController light_controller_;
  };
  
  class StaticLightColorConfigTile : public AppTile {
    public:
      explicit StaticLightColorConfigTile(App* parent_ptr);
      void change_hue(int8_t delta) {
        parent_ptr_->config_.led_fixed_color_hue = (parent_ptr_->config_.led_fixed_color_hue + delta) % 255;
        render_queued_ = true;
      }
      void change_contrast(int8_t contrast) {
        parent_ptr_->config_.led_fixed_color_contrast = (parent_ptr_->config_.led_fixed_color_contrast + contrast) % 255;
        render_queued_ = true;
      }
      void change_brightness(int8_t brightness) {
        parent_ptr_->config_.led_fixed_color_brightness = (parent_ptr_->config_.led_fixed_color_brightness + brightness) % 255;
        render_queued_ = true;
      }
      virtual void update(const Input& input);
      virtual void custom_draw(LGFX_Device* gfx);
    protected:
      StaticLightController light_controller_;
  };

  class WavingLightHomeTile : public AppTile {
    public:
      explicit WavingLightHomeTile(App* parent_ptr);
      virtual void update(const Input& input);
      virtual void custom_draw(LGFX_Device* gfx);

    protected:
      WavingLightController light_controller_;
  };

  class MidiKeyTrackHomeTile : public AppTile {
    public:
      explicit MidiKeyTrackHomeTile(App* parent_ptr);
      virtual void update(const Input& input);
      virtual void custom_draw(LGFX_Device* gfx);

    protected:
      MidiKeyTrackLightController light_controller_;
      
  };
  
}


#endif // QUADUIAPP_HPP
