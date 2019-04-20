//
// Created by Jan on 20.04.2019.
//

#include "ParticleContainer.h"
unsigned int ParticleContainer::particleCount() {
  return particles.size() / 3;
}
void ParticleContainer::addParticle(float x, float y, float z) {
  particles.emplace_back(x);
  particles.emplace_back(y);
  particles.emplace_back(z);
}
bool ParticleContainer::isEmpty() {
  return particleCount() == 0;
}
