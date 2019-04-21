/*#include <maya/MSimple.h>
#include <maya/MIOStream.h>
#include <maya/MFnPlugin.h>
DeclareSimpleCommand( helloWorld, "Jan Honsbrok", "2018");


MStatus initializePlugin( MObject obj )
{
  MFnPlugin plugin( obj,"Autodesk","2.0", "Any");
  plugin.
  return plugin.registerShape( "yourShape", yourShape::id,
                               &yourShape::creator,
                               &yourShape::initialize,
                               &yourShapeUI::creator );
}

MStatus helloWorld::doIt( const MArgList& )
{
  cout << "Hello World\n" << endl;
  return MS::kSuccess;*/
//-
// ==========================================================================
// Copyright 2015 Autodesk, Inc.  All rights reserved.
// Use of this software is subject to the terms of the Autodesk license agreement
// provided at the time of installation or download, or which otherwise
// accompanies this software in either electronic or hard copy form.
// ==========================================================================
//+

////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//
//
// This plug-in demonstrates how to draw a simple mesh like foot Print in an easy way.
//
// This easy way is supported in Viewport 2.0.
// In Viewport 2.0, MUIDrawManager can used to draw simple UI elements in method addUIDrawables().
//
// For comparison, you can reference a Maya Developer Kit sample named rawfootPrintNode.
// In that sample, we draw the footPrint with OpenGL\DX in method rawFootPrintDrawOverride::draw().
//
// Note the method
//   footPrint::draw()
// is only called in legacy default viewport to draw foot Print.
// while the methods
//   FootPrintDrawOverride::prepareForDraw()
//   FootPrintDrawOverride::addUIDrawables()
// are only called in Viewport 2.0 to prepare and draw foot Print.
//
////////////////////////////////////////////////////////////////////////

#include <maya/MPxLocatorNode.h>
#include <maya/MString.h>
#include <maya/MTypeId.h>
#include <maya/MPlug.h>
#include <maya/MVector.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MColor.h>
#include <maya/M3dView.h>
#include <maya/MFnPlugin.h>
#include <maya/MDistance.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MArrayDataBuilder.h>
#include <maya/MEvaluationNode.h>

// Viewport 2.0 includes
#include <maya/MDrawRegistry.h>
#include <maya/MPxDrawOverride.h>
#include <maya/MUserData.h>
#include <maya/MDrawContext.h>
#include <maya/MHWGeometryUtilities.h>
#include <maya/MPointArray.h>
#include <maya/MGlobal.h>
#include <maya/MEventMessage.h>
#include <maya/MFnDependencyNode.h>

#include <assert.h>

static bool sUseLegacyDraw = (getenv("MAYA_ENABLE_VP2_PLUGIN_LOCATOR_LEGACY_DRAW") != NULL);

// Foot Data
//
static float sole[][3] = { {  0.00f, 0.0f, -0.70f },
                           {  0.04f, 0.0f, -0.69f },
                           {  0.09f, 0.0f, -0.65f },
                           {  0.13f, 0.0f, -0.61f },
                           {  0.16f, 0.0f, -0.54f },
                           {  0.17f, 0.0f, -0.46f },
                           {  0.17f, 0.0f, -0.35f },
                           {  0.16f, 0.0f, -0.25f },
                           {  0.15f, 0.0f, -0.14f },
                           {  0.13f, 0.0f,  0.00f },
                           {  0.00f, 0.0f,  0.00f },
                           { -0.13f, 0.0f,  0.00f },
                           { -0.15f, 0.0f, -0.14f },
                           { -0.16f, 0.0f, -0.25f },
                           { -0.17f, 0.0f, -0.35f },
                           { -0.17f, 0.0f, -0.46f },
                           { -0.16f, 0.0f, -0.54f },
                           { -0.13f, 0.0f, -0.61f },
                           { -0.09f, 0.0f, -0.65f },
                           { -0.04f, 0.0f, -0.69f },
                           { -0.00f, 0.0f, -0.70f } };
static float heel[][3] = { {  0.00f, 0.0f,  0.06f },
                           {  0.13f, 0.0f,  0.06f },
                           {  0.14f, 0.0f,  0.15f },
                           {  0.14f, 0.0f,  0.21f },
                           {  0.13f, 0.0f,  0.25f },
                           {  0.11f, 0.0f,  0.28f },
                           {  0.09f, 0.0f,  0.29f },
                           {  0.04f, 0.0f,  0.30f },
                           {  0.00f, 0.0f,  0.30f },
                           { -0.04f, 0.0f,  0.30f },
                           { -0.09f, 0.0f,  0.29f },
                           { -0.11f, 0.0f,  0.28f },
                           { -0.13f, 0.0f,  0.25f },
                           { -0.14f, 0.0f,  0.21f },
                           { -0.14f, 0.0f,  0.15f },
                           { -0.13f, 0.0f,  0.06f },
                           { -0.00f, 0.0f,  0.06f } };
