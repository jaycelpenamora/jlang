#ifndef CODEGEN_H
#define CODEGEN_H

#include "parser.h"

class CodeGenerator {
public:
  static string generate(ASTNode *node);
};

#endif // CODEGEN_H
