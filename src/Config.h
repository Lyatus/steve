#pragma once

#include <cstdint>
#include <vector>

#include "Chord.h"
#include "ConfigItemList.h"
#include "Instrument.h"
#include "Scale.h"
#include "creator/Creator.h"

namespace steve {
  class Config {
  protected:
    uint32_t min_tempo = 0, max_tempo = 360;
    std::vector<TimeSignature> _signatures;
    ConfigItemList<CreatorDescription> _creators;
    ConfigItemList<ChordDescription> _chords;
    ConfigItemList<ScaleDescription> _scales;
    ConfigItemList<Instrument> _instruments;

  public:
    Config();
    void compute_cache();
    void list_scales(std::ostream&) const;

    uint32_t get_random_tempo() const;
    TimeSignature get_random_time_signature() const;
    std::vector<Chord> get_chords_inside(ToneSet tones) const;
    std::vector<Chord> get_chord_progression(const Scale&) const;
    Scale get_random_scale() const;
    std::shared_ptr<const Instrument> get_random_instrument() const;
    std::vector<std::shared_ptr<const CreatorDescription>> get_creators() const;
  };
}
