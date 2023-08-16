#include <functional>

#include <BLEMIDI_Transport.h>
#include <hardware/BLEMIDI_ESP32.h>

#ifndef MIDI
#define MIDI MIDI_NAMESPACE::MidiInterface<BLEMIDI_NAMESPACE::BLEMIDI_Transport<BLEMIDI_NAMESPACE::BLEMIDI_ESP32>, BLEMIDI_NAMESPACE::MySettings>
#endif

class MidiWrapper {
  public:
    static void handle_note_on(midi::Channel channel, byte pitch, byte velocity);
    static void handle_note_off(midi::Channel channel, byte pitch, byte velocity);
    static void init(MIDI* midi_interface);
    static MIDI* get_midi_interface();

  protected:
    static MIDI* midi_interface_;
    static std::function<void(midi::Channel, byte, byte)> note_on_callback_;

};
