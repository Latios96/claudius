//
// Created by Jan on 08.05.2019.
//

#include "ClaudiusVisualizerTranslator.h"
#include "extension/Extension.h"

extern "C"
{
DLLEXPORT void initializeExtension(CExtension &plugin) {
    cout << "Register Extension" << std::endl;
    plugin.RegisterTranslator("ClaudiusVisualizer",
                              "",
                              CTestTranslator::creator);
}
DLLEXPORT void deinitializeExtension(CExtension &plugin) {
}
}