//
// Created by Jan on 20.04.2019.
//

#include <iostream>
#include "ParticleReaderFactory.h"
#include "PtsParticleReader.h"
std::shared_ptr<ParticleReader> ParticleReaderFactory::createParticleReader(const std::string &path) {
    const std::string &fileExtension = path.substr(path.find_last_of('.') + 1);
    if(fileExtension == "pts") {
      return std::shared_ptr<ParticleReader>(new PtsParticleReader());
    }
    std::cerr << "No reader supporting file extension " + fileExtension << std::endl;
}
