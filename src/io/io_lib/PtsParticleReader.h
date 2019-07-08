//
// Created by Jan on 20.04.2019.
//

#ifndef CLAUDIUS_IO_LIB_PTSPARTICLEREADER_H_
#define CLAUDIUS_IO_LIB_PTSPARTICLEREADER_H_

#include "ParticleReader.h"

class PtsParticleReader : public ParticleReader{
 public:
  void readParticles(std::istream &file, ParticleContainer &particleContainer) override;
  std::string getReaderName() override;
};

#endif //CLAUDIUS_IO_LIB_PTSPARTICLEREADER_H_
