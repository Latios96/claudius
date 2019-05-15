//
// Created by Jan on 21.04.2019.
//

#ifndef CLAUDIUS_SRC_PLUGIN_CLAUDIUSVISUALIZER_H_
#define CLAUDIUS_SRC_PLUGIN_CLAUDIUSVISUALIZER_H_

#include <maya/MPxLocatorNode.h>
#include <maya/MObject.h>
#include <ParticleContainer.h>
#include <maya/MNodeMessage.h>

class ClaudiusVisualizer:  public MPxLocatorNode{
 public:
    ClaudiusVisualizer();
    virtual ~ClaudiusVisualizer();

    static void* creator();
    static MStatus initialize();

    virtual void postConstructor();
    virtual MStatus compute( const MPlug& plug, MDataBlock& data );

    virtual void draw( M3dView & view, const MDagPath & path, M3dView::DisplayStyle style, M3dView::DisplayStatus status);
    virtual MBoundingBox boundingBox() const;
    virtual bool isBounded() const;
    static MTypeId id;
    static MString drawDbClassification;
    static MObject filePathAttribute;
    static MObject renderWithColorAttribute;
    ParticleContainer *particleContainer;
 protected:
  MCallbackId m_attrNameCallbackId;
  static void attributeChangedCallback(MNodeMessage::AttributeMessage msg, MPlug & plug, MPlug & otherPlug, void* clientData);
  void readParticles();
};

#endif //CLAUDIUS_SRC_PLUGIN_CLAUDIUSVISUALIZER_H_
