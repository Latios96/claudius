//
// Created by Jan on 20.04.2019.
//
#include <catch.hpp>
#include <PtsParticleReader.h>
#include <iostream>
#include <CharBufferOperator.h>

TEST_CASE("PtsParticleReader"){

  SECTION("shouldReadPositionsCorrectly"){
      const std::string data = "4      \n"
                               "-1.0 0.0 1.0\n"
                               "2.0 3.0 4.0\n"
                               "5.0 6.0 7.0\n"
                               "8.0 9.0 10.0\n";
      std::stringstream ss(data);
      PtsParticleReader ptsParticleReader;
      ParticleContainer particleContainer;

      ptsParticleReader.readParticles(ss, particleContainer);

      REQUIRE(particleContainer.particleCount() == 4);

      for(int i=0;i<12;i++){
        std::cout << particleContainer.getParticleData()[i] << std::endl;
        REQUIRE(particleContainer.getParticleData()[i] == i-1);
      }
  }

    SECTION("shouldReadPositionsAndRgbCorrectly"){
        const std::string data = "4      \n"
                                 "-1.0 0.0 1.0 100 101 102\n"
                                 "2.0 3.0 4.0 103 104 105\n"
                                 "5.0 6.0 7.0 106 107 108\n"
                                 "8.0 9.0 10.0 109 110 111\n";
        std::stringstream ss(data);
        PtsParticleReader ptsParticleReader;
        ParticleContainer particleContainer;

        ptsParticleReader.readParticles(ss, particleContainer);

        REQUIRE(particleContainer.particleCount() == 4);
        REQUIRE(particleContainer.hasColorData());

        for(int i=0;i<12;i++){
            std::cout << particleContainer.getParticleData()[i] << std::endl;
            REQUIRE(particleContainer.getParticleData()[i] == i-1);
            REQUIRE(particleContainer.getColorData()[i] == 100+i);
        }
    }

    SECTION("shouldReadPositionsRemissionAndRgbCorrectly"){
        const std::string data = "4      \n"
                                 "-1.0 0.0 1.0 50 100 101 102\n"
                                 "2.0 3.0 4.0 51 103 104 105\n"
                                 "5.0 6.0 7.0 52 106 107 108\n"
                                 "8.0 9.0 10.0 53 109 110 111\n";
        std::stringstream ss(data);
        PtsParticleReader ptsParticleReader;
        ParticleContainer particleContainer;

        ptsParticleReader.readParticles(ss, particleContainer);

        REQUIRE(particleContainer.particleCount() == 4);

        REQUIRE(particleContainer.hasColorData());
        REQUIRE(particleContainer.hasRemissionData());

        for(int i=0;i<12;i++){
            std::cout << particleContainer.getParticleData()[i] << std::endl;
            REQUIRE(particleContainer.getParticleData()[i] == i-1);
            REQUIRE(particleContainer.getColorData()[i] == 100+i);
        }
        for(int i=0; i<4; i++){
            REQUIRE(particleContainer.getRemissionsData()[i] == 50+i);
        }
    }
}
