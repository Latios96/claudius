//
// Created by Jan on 08.05.2019.
//

#include "Translator.h"

#include <ai_msg.h>
#include <ai_nodes.h>


void CTestTranslator::Export(AtNode* shader)
{
    AiMsgInfo("[test extension] Exporting %s", GetMayaNodeName().asChar());
    AiNodeSetFlt(shader, "base", FindMayaPlug("diffuse").asFloat() );
    AiNodeSetRGB(shader, "base_color",
                 FindMayaPlug("colorR").asFloat(),
                 FindMayaPlug("colorG").asFloat(),
                 FindMayaPlug("colorB").asFloat());
}

AtNode* CTestTranslator::CreateArnoldNodes()
{
    return AddArnoldNode("standard_surface");
}

void* CTestTranslator::creator()
{
    return new CTestTranslator();
}