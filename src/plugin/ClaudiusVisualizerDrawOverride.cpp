//
// Created by Jan on 21.04.2019.
//

#include <fstream>
#include <ParticleReaderFactory.h>
#include "ClaudiusVisualizerDrawOverride.h"
#include <maya/MFnDependencyNode.h>

ClaudiusVisualizerDrawOverride::ClaudiusVisualizerDrawOverride(const MObject &obj) : MPxDrawOverride(obj, nullptr) {
  claudiusVisualizer = nullptr;
  MStatus status;
  MFnDependencyNode dnode(obj, &status);
  if (status)
    claudiusVisualizer = dynamic_cast<ClaudiusVisualizer*>(dnode.userNode());
}
ClaudiusVisualizerDrawOverride::~ClaudiusVisualizerDrawOverride() {

}

MUserData *ClaudiusVisualizerDrawOverride::prepareForDraw(const MDagPath &objPath,
                                                      const MDagPath &cameraPath,
                                                      const MFrameContext &frameContext,
                                                      MUserData *oldData) {
  auto *visualizerData = dynamic_cast<PartioVisualizerData*>(oldData);

  if(visualizerData == nullptr){
    auto *pData = new PartioVisualizerData();
    if(claudiusVisualizer != nullptr && claudiusVisualizer->particleContainer != nullptr){
      pData->particleContainer = claudiusVisualizer->particleContainer;
    }
    else{
      pData->particleContainer = nullptr;
    }
    return pData;
  }
  else{
    return visualizerData;
  }
  return new PartioVisualizerData();
}
MHWRender::DrawAPI ClaudiusVisualizerDrawOverride::supportedDrawAPIs() const {
#if MAYA_API_VERSION >= 201600
  return MHWRender::kOpenGL | MHWRender::kOpenGLCoreProfile;
#else
  return MHWRender::kOpenGL;
#endif
}
MPxDrawOverride *ClaudiusVisualizerDrawOverride::creator(const MObject &obj) {
  return new ClaudiusVisualizerDrawOverride(obj);
}
void ClaudiusVisualizerDrawOverride::addUIDrawables(const MDagPath &objPath,
                                                    MHWRender::MUIDrawManager &drawManager,
                                                    const MHWRender::MFrameContext &frameContext,
                                                    const MUserData *data) {
  drawManager.beginDrawable();

  // Draw a text "Foot"
  MPoint pos( 0.0, 0.0, 0.0 ); // Position of the text
  MColor textColor( 0.1f, 0.8f, 0.8f, 1.0f ); // Text color

  drawManager.setColor( textColor );
  drawManager.setFontSize( MHWRender::MUIDrawManager::kSmallFontSize );
  drawManager.text( pos,  MString("ClaudiusVisualizer"), MHWRender::MUIDrawManager::kCenter );

  auto *visualizerData = dynamic_cast<const PartioVisualizerData*>(data);

  if(visualizerData != nullptr){
    if(visualizerData->particleContainer != nullptr){
      const float* particleData = visualizerData->particleContainer->getParticleData();

      for(unsigned int i=0; i<visualizerData->particleContainer->particleCount() * 3; i = i+3){
        // cout << i << particleData[i] << " " << particleData[i+1] << " " << particleData[i+2] << std::endl;
        drawManager.point(MPoint(particleData[i], particleData[i+1], particleData[i+2]));
      }

    }
  }
  
  drawManager.endDrawable();

}
bool ClaudiusVisualizerDrawOverride::dataIsResuable(PartioVisualizerData *pData) {
  return pData != nullptr;
}
PartioVisualizerData *ClaudiusVisualizerDrawOverride::loadNewData() {

  //const std::string filepath = R"(M:\Projekte\2019\recap_test\StanfordBunny.pts)";
  const std::string filepath = R"(M:\Projekte\2019\recap_test\test.pts)";
  cout << "Opening streamd" << std::endl;
  std::ifstream filestream(filepath);

  auto particleReader = ParticleReaderFactory::createParticleReader(filepath);
  auto* particleContainer = new ParticleContainer();
  cout << "reading data" << std::endl;
  particleReader->readParticles(filestream, *particleContainer);
  cout << "data reading finished" << std::endl;

  cout << particleContainer->particleCount() << std::endl;
  auto *partioVisualizerData = new PartioVisualizerData();
  partioVisualizerData->particleContainer = particleContainer;
  return partioVisualizerData;
}
