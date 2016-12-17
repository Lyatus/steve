#include <cstdio>
#include <iostream>
#include <string>
#include <ctype.h>
#include "Chord.h"
#include "Music.h"
#include "Player.h"
#include "Scale.h"
#include "Steve.h"
#include <conio.h>

using namespace Steve;

int main() {
  // Scales
  Scale::add(Scale("Blues",{0,3,5,6,7}));
  //Scale::add(Scale("Byzantine",{0,1,4,5,7,8,11}));
  Scale::add(Scale("Harmonic minor",{0,2,3,5,7,8,11}));
  //Scale::add(Scale("Kumoi",{0,2,3,7,9}));
  Scale::add(Scale("Major",{0,2,4,5,7,9,11}));
  //Scale::add(Scale("Melodic minor",{0,2,3,5,7,9,11}));
  Scale::add(Scale("Pentatonic major",{0,2,4,7,9}));

  // Chords
  Chord::add({0,4});      // Major third
  Chord::add({0,3});      // Minor third
  Chord::add({0,7});      // Perfect fifth
  Chord::add({0,4,7});    // Major
  Chord::add({0,3,7});    // Minor
  Chord::add({0,4,7,10}); // 7th

  // Instruments
  Instrument::add(Instrument(0,"Acoustic Grand Piano",21,108));
  Instrument::add(Instrument(1,"Bright Acoustic Piano",21,108));
  Instrument::add(Instrument(2,"Electric Grand Piano",21,108));
  Instrument::add(Instrument(3,"Honky Tonk Piano",21,108));
  Instrument::add(Instrument(4,"Electric Piano 1",21,108));
  Instrument::add(Instrument(5,"Electric Piano 2",21,108));
  Instrument::add(Instrument(6,"Harpsichord",21,108));
  Instrument::add(Instrument(7,"Clavinet",21,108));
  Instrument::add(Instrument(25,"Acoustic Guitar (nylon)",40,77));
  Instrument::add(Instrument(26,"Acoustic Guitar (steel)",40,77));
  Instrument::add(Instrument(27,"Electric Guitar (jazz)",40,77));
  Instrument::add(Instrument(28,"Electric Guitar (clean)",40,77));
  Instrument::add(Instrument(29,"Electric Guitar (muted)",40,77));
  Instrument::add(Instrument(30,"Overdriven Guitar",40,77));
  Instrument::add(Instrument(31,"Distortion Guitar",40,77));
  Instrument::add(Instrument(32,"Guitar Harmonics",40,77));
  Instrument::add(Instrument(40,"Violin",55,103));
  Instrument::add(Instrument(41,"Viola",48,91));
  Instrument::add(Instrument(42,"Cello",36,76));
  Instrument::add(Instrument(43,"Contrabass",28,67));
  Instrument::add(Instrument(48,"String Ensemble",36,91));
  Instrument::add(Instrument(57,"Trumpet",55,82));
  Instrument::add(Instrument(58,"Trombone",40,72));
  Instrument::add(Instrument(59,"Tuba",28,58));
  Instrument::add(Instrument(60,"Muted Trumpet",55,82));
  Instrument::add(Instrument(61,"French Horn",34,77));
  //Instrument::add(Instrument(62,"Brass Section",28,67));
  //Instrument::add(Instrument(63,"Synth Brass 1",28,67));
  //Instrument::add(Instrument(64,"Synth Brass 2",28,67));

  std::cout << "Press space to skip a music." << std::endl << std::endl;
  Player player;
  while(true) {
    uint64_t musicId(Rand::state());
    std::cout << "...\r";
    Music music;
    std::cout << "New music: " << musicId << std::endl;
    for(auto&& instrument : music.instruments())
      std::cout << "\t- " << instrument->name() << std::endl;
    std::cout << "\tScale: " << music.scale().name() << std::endl
      << "\tTempo: " << music.tempo() << std::endl
      << "\tDuration: " << music.duration() << std::endl << std::endl;
    player.start(&music);
    while(!player.free()){
      if(_kbhit() && _getch()==' ')
        player.stop();
      else
        player.play();
    }
  }
  return 0;
}