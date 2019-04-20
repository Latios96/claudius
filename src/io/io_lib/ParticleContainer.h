//
// Created by Jan on 20.04.2019.
//

#ifndef RASPITOLIGHT_SRC_IO_IO_LIB_PARTICLECONTAINER_H_
#define RASPITOLIGHT_SRC_IO_IO_LIB_PARTICLECONTAINER_H_

#include <vector>

class ParticleContainer {
 public:
  unsigned int particleCount();
  void addParticle(float x, float y, float z);
 private:
  std::vector<float> particles;
};

#endif //RASPITOLIGHT_SRC_IO_IO_LIB_PARTICLECONTAINER_H_
