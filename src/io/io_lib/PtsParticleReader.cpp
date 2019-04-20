//
// Created by Jan on 20.04.2019.
//

#include <fstream>
#include <iostream>
#include "PtsParticleReader.h"

void PtsParticleReader::readParticles(std::istream &file, ParticleContainer &particleContainer) {
  std::vector<char> str;
  std::vector<float> floats;
  file.seekg(0, std::ios::end);
  str.resize(file.tellg());
  file.seekg(0);
  file.read(str.data(), str.size());
  std::string bufferString;
  unsigned int i=0;

  while(str[i] != '\n' && i < str.size()){
    i++;
  }
  i++;
  while(i < str.size()){
    if(str[i] == '\n' || str[i] == ' '){
      floats.push_back(std::stof(bufferString));
      if (floats.size() == 3){
        particleContainer.addParticle(floats[0], floats[1], floats[2]);
        floats.clear();
      }
      bufferString.clear();
      while(i < str.size() && str[i] =='\n'){
        i++;
      }
      i++;
    }
    else{
      bufferString.push_back(str[i]);
    }
    i++;
  }
}
std::string PtsParticleReader::getReaderName() {
  return "pts";
}
