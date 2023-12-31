#include "quadui.hpp"
#include "quadui_variants.hpp"
#include "app.hpp"
#include "midi_wrapper.hpp"
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
    set_static_light_home_tile();
  }

  void App::update() {
    TileManager::update();
  }
  void App::draw(LGFX_Device* gfx) {
    TileManager::draw(gfx);
  }

  void App::set_static_light_home_tile() {
    tile_ = std::make_shared<StaticLightHomeTile>(this);
  }

  void App::set_static_light_color_config_tile() {
    tile_ = std::make_shared<StaticLightColorConfigTile>(this);
  }

  void App::set_waving_light_home_tile() {
    tile_ = std::make_shared<WavingLightHomeTile>(this);
  }

  void App::set_midi_keytrack_home_tile() {
    //tile_ = std::make_shared<MidiKeyTrackHomeTile>(this);
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
    last_update_time_ = millis();
  }

  WavingLightController::WavingLightController(Adafruit_NeoPixel* pixels, AppConfig* config) :
    pixels_(pixels),
    config_(config),
    last_update_time_(0),
    update_interval_(100),
    wave_position_(0)
  {}

  void WavingLightController::update() {
    // update all pixels to the same color
    if(millis() - last_update_time_ < update_interval_) {
      return;
    };

    // update wave position
    wave_position_ = (wave_position_ + 1) % pixels_->numPixels();

    for(uint16_t i = 0; i < pixels_->numPixels(); i++) {
      // calculate distance, circular
      uint16_t distance = abs(i - wave_position_);
      if(distance > pixels_->numPixels() / 2) distance = pixels_->numPixels() - distance;
      // calculate brightness
      uint8_t brightness = 255 - (distance * 511 / pixels_->numPixels());
      uint32_t color = pixels_->ColorHSV(config_->led_fixed_color_hue, config_->led_fixed_color_contrast, brightness);
      pixels_->setPixelColor(i, color);
    }
    // show
    pixels_->show();
    last_update_time_ = millis();
  }

  MidiKeyTrackLightController::MidiKeyTrackLightController(Adafruit_NeoPixel* pixels, AppConfig* config) :
    pixels_(pixels),
    config_(config)
  {
    // reset all pixels
    pixels_->clear();
    pixels_->show();
    // activate callback
    // note on (handle_note_on)
    //MidiWrapper::set_note_on_callback(std::bind(&MidiKeyTrackLightController::handle_note_on, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));

  }

  MidiKeyTrackLightController::~MidiKeyTrackLightController() {
    // deactivate callback
    //MidiWrapper::reset_callbacks();
  }

  void MidiKeyTrackLightController::update() {
  }

  void MidiKeyTrackLightController::handle_note_on(midi::Channel channel, byte pitch, byte velocity) {
    // calculate brightness
    uint8_t brightness = velocity * 255 / 127;
    uint32_t color = pixels_->ColorHSV(channel * 4096, config_->led_fixed_color_contrast, brightness);
    // calculate pixel index
    int16_t pixel_index = pitch - config_->midi_keytrack_start;
    // check pixel index is in range
    if(pixel_index >= pixels_->numPixels()) return;
    if(pixel_index < 0) return;

    pixels_->setPixelColor(pixel_index, color);
    // show
    pixels_->show();
  }

  void MidiKeyTrackLightController::handle_note_off(midi::Channel channel, byte pitch, byte velocity) {
    // calculate pixel index
    int16_t pixel_index = pitch - config_->midi_keytrack_start;
    // check pixel index is in range
    if(pixel_index >= pixels_->numPixels()) return;
    if(pixel_index < 0) return;

    pixels_->setPixelColor(pixel_index, 0);
    // show
    pixels_->show();
  }
  
  StaticLightHomeTile::StaticLightHomeTile(App* parent_ptr) :
    AppTile(parent_ptr),
    light_controller_(parent_ptr->get_pixels(), &parent_ptr->config_)
  {
    // add controls
    // right button
    controls_.push_back(std::make_shared<SingleActionButton>(112, 32, 16, 64, [this]{this->parent_ptr_->set_waving_light_home_tile();}, ">"));
    // bottom button (config)
    controls_.push_back(std::make_shared<SingleActionButton>(32, 112, 64, 16, [this]{this->parent_ptr_->set_static_light_color_config_tile();}, "Config"));
    // left button
    controls_.push_back(std::make_shared<SingleActionButton>(0, 32, 16, 64, [this]{this->parent_ptr_->set_midi_keytrack_home_tile();}, "<"));
  }

  void StaticLightHomeTile::update(const Input& input) {
    AppTile::update(input);
    light_controller_.update();
  }

  void StaticLightHomeTile::custom_draw(LGFX_Device* gfx) {
    // draw background
    gfx->fillRect(0, 0, 128, 128, UI_COLOR_BG);
    // draw text
    gfx->setTextColor(UI_COLOR_FG, UI_COLOR_BG);
    gfx->setTextDatum(MC_DATUM);
    gfx->drawString("Static Light", 64, 64);
  }

  StaticLightColorConfigTile::StaticLightColorConfigTile(App* parent_ptr) :
    AppTile(parent_ptr),
    light_controller_(parent_ptr->get_pixels(), &parent_ptr->config_)
  {
    // add controls
    // home button
    controls_.push_back(std::make_shared<SingleActionButton>(0, 0, 128, 16, [this]{this->parent_ptr_->set_static_light_home_tile();}, "Home"));
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

  void StaticLightColorConfigTile::update(const Input& input) {
    AppTile::update(input);
    light_controller_.update();
  }

  void StaticLightColorConfigTile::custom_draw(LGFX_Device* gfx) {
    // draw background
    gfx->fillRect(16, 16, 96, 96, UI_COLOR_BG);
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

  WavingLightHomeTile::WavingLightHomeTile(App* parent_ptr) :
    AppTile(parent_ptr),
    light_controller_(parent_ptr->get_pixels(), &parent_ptr->config_)
  {
    // add controls
    // right button
    controls_.push_back(std::make_shared<SingleActionButton>(112, 32, 16, 64, [this]{this->parent_ptr_->set_midi_keytrack_home_tile();}, ">"));
    // bottom button (config)
    controls_.push_back(std::make_shared<SingleActionButton>(32, 112, 64, 16, [this]{this->parent_ptr_->set_waving_light_home_tile();}, "Config"));
    // left button
    controls_.push_back(std::make_shared<SingleActionButton>(0, 32, 16, 64, [this]{this->parent_ptr_->set_static_light_home_tile();}, "<"));
  }

  void WavingLightHomeTile::update(const Input& input) {
    AppTile::update(input);
    light_controller_.update();
  }

  void WavingLightHomeTile::custom_draw(LGFX_Device* gfx) {
    // draw background
    gfx->fillRect(16, 16, 96, 96, UI_COLOR_BG);
    // draw text
    gfx->setTextColor(UI_COLOR_FG, UI_COLOR_BG);
    gfx->setTextDatum(MC_DATUM);
    gfx->drawString("Waving Light", 64, 64);
  }

  MidiKeyTrackHomeTile::MidiKeyTrackHomeTile(App* parent_ptr) :
    AppTile(parent_ptr),
    light_controller_(parent_ptr->get_pixels(), &parent_ptr->config_)
  {
    // add controls
    // right button
    controls_.push_back(std::make_shared<SingleActionButton>(112, 32, 16, 64, [this]{this->parent_ptr_->set_static_light_home_tile();}, ">"));
    // bottom button (config)
    controls_.push_back(std::make_shared<SingleActionButton>(32, 112, 64, 16, []{}, "Config"));
    // left button
    controls_.push_back(std::make_shared<SingleActionButton>(0, 32, 16, 64, [this]{this->parent_ptr_->set_waving_light_home_tile();}, "<"));
  }

  void MidiKeyTrackHomeTile::update(const Input& input) {
    AppTile::update(input);
    light_controller_.update();
  }

  void MidiKeyTrackHomeTile::custom_draw(LGFX_Device* gfx) {
    // draw background
    gfx->fillRect(16, 16, 96, 96, UI_COLOR_BG);
    // draw text
    gfx->setTextColor(UI_COLOR_FG, UI_COLOR_BG);
    gfx->setTextDatum(MC_DATUM);
    gfx->drawString("Midi Key Track", 64, 64);
  }

}



