//
// Created by Jan on 21.04.2019.
//

#include <fstream>
#include <ParticleReaderFactory.h>
#include "ClaudiusVisualizerDrawOverride.h"

ClaudiusVisualizerDrawOverride::ClaudiusVisualizerDrawOverride(const MObject &obj) : MPxDrawOverride(obj, nullptr) {

}
ClaudiusVisualizerDrawOverride::~ClaudiusVisualizerDrawOverride() {

}

MUserData *ClaudiusVisualizerDrawOverride::prepareForDraw(const MDagPath &objPath,
                                                      const MDagPath &cameraPath,
                                                      const MFrameContext &frameContext,
                                                      MUserData *oldData) {
  cout << "prepareForDraw" << std::endl;
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

  const std::string filepath = R"(M:\Projekte\2019\recap_test\StanfordBunny.pts)";
  //const std::string filepath = R"(M:\Projekte\2019\recap_test\test.pts)";
  std::ifstream filestream(filepath);

  auto particleReader = ParticleReaderFactory::createParticleReader(filepath);
  ParticleContainer particleContainer;
  particleReader->readParticles(filestream, particleContainer);

  const float *particleData = particleContainer.getParticleData();

  cout << particleContainer.particleCount() << std::endl;

  for(unsigned int i=0; i<particleContainer.particleCount(); i++){
    drawManager.point(MPoint(particleData[i], particleData[i+1], particleData[i+2]));
  }
  
  drawManager.endDrawable();

}
