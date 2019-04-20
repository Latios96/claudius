//
// Created by Jan on 20.04.2019.
//

#ifndef RASPITOLIGHT_SRC_IO_IO_LIB_PARTICLEREADER_H_
#define RASPITOLIGHT_SRC_IO_IO_LIB_PARTICLEREADER_H_

#include <string>
#include "ParticleContainer.h"
class ParticleReader{
  virtual void readParticles(const std::string &path, const ParticleContainer &particleContainer) = 0;
  virtual std::string getReaderName() = 0;
  virtual ~ParticleReader() = default;
};
#endif //RASPITOLIGHT_SRC_IO_IO_LIB_PARTICLEREADER_H_
