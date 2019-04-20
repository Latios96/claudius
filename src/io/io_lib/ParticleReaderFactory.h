//
// Created by Jan on 20.04.2019.
//

#ifndef RASPITOLIGHT_SRC_IO_IO_LIB_PARTICLEREADERFACTORY_H_
#define RASPITOLIGHT_SRC_IO_IO_LIB_PARTICLEREADERFACTORY_H_

#include "ParticleReader.h"
#include <memory>

class ParticleReaderFactory {
 public:
  static std::shared_ptr<ParticleReader> createParticleReader(const std::string &path);
};

#endif //RASPITOLIGHT_SRC_IO_IO_LIB_PARTICLEREADERFACTORY_H_
