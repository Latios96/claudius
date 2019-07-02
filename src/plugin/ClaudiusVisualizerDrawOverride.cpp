//
// Created by Jan on 21.04.2019.
//
#include "ClaudiusVisualizerDrawOverride.h"

ClaudiusVisualizerDrawOverride::ClaudiusVisualizerDrawOverride(const MObject &obj) : MPxDrawOverride(obj,
                                                                                                     &drawCallback) {
    claudiusVisualizer = nullptr;
    MStatus status;
    MFnDependencyNode dnode(obj, &status);
    if (status) {
        claudiusVisualizer = dynamic_cast<ClaudiusVisualizer *>(dnode.userNode());
    }

}
ClaudiusVisualizerDrawOverride::~ClaudiusVisualizerDrawOverride() {

}

MUserData *ClaudiusVisualizerDrawOverride::prepareForDraw(const MDagPath &objPath,
                                                          const MDagPath &cameraPath,
                                                          const MFrameContext &frameContext,
                                                          MUserData *oldData) {
    auto *visualizerData = dynamic_cast<PartioVisualizerData *>(oldData);

    MFnDagNode me(objPath);
    MFnTransform myTransform(me.parent(0));
    MFnDependencyNode depNode(me.parent(0));
    MObject worldMatrixAttr = depNode.attribute("worldMatrix");

    MMatrix matrix = getWorldSpaceTransform(me.parent(0), myTransform);

    if (visualizerData == nullptr) {
        auto *pData = new PartioVisualizerData();
        if (claudiusVisualizer != nullptr && claudiusVisualizer->particleContainer != nullptr) {
            pData->particleContainer = claudiusVisualizer->particleContainer;
            DisplayOptions displayOptions = createDisplayOptions(matrix);
            generateDisplayList(pData, displayOptions);
        } else {
            pData->particleContainer = nullptr;
        }
        pData->matrix = matrix;
        return pData;
    } else {
        if (claudiusVisualizer != nullptr && claudiusVisualizer->particleContainer != nullptr) {
            visualizerData->particleContainer = claudiusVisualizer->particleContainer;
            DisplayOptions displayOptions = createDisplayOptions(matrix);
            generateDisplayList(visualizerData, displayOptions);
        }
        visualizerData->matrix = matrix;
        return visualizerData;
    }
}
MHWRender::DrawAPI ClaudiusVisualizerDrawOverride::supportedDrawAPIs() const {
#if MAYA_API_VERSION >= 201600
    return MHWRender::kOpenGL | MHWRender::kOpenGLCoreProfile;
#else
    return MHWRender::kOpenGL;
#endif
}
MPxDrawOverride *ClaudiusVisualizerDrawOverride::creator(const MObject &obj) {
    return new ClaudiusVisualizerDrawOverride(obj);
}
void ClaudiusVisualizerDrawOverride::addUIDrawables(const MDagPath &objPath,
                                                    MHWRender::MUIDrawManager &drawManager,
                                                    const MHWRender::MFrameContext &frameContext,
                                                    const MUserData *data) {

}
void ClaudiusVisualizerDrawOverride::drawCallback(const MDrawContext &context, const MUserData *data) {
    MHardwareRenderer *rend = MHardwareRenderer::theRenderer();
    MGLFunctionTable *gGLFT;
    if (rend) {

        gGLFT = rend->glFunctionTable();

        auto *visualizerData = dynamic_cast<const PartioVisualizerData *>(data);

        if (visualizerData != nullptr) {
            if (visualizerData->particleContainer != nullptr) {
                cout << "drawing using " << visualizerData->currentDisplayList << std::endl;
                gGLFT->glCallList(visualizerData->currentDisplayList);
            }
        }
    }
}
void ClaudiusVisualizerDrawOverride::generateDisplayList(PartioVisualizerData *visualizerData,
                                                         DisplayOptions &displayOptions) {
    cout << "generate display list called" << std::endl;
    MHardwareRenderer *rend = MHardwareRenderer::theRenderer();
    MGLFunctionTable *gGLFT;
    if (rend) {
        gGLFT = rend->glFunctionTable();

        if (visualizerData != nullptr) {
            if (visualizerData->particleContainer != nullptr) {
                // delete old display list
                if (visualizerData->currentDisplayList != -1) {
                    if (visualizerData->displayOptions == displayOptions) {
                        return;
                    } else {
                        glDeleteLists(visualizerData->currentDisplayList, 1);
                    }
                }
                cout << "generating" << displayOptions.matrix << std::endl;
                visualizerData->displayOptions = displayOptions;

                const float *particleData = visualizerData->particleContainer->getParticleData();
                const int *colorData = visualizerData->particleContainer->getColorData();

                visualizerData->currentDisplayList = gGLFT->glGenLists(1);

                gGLFT->glNewList(visualizerData->currentDisplayList, GL_COMPILE);

                ////////// list begin
                gGLFT->glMultMatrixd((double *) &displayOptions.matrix[0][0]);
                gGLFT->glPointSize(1.0f);
                gGLFT->glColor3f(0.0f, 0.0f, 1.0f);
                gGLFT->glBegin(MGL_POINTS);
                for (unsigned int i = 0; i < visualizerData->particleContainer->particleCount() * 3;
                     i = i + 3 * displayOptions.renderEveryNth) {
                    if (displayOptions.displayColor) {
                        gGLFT->glColor3f(colorData[i] / 255.0, colorData[i + 1] / 255.0, colorData[i + 2] / 255.0);
                    }
                    gGLFT->glVertex3f(particleData[i], particleData[i + 1], particleData[i + 2]);
                }
                gGLFT->glEnd();
                ////////// list end
                gGLFT->glEndList();
            }
        }
    }

}
DisplayOptions ClaudiusVisualizerDrawOverride::createDisplayOptions(MMatrix matrix) {
    return DisplayOptions(MPlug(claudiusVisualizer->thisMObject(), ClaudiusVisualizer::filePathAttribute).asString(),
                          MPlug(claudiusVisualizer->thisMObject(),
                                ClaudiusVisualizer::renderWithColorAttribute).asBool(),
                          MPlug(claudiusVisualizer->thisMObject(),
                                ClaudiusVisualizer::displayEveryNthAttribute).asInt(),
                          matrix);
}
MMatrix ClaudiusVisualizerDrawOverride::getWorldSpaceTransform(const MObject& mObject, const MFnTransform& transform) {
    auto worldMatrixAttr = transform.attribute("worldMatrix");

    MPlug matrixPlug = MPlug(mObject, worldMatrixAttr);
    matrixPlug = matrixPlug.elementByLogicalIndex(0);

    MObject matrixObject = matrixPlug.asMObject();
    MStatus status;
    MFnMatrixData worldMatrixData(matrixObject, &status);
    return worldMatrixData.matrix();
}

DisplayOptions::DisplayOptions(const MString &particleFilePath,
                               bool displayColor,
                               int displayPercentage,
                               MMatrix matrix)
    : particleFilePath(particleFilePath),
      displayColor(displayColor),
      renderEveryNth(displayPercentage),
      matrix(matrix) {}

bool DisplayOptions::operator==(const DisplayOptions &rhs) const {
    return particleFilePath == rhs.particleFilePath &&
        displayColor == rhs.displayColor &&
        renderEveryNth == rhs.renderEveryNth &&
        matrix == rhs.matrix;
}
bool DisplayOptions::operator!=(const DisplayOptions &rhs) const {
    return !(rhs == *this);
}
DisplayOptions::DisplayOptions() = default;
