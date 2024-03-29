#include <cstdio>
#include <fstream>
#include <iostream>

#include "ConfigJson.h"
#include "Music.h"
#include "Steve.h"
#include "ext/cmdline.h"

using namespace steve;

int main(int argc, char** argv) {
  note_name_init();

  cmdline::parser args;
  args.set_program_name("steve");
  args.add("list-scales", 0, "list scales and their information");
  args.add("mid", 'm', "output .mid file");
  args.add("txt", 't', "output .txt file");
  args.add<std::string>("config", 'c', "path to a JSON configuration file", true);
  args.add<std::string>("out", 'o', "path to the files to write (w/o extension)", false);
  args.add<uint32_t>("num", 'n', "generation count", false, 1);
  args.parse_check(argc, argv);

  ConfigJson config;
  config.parse_file(args.get<std::string>("config").c_str());
  config.compute_cache();

  if(args.exist("list-scales")) {
    config.list_scales(std::cout);
    return 0;
  }

  const uint32_t num = args.get<uint32_t>("num");
  const bool output_mid = args.exist("mid");
  const bool output_txt = args.exist("txt");
  for(uint32_t i = 0; i < num; i++) {
    std::string music_path = args.get<std::string>("out");
    Music music(config);
    if(music_path.empty()) {
      music_path = music.to_short_string();
    }
    std::cout << "Created " << music_path << std::endl;
    if(output_mid) {
      std::ofstream fs(music_path + ".mid", std::ofstream::binary);
      music.write_mid(fs);
    }
    if(output_txt) {
      std::ofstream fs(music_path + ".txt");
      music.write_txt(fs);
    }
  }

  return 0;
}
