//
// Created by Jan on 01.07.2019.
//

#ifndef CLAUDIUS_SRC_IO_IO_LIB_REMAININGTIMECALCULATOR_H_
#define CLAUDIUS_SRC_IO_IO_LIB_REMAININGTIMECALCULATOR_H_

#include <chrono>

class RemainingTimeCalculator {

 public:
  explicit RemainingTimeCalculator(std::chrono::steady_clock::time_point startTime);

  /**
   * Returns the remaining time for the given progress
   * @param progress progress in percent, for example 5
   * @return the remaining time in seconds
   */
  float getRemainingTimeByProgress(int progress);

 private:
  std::chrono::steady_clock::time_point startTime;
};

#endif //CLAUDIUS_SRC_IO_IO_LIB_REMAININGTIMECALCULATOR_H_
