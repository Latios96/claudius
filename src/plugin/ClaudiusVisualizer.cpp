//
// Created by Jan on 21.04.2019.
//

#include <fstream>
#include <ParticleReaderFactory.h>
#include "ClaudiusVisualizer.h"

MString ClaudiusVisualizer::drawDbClassification("drawdb/geometry/claudiusVisualizer");
MTypeId ClaudiusVisualizer::id( 0x80007 );

ClaudiusVisualizer::ClaudiusVisualizer(){
  particleContainer = nullptr;
}

void *ClaudiusVisualizer::creator() {
  cout << "creator" << std::endl;
  return new ClaudiusVisualizer();
}

MStatus ClaudiusVisualizer::initialize() {
  cout << "initialize" << std::endl;
  return MStatus::kSuccess;
}

void ClaudiusVisualizer::postConstructor() {
  cout << "postConstructor" << std::endl;
}

MStatus ClaudiusVisualizer::compute(const MPlug &plug, MDataBlock &data) {
  cout << "Compute" << std::endl;
  if(particleContainer == nullptr){
    const std::string filepath = R"(M:\Projekte\2019\recap_test\test.pts)";
    cout << "Opening stream" << std::endl;
    std::ifstream filestream(filepath);

    auto particleReader = ParticleReaderFactory::createParticleReader(filepath);
    particleContainer = new ParticleContainer();
    cout << "reading data" << std::endl;
    particleReader->readParticles(filestream, *particleContainer);
    cout << "data reading finished" << std::endl;

    cout << particleContainer->particleCount() << std::endl;
  }
  else{
    cout << "already computed" << std::endl;
  }
  return MStatus::kSuccess;
}

void ClaudiusVisualizer::draw(M3dView &view,
                              const MDagPath &path,
                              M3dView::DisplayStyle style,
                              M3dView::DisplayStatus status) {
  cout << "draw" << std::endl;
}

MBoundingBox ClaudiusVisualizer::boundingBox() const {
  cout << "boundingBox" << std::endl;
  return MBoundingBox();
}

bool ClaudiusVisualizer::isBounded() const {
  return true;
}
