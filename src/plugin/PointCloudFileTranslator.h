//
// Created by Jan on 27.04.2019.
//

#ifndef CLAUDIUS_SRC_PLUGIN_POINTCLOUDFILETRANSLATOR_H_
#define CLAUDIUS_SRC_PLUGIN_POINTCLOUDFILETRANSLATOR_H_


#include <maya/MPxFileTranslator.h>
#include <maya/MFStream.h>
// canbeopeneed -> false for importers
// haveReadMethod() and reader()
class PointCloudFileTranslator : public MPxFileTranslator {
 public:
  PointCloudFileTranslator ();
  virtual ~PointCloudFileTranslator ();

  virtual bool haveReadMethod () const;

  virtual MStatus reader (const MFileObject &file,
      const MString &optionsString,
      MPxFileTranslator::FileAccessMode mode);

  virtual MFileKind identifyFile (
      const MFileObject &,
      const char *buffer,
      short size
  ) const; // check if file can be translated by this translator

  static void *creator ();

 private:
  void nameVisualizerNodeCorrectly(MString file, MFnDagNode &visualizerDagNode);
  MString generateNodeName(MString fileNameMString);
};

#endif //CLAUDIUS_SRC_PLUGIN_POINTCLOUDFILETRANSLATOR_H_
