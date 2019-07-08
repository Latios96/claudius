//
// Created by Jan on 20.04.2019.
//

#ifndef CLAUDIUS_IO_LIB_PARTICLEREADERFACTORY_H_
#define CLAUDIUS_IO_LIB_PARTICLEREADERFACTORY_H_

#include "ParticleReader.h"
#include <memory>

class ParticleReaderFactory {
 public:
  static std::shared_ptr<ParticleReader> createParticleReader(const std::string &path);
};

#endif //CLAUDIUS_IO_LIB_PARTICLEREADERFACTORY_H_
