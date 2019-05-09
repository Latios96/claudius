//
// Created by Jan on 21.04.2019.
//

#include <fstream>
#include <ParticleReaderFactory.h>
#include "ClaudiusVisualizerDrawOverride.h"
#include <maya/MFnDependencyNode.h>
#include <maya/MHardwareRenderer.h>
#include <maya/MGLFunctionTable.h>


ClaudiusVisualizerDrawOverride::ClaudiusVisualizerDrawOverride(const MObject &obj) : MPxDrawOverride(obj, &drawCallback) {
  claudiusVisualizer = nullptr;
  MStatus status;
  MFnDependencyNode dnode(obj, &status);
  if (status){
      claudiusVisualizer = dynamic_cast<ClaudiusVisualizer*>(dnode.userNode());
  }

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
      if(claudiusVisualizer != nullptr && claudiusVisualizer->particleContainer != nullptr){
          visualizerData->particleContainer = claudiusVisualizer->particleContainer;
      }
      return visualizerData;
  }
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
  /*drawManager.beginDrawable();

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
  
  drawManager.endDrawable();*/

}
void ClaudiusVisualizerDrawOverride::drawCallback(const MDrawContext& context, const MUserData* data) {
    MHardwareRenderer *rend = MHardwareRenderer::theRenderer();
    MGLFunctionTable *gGLFT;
    if (rend){

        gGLFT = rend->glFunctionTable();

        auto *visualizerData = dynamic_cast<const PartioVisualizerData*>(data);

        if(visualizerData != nullptr){
            if(visualizerData->particleContainer != nullptr){
                const float* particleData = visualizerData->particleContainer->getParticleData();

                gGLFT->glPointSize(10.0f);
                gGLFT->glColor3f( 0.0f, 0.0f, 1.0f );
                gGLFT->glBegin(MGL_POINTS);
                for(unsigned int i=0; i<visualizerData->particleContainer->particleCount() * 3; i = i+3){
                    // cout << i << particleData[i] << " " << particleData[i+1] << " " << particleData[i+2] << std::endl;
                    //drawManager.point(MPoint(particleData[i], particleData[i+1], particleData[i+2]));
                    gGLFT->glVertex3f( particleData[i], particleData[i+1], particleData[i+2] );
                }
                gGLFT->glEnd();
            }
        }
    }
}