static int soleCount = 21;
static int heelCount = 17;

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Node implementation with standard viewport draw
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

class footPrint : public MPxLocatorNode
{
 public:
  footPrint();
  ~footPrint() override;

  MStatus   		compute( const MPlug& plug, MDataBlock& data ) override;

  void            draw( M3dView & view, const MDagPath & path,
                        M3dView::DisplayStyle style,
                        M3dView::DisplayStatus status ) override;

  bool            isBounded() const override;
  MBoundingBox    boundingBox() const override;

  MStatus preEvaluation(const MDGContext& context, const MEvaluationNode& evaluationNode) override;

  static  void *          creator();
  static  MStatus         initialize();

  static  MObject         size;         // The size of the foot

 public:
  static	MTypeId		id;
  static	MString		drawDbClassification;
  static	MString		drawRegistrantId;

  static MObject      worldS;
};

MObject footPrint::size;
MTypeId footPrint::id( 0x80007 );
MString	footPrint::drawDbClassification("drawdb/geometry/footPrint");
MString	footPrint::drawRegistrantId("FootprintNodePlugin");
MObject footPrint::worldS;

footPrint::footPrint() {}
footPrint::~footPrint() {}

MStatus footPrint::compute( const MPlug& plug/*plug*/, MDataBlock& dataBlock/*data*/ )
{
  MStatus s;
  if (plug == worldS)
  {
    if (plug.isElement())
    {
      MArrayDataHandle outputArrayHandle = dataBlock.outputArrayValue( worldS );
      outputArrayHandle.setAllClean();
    }

    dataBlock.setClean(plug);

    return MS::kSuccess;
  }

  return MS::kUnknownParameter;;
}

// called by legacy default viewport
void footPrint::draw( M3dView & view, const MDagPath & /*path*/,
                      M3dView::DisplayStyle style,
                      M3dView::DisplayStatus status )
{
  // Get the size
  //
  MObject thisNode = thisMObject();
  MPlug plug( thisNode, size );
  MDistance sizeVal;
  plug.getValue( sizeVal );

  float multiplier = (float) sizeVal.asCentimeters();

  view.beginGL();


  if ( ( style == M3dView::kFlatShaded ) ||
      ( style == M3dView::kGouraudShaded ) )
  {
    // Push the color settings
    //
    glPushAttrib( GL_CURRENT_BIT );

    if ( status == M3dView::kActive ) {
      view.setDrawColor( 13, M3dView::kActiveColors );
    } else {
      view.setDrawColor( 13, M3dView::kDormantColors );
    }

    glBegin( GL_TRIANGLE_FAN );
    int i;
    int last = soleCount - 1;
    for ( i = 0; i < last; ++i ) {
      glVertex3f( sole[i][0] * multiplier,
                  sole[i][1] * multiplier,
                  sole[i][2] * multiplier );
    }
    glEnd();
    glBegin( GL_TRIANGLE_FAN );
    last = heelCount - 1;
    for ( i = 0; i < last; ++i ) {
      glVertex3f( heel[i][0] * multiplier,
                  heel[i][1] * multiplier,
                  heel[i][2] * multiplier );
    }
    glEnd();

    glPopAttrib();
  }

  // Draw the outline of the foot
  //
  glBegin( GL_LINES );
  int i;
  int last = soleCount - 1;
  for ( i = 0; i < last; ++i ) {
    glVertex3f( sole[i][0] * multiplier,
                sole[i][1] * multiplier,
                sole[i][2] * multiplier );
    glVertex3f( sole[i+1][0] * multiplier,
                sole[i+1][1] * multiplier,
                sole[i+1][2] * multiplier );
  }
  last = heelCount - 1;
  for ( i = 0; i < last; ++i ) {
    glVertex3f( heel[i][0] * multiplier,
                heel[i][1] * multiplier,
                heel[i][2] * multiplier );
    glVertex3f( heel[i+1][0] * multiplier,
                heel[i+1][1] * multiplier,
                heel[i+1][2] * multiplier );
  }
  glEnd();


  view.endGL();

  // Draw the name of the footPrint
  view.setDrawColor( MColor( 0.1f, 0.8f, 0.8f, 1.0f ) );
  view.drawText( MString("Footprint"), MPoint( 0.0, 0.0, 0.0 ), M3dView::kCenter );
}

