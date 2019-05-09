//
// Created by Jan on 21.04.2019.
//

#ifndef CLAUDIUS_SRC_PLUGIN_CLAUDIUSVISUALIZERDRAWOVERRIDE_H_
#define CLAUDIUS_SRC_PLUGIN_CLAUDIUSVISUALIZERDRAWOVERRIDE_H_
#include <maya/MPxDrawOverride.h>
#include "ParticleContainer.h"
#include "ClaudiusVisualizer.h"
// todo move to own files
class PartioVisualizerData : public MUserData
{
 public:
  PartioVisualizerData() : MUserData(false) {
	  // don't delete after draw
	  particleContainer = nullptr;
  } 
  ParticleContainer *particleContainer;
  MMatrix matrix;
  ~PartioVisualizerData() override {};
};

class ClaudiusVisualizerDrawOverride : public MPxDrawOverride
{
 public:
  ClaudiusVisualizerDrawOverride(const MObject& obj);
  virtual ~ClaudiusVisualizerDrawOverride();

  static MPxDrawOverride* creator(const MObject& obj);

 public:
  static MTypeId id;

  static void drawCallback(const MDrawContext& context, const MUserData* data);

 protected:
  virtual MUserData* prepareForDraw(
      const MDagPath& objPath,
      const MDagPath& cameraPath,
      const MFrameContext& frameContext,
      MUserData* oldData);
  void addUIDrawables(
      const MDagPath& objPath,
      MHWRender::MUIDrawManager& drawManager,
      const MHWRender::MFrameContext& frameContext,
      const MUserData* data) override;

  bool hasUIDrawables() const override { return true; }

  MHWRender::DrawAPI supportedDrawAPIs() const override;

  ClaudiusVisualizer* claudiusVisualizer;
};

#endif //CLAUDIUS_SRC_PLUGIN_CLAUDIUSVISUALIZERDRAWOVERRIDE_H_
