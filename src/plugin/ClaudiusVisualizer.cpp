//
// Created by Jan on 21.04.2019.
//

#include "ClaudiusVisualizer.h"

MString ClaudiusVisualizer::drawDbClassification("drawdb/geometry/claudiusVisualizer");
MTypeId ClaudiusVisualizer::id( 0x80007 );

ClaudiusVisualizer::ClaudiusVisualizer() = default;

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
