#include "Bass.h"

#include "../Music.h"
#include "../cfg/Chord.h"
#include "../cfg/Instrument.h"

using namespace steve;

Bass::Bass(Music* music)
  : Creator(music) {}
void Bass::init() {
  Creator::init();

  _min_tone = 36;
  _max_tone = _min_tone + 12;
}
Notes Bass::get(size_t start, size_t size) const {
  Notes notes;
  auto times = generate_times(start, size, true);
  uintptr_t i = 0;
  while(i < size) {
    for(uintptr_t j = 0; j < times.size() - 1; j++) {
      const auto duration = times[j + 1] - times[j];
      const Chord& chord = _music->chord_at(start + i);
      const uint8_t tone = _min_tone + chord.key;
      add_note(notes, _channel, tone, i, duration);
      i += duration;
    }
  }
  return notes;
}
bool Bass::is_valid_instrument(const Instrument& instrument) const {
  return instrument.min_tone <= 36 && instrument.max_tone >= 48;
}
