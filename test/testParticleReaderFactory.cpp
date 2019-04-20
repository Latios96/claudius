//
// Created by Jan on 20.04.2019.
//
#include <catch.hpp>
#include <ParticleReaderFactory.h>

TEST_CASE("ParticleReaderFactory"){
  SECTION("shouldReturnReaderWithNamePts"){
    const std::shared_ptr<ParticleReader> &particleReader = ParticleReaderFactory::createParticleReader("test.pts");
    REQUIRE(particleReader->getReaderName() == "pts");
  }
}