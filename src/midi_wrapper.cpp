#include <functional>

#include <BLEMIDI_Transport.h>
#include <hardware/BLEMIDI_ESP32.h>

#include "midi_wrapper.hpp"

void MidiWrapper::init(MIDI* midi_interface) {
  midi_interface_ = midi_interface;
  midi_interface_->setHandleNoteOn(handle_note_on);
  midi_interface_->setHandleNoteOff(handle_note_off);
}
