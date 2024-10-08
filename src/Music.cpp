#include "Music.h"

#include <algorithm>
#include <cassert>

#include "Rand.h"
#include "cfg/Chord.h"
#include "cfg/Config.h"
#include "cfg/Instrument.h"
#include "ctr/Creator.h"

using namespace steve;

Music::Music(const Config& config)
  : _config(config),
    _scale(Scale(_config.get_scales().get_random_item(), Rand::next(0, 11))),
    _tempo(_config.get_tempo().get_value()),
    _signature(_config.get_signatures().get_random_item()) {
  { // Generate chord progression
    std::vector<steve::Chord> chords = _config.get_progression(_scale, 4);
    for(const Chord& chord : chords) {
      for(int i = 0; i < get_signature().beats_per_bar; i++) {
        _progression.push_back(chord);
      }
    }
    assert(_progression.size() % _signature->beats_per_bar == 0);
    _pattern_size = _progression.size() / _signature->beats_per_bar;
  }

  { // Compute duration
    const float target_duration = _config.get_duration().get_value();
    const float bar_duration = (get_bar_ticks() * get_tick_ms()) / 1000.f;
    uint32_t bar_count = uint32_t(target_duration / bar_duration);
    bar_count -= bar_count % _pattern_size;
    bar_count = std::max<uint32_t>(bar_count, _pattern_size);
    _ticks = bar_count * get_bar_ticks();
  }

  { // Generate beats
    _beats.resize(get_bar_ticks());
    for(uint32_t i = 0; i < _beats.size(); i++) {
      _beats[i] = false;
      for(NoteValue j = get_beat_value(); j >= NoteValue(0); j = NoteValue(uint32_t(j) - 1)) {
        const auto ticks = ticks_for(j);
        if((i / ticks) * ticks == i) {
          if(Rand::next(uint32_t(0), (uint32_t(get_beat_value()) - uint32_t(j)) * 2) == 0) {
            _beats[i] = true;
            break;
          }
        }
      }
    }
  }

  for(const auto& creator : _config.get_creators()) {
    add_creator(creator->func(this));
  }
}
void Music::add_creator(Creator* creator) {
  creator->init();
  creator->post_init();
  paste(creator->compose(), _notes);
  _creators.push_back(std::unique_ptr<Creator>(creator));
}
const Chord& Music::chord_at(size_t i) const {
  return _progression[(i / get_beat_ticks()) % _progression.size()];
}
ToneSet Music::tones_at(size_t start, size_t size) const {
  ToneSet tones = ~0;
  const uintptr_t start_beat = start / get_beat_ticks();
  const uintptr_t end_beat = (start + size - 1) / get_beat_ticks();
  for(uintptr_t i = start_beat; i <= end_beat; i++) {
    tones &= _progression[i % _progression.size()].tones;
  }
  return tones;
}
bool Music::is_beat(uintptr_t i) const {
  return _beats[i % _beats.size()];
}
std::vector<uintptr_t> Music::beats_inside(uintptr_t min, uintptr_t max) const {
  std::vector<uintptr_t> beats;
  for(uintptr_t i = min; i <= max; i++) {
    if(is_beat(i)) {
      beats.push_back(i);
    }
  }
  return beats;
}
std::string Music::to_short_string() const {
  std::string short_string;
  short_string += get_scale().desc->name + "_" + key_name(get_scale().key);
  short_string += "_" + std::to_string(_signature->beats_per_bar);
  short_string += std::to_string(get_signature().beat_value_numeral);
  short_string += "_" + std::to_string(get_tempo());

  std::replace(short_string.begin(), short_string.end(), ' ', '_');

  return short_string;
}
