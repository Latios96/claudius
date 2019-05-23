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
    return !colors.empty();
}
const int *ParticleContainer::getColorData() {
    return colors.data();
}
void ParticleContainer::addColor(int r, int g, int b) {
   colors.emplace_back(r);
   colors.emplace_back(g);
   colors.emplace_back(b);
}
const bool ParticleContainer::hasRemissionData() {
    return !remissions.empty();
}
const float *ParticleContainer::getRemissionsData() {
    return remissions.data();
}
void ParticleContainer::addRemission(float r) {
    remissions.emplace_back(r);
}
void ParticleContainer::reservePositions(unsigned int count) {
    particles.reserve(count * 3);
}
void ParticleContainer::reserveColors(unsigned int count) {
    colors.reserve(count * 3);
}
void ParticleContainer::reserveRemissions(unsigned int count) {
    remissions.reserve(count);
}
