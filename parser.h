#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <vector>

// AST Node Structure
struct ASTNode {
  string type;
  string value;
  vector<ASTNode *> children;
  void print(int depth = 0) {
    std::cout << std::string(depth * 2, ' ') << type;
    if (!value.empty())
      std::cout << " (" << value << ")";
    std::cout << std::endl;

    for (auto child : children) {
      if (child)
        child->print(depth + 1);
    }
  }
};

// Parser Class
class Parser {
private:
  vector<Token> tokens;
  int currentToken = 0;

  Token peek();
  Token consume();

public:
  Parser(vector<Token> inputTokens) : tokens(inputTokens) {}
  ASTNode *parseExpression();
  ASTNode *parseStatement();
  ASTNode *parseBlock();
  ASTNode *parseTerm();
  ASTNode *parseFactor();
  ASTNode *parse();
};

#endif // PARSER_H
