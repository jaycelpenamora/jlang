#include "parser.h"

Token Parser::peek() { return tokens[currentToken]; }
Token Parser::consume() { return tokens[currentToken++]; }

ASTNode *Parser::parseExpression() {
  ASTNode *left = new ASTNode{"EXPR",
                              consume().value,
                              {}}; // Consume first token (number or identifier)
  // Handle operations like "x + 5"
  while (peek().type == OPERATOR) {
    Token op = consume(); // Consume the operator
    ASTNode *right = new ASTNode{
        "EXPR", consume().value, {}}; // Consume next number/identifier
    ASTNode *operationNode = new ASTNode{"OP", op.value, {left, right}};
    left = operationNode; // Set the operation as the new left expression
  }
  return left;
}

ASTNode *Parser::parseStatement() {
  Token token = peek();
  cout << "[PARSER] Checking token: " << token.value << endl;

  if (token.type == PUNCTUATION && token.value == ";") {
    consume();      // Skip semicolons
    return nullptr; // No need to return a node for semicolons
  }

  if (token.type == IDENTIFIER && tokens[currentToken + 1].value == "=") {
    Token varName = consume();
    consume(); // '='
    ASTNode *expr = parseExpression();
    consume(); // Consume the trailing semicolon `;`
    cout << "[PARSER] Parsed Assignment: " << varName.value << " = "
         << expr->value << endl;
    return new ASTNode{"ASSIGN", varName.value, {expr}};
  }

  if (token.type == KEYWORD && token.value == "print") {
    consume();                         // Consume the 'print' keyword
    consume();                         // Consume the opening parenthesis '('
    ASTNode *expr = parseExpression(); // Parse the expression inside print
    consume();                         // Consume the closing parenthesis ')'
    consume();                         // Consume the semicolon
    cout << "[PARSER] Parsed Print Statement" << endl;
    return new ASTNode{"PRINT", "", {expr}};
  }

  cout << "[PARSER] Unknown Statement: " << token.value << endl;
  consume(); // Ensure forward movement
  return nullptr;
}

ASTNode *Parser::parse() {
  ASTNode *root = new ASTNode{"PROGRAM", "", {}};
  while (peek().type != END_OF_FILE) {
    ASTNode *stmt = parseStatement();
    if (stmt)
      root->children.push_back(stmt);
  }
  return root;
}
