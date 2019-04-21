//
// Created by Jan on 21.04.2019.
//

#ifndef CLAUDIUS_SRC_PLUGIN_CLAUDIUSVISUALIZERDRAWOVERRIDE_H_
#define CLAUDIUS_SRC_PLUGIN_CLAUDIUSVISUALIZERDRAWOVERRIDE_H_
#include <maya/MPxDrawOverride.h>

class PartioVisualizerData : public MUserData
{
 public:
  PartioVisualizerData() : MUserData(false) {} // don't delete after draw
  ~PartioVisualizerData() override {}
};

class ClaudiusVisualizerDrawOverride : public MPxDrawOverride
{
 public:
  ClaudiusVisualizerDrawOverride(const MObject& obj);
  virtual ~ClaudiusVisualizerDrawOverride();

  static MPxDrawOverride* creator(const MObject& obj);

 public:
  static MTypeId id;

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

};

#endif //CLAUDIUS_SRC_PLUGIN_CLAUDIUSVISUALIZERDRAWOVERRIDE_H_
