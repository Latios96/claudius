//
// Created by Jan on 20.04.2019.
//

#include <fstream>
#include <iostream>
#include "PtsParticleReader.h"
#include "CharBufferOperator.h"
#include <string.h>
#include <sstream>
#include <memory>

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
      float x,y,z;
      std::string bufferString;
      while(charBufferOperator.canContinue()){
          if(charBufferOperator.readUntilSpace(bufferString)){
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

          charBufferOperator.toFirstCharAfterSpace();
          if(charBufferOperator.readUntilSpace(bufferString)){
              y = std::stof(bufferString);
              bufferString.clear();
          }
          else{
              break;
          }

          charBufferOperator.toFirstCharAfterSpace();
          if(charBufferOperator.readUntilNewLine(bufferString)){
              z = std::stof(bufferString);
              bufferString.clear();
          }
          else{
              break;
          }

          particleContainer.addParticle(x,y,z);
      }
  }
};

// remission: float
// RGB: rgb
// quality: float

std::vector<std::string> split(std::string str, char delimiter) {
    std::vector<std::string> internal;
    std::stringstream ss(str); // Turn the string into a stream.
    std::string tok;

    while(getline(ss, tok, delimiter)) {
        internal.push_back(tok);
    }

    return internal;
}

void PtsParticleReader::readParticles(std::istream &file, ParticleContainer &particleContainer) {
    std::vector<char> str;
    std::vector<float> floats;
    file.seekg(0, std::ios::end);
    str.resize(file.tellg());
    file.seekg(0);
    file.read(str.data(), str.size());

    std::string bufferString;
    unsigned int i=0;

    CharBufferOperator determineData(str);
    // skip particle count information
    determineData.toFirstCharAfterNewLine();
    // read first line and check how many values we have
    determineData.readUntilNewLine(bufferString);

    const std::vector<std::string> vector = split(bufferString, ' ');

    const bool positionOnly = vector.size() == 3;
    const bool positionAndRgb = vector.size() == 6;
    const bool positionRemissionAndRgb = vector.size() == 7;
    const bool positionRemissionQualityAndRgb = vector.size() == 8;

    CharBufferOperator bufferOperator(str);
    bufferOperator.toFirstCharAfterNewLine();
	bufferString.clear();

    while(bufferOperator.canContinue()){
        bufferOperator.readUntilNewLine(bufferString);

        const std::vector<std::string> vector = split(bufferString, ' ');

        if(positionOnly){
            particleContainer.addParticle(std::stof(vector[0]), std::stof(vector[1]), std::stof(vector[2]));
        }
        else if(positionAndRgb){
			particleContainer.addParticle(std::stof(vector[0]), std::stof(vector[1]), std::stof(vector[2]));
			particleContainer.addColor(std::stoi(vector[3]), std::stoi(vector[4]), std::stoi(vector[5]));
        }
        else if(positionRemissionAndRgb){

        }
        else if(positionRemissionQualityAndRgb){

        }
		bufferString.clear();
        bufferOperator.toFirstCharAfterNewLine();
    }

  /*float x,y,z;
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
  }*/

}
std::string PtsParticleReader::getReaderName() {
  return "pts";
}
