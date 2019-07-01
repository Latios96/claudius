//
// Created by Jan on 21.04.2019.
//

#ifndef CLAUDIUS_SRC_PLUGIN_CLAUDIUSVISUALIZERDRAWOVERRIDE_H_
#define CLAUDIUS_SRC_PLUGIN_CLAUDIUSVISUALIZERDRAWOVERRIDE_H_
#include <maya/MPxDrawOverride.h>
#include "ParticleContainer.h"
#include "ClaudiusVisualizer.h"
#include <fstream>
#include <ParticleReaderFactory.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MHardwareRenderer.h>
#include <maya/MGLFunctionTable.h>
#include <maya/MFnDagNode.h>
#include <maya/MFnTransform.h>

struct DisplayOptions {
  DisplayOptions();
  DisplayOptions(const MString &particleFilePath, bool displayColor, int displayPercentage, MMatrix matrix);
  MString particleFilePath;
  bool displayColor = true;
  int renderEveryNth = 100;
  MMatrix matrix;
  bool operator==(const DisplayOptions &rhs) const;
  bool operator!=(const DisplayOptions &rhs) const;
};

// todo move to own files
class PartioVisualizerData : public MUserData {
 public:
  PartioVisualizerData() : MUserData(false) {
      // don't delete after draw
      particleContainer = nullptr;
  }
  ParticleContainer *particleContainer;
  MMatrix matrix;
  DisplayOptions displayOptions;

  MGLuint currentDisplayList = -1;
  ~PartioVisualizerData() override {};
};
class ClaudiusVisualizerDrawOverride : public MPxDrawOverride {
 public:
  ClaudiusVisualizerDrawOverride(const MObject &obj);
  virtual ~ClaudiusVisualizerDrawOverride();

  static MPxDrawOverride *creator(const MObject &obj);

 public:
  static MTypeId id;

  static void drawCallback(const MDrawContext &context, const MUserData *data);

 protected:
  virtual MUserData *prepareForDraw(
      const MDagPath &objPath,
      const MDagPath &cameraPath,
      const MFrameContext &frameContext,
      MUserData *oldData);
  void addUIDrawables(
      const MDagPath &objPath,
      MHWRender::MUIDrawManager &drawManager,
      const MHWRender::MFrameContext &frameContext,
      const MUserData *data) override;

  bool hasUIDrawables() const override { return true; }

  MHWRender::DrawAPI supportedDrawAPIs() const override;

  ClaudiusVisualizer *claudiusVisualizer;
  void generateDisplayList(PartioVisualizerData *visualizerData, DisplayOptions &displayOptions);
  DisplayOptions createDisplayOptions(MMatrix matrix);
};

#endif //CLAUDIUS_SRC_PLUGIN_CLAUDIUSVISUALIZERDRAWOVERRIDE_H_
