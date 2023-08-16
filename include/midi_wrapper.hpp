#ifndef MIDI_WRAPPER_HPP
#define MIDI_WRAPPER_HPP

#include <functional>

#include <BLEMIDI_Transport.h>
#include <hardware/BLEMIDI_ESP32.h>

#ifndef midi_t
#define midi_t MIDI_NAMESPACE::MidiInterface<BLEMIDI_NAMESPACE::BLEMIDI_Transport<BLEMIDI_NAMESPACE::BLEMIDI_ESP32>, BLEMIDI_NAMESPACE::MySettings>
#endif

class MidiWrapper {
  public:
    MidiWrapper(midi_t* midi_interface);

    static void handle_note_on(midi::Channel channel, byte pitch, byte velocity);
    static void handle_note_off(midi::Channel channel, byte pitch, byte velocity);

    static midi_t* get_midi_interface();

    static void set_note_on_callback(std::function<void(midi::Channel, byte, byte)> callback);
    static void set_note_off_callback(std::function<void(midi::Channel, byte, byte)> callback);

    static void reset_callbacks();

  protected:
    static midi_t* midi_interface_;
    static std::function<void(midi::Channel, byte, byte)> note_on_callback_;
    static std::function<void(midi::Channel, byte, byte)> note_off_callback_;

};

#endif // MIDI_WRAPPER_HPP