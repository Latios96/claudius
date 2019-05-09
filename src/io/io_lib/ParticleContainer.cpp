//
// Created by Jan on 20.04.2019.
//

#include "ParticleContainer.h"
const unsigned int ParticleContainer::particleCount() {
  return particles.size() / 3;
}
void ParticleContainer::addParticle(float x, float y, float z) {
  particles.emplace_back(x);
  particles.emplace_back(y);
  particles.emplace_back(z);
}
const bool ParticleContainer::isEmpty() {
  return particleCount() == 0;
}
const float *ParticleContainer::getParticleData() {
  return particles.data();
}
const bool ParticleContainer::hasColorData() {
    return !particles.empty();
}
const int *ParticleContainer::getColorData() {
    return colors.data();
}
void ParticleContainer::addColor(int r, int g, int b) {
   colors.emplace_back(r);
   colors.emplace_back(g);
   colors.emplace_back(b);
}
