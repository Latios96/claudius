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
