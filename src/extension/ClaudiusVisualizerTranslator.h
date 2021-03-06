//
// Created by Jan on 08.05.2019.
//

#ifndef CLAUDIUS_EXTENSION_CLAUDIUSVISUALIZERTRANSLATOR_H_
#define CLAUDIUS_EXTENSION_CLAUDIUSVISUALIZERTRANSLATOR_H_

#include "translators/DagTranslator.h"

class ClaudiusVisializerTranslator: public CDagTranslator {
 public:
  AtNode *CreateArnoldNodes();
  virtual void Export(AtNode *);
  void Update(AtNode *);
  static void *creator();
};

#endif //CLAUDIUS_SRC_EXTENSION_CLAUDIUSVISUALIZERTRANSLATOR_H_
