#include <fstream>
#include <iostream>
#include "PtsParticleReader.h"
#include <string>
#include <sstream>
#include <memory>
#include <algorithm>
#include <spdlog/spdlog.h>
#include <ProgressReporter.h>

// remission: float
// RGB: rgb
// quality: float

void split(std::string const &original, std::vector<std::string> &results) {
    std::string::const_iterator start = original.begin();
    std::string::const_iterator end = original.end();
    std::string::const_iterator next = std::find(start, end, ' ');
    while (next != end) {
        auto x = std::string(start, next);
        if (!x.empty()) {
            results.push_back(x);
        }
        start = next + 1;
        next = std::find(start, end, ' ');
    }
    if (!std::string(start, next).empty()) {
        results.emplace_back(start, next);
    }

}

// we need to flip Y and Z, because they are the other way around in .pts files
const unsigned int PTS_X_INDEX = 0;
const unsigned int PTS_Y_INDEX = 2;
const unsigned int PTS_Z_INDEX = 1;

void PtsParticleReader::readParticles(std::istream &file, ParticleContainer &particleContainer) {

    if (file.fail()) {
        spdlog::error("File does not exist: {}");
        return;
    }

    // todo we can do that nicer
    std::string line;
    std::getline(file, line);
    const int particleCount = std::stoi(line);
    spdlog::info("Estimated particle count: {}", particleCount);
    std::getline(file, line);

    std::vector<std::string> vec;
    split(line, vec);

    const bool positionOnly = vec.size() == 3;
    const bool positionAndRgb = vec.size() == 6;
    const bool positionRemissionAndRgb = vec.size() == 7;
    const bool positionRemissionQualityAndRgb = vec.size() == 8;

    spdlog::info("positionOnly: {}", positionOnly );
    spdlog::info("positionAndRgb: {}", positionAndRgb );
    spdlog::info("positionRemissionAndRgb: {}", positionRemissionAndRgb );
    spdlog::info("positionRemissionQualityAndRgb: {}", positionRemissionQualityAndRgb );

    if (positionOnly) {
        spdlog::info("positionOnly");
        spdlog::info("resize positions");
        particleContainer.reservePositions(particleCount);
    } else if (positionAndRgb) {
        spdlog::info("positionAndRgb");
        spdlog::info("resize positions");
        particleContainer.reservePositions(particleCount);
        spdlog::info("resize colors");
        particleContainer.reserveColors(particleCount);
    } else if (positionRemissionAndRgb) {
        spdlog::info("positionRemissionAndRgb");
        spdlog::info("resize positions");
        particleContainer.reservePositions(particleCount);
        spdlog::info("resize colors");
        particleContainer.reserveColors(particleCount);
        spdlog::info("resize remissions");
        particleContainer.reserveRemissions(particleCount);
    } else if (positionRemissionQualityAndRgb) {
        spdlog::info("positionRemissionQualityAndRgb");
        spdlog::info("resize positions");
        particleContainer.reservePositions(particleCount);
        spdlog::info("resize colors");
        particleContainer.reserveColors(particleCount);
        spdlog::info("resize remissions");
        particleContainer.reserveRemissions(particleCount);
    }


    file.seekg(0, std::ios::beg);
    // skip first empty line
    std::getline(file, line);

    std::vector<std::string> vector;
    ProgressReporter progressReporter = ProgressReporter::createLoggingProgressReporter(particleCount);
    spdlog::info("Reading particles..");
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }
        else{
            if (positionOnly) {
                split(line, vector);
                particleContainer.addParticle(std::stof(vector[PTS_X_INDEX]), std::stof(vector[PTS_Y_INDEX]),
                                              std::stof(vector[PTS_Z_INDEX]));
            } else if (positionAndRgb) {
                split(line, vector);
                particleContainer.addParticle(std::stof(vector[PTS_X_INDEX]), std::stof(vector[PTS_Y_INDEX]),
                                              std::stof(vector[PTS_Z_INDEX]));
                particleContainer.addColor(std::stoi(vector[3]), std::stoi(vector[4]), std::stoi(vector[5]));
            } else if (positionRemissionAndRgb) {
                split(line, vector);
                particleContainer.addParticle(std::stof(vector[PTS_X_INDEX]), std::stof(vector[PTS_Y_INDEX]),
                                              std::stof(vector[PTS_Z_INDEX]));
                particleContainer.addRemission(std::stof(vector[3]));
                particleContainer.addColor(std::stoi(vector[4]), std::stoi(vector[5]), std::stoi(vector[6]));
            } else if (positionRemissionQualityAndRgb) {

            }
            vector.clear();
            progressReporter.iterationDone();
        }

    }
    spdlog::info("Reading done");
    spdlog::info("Particle count: {}", particleContainer.particleCount());

}

std::string PtsParticleReader::getReaderName() {
    return "pts";
}