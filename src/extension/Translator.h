//
// Created by Jan on 08.05.2019.
//

#ifndef CLAUDIUS_SRC_EXTENSION_TRANSLATOR_H_
#define CLAUDIUS_SRC_EXTENSION_TRANSLATOR_H_

#include <translators/shader/ShaderTranslator.h>


class CTestTranslator
    : public CShaderTranslator
{
 public:
  AtNode* CreateArnoldNodes();
  virtual void Export(AtNode*);
  static void* creator();
};

#endif //CLAUDIUS_SRC_EXTENSION_TRANSLATOR_H_
