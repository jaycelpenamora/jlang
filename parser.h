#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include <vector>

// AST Node Structure
struct ASTNode {
  string type;
  string value;
  vector<ASTNode *> children;
};

// Parser Class
class Parser {
private:
  vector<Token> tokens;
  int currentToken = 0;

  Token peek();
  Token consume();
  ASTNode *parseExpression();
  ASTNode *parseStatement();

public:
  Parser(vector<Token> inputTokens) : tokens(inputTokens) {}
  ASTNode *parse();
};

#endif // PARSER_H
