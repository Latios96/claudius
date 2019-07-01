//
// Created by Jan on 01.07.2019.
//

#include "RemainingTimeCalculator.h"
RemainingTimeCalculator::RemainingTimeCalculator(std::chrono::steady_clock::time_point startTime) : startTime{startTime}{


}

float RemainingTimeCalculator::getRemainingTimeByProgress(int progress) {
    const bool notStartedYes = progress == 0;
    const bool alreadyDone = progress == 100;
    if(notStartedYes){
        return -1;
    }
    else if (alreadyDone){
        return 0;
    }
    else{
        auto timeElapsed = std::chrono::steady_clock::now() - startTime;

        const long timeElapsedInSeconds = std::chrono::duration_cast<std::chrono::seconds>(timeElapsed).count();
        const float secondsPerPercent = static_cast<float>(timeElapsedInSeconds) / static_cast<float>(progress);
        return secondsPerPercent * (100 - progress);
    }
}