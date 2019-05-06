//
// Created by Jan on 04.05.2019.
//

#include <ai.h>
#include <sstream>
#include <vector>
#include <fstream>
#include <ParticleReaderFactory.h>

// init: read ui parameters
// get node: create


AI_PROCEDURAL_NODE_EXPORT_METHODS(InstancerMtd);

node_parameters
{
    AiParameterArray("path_indices"       , AiArray(0, 1, AI_TYPE_INT));
    AiParameterArray("path_start_indices" , AiArray(0, 1, AI_TYPE_INT));
    AiParameterArray("objects"            , AiArray(0, 1, AI_TYPE_STRING));
    AiParameterArray("instance_matrix"    , AiArray(0, 1, AI_TYPE_MATRIX));
}

procedural_init
{
    // todo read path from ui
    // todo read particles from disk

    /*Instancer *instancer = new Instancer();
    *user_ptr = instancer;

    // get procedural name so it's the prefix of instanced nodes' name
    instancer->name = AiNodeGetStr(node, "name");
    instancer->path_indices = AiNodeGetArray(node, "path_indices");
    instancer->num_instances = AiArrayGetNumElements(instancer->path_indices);

    // this is the start index of each instance
    instancer->path_start_indices = AiNodeGetArray(node, "path_start_indices");
    AiMsgInfo("[instancer] number of instances: %d", AiArrayGetNumElements(instancer->path_indices));

    AtArray *object_names = AiNodeGetArray(node, "objects"); // to get the AtNode pointers for all the master nodes

    // allocate and create AtNode pointer array
    AiMsgInfo("[instancer] getting master node list ...");

    instancer->objects.resize(AiArrayGetNumElements(object_names));
    for (unsigned int i = 0; i < AiArrayGetNumElements(object_names); i++)
    {
        const char *master_name = AiArrayGetStr(object_names, i);
        AiMsgInfo("[instancer] adding node %s to master list", master_name);

        AtNode *node = AiNodeLookUpByName(master_name);
        instancer->objects[i] = node;
        if (node == NULL)
            AiMsgWarning("[instancer] node %s does not exist!", master_name);
    }

    // get matrices pointer and number of motion keys
    AtArray *instance_matrix = AiNodeGetArray(node, "instance_matrix");
    instancer->instance_matrix = instance_matrix;
    AiMsgInfo("[instancer] instance_matrix elements: %d, motion keys: %d", AiArrayGetNumElements(instance_matrix), AiArrayGetNumKeys(instance_matrix));

    return true;*/
    return true;
}

procedural_cleanup
{
    // todo delete particle container
    /*Instancer *instancer = (Instancer*)user_ptr;
    delete instancer;*/
    return true;
}

procedural_num_nodes
{
    // return how many nodes to generate
    /*Instancer *instancer = (Instancer*)user_ptr;
    return AiArrayGetNumElements(instancer->instance_matrix);*/
    return 1;
}

procedural_get_node
{
    /*Instancer *instancer = (Instancer*)user_ptr;

    int instance_index = AiArrayGetInt(instancer->path_start_indices, i); // get particle index for this instance
    int path_index = AiArrayGetInt(instancer->path_indices, instance_index);
    AtNode *object = instancer->objects[path_index];

    AtNode *currentInstance = AiNode("ginstance", "instancer", node); // initialize node as child of procedural node

    std::stringstream numStr; // setup node name by concatenate procedural name with instance node number
    numStr << "_" << instance_index;
    std::string currentName(instancer->name + numStr.str());

    AiNodeSetStr(currentInstance, "name", currentName.c_str());
    AiNodeSetPtr(currentInstance, "node", (void *)object); // setup ginstance node parameter
    AiNodeSetBool(currentInstance, "inherit_xform", false); // usually instancer doesn't move around, but maybe this should be set to true?

    int num_motion_keys = AiArrayGetNumKeys(instancer->instance_matrix);
    if (num_motion_keys > 1)
    {
        // allocate and assign matrices
        AtArray *matrices = AiArrayAllocate(1, num_motion_keys, AI_TYPE_MATRIX);
        for (int j = 0; j < num_motion_keys; j++)
        {
            AtMatrix matrix = AiArrayGetMtx(instancer->instance_matrix, instance_index + instancer->num_instances * j);
            AiArraySetMtx(matrices, j, matrix);
        }
        AiNodeSetArray(currentInstance, "matrix", matrices);
        }
    else
    {
        AtMatrix matrix = AiArrayGetMtx(instancer->instance_matrix, instance_index);
        AiNodeSetMatrix(currentInstance, "matrix", matrix);
    }

    return currentInstance;*/
    AiMsgInfo("[claudius] reading particles from disk..");
    //const std::string filepath = R"(M:\Projekte\2019\recap_test\Dormagen_City_Export.0001.pts)";
    const std::string filepath = R"(M:\Projekte\2019\recap_test\test.pts)";
    
    std::ifstream filestream(filepath);

    auto particleReader = ParticleReaderFactory::createParticleReader(filepath);
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
        AiArraySetFlt(radiusarray, i, 0.01f);
    }

    AtNode *pointsNode = AiNode("points");
    AiNodeSetArray(pointsNode, "points", pointarray);
    AiNodeSetArray(pointsNode, "radius", radiusarray);
    AiNodeSetStr(pointsNode, "mode", "sphere");
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