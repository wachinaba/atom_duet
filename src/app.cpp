#include "quadui.hpp"
#include "quadui_variants.hpp"
#include "app.hpp"
#include <Adafruit_NeoPixel.h>


namespace QuadUI{
  
  App::App() :
    lcd_(nullptr),
    pixels_(nullptr),
    TileManager(nullptr, PhysicalButton(41, false))
  {
  }

  void App::init(LGFX_Device* lcd, Adafruit_NeoPixel* pixels) {
    lcd_ = lcd;
    pixels_ = pixels;
    // tile init
    set_home_static_light_tile();
  }

  void App::update() {
    TileManager::update();
  }
  void App::draw(LGFX_Device* gfx) {
    TileManager::draw(gfx);
  }

  void App::set_home_static_light_tile() {
    tile_ = std::make_shared<HomeStaticLightTile>(this);
  }

  void App::set_home_static_light_color_config_tile() {
    tile_ = std::make_shared<HomeStaticLightColorConfigTile>(this);
  }

  AppTile::AppTile(App* parent_ptr) :
    Tile(),
    parent_ptr_(parent_ptr),
    render_queued_(true)
  {}

  void AppTile::update(const Input& input) {
    Tile::update(input);
  }

  void AppTile::draw(LGFX_Device* gfx) {
    if(render_queued_) {
      this->custom_draw(gfx);
      render_queued_ = false;
    }
    Tile::draw(gfx);
  }

  StaticLightController::StaticLightController(Adafruit_NeoPixel* pixels, AppConfig* config) :
    pixels_(pixels),
    config_(config),
    last_update_time_(0),
    update_interval_(100)
  {}

  void StaticLightController::update() {
    // update all pixels to the same color
    if(millis() - last_update_time_ < update_interval_) return;

    uint32_t color = pixels_->ColorHSV(config_->led_fixed_color_hue * 256, config_->led_fixed_color_contrast, config_->led_fixed_color_brightness);
    for(uint16_t i = 0; i < pixels_->numPixels(); i++) {
      pixels_->setPixelColor(i, color);
    }
    // show
    pixels_->show();
  }

  HomeStaticLightTile::HomeStaticLightTile(App* parent_ptr) :
    AppTile(parent_ptr),
    light_controller_(parent_ptr->get_pixels(), &parent_ptr->config_)
  {
    // add controls
    // right button
    controls_.push_back(std::make_shared<SingleActionButton>(112, 32, 16, 64, []{}, ">"));
    // bottom button (config)
    controls_.push_back(std::make_shared<SingleActionButton>(32, 112, 64, 16, [this]{this->parent_ptr_->set_home_static_light_color_config_tile();}, "Config"));
    // left button
    controls_.push_back(std::make_shared<SingleActionButton>(0, 32, 16, 64, []{}, "<"));
  }

  void HomeStaticLightTile::update(const Input& input) {
    AppTile::update(input);
    light_controller_.update();
  }

  void HomeStaticLightTile::custom_draw(LGFX_Device* gfx) {
    // draw background
    gfx->fillRect(0, 0, 128, 128, UI_COLOR_BG);
    // draw text
    gfx->setTextColor(UI_COLOR_FG, UI_COLOR_BG);
    gfx->setTextDatum(MC_DATUM);
    gfx->drawString("Static Light", 64, 64);
  }

  HomeStaticLightColorConfigTile::HomeStaticLightColorConfigTile(App* parent_ptr) :
    AppTile(parent_ptr),
    light_controller_(parent_ptr->get_pixels(), &parent_ptr->config_)
  {
    // add controls
    // home button
    controls_.push_back(std::make_shared<SingleActionButton>(0, 0, 128, 16, [this]{this->parent_ptr_->set_home_static_light_tile();}, "Home"));
    // hue add button
    controls_.push_back(std::make_shared<SingleActionButton>(112, 32, 16, 16, [this]{this->change_hue(10);this->render_queued_=true;}, ">"));
    // hue sub button
    controls_.push_back(std::make_shared<SingleActionButton>(0, 32, 16, 16, [this]{this->change_hue(-10);this->render_queued_=true;}, "<"));
    // contrast add button
    controls_.push_back(std::make_shared<SingleActionButton>(112, 56, 16, 16, [this]{this->change_contrast(10);this->render_queued_=true;}, ">"));
    // contrast sub button
    controls_.push_back(std::make_shared<SingleActionButton>(0, 56, 16, 16, [this]{this->change_contrast(-10);this->render_queued_=true;}, "<"));
    // brightness add button
    controls_.push_back(std::make_shared<SingleActionButton>(112, 80, 16, 16, [this]{this->change_brightness(10);this->render_queued_=true;}, ">"));
    // brightness sub button
    controls_.push_back(std::make_shared<SingleActionButton>(0, 80, 16, 16, [this]{this->change_brightness(-10);this->render_queued_=true;}, "<"));
  }

  void HomeStaticLightColorConfigTile::update(const Input& input) {
    AppTile::update(input);
    light_controller_.update();
  }

  void HomeStaticLightColorConfigTile::custom_draw(LGFX_Device* gfx) {
    // draw background
    gfx->fillRect(0, 0, 128, 128, UI_COLOR_BG);
    // draw text
    gfx->setTextColor(UI_COLOR_FG, UI_COLOR_BG);
    // draw color
    uint32_t color = parent_ptr_->get_pixels()->ColorHSV(parent_ptr_->config_.led_fixed_color_hue, parent_ptr_->config_.led_fixed_color_contrast, parent_ptr_->config_.led_fixed_color_brightness);
    gfx->fillRect(32, 32, 64, 64, color);
    // draw hue
    gfx->drawString("Hue", 64, 48);
    gfx->drawString(String(parent_ptr_->config_.led_fixed_color_hue), 64, 56);
    // draw contrast
    gfx->drawString("Contrast", 64, 72);
    gfx->drawString(String(parent_ptr_->config_.led_fixed_color_contrast), 64, 80);
    // draw brightness
    gfx->drawString("Brightness", 64, 96);
    gfx->drawString(String(parent_ptr_->config_.led_fixed_color_brightness), 64, 104);
  }


}



