//
// Created by Jan on 27.04.2019.
//

#include "PointCloudFileTranslator.h"
#include "ClaudiusVisualizer.h"
#include <ParticleReaderFactory.h>
#include <maya/MFnDagNode.h>
#include <maya/MPlug.h>

void *PointCloudFileTranslator::creator() {
	return new PointCloudFileTranslator();
}


bool PointCloudFileTranslator::haveReadMethod() const {
    return true;
}
MStatus PointCloudFileTranslator::reader(const MFileObject &file,
                                         const MString &optionsString,
                                         MPxFileTranslator::FileAccessMode mode) {
    cout << "Reading file " << file.rawFullName() << std::endl;
    MFnDagNode visualizerDagNode;
    visualizerDagNode.create("ClaudiusVisualizer");

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
PointCloudFileTranslator::PointCloudFileTranslator() = default;
PointCloudFileTranslator::~PointCloudFileTranslator() = default;
