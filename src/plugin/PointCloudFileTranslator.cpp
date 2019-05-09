//
// Created by Jan on 27.04.2019.
//

#include "PointCloudFileTranslator.h"
#include "ClaudiusVisualizer.h"
#include <ParticleReaderFactory.h>
#include <maya/MFnDagNode.h>
#include <maya/MPlug.h>

PointCloudFileTranslator::PointCloudFileTranslator() = default;
PointCloudFileTranslator::~PointCloudFileTranslator() = default;

void *PointCloudFileTranslator::creator() {
	return new PointCloudFileTranslator();
}


bool PointCloudFileTranslator::haveReadMethod() const {
    return true;
}

MString _generateNodeName(MString fileNameMString){
    MString copy(fileNameMString);
    std::string fileName(copy.asChar());

    auto baseName = fileName.substr(fileName.find_last_of('/') + 1);
    auto baseNameWithoutExtension = baseName.substr(0,baseName.find_last_of('.'));

    return MString(baseNameWithoutExtension.c_str());
}

MStatus PointCloudFileTranslator::reader(const MFileObject &file,
                                         const MString &optionsString,
                                         MPxFileTranslator::FileAccessMode mode) {
    MFnDagNode visualizerDagNode;
    visualizerDagNode.create("ClaudiusVisualizer");

    nameVisualizerNodeCorrectly(file.rawFullName(), visualizerDagNode);

    MPlug filePathAttributePlug (visualizerDagNode.child(0), ClaudiusVisualizer::filePathAttribute);
    filePathAttributePlug.setValue(file.rawFullName());

    return MStatus::kSuccess;
}

PointCloudFileTranslator::MFileKind PointCloudFileTranslator::identifyFile(const MFileObject &file, const char *buffer, short size) const {
    auto particleReader = ParticleReaderFactory::createParticleReader(file.rawFullName().asChar());
    if(particleReader){
        return MFileKind::kIsMyFileType;
    }
    else{
        return MFileKind::kNotMyFileType ;
    }
}

void PointCloudFileTranslator::nameVisualizerNodeCorrectly(MString file, MFnDagNode &visualizerDagNode){
    const MString &string = generateNodeName(file);
    visualizerDagNode.setName(string);

    MFnDagNode shapeNode(visualizerDagNode.child(0));
    shapeNode.setName(visualizerDagNode.name() + "Shape");
}

MString PointCloudFileTranslator::generateNodeName(MString fileNameMString) {
    MString copy(fileNameMString);
    std::string fileName(copy.asChar());

    auto baseName = fileName.substr(fileName.find_last_of('/') + 1);
    auto baseNameWithoutExtension = baseName.substr(0,baseName.find_last_of('.'));

    return MString(baseNameWithoutExtension.c_str());
}

