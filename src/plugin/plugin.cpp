#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>
#include "ClaudiusVisualizer.h"
#include "ClaudiusVisualizerDrawOverride.h"
#include "PointCloudFileTranslator.h"
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

    // Run MEL script for user interface initialization.
    if (MGlobal::mayaState() == MGlobal::kInteractive)
    {
        MString sCmd = R"(python "from claudius import menu;menu.create_menu();")";
        MGlobal::executeCommand( sCmd );
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

    bool requiresFullMel = false;
    status = plugin.registerFileTranslator("ClaudiusPointCloudFileTranslator",
                                           nullptr, //the pathname of the icon used in file selection dialogs
                                           PointCloudFileTranslator::creator,
                                           nullptr, //the name of a MEL script that will be used to display the contents of the options dialog during file open and save.
                                           nullptr, //the default value of the options string that will be passed to the options script.
                                           requiresFullMel); //this should be set to true if the reader method in the derived class intends to issue MEL commands via the MGlobal::executeCommand method.
    if (!status)
    {
        status.perror("registerFileTranslator");
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

    status = plugin.deregisterFileTranslator("ClaudiusPointCloudFileTranslator");
    if (!status)
    {
        status.perror("deregisterFileTranslator");
    }

    if (MGlobal::mayaState() == MGlobal::kInteractive)
    {
        MString sCmd = R"(python "from claudius import menu;menu.remove_menu();")";
        MGlobal::executeCommand( sCmd );
    }

  return status;
}