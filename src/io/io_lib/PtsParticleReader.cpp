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
/*

std::vector<std::string> split(std::string str, char delimiter) {
    std::vector<std::string> internal;
    std::stringstream ss(str); // Turn the string into a stream.
    std::string tok;

    while(getline(ss, tok, delimiter)) {
        internal.push_back(tok);
    }

    return internal;
}*/
/*
void split(const std::string &str, std::vector<std::string> &splitted) {
	std::string buff;
	for (char c : str) {
		if (c != ' ') {
			buff += c;
		}
		else {
			splitted.push_back(buff);
			buff.clear();
		}
	}
}*/

void split(std::string const& original, std::vector<std::string> &results)
{
	std::string::const_iterator start = original.begin();
	std::string::const_iterator end = original.end();
	std::string::const_iterator next = std::find(start, end, ' ');
	while (next != end) {
		results.push_back(std::string(start, next));
		start = next + 1;
		next = std::find(start, end, ' ');
	}
	results.push_back(std::string(start, next));
}
// we need to flip Y and Z, because they are the other way around in .pts files
const unsigned int PTS_X_INDEX = 0;
const unsigned int PTS_Y_INDEX = 2;
const unsigned int PTS_Z_INDEX = 1;

void PtsParticleReader::readParticles(std::istream &file, ParticleContainer &particleContainer) {
    if(file.fail()){
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
    determineData.readUntilSpace(bufferString);
    int particleCount = std::stoi(bufferString);
    std::cout << "estimated particleCount: " << particleCount << std::endl;
    // read first line and check how many values we have
    determineData.readUntilNewLine(bufferString);

	std::vector<std::string> vec;
	split(bufferString, vec);

	const bool positionOnly = true;// vector.size() == 3;
    const bool positionAndRgb = vec.size() == 6;
    const bool positionRemissionAndRgb = vec.size() == 7;
    const bool positionRemissionQualityAndRgb = vec.size() == 8;

    if(positionOnly){
		std::cout << "positionOnly" << std::endl;
		std::cout << "resize positions" << std::endl;
        particleContainer.reservePositions(particleCount);
    }
	else if (positionAndRgb) {
		std::cout << "positionAndRgb" << std::endl;
		std::cout << "resize positions" << std::endl;
        particleContainer.reservePositions(particleCount);
		std::cout << "resize colors" << std::endl;
        particleContainer.reserveColors(particleCount);
	}
	else if (positionRemissionAndRgb) {
		std::cout << "positionRemissionAndRgb" << std::endl;
		std::cout << "resize positions" << std::endl;
        particleContainer.reservePositions(particleCount);
		std::cout << "resize colors" << std::endl;
        particleContainer.reserveColors(particleCount);
		std::cout << "resize remissions" << std::endl;
        particleContainer.reserveRemissions(particleCount);
	}
	else if (positionRemissionQualityAndRgb) {
		std::cout << "positionRemissionQualityAndRgb" << std::endl;
		std::cout << "resize positions" << std::endl;
        particleContainer.reservePositions(particleCount);
		std::cout << "resize colors" << std::endl;
        particleContainer.reserveColors(particleCount);
		std::cout << "resize remissions" << std::endl;
        particleContainer.reserveRemissions(particleCount);
	}

    CharBufferOperator bufferOperator(str);
    bufferOperator.toFirstCharAfterNewLine();
	bufferString.clear();

	std::vector<std::string> vector;

	float tenth = float(particleCount) / float(100);
	int percentageCounter = 1;

	std::cout << "reading particles" << std::endl;

    while(bufferOperator.canContinue()){
        bufferOperator.readUntilNewLine(bufferString);

        //const std::vector<std::string> vector = split(bufferString, ' ');
		

        if(positionOnly){
			/*
			bufferOperator.readUntilSpace(bufferString);
			float x = std::stof(bufferString);
			bufferString.clear();
			bufferOperator.toFirstCharAfterSpace();
			bufferOperator.readUntilSpace(bufferString);
			float z = std::stof(bufferString);
			bufferString.clear();
			bufferOperator.toFirstCharAfterSpace();
			bufferOperator.readUntilSpace(bufferString);
			float y = std::stof(bufferString);
			bufferString.clear();
            particleContainer.addParticle(x,y,z);*/
			/*split(bufferString, vector);
            particleContainer.addParticle(std::stof(vector[PTS_X_INDEX]), std::stof(vector[PTS_Y_INDEX]), std::stof(vector[PTS_Z_INDEX]));*/
        }
        else if(positionAndRgb){
			
			bufferOperator.readUntilSpace(bufferString);
			float x = std::stof(bufferString);
			bufferString.clear();
			bufferOperator.toFirstCharAfterSpace();
			bufferOperator.readUntilSpace(bufferString);
			float z = std::stof(bufferString);
			bufferString.clear();
			bufferOperator.toFirstCharAfterSpace();
			bufferOperator.readUntilSpace(bufferString);
			float y = std::stof(bufferString);
			bufferString.clear();
			bufferOperator.toFirstCharAfterSpace();
			particleContainer.addParticle(x, y, z);

			bufferOperator.readUntilSpace(bufferString);
			int r = std::stoi(bufferString);
			bufferString.clear();
			bufferOperator.toFirstCharAfterSpace();
			bufferOperator.readUntilSpace(bufferString);
			int g = std::stoi(bufferString);
			bufferString.clear();
			bufferOperator.toFirstCharAfterSpace();
			bufferOperator.readUntilSpace(bufferString);
			int b = std::stoi(bufferString);
			bufferString.clear();
			bufferOperator.toFirstCharAfterSpace();
			particleContainer.addColor(r,g,b);
			/*split(bufferString, vector);
			particleContainer.addParticle(std::stof(vector[PTS_X_INDEX]), std::stof(vector[PTS_Y_INDEX]), std::stof(vector[PTS_Z_INDEX]));
			particleContainer.addColor(std::stoi(vector[3]), std::stoi(vector[4]), std::stoi(vector[5]));*/
        }
        else if(positionRemissionAndRgb){
			split(bufferString, vector);
            particleContainer.addParticle(std::stof(vector[PTS_X_INDEX]), std::stof(vector[PTS_Y_INDEX]), std::stof(vector[PTS_Z_INDEX]));
            particleContainer.addRemission(std::stof(vector[3]));
            particleContainer.addColor(std::stoi(vector[4]), std::stoi(vector[5]), std::stoi(vector[6]));
        }
        else if(positionRemissionQualityAndRgb){

        }
		bufferString.clear();
		vector.clear();
        bufferOperator.toFirstCharAfterNewLine();

		if (particleContainer.particleCount() > tenth * percentageCounter) {
			std::cout << percentageCounter << "% Done" << std::endl;
			percentageCounter++;
		}
    }

}
std::string PtsParticleReader::getReaderName() {
  return "pts";
}

