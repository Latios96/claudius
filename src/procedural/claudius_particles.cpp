//
// Created by Jan on 04.05.2019.
//

#include <ai.h>
#include <sstream>
#include <vector>
#include <fstream>
#include <ParticleReaderFactory.h>


struct PointCloudData{
  std::string filePath;
  float particleRadius;
};

AI_PROCEDURAL_NODE_EXPORT_METHODS(InstancerMtd);

node_parameters
{
    AiParameterStr("file_path", "");
    AiParameterFlt("particle_radius", 0.01f);
}

procedural_init
{
    auto *pointCloudData = new PointCloudData();
    *user_ptr = pointCloudData;

    pointCloudData->filePath = AiNodeGetStr(node, "file_path");
    pointCloudData->particleRadius = AiNodeGetFlt(node, "particle_radius");

    return true;
}

procedural_cleanup
{
    return true;
}

procedural_num_nodes
{
    return 1;
}

float color_int_to_float(int value){
    return ((float) value) / 255;
}

procedural_get_node
{
    auto *pointCloudData = (PointCloudData*)user_ptr;

    AiMsgInfo("[claudius] reading particles from disk..");
    AiMsgInfo("[claudius] reading particles from %s", pointCloudData->filePath.c_str());
    
    std::ifstream filestream(pointCloudData->filePath);

    auto particleReader = ParticleReaderFactory::createParticleReader(pointCloudData->filePath);
    ParticleContainer particleContainer;
    particleReader->readParticles(filestream, particleContainer);
    AiMsgInfo("[claudius] reading particles done.");
    
    AiMsgInfo("[claudius] copy particles to arnold..");
    AtArray *pointarray  = AiArrayAllocate(particleContainer.particleCount(), 1, AI_TYPE_VECTOR);
    AtArray *radiusarray = AiArrayAllocate(particleContainer.particleCount(), 1, AI_TYPE_FLOAT);

    const float *particleData = particleContainer.getParticleData();
    
    for(unsigned int i=0; i<particleContainer.particleCount(); i++){
        AiArraySetVec(pointarray, i, AtVector(particleData[i*3], particleData[i*3+1], particleData[i*3+2]));
    }
    
    for(int i=0; i<particleContainer.particleCount(); i++){
        AiArraySetFlt(radiusarray, i, pointCloudData->particleRadius);
    }

    AtNode *pointsNode = AiNode("points");
    AiNodeSetArray(pointsNode, "points", pointarray);
    AiNodeSetArray(pointsNode, "radius", radiusarray);
    AiNodeSetStr(pointsNode, "mode", "sphere");

    AiMsgInfo("[claudius] particles have color: %s", particleContainer.hasColorData() ? "yes" : "no");
    if(particleContainer.hasColorData()){

        bool b = AiNodeDeclare(pointsNode, "particle_color", "uniform RGB");
        AiMsgInfo("[claudius] created color userdata attribute: %s", b ? "true": "false");
        AtArray *colorarray  = AiArrayAllocate(particleContainer.particleCount(), 1, AI_TYPE_RGB);

        AiMsgInfo("[claudius] copy particles color to arnold..");
        const int *colorData = particleContainer.getColorData();
        for(unsigned int i=0; i<particleContainer.particleCount(); i++){
            const AtRGB rgb = AtRGB(color_int_to_float(colorData[i * 3]),
                                     color_int_to_float(colorData[i * 3 + 1]),
                                     color_int_to_float(colorData[i * 3 + 2]));
            AiArraySetRGB(colorarray, i, rgb);
        }
        AiNodeSetArray(pointsNode, "particle_color", colorarray);
    }

    AiMsgInfo("[claudius] copy particles to arnold done.");
    return pointsNode;
}

node_loader
{
    if (i>0)
        return false;
    node->methods      = InstancerMtd;
    node->output_type  = AI_TYPE_NONE;
    node->name         = "claudiusparticlecloud";
    node->node_type    = AI_NODE_SHAPE_PROCEDURAL;
    strcpy(node->version, AI_VERSION);
    return true;
}