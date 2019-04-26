//
// Created by Jan on 20.04.2019.
//

#ifndef RASPITOLIGHT_SRC_IO_IO_LIB_PTSPARTICLEREADER_H_
#define RASPITOLIGHT_SRC_IO_IO_LIB_PTSPARTICLEREADER_H_

#include "ParticleReader.h"

class PtsParticleReader : public ParticleReader{
 public:
  void readParticles(std::istream &file, ParticleContainer &particleContainer) override;
  std::string getReaderName() override;
};

#endif //RASPITOLIGHT_SRC_IO_IO_LIB_PTSPARTICLEREADER_H_
