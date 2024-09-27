#pragma once



namespace steve {
  struct ConfigRangedNumber {
    float min = 0.f;
    float max = 1.f;
    float step = 1.f;
    float sigma = 1.f;

    float get_value() const;
  };
}
