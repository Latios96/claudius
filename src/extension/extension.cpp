//
// Created by Jan on 08.05.2019.
//

#include "Translator.h"
#include "extension/Extension.h"


extern "C"
{
DLLEXPORT void initializeExtension(CExtension &plugin)
{
    plugin.RegisterTranslator("phong",
                              "",
                              CTestTranslator::creator);
}
DLLEXPORT void deinitializeExtension(CExtension &plugin)
{
}
}