//
// Created by Jan on 08.05.2019.
//

#include "Translator.h"

#include <ai_msg.h>
#include <ai_nodes.h>


void CTestTranslator::Export(AtNode* claudius_particle_cloud)
{
    AiNodeSetStr(claudius_particle_cloud, "file_path", FindMayaPlug("particleFile").asString().asChar());
    AiNodeSetFlt(claudius_particle_cloud, "particle_radius", 0.01);

    auto user_data_rgb = AddArnoldNode("user_data_rgb");
    AiNodeSetStr(user_data_rgb, "name", (std::string(AiNodeGetName(claudius_particle_cloud)) + "_user_data_rgb").c_str());
    if(FindMayaPlug("renderWithColor").asBool()){
        AiNodeSetStr(user_data_rgb, "attribute", "particle_color");
    };
    AiNodeSetRGB(user_data_rgb, "default", 1,1,1);

    auto shader = AddArnoldNode("standard_surface");
    AiNodeSetStr(shader, "name", (std::string(AiNodeGetName(claudius_particle_cloud)) + "_standard_surface").c_str());
    AiNodeSetFlt(shader, "base", 1);
    AiNodeSetFlt(shader, "specular", 0);
    AiNodeLink(user_data_rgb, "base_color", shader);

    AiNodeSetPtr(claudius_particle_cloud, "shader", shader);
    ExportMatrix(claudius_particle_cloud);
}

AtNode* CTestTranslator::CreateArnoldNodes()
{
    return AddArnoldNode("claudiusparticlecloud");
}

void* CTestTranslator::creator()
{
    return new CTestTranslator();
}
void CTestTranslator::Update(AtNode *node) {
}

