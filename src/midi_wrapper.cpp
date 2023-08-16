#include <functional>

#include "midi_wrapper.hpp"

MidiWrapper::MidiWrapper(midi_t* midi_interface) {
  /*
  MidiWrapper::midi_interface_ = midi_interface;
  reset_callbacks();
  MidiWrapper::midi_interface_->setHandleNoteOn(handle_note_on);
  MidiWrapper::midi_interface_->setHandleNoteOff(handle_note_off);
  */
}

midi_t* MidiWrapper::get_midi_interface() {
  return nullptr;
  //return MidiWrapper::midi_interface_;
}

void MidiWrapper::set_note_on_callback(std::function<void(midi::Channel, byte, byte)> callback) {
  //MidiWrapper::note_on_callback_ = callback;
}
void MidiWrapper::set_note_off_callback(std::function<void(midi::Channel, byte, byte)> callback) {
  //MidiWrapper::note_off_callback_ = callback;
}

void MidiWrapper::reset_callbacks() {
  //MidiWrapper::note_on_callback_ = nullptr;
  //MidiWrapper::note_off_callback_ = nullptr;
}

void MidiWrapper::handle_note_on(midi::Channel channel, byte pitch, byte velocity) {
  /*
  if (MidiWrapper::note_on_callback_) {
    MidiWrapper::note_on_callback_(channel, pitch, velocity);
  }
  */
}

void MidiWrapper::handle_note_off(midi::Channel channel, byte pitch, byte velocity) {
  /*
  if (MidiWrapper::note_off_callback_) {
    MidiWrapper::note_off_callback_(channel, pitch, velocity);
  }
  */
}