bool footPrint::isBounded() const
{
  return true;
}

MBoundingBox footPrint::boundingBox() const
{
  // Get the size
  //
  MObject thisNode = thisMObject();
  MPlug plug( thisNode, size );
  MDistance sizeVal;
  plug.getValue( sizeVal );

  double multiplier = sizeVal.asCentimeters();

  MPoint corner1( -0.17, 0.0, -0.7 );
  MPoint corner2( 0.17, 0.0, 0.3 );

  corner1 = corner1 * multiplier;
  corner2 = corner2 * multiplier;

  return MBoundingBox( corner1, corner2 );
}

// Called before this node is evaluated by Evaluation Manager
MStatus footPrint::preEvaluation(
    const MDGContext& context,
    const MEvaluationNode& evaluationNode)
{
  if (context.isNormal())
  {
    MStatus status;
    if (evaluationNode.dirtyPlugExists(size, &status) && status)
    {
      MHWRender::MRenderer::setGeometryDrawDirty(thisMObject());
    }
  }

  return MStatus::kSuccess;
}

void* footPrint::creator()
{
  return new footPrint();
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Viewport 2.0 override implementation
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

class FootPrintData : public MUserData
{
 public:
  FootPrintData() : MUserData(false) {} // don't delete after draw
  ~FootPrintData() override {}

  MColor fColor;
  unsigned int fDepthPriority;
  MPointArray fLineList;
  MPointArray fTriangleList;
};

class FootPrintDrawOverride : public MHWRender::MPxDrawOverride
{
 public:
  static MHWRender::MPxDrawOverride* Creator(const MObject& obj)
  {
    return new FootPrintDrawOverride(obj);
  }

  ~FootPrintDrawOverride() override;

  MHWRender::DrawAPI supportedDrawAPIs() const override;

  bool isBounded(
      const MDagPath& objPath,
      const MDagPath& cameraPath) const override;

  MBoundingBox boundingBox(
      const MDagPath& objPath,
      const MDagPath& cameraPath) const override;

  MUserData* prepareForDraw(
      const MDagPath& objPath,
      const MDagPath& cameraPath,
      const MHWRender::MFrameContext& frameContext,
      MUserData* oldData) override;

  bool hasUIDrawables() const override { return true; }

  void addUIDrawables(
      const MDagPath& objPath,
      MHWRender::MUIDrawManager& drawManager,
      const MHWRender::MFrameContext& frameContext,
      const MUserData* data) override;

  bool traceCallSequence() const override
  {
    // Return true if internal tracing is desired.
    return false;
  }
  void handleTraceMessage( const MString &message ) const override
  {
    MGlobal::displayInfo("footPrintDrawOverride: " + message);

    // Some simple custom message formatting.
    fputs("footPrintDrawOverride: ",stderr);
    fputs(message.asChar(),stderr);
    fputs("\n",stderr);
  }


 private:
  FootPrintDrawOverride(const MObject& obj);
  float getMultiplier(const MDagPath& objPath) const;

  static void OnModelEditorChanged(void *clientData);

  footPrint*  fFootPrint;
  MCallbackId fModelEditorChangedCbId;
};

// By setting isAlwaysDirty to false in MPxDrawOverride constructor, the
// draw override will be updated (via prepareForDraw()) only when the node
// is marked dirty via DG evaluation or dirty propagation. Additional
// callback is also added to explicitly mark the node as being dirty (via
// MRenderer::setGeometryDrawDirty()) for certain circumstances. Note that
// the draw callback in MPxDrawOverride constructor is set to NULL in order
// to achieve better performance.
FootPrintDrawOverride::FootPrintDrawOverride(const MObject& obj)
    : MHWRender::MPxDrawOverride(obj, NULL, false)
{
  fModelEditorChangedCbId = MEventMessage::addEventCallback(
      "modelEditorChanged", OnModelEditorChanged, this);

  MStatus status;
  MFnDependencyNode node(obj, &status);
  fFootPrint = status ? dynamic_cast<footPrint*>(node.userNode()) : NULL;
}

FootPrintDrawOverride::~FootPrintDrawOverride()
{
  fFootPrint = NULL;

  if (fModelEditorChangedCbId != 0)
  {
    MMessage::removeCallback(fModelEditorChangedCbId);
    fModelEditorChangedCbId = 0;
  }
}

void FootPrintDrawOverride::OnModelEditorChanged(void *clientData)
{
  // Mark the node as being dirty so that it can update on display appearance
  // switch among wireframe and shaded.
  FootPrintDrawOverride *ovr = static_cast<FootPrintDrawOverride*>(clientData);
  if (ovr && ovr->fFootPrint)
  {
    MHWRender::MRenderer::setGeometryDrawDirty(ovr->fFootPrint->thisMObject());
  }
}

MHWRender::DrawAPI FootPrintDrawOverride::supportedDrawAPIs() const
{
  // this plugin supports both GL and DX
  return (MHWRender::kOpenGL | MHWRender::kDirectX11 | MHWRender::kOpenGLCoreProfile);
}

float FootPrintDrawOverride::getMultiplier(const MDagPath& objPath) const
{
  // Retrieve value of the size attribute from the node
  MStatus status;
  MObject footprintNode = objPath.node(&status);
  if (status)
  {
    MPlug plug(footprintNode, footPrint::size);
    if (!plug.isNull())
    {
      MDistance sizeVal;
      if (plug.getValue(sizeVal))
      {
        return (float)sizeVal.asCentimeters();
      }
    }
  }

  return 1.0f;
}

bool FootPrintDrawOverride::isBounded(const MDagPath& /*objPath*/,
                                      const MDagPath& /*cameraPath*/) const
{
  return true;
}

MBoundingBox FootPrintDrawOverride::boundingBox(
    const MDagPath& objPath,
    const MDagPath& cameraPath) const
{
  MPoint corner1( -0.17, 0.0, -0.7 );
  MPoint corner2( 0.17, 0.0, 0.3 );

  float multiplier = getMultiplier(objPath);
  corner1 = corner1 * multiplier;
  corner2 = corner2 * multiplier;

  return MBoundingBox( corner1, corner2 );
}

// Called by Maya each time the object needs to be drawn.
MUserData* FootPrintDrawOverride::prepareForDraw(
    const MDagPath& objPath,
    const MDagPath& cameraPath,
    const MHWRender::MFrameContext& frameContext,
    MUserData* oldData)
{
  // Any data needed from the Maya dependency graph must be retrieved and cached in this stage.
  // There is one cache data for each drawable instance, if it is not desirable to allow Maya to handle data
  // caching, simply return null in this method and ignore user data parameter in draw callback method.
  // e.g. in this sample, we compute and cache the data for usage later when we create the
  // MUIDrawManager to draw footprint in method addUIDrawables().
  FootPrintData* data = dynamic_cast<FootPrintData*>(oldData);
  if (!data)
  {
    data = new FootPrintData();
  }

  float fMultiplier = getMultiplier(objPath);

  data->fLineList.clear();

  for (int i = 0; i <= soleCount - 2; i++)
  {
    data->fLineList.append(sole[i][0] * fMultiplier, sole[i][1] * fMultiplier, sole[i][2] * fMultiplier);
    data->fLineList.append(sole[i+1][0] * fMultiplier, sole[i+1][1] * fMultiplier, sole[i+1][2] * fMultiplier);
  }

  for (int i = 0; i <= heelCount - 2; i++)
  {
    data->fLineList.append(heel[i][0] * fMultiplier, heel[i][1] * fMultiplier, heel[i][2] * fMultiplier);
    data->fLineList.append(heel[i+1][0] * fMultiplier, heel[i+1][1] * fMultiplier, heel[i+1][2] * fMultiplier);
  }

  data->fTriangleList.clear();

  for (int i = 1; i <= soleCount - 2; i++)
  {
    data->fTriangleList.append(sole[0][0] * fMultiplier, sole[0][1] * fMultiplier, sole[0][2] * fMultiplier);
    data->fTriangleList.append(sole[i][0] * fMultiplier, sole[i][1] * fMultiplier, sole[i][2] * fMultiplier);
    data->fTriangleList.append(sole[i+1][0] * fMultiplier, sole[i+1][1] * fMultiplier, sole[i+1][2] * fMultiplier);
  }

  for (int i = 1; i <= heelCount - 2; i++)
  {
    data->fTriangleList.append(heel[0][0] * fMultiplier, heel[0][1] * fMultiplier, heel[0][2] * fMultiplier);
    data->fTriangleList.append(heel[i][0] * fMultiplier, heel[i][1] * fMultiplier, heel[i][2] * fMultiplier);
    data->fTriangleList.append(heel[i+1][0] * fMultiplier, heel[i+1][1] * fMultiplier, heel[i+1][2] * fMultiplier);
  }

  // get correct color and depth priority based on the state of object, e.g. active or dormant
  data->fColor = MHWRender::MGeometryUtilities::wireframeColor(objPath);
  switch (MHWRender::MGeometryUtilities::displayStatus(objPath))
  {
    case MHWRender::kLead:
    case MHWRender::kActive:
    case MHWRender::kHilite:
    case MHWRender::kActiveComponent:
      data->fDepthPriority = MHWRender::MRenderItem::sActiveWireDepthPriority;
      break;
    default:
      data->fDepthPriority = MHWRender::MRenderItem::sDormantFilledDepthPriority;
      break;
  }

  return data;
}

// addUIDrawables() provides access to the MUIDrawManager, which can be used
// to queue up operations for drawing simple UI elements such as lines, circles and
// text. To enable addUIDrawables(), override hasUIDrawables() and make it return true.
void FootPrintDrawOverride::addUIDrawables(
    const MDagPath& objPath,
    MHWRender::MUIDrawManager& drawManager,
    const MHWRender::MFrameContext& frameContext,
    const MUserData* data)
{
  // Get data cached by prepareForDraw() for each drawable instance, then MUIDrawManager
  // can draw simple UI by these data.
  FootPrintData* pLocatorData = (FootPrintData*)data;
  if (!pLocatorData)
  {
    return;
  }

  drawManager.beginDrawable();

  // Draw the foot print solid/wireframe
  drawManager.setColor( pLocatorData->fColor );
  drawManager.setDepthPriority( pLocatorData->fDepthPriority );

  if (frameContext.getDisplayStyle() & MHWRender::MFrameContext::kGouraudShaded) {
    drawManager.mesh(MHWRender::MUIDrawManager::kTriangles, pLocatorData->fTriangleList);
  }

  drawManager.mesh(MHWRender::MUIDrawManager::kLines, pLocatorData->fLineList);

  // Draw a text "Foot"
  MPoint pos( 0.0, 0.0, 0.0 ); // Position of the text
  MColor textColor( 0.1f, 0.8f, 0.8f, 1.0f ); // Text color

  drawManager.setColor( textColor );
  drawManager.setFontSize( MHWRender::MUIDrawManager::kSmallFontSize );
  drawManager.text( pos,  MString("Footprint"), MHWRender::MUIDrawManager::kCenter );

  drawManager.endDrawable();
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Plugin Registration
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

MStatus footPrint::initialize()
{
  MFnUnitAttribute unitFn;
  MStatus			 stat;

  size = unitFn.create( "size", "sz", MFnUnitAttribute::kDistance );
  unitFn.setDefault( 1.0 );

  stat = addAttribute( size );
  if (!stat) {
    stat.perror("addAttribute");
    return stat;
  }

  worldS = unitFn.create("worldS", "ws", MFnUnitAttribute::kDistance, 1.0);
  unitFn.setWritable(true);
  unitFn.setCached(false);
  unitFn.setArray( true );
  unitFn.setUsesArrayDataBuilder( true );
  unitFn.setWorldSpace( true );

  addAttribute( worldS );
  attributeAffects(size, worldS);

  return MS::kSuccess;
}

MStatus initializePlugin( MObject obj )
{
  MStatus   status;
  MFnPlugin plugin( obj, PLUGIN_COMPANY, "3.0", "Any");

  status = plugin.registerNode(
      "footPrint",
      footPrint::id,
      &footPrint::creator,
      &footPrint::initialize,
      MPxNode::kLocatorNode,
      sUseLegacyDraw ? NULL : &footPrint::drawDbClassification);
  if (!status) {
    status.perror("registerNode");
    return status;
  }

  if (!sUseLegacyDraw)
  {
    status = MHWRender::MDrawRegistry::registerDrawOverrideCreator(
        footPrint::drawDbClassification,
        footPrint::drawRegistrantId,
        FootPrintDrawOverride::Creator);
    if (!status) {
      status.perror("registerDrawOverrideCreator");
      return status;
    }
  }

  return status;
}

MStatus uninitializePlugin( MObject obj)
{
  MStatus   status;
  MFnPlugin plugin( obj );

  if (!sUseLegacyDraw)
  {
    status = MHWRender::MDrawRegistry::deregisterDrawOverrideCreator(
        footPrint::drawDbClassification,
        footPrint::drawRegistrantId);
    if (!status) {
      status.perror("deregisterDrawOverrideCreator");
      return status;
    }
  }

  status = plugin.deregisterNode( footPrint::id );
  if (!status) {
    status.perror("deregisterNode");
    return status;
  }
  return status;
}
