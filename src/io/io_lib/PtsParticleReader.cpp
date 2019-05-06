//
// Created by Jan on 20.04.2019.
//

#include <fstream>
#include <iostream>
#include "PtsParticleReader.h"
#include "CharBufferOperator.h"

/**
 * Reads data for a single particle row
 */
class ReaderDelegate{
 public:
  ReaderDelegate(ParticleContainer &particleContainer, CharBufferOperator &charBufferOperator) : particleContainer(
      particleContainer), charBufferOperator(charBufferOperator) {}
      virtual void doRead() = 0;
 protected:
  ParticleContainer &particleContainer;
  CharBufferOperator &charBufferOperator;
};

class PositionReaderDelegate : public ReaderDelegate{
 public:
  PositionReaderDelegate(ParticleContainer &particleContainer, CharBufferOperator &charBufferOperator) : ReaderDelegate(
      particleContainer,
      charBufferOperator) {}
  void doRead() override{
      // todo read positions
  }
};

void PtsParticleReader::readParticles(std::istream &file, ParticleContainer &particleContainer) {
  std::vector<char> str;
  std::vector<float> floats;
  file.seekg(0, std::ios::end);
  str.resize(file.tellg());
  file.seekg(0);
  file.read(str.data(), str.size());

  std::string bufferString;
  unsigned int i=0;
  
  CharBufferOperator bufferOperator(str);
  // skip particle count information
  bufferOperator.toFirstCharAfterNewLine();
  // read first line and check how many values we have
  std::vector<ReaderDelegate> readerDelegates;
  readerDelegates.push_back(PositionReaderDelegate(particleContainer, bufferOperator));
  for(auto &readerDelegate : readerDelegates){
      readerDelegate.doRead();
  }
  float x,y,z;
  while(bufferOperator.canContinue()){
    if(bufferOperator.readUntilSpace(bufferString)){
		if (!bufferString.empty()) {
			x = std::stof(bufferString);
			bufferString.clear();
		}
		else {
			break;
		}
      
    }
    else{
      break;
    }

    bufferOperator.toFirstCharAfterSpace();
    if(bufferOperator.readUntilSpace(bufferString)){
      y = std::stof(bufferString);
      bufferString.clear();
    }
    else{
      break;
    }

	bufferOperator.toFirstCharAfterSpace();
    if(bufferOperator.readUntilNewLine(bufferString)){
      z = std::stof(bufferString);
      bufferString.clear();
    }
    else{
      break;
    }

    particleContainer.addParticle(x,y,z);
    bufferOperator.toFirstCharAfterNewLine();
  }

}
std::string PtsParticleReader::getReaderName() {
  return "pts";
}
