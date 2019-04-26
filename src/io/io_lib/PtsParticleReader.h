//
// Created by Jan on 20.04.2019.
//

#ifndef RASPITOLIGHT_SRC_IO_IO_LIB_PTSPARTICLEREADER_H_
#define RASPITOLIGHT_SRC_IO_IO_LIB_PTSPARTICLEREADER_H_

#include "ParticleReader.h"

class BufferOperator{
  std::vector<char> &charBuffer;
 public:
  explicit BufferOperator(std::vector<char> &charBuffer) : charBuffer(charBuffer){
    bufferIndex = 0;
  }
  char currentChar(){
    return charBuffer[bufferIndex];
  };
  bool toFirstCharAfterNewLine(){
    unsigned oldBufferIndex = bufferIndex;
    while(bufferIndex < charBuffer.size() && currentChar() != '\n'){
      bufferIndex++;
    }
    if(bufferIndex <= charBuffer.size()){
      bufferIndex++;
    }
    return oldBufferIndex < bufferIndex;
  }
  bool toFirstCharAfterSpace(){
    unsigned oldBufferIndex = bufferIndex;
    while(bufferIndex < charBuffer.size() && currentChar() != ' '){
      bufferIndex++;
    }
    if(bufferIndex <= charBuffer.size()){
      bufferIndex++;
    }
    return oldBufferIndex < bufferIndex;
  }
  bool readUntilSpace(std::string &bufferString){
    unsigned int i = 0;
    unsigned oldBufferIndex = bufferIndex;
    while(bufferIndex < charBuffer.size() && currentChar() != ' '){
		bufferString.push_back(currentChar());
		bufferIndex++;
    }
    return oldBufferIndex < bufferIndex;
  }
  bool readUntilNewLine(std::string &bufferString) {
	  unsigned int i = 0;
      unsigned oldBufferIndex = bufferIndex;
	  while (bufferIndex < charBuffer.size() && currentChar() != '\n') {
		  bufferString.push_back(currentChar());
		  bufferIndex++;
	  }
      return oldBufferIndex < bufferIndex;
  }
  bool canContinue(){
    return bufferIndex < charBuffer.size() && currentChar() != '\0';
  }
 private:
  unsigned int bufferIndex;
};

class PtsParticleReader : public ParticleReader{
 public:
  void readParticles(std::istream &file, ParticleContainer &particleContainer) override;
  std::string getReaderName() override;
};

#endif //RASPITOLIGHT_SRC_IO_IO_LIB_PTSPARTICLEREADER_H_
