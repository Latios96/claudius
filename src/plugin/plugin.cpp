#include <maya/MFnPlugin.h>
#include "ClaudiusVisualizer.h"

MStatus initializePlugin(MObject obj)
{
  MStatus status;
  // TODO get version from cmake
  MFnPlugin plugin(obj, "Jan Honsbrok", "1.0", "Any");

  status = plugin.registerNode("claudiusVisualizer", ClaudiusVisualizer::id,
                               &ClaudiusVisualizer::creator, &ClaudiusVisualizer::initialize,
                               MPxNode::kLocatorNode,
                               &ClaudiusVisualizer::drawDbClassification);
  if (!status)
  {
    status.perror("register PartioVisualizer");
    return status;
  }

  /*status = MHWRender::MDrawRegistry::registerDrawOverrideCreator(
      PartioVisualizer::drawDbClassification,
      "PartioVisualizerDrawOverride",
      PartioVisualizerDrawOverride::creator);
  if (!status)
  {
    status.perror("register PartioVisualizerDrawOverride");
    return status;
  }*/

  return status;
}


MStatus uninitializePlugin(MObject obj)
{
  MStatus status;
  MFnPlugin plugin(obj);

  status = plugin.deregisterNode(ClaudiusVisualizer::id);
  if (!status)
  {
    status.perror("deregister PartioVisualizer");
    return status;
  }

  /*status = MHWRender::MDrawRegistry::deregisterDrawOverrideCreator(
      PartioVisualizer::drawDbClassification,
      "PartioVisualizerDrawOverride");
  if (!status)
  {
    status.perror("deregister PartioVisualizerDrawOverride");
    return status;
  }*/

  return status;
}