//
// Created by Jan on 20.04.2019.
//

#include "version.h"
#ifndef GIT_COMMIT_HASH
#define GIT_COMMIT_HASH "0000000" // 0000000 means uninitialized
#endif

std::string getClaudiusVersion() {
  return "Claudius, Commit hash " + std::string(GIT_COMMIT_HASH);
}

