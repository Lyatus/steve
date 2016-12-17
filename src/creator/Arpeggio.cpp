#include "Arpeggio.h"

using namespace Steve;

Notes Arpeggio::get(size_t start,size_t size,const std::vector<std::set<uint8_t>>& base) const {
  Notes notes;
  uint32_t i(0);
  while(i<size) {
    uint32_t d(time(i,size));
    std::vector<std::set<uint8_t>> chords(Chord::in(_scale,tonesAt(base,start+i,d)));
    while(chords.empty()) {
      if(!(d /= 2)) break;
      chords = Chord::in(_scale,tonesAt(base,start+i,d));
    }
    if(d>=pow(2,_mintime)) {
      std::set<uint8_t> chord(Rand::in(chords));
      uint8_t octave(_instrument.randomOctave());
      std::vector<uint8_t> tones;
      while(!chord.empty()) { // Transfering chord tones to arpeggio tones in random order
        tones.push_back(Rand::in(chord));
        chord.erase(tones.back());
      }
      for(uint32_t k(0); k*_md<d; k++)
        addNote(notes,_channel,octave+tones[k%tones.size()],i+k*_md,_md);
      i += d;
    } else i++;
  }
  return notes;
}