//
// Created by Jan on 26.04.2019.
//

#ifndef CLAUDIUS_SRC_IO_IO_LIB_CHARBUFFEROPERATOR_H_
#define CLAUDIUS_SRC_IO_IO_LIB_CHARBUFFEROPERATOR_H_

#include <vector>

class CharBufferOperator{
  std::vector<char> &charBuffer;
 public:
  explicit CharBufferOperator(std::vector<char> &charBuffer) : charBuffer(charBuffer){
      bufferIndex = 0;
  }
  char currentChar();
  
  bool toFirstCharAfterNewLine();
  
  bool toFirstCharAfterSpace();
  
  bool readUntilSpace(std::string &bufferString);

  bool readUntilNewLine(std::string &bufferString);
  
  bool canContinue();
  
 private:
  unsigned int bufferIndex;

  bool readUntilFirstCharacterAfter(char untilThis);

  bool readUntil(std::string &bufferString,char untilThis);
};

#endif //CLAUDIUS_SRC_IO_IO_LIB_CHARBUFFEROPERATOR_H_
