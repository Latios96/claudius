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
// we need to flip Y and Z, because they are the other way around in .pts files
const unsigned int PTS_X_INDEX = 0;
const unsigned int PTS_Y_INDEX = 2;
const unsigned int PTS_Z_INDEX = 1;

const unsigned int CHUNK_SIZE = 100000000; // 100 MB

bool readChunk(std::istream &file, std::vector<char> buffer){
    file.read(buffer.data(), CHUNK_SIZE);
    bool reachedEndOfFile = file.gcount() < CHUNK_SIZE; // todo respect that we could end in the middle of a line
    return reachedEndOfFile;
}

void PtsParticleReader::readParticles(std::istream &file, ParticleContainer &particleContainer) {
    /*if(file.fail()){
        std::cout << "File does not exist!" << std::endl;
        return;
    }
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
            particleContainer.addParticle(std::stof(vector[PTS_X_INDEX]), std::stof(vector[PTS_Y_INDEX]), std::stof(vector[PTS_Z_INDEX]));
        }
        else if(positionAndRgb){
			particleContainer.addParticle(std::stof(vector[PTS_X_INDEX]), std::stof(vector[PTS_Y_INDEX]), std::stof(vector[PTS_Z_INDEX]));
			particleContainer.addColor(std::stoi(vector[3]), std::stoi(vector[4]), std::stoi(vector[5]));
        }
        else if(positionRemissionAndRgb){
            particleContainer.addParticle(std::stof(vector[PTS_X_INDEX]), std::stof(vector[PTS_Y_INDEX]), std::stof(vector[PTS_Z_INDEX]));
            particleContainer.addRemission(std::stof(vector[3]));
            particleContainer.addColor(std::stoi(vector[4]), std::stoi(vector[5]), std::stoi(vector[6]));
        }
        else if(positionRemissionQualityAndRgb){

        }
		bufferString.clear();
        bufferOperator.toFirstCharAfterNewLine();
    }*/
    std::vector<char> str;
    str.resize(CHUNK_SIZE);
    bool reachedEndOfFile = readChunk(file, str);
    processChunk(str, particleContainer);
    while(!reachedEndOfFile){
        reachedEndOfFile = readChunk(file, str);
        processChunk(str, particleContainer);
    }

}
std::string PtsParticleReader::getReaderName() {
  return "pts";
}
void PtsParticleReader::processChunk(std::vector<char> &str, ParticleContainer &particleContainer) {
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
            particleContainer.addParticle(std::stof(vector[PTS_X_INDEX]), std::stof(vector[PTS_Y_INDEX]), std::stof(vector[PTS_Z_INDEX]));
        }
        else if(positionAndRgb){
            particleContainer.addParticle(std::stof(vector[PTS_X_INDEX]), std::stof(vector[PTS_Y_INDEX]), std::stof(vector[PTS_Z_INDEX]));
            particleContainer.addColor(std::stoi(vector[3]), std::stoi(vector[4]), std::stoi(vector[5]));
        }
        else if(positionRemissionAndRgb){
            particleContainer.addParticle(std::stof(vector[PTS_X_INDEX]), std::stof(vector[PTS_Y_INDEX]), std::stof(vector[PTS_Z_INDEX]));
            particleContainer.addRemission(std::stof(vector[3]));
            particleContainer.addColor(std::stoi(vector[4]), std::stoi(vector[5]), std::stoi(vector[6]));
        }
        else if(positionRemissionQualityAndRgb){

        }
        bufferString.clear();
        bufferOperator.toFirstCharAfterNewLine();
    }
}

