//
// Created by Jan on 20.04.2019.
//
#include <catch.hpp>
#include <PtsParticleReader.h>

TEST_CASE("PtsParticleReader"){

  SECTION("shouldReadPositionsCorrectly"){
      const std::string data = "4105      \n"
                               "-0.06609 0.0446 0.00046\n"
                               "-0.07009 0.0406 0.00346\n"
                               "-0.06809 0.0436 0.00146\n"
                               "-0.06709 0.0446 0.00446\n";
      std::stringstream ss(data);
      PtsParticleReader ptsParticleReader;
      ParticleContainer particleContainer;

      ptsParticleReader.readParticles(ss, particleContainer);

      REQUIRE(particleContainer.particleCount() == 4);

  }
}