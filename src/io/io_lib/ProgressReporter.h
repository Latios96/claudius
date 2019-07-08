#include <utility>

//
// Created by Jan Honsbrok on 11.11.18.
//

#ifndef CRAYG_PROGRESSREPORTER_H
#define CRAYG_PROGRESSREPORTER_H

#include <functional>
#include <iostream>
#include "fmt/format.h"
#include "RemainingTimeCalculator.h"

// todo move into .cpp file
class ProgressReporter {
 public:
  ProgressReporter(int maxIterations, std::function<void(int, float)> progressionCallback) :
      maxIterations(maxIterations),
      progressionCallback(std::move(progressionCallback)),
      iterationsDone(0),
      progress(0),
      remainingTimeCalculator(std::chrono::steady_clock::now()) {
      startTime = std::chrono::steady_clock::now();
  }

  static ProgressReporter createLoggingProgressReporter(int maxIterations, std::string logMessage) {
      std::function<void(int, float)> logProgress = [logMessage](int progress, float timeRemaining) -> void {
        std::cout << (logMessage.c_str(), progress, timeRemaining) << std::endl;
      };
      return {maxIterations, logProgress};
  }

  static ProgressReporter createLoggingProgressReporter(int maxIterations) {
      return ProgressReporter::createLoggingProgressReporter(maxIterations,
                                                             "(Estimated time remaining: {1}s): {0}% complete.");
  }

  void iterationDone() {

      iterationsDone++;

      int newProgress = (int) ((float) iterationsDone) / ((float) maxIterations) * 100;
      if (newProgress > progress) {
          progress = newProgress;
          if (progress % 10 == 0) {
              const float remainingTime = remainingTimeCalculator.getRemainingTimeByProgress(progress);
              progressionCallback(progress, remainingTime);
          }
      }
  }

  void finish() {
      std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
      auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - startTime).count();

      std::cout << fmt::format("Rendering took {} seconds.", microseconds * 0.0000006) << std::endl;
  }

  int iterationsDone;
 private:
  int maxIterations;

  int progress;
  std::function<void(int, float)> progressionCallback;
  std::chrono::steady_clock::time_point startTime;
  RemainingTimeCalculator remainingTimeCalculator;
};

#endif //CRAYG_PROGRESSREPORTER_H