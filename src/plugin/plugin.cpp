#include <maya/MFnPlugin.h>
#include "ClaudiusVisualizer.h"
#include "ClaudiusVisualizerDrawOverride.h"
#include <maya/MDrawRegistry.h>
#include <version.h>

MStatus initializePlugin(MObject obj)
{
  MStatus status;
  // TODO get version from cmake
  MFnPlugin plugin(obj, "Jan Honsbrok", getClaudiusVersion().c_str(), "Any");

  status = plugin.registerNode("ClaudiusVisualizer", ClaudiusVisualizer::id,
                               &ClaudiusVisualizer::creator, &ClaudiusVisualizer::initialize,
                               MPxNode::kLocatorNode,
                               &ClaudiusVisualizer::drawDbClassification);
  if (!status)
  {
    status.perror("register PartioVisualizer");
    return status;
  }

  status = MHWRender::MDrawRegistry::registerDrawOverrideCreator(
      ClaudiusVisualizer::drawDbClassification,
      "ClaudiusVisualizerDrawOverride",
      ClaudiusVisualizerDrawOverride::creator);
  if (!status)
  {
    status.perror("register ClaudiusVisualizerDrawOverride");
    return status;
  }

  return status;
}


MStatus uninitializePlugin(MObject obj)
{
  MStatus status;
  MFnPlugin plugin(obj);

  status = plugin.deregisterNode(ClaudiusVisualizer::id);
  if (!status)
  {
    status.perror("deregister ClaudiusVisualizer");
    return status;
  }

  status = MHWRender::MDrawRegistry::deregisterDrawOverrideCreator(
      ClaudiusVisualizer::drawDbClassification,
      "ClaudiusVisualizerDrawOverride");
  if (!status)
  {
    status.perror("deregister ClaudiusVisualizerDrawOverride");
    return status;
  }

  return status;
}