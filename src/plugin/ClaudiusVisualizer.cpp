//
// Created by Jan on 21.04.2019.
//
#include "ClaudiusVisualizer.h"
#include <fstream>
#include <ParticleReaderFactory.h>
#include <maya/MFnStringData.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MEventMessage.h>

MString ClaudiusVisualizer::drawDbClassification("drawdb/geometry/claudiusVisualizer");
MTypeId ClaudiusVisualizer::id(0x80007);
MObject ClaudiusVisualizer::filePathAttribute;

ClaudiusVisualizer::ClaudiusVisualizer(){
  particleContainer = nullptr;
}

void *ClaudiusVisualizer::creator() {
  cout << "creator" << std::endl;
  return new ClaudiusVisualizer();
}

MStatus ClaudiusVisualizer::initialize() {
    cout << "initialize" << std::endl;

    MStatus status;

    MFnStringData fnStringData;
    MObject defaultString;

    // Create the default string.
    defaultString = fnStringData.create("");

    MFnTypedAttribute typedStringAttribute;
    filePathAttribute = typedStringAttribute.create("particleFile", "pFile", MFnStringData::kString, defaultString);
    typedStringAttribute.setReadable(true);
    typedStringAttribute.setWritable(true);
    typedStringAttribute.setKeyable(false);
    typedStringAttribute.setConnectable(true);
    typedStringAttribute.setStorable(true);
    addAttribute(filePathAttribute);

    return MStatus::kSuccess;
}

void ClaudiusVisualizer::postConstructor() {
    cout << "postConstructor" << std::endl;

}

MStatus ClaudiusVisualizer::compute(const MPlug &plug, MDataBlock &data) {
    cout << "Compute" << std::endl;

    if(particleContainer == nullptr){
        //const std::string filepath = R"(M:\Projekte\2019\recap_test\Aero_Scan.pts)";
        //const std::string filepath = R"(M:\Projekte\2019\recap_test\test.pts)";
        const std::string filepath = R"(M:\Projekte\2019\recap_test\StanfordBunny.pts)";

        cout << "Opening streame" << std::endl;
        std::ifstream filestream(filepath);

        auto particleReader = ParticleReaderFactory::createParticleReader(filepath);
        particleContainer = new ParticleContainer();
        cout << "reading data" << std::endl;
        particleReader->readParticles(filestream, *particleContainer);
        cout << "data reading finished" << std::endl;

        cout << particleContainer->particleCount() << std::endl;
    }
    else{
        cout << "already computed with " << particleContainer->particleCount() << " particles" << std::endl;
    }
    return MStatus::kSuccess;
}

void ClaudiusVisualizer::draw(M3dView &view,
                              const MDagPath &path,
                              M3dView::DisplayStyle style,
                              M3dView::DisplayStatus status) {
    cout << "draw" << std::endl;
}

MBoundingBox ClaudiusVisualizer::boundingBox() const {
    cout << "boundingBox" << std::endl;
    return MBoundingBox();
}

bool ClaudiusVisualizer::isBounded() const {
    return true;
}
