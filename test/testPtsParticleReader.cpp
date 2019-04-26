//
// Created by Jan on 20.04.2019.
//
#include <catch.hpp>
#include <PtsParticleReader.h>
#include <iostream>
#include <CharBufferOperator.h>

TEST_CASE("PtsParticleReader"){

  SECTION("shouldReadPositionsCorrectly"){
      const std::string data = "4105      \n"
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
}
