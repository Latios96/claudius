//
// Created by Jan on 20.04.2019.
//

#ifndef RASPITOLIGHT_SRC_IO_IO_LIB_PARTICLECONTAINER_H_
#define RASPITOLIGHT_SRC_IO_IO_LIB_PARTICLECONTAINER_H_

#include <vector>
// todo do 3float iterator
class ParticleContainer {
 public:
  const unsigned int particleCount();
  void addParticle(float x, float y, float z);
  void addColor(int r, int g, int b);
  const bool isEmpty();
  const bool hasColorData();
  const float* getParticleData();
  const int* getColorData();
 private:
  std::vector<float> particles;
  std::vector<int> colors;
};

#endif //RASPITOLIGHT_SRC_IO_IO_LIB_PARTICLECONTAINER_H_
