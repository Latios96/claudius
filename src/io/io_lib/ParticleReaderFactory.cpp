//
// Created by Jan on 20.04.2019.
//

#include "ParticleReaderFactory.h"
std::shared_ptr<ParticleReader> ParticleReaderFactory::createParticleReader(const std::string &path) {
  return std::shared_ptr<ParticleReader>();
}
