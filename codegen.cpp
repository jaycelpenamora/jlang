#include "codegen.h"

string CodeGenerator::generate(ASTNode *node) {
  cout << "[CODEGEN] Generating Code for: " << node->type << " (" << node->value
       << ")" << endl;

  if (node->type == "ASSIGN") {
    return node->value + " = " + generate(node->children[0]) + ";\n";
  } else if (node->type == "EXPR") {
    return node->value;
  }

  cout << "[CODEGEN] Unknown Node: " << node->type << endl;
  return "";
}
