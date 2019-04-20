//
// Created by Jan on 20.04.2019.
//

#include <catch.hpp>
#include <ParticleContainer.h>

TEST_CASE("ParticleContainer") {
  ParticleContainer particleContainer;

  SECTION("containerShouldBeEmpty"){
    REQUIRE(particleContainer.isEmpty());
  }
  
  SECTION("particleCountShouldBeIncreasedByOne"){
    unsigned int oldParticleCount = particleContainer.particleCount();
    particleContainer.addParticle(1,2,3);
    REQUIRE(particleContainer.particleCount() == oldParticleCount +1);
  }
}