#include "quadui.hpp"
#include "quadui_variants.hpp"

#ifndef QUADUIAPP_HPP
#define QUADUIAPP_HPP

#include <LGFX_AUTODETECT.hpp>

#include <Adafruit_NeoPixel.h>

namespace QuadUI{

  struct AppConfig {
    uint8_t led_fixed_color_brightness=255;
    uint8_t led_fixed_color_contrast=50;
    uint8_t led_fixed_color_hue=120;
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

  class App : public TileManager{
    public:
      App();
      virtual void update();
      virtual void draw(LGFX_Device* gfx);

      void init(LGFX_Device* lcd, Adafruit_NeoPixel* pixels);

      // factories
      void set_home_static_light_tile();
      void set_home_static_light_color_config_tile();

      LGFX_Device* get_lcd() {
        return lcd_;
      }
      Adafruit_NeoPixel* get_pixels() {
        return pixels_;
      }

      AppConfig config_;

      protected:
      // io
      LGFX_Device* lcd_;
      Adafruit_NeoPixel* pixels_;
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

  class HomeStaticLightTile : public AppTile {
    public:
      explicit HomeStaticLightTile(App* parent_ptr);
      virtual void update(const Input& input);
      virtual void custom_draw(LGFX_Device* gfx);

    protected:
      StaticLightController light_controller_;
  };
  
  class HomeStaticLightColorConfigTile : public AppTile {
    public:
      explicit HomeStaticLightColorConfigTile(App* parent_ptr);
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
}


#endif // QUADUIAPP_HPP
