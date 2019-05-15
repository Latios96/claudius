//
// Created by Jan on 21.04.2019.
//
#include "ClaudiusVisualizer.h"
#include <fstream>
#include <ParticleReaderFactory.h>
#include <maya/MFnStringData.h>
#include <maya/MFnNumericData.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MEventMessage.h>
#include <maya/MGlobal.h>

MString ClaudiusVisualizer::drawDbClassification("drawdb/geometry/claudiusVisualizer");
MTypeId ClaudiusVisualizer::id(0x80007);
MObject ClaudiusVisualizer::filePathAttribute;
MObject ClaudiusVisualizer::renderWithColorAttribute;

ClaudiusVisualizer::ClaudiusVisualizer(){
  particleContainer = nullptr;
}

ClaudiusVisualizer::~ClaudiusVisualizer() {
    delete particleContainer;
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

    MFnNumericAttribute typedBoolAttribute;
    renderWithColorAttribute = typedBoolAttribute.create("renderWithColor", "withColor", MFnNumericData::kBoolean);
    typedBoolAttribute.setReadable(true);
    typedBoolAttribute.setWritable(true);
    typedBoolAttribute.setKeyable(false);
    typedBoolAttribute.setConnectable(true);
    typedBoolAttribute.setStorable(true);
    addAttribute(renderWithColorAttribute);

    return MStatus::kSuccess;
}

void ClaudiusVisualizer::postConstructor() {
    MStatus stat;
    MObject node = thisMObject();
    m_attrNameCallbackId = MNodeMessage::addAttributeChangedCallback(node, attributeChangedCallback, this, &stat);
    if (stat != MStatus::kSuccess){
        std::cerr << "Failed add event callback\n";
    }

}

MStatus ClaudiusVisualizer::compute(const MPlug &plug, MDataBlock &data) {
    if(particleContainer == nullptr){
        readParticles();
    }
    return MStatus::kSuccess;
}

bool fileExists(const char *filename) {
    std::ifstream ifile(filename);
    return (bool)ifile;
}

void ClaudiusVisualizer::readParticles() {
    //const std::string filepath = R"(M:\Projekte\2019\recap_test\Aero_Scan.pts)";
    //const std::string filepath = R"(M:\Projekte\2019\recap_test\test.pts)";
    //const std::string filepath = R"(M:\Projekte\2019\recap_test\StanfordBunny.pts)";

    MString particleFile = MPlug(thisMObject(), ClaudiusVisualizer::filePathAttribute).asString();

    const char *filepath = particleFile.asChar();

    if(fileExists(filepath)){
        const bool hasOldParticles = particleContainer != nullptr;
        if (hasOldParticles){
            ParticleContainer *oldParticleContainer = particleContainer;
            particleContainer = nullptr;
            delete oldParticleContainer;
        }
        std::ifstream filestream(filepath);

        auto particleReader = ParticleReaderFactory::createParticleReader(filepath);
        particleContainer = new ParticleContainer();
        particleReader->readParticles(filestream, *particleContainer);
    }
}

void ClaudiusVisualizer::draw(M3dView &view,
                              const MDagPath &path,
                              M3dView::DisplayStyle style,
                              M3dView::DisplayStatus status) {
}

MBoundingBox ClaudiusVisualizer::boundingBox() const {
    return MBoundingBox();
}

bool ClaudiusVisualizer::isBounded() const {
    return true;
}

void ClaudiusVisualizer::attributeChangedCallback(MNodeMessage::AttributeMessage msg,
                                                  MPlug &plug,
                                                  MPlug &otherPlug,
                                                  void *clientData) {
    const bool attributeWasSet = msg & MNodeMessage::kAttributeSet;
    const bool filePathAttributeWasSet =  attributeWasSet & plug == ClaudiusVisualizer::filePathAttribute;

    if(filePathAttributeWasSet) {
        auto *claudiusVisualizer = static_cast<ClaudiusVisualizer *>(clientData);
        MString particleFile = MPlug(claudiusVisualizer->thisMObject(), ClaudiusVisualizer::filePathAttribute).asString();

        cout << particleFile << std::endl;

        claudiusVisualizer->readParticles();
    }
}

