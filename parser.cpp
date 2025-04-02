#include "parser.h"

// Peek the current token without consuming it
Token Parser::peek() {
  if (currentToken < tokens.size())
    return tokens[currentToken];
  return {END_OF_FILE, ""};
}

// Consume the current token and move to the next
Token Parser::consume() {
  if (currentToken < tokens.size()) {
    return tokens[currentToken++];
  }
  return {END_OF_FILE, ""};
}

// Parse a single statement
ASTNode *Parser::parseStatement() {
  Token token = peek();
  std::cout << "[PARSER] Checking token: " << token.value << std::endl;

  // Skip semicolon-only statements
  if (token.type == PUNCTUATION && token.value == ";") {
    consume();
    return nullptr;
  }

  // Handle the 'if' keyword
  if (token.type == KEYWORD && token.value == "if") {
    consume(); // Consume 'if'
    // Parse the condition (assuming there's a `parseExpression` method)
    ASTNode *condition = parseExpression();
    if (!condition)
      return nullptr;

    // Expect a block after 'if' condition
    ASTNode *body = parseBlock();
    if (!body)
      return nullptr;

    // Check for 'else'
    if (peek().type == KEYWORD && peek().value == "else") {
      consume();                        // Consume 'else'
      ASTNode *elseBody = parseBlock(); // Parse the block after 'else'
      return new ASTNode{
          "IF_ELSE",
          "",
          {condition, body, elseBody}}; // Return the 'if-else' ASTNode
    }

    return new ASTNode{"IF", "", {condition, body}}; // Return the 'if' ASTNode
  }

  // Handle assignments (e.g., x = expression;)
  if (token.type == IDENTIFIER && currentToken + 1 < tokens.size() &&
      tokens[currentToken + 1].value == "=") {
    Token varName = consume(); // Consume variable name
    consume();                 // Consume '='

    ASTNode *expr = parseExpression();
    if (!expr) {
      std::cerr << "[PARSER ERROR] Invalid expression in assignment"
                << std::endl;
      return nullptr;
    }

    if (peek().type == PUNCTUATION && peek().value == ";") {
      consume(); // Consume the semicolon
    } else {
      std::cerr << "[PARSER ERROR] Expected ';' after assignment" << std::endl;
      return nullptr;
    }

    std::cout << "[PARSER] Parsed Assignment: " << varName.value << " = "
              << expr->value << std::endl;
    return new ASTNode{"ASSIGN", varName.value, {expr}};
  }

  // Handle print statements (print(expression);)
  if (token.type == KEYWORD && token.value == "print") {
    consume(); // Consume 'print' keyword

    if (peek().type == PUNCTUATION && peek().value == "(") {
      consume(); // Consume '('
    } else {
      std::cerr << "[PARSER ERROR] Expected '(' after 'print'" << std::endl;
      return nullptr;
    }

    ASTNode *expr = parseExpression();
    if (!expr) {
      std::cerr << "[PARSER ERROR] Invalid expression in print statement"
                << std::endl;
      return nullptr;
    }

    if (peek().type == PUNCTUATION && peek().value == ")") {
      consume(); // Consume ')'
    } else {
      std::cerr << "[PARSER ERROR] Expected ')' after print expression"
                << std::endl;
      return nullptr;
    }

    if (peek().type == PUNCTUATION && peek().value == ";") {
      consume(); // Consume the semicolon
    } else {
      std::cerr << "[PARSER ERROR] Expected ';' after print statement"
                << std::endl;
      return nullptr;
    }

    std::cout << "[PARSER] Parsed Print Statement" << std::endl;
    return new ASTNode{"PRINT", "", {expr}};
  }

  // If no valid statement was found, print an error
  std::cerr << "[PARSER ERROR] Unexpected token: " << token.value << std::endl;
  return nullptr;
}

ASTNode *Parser::parseBlock() {
  Token token = peek();

  // Ensure the token is a '{' for a block
  if (token.type == PUNCTUATION && token.value == "{") {
    consume(); // Consume the '{'
    ASTNode *blockNode = new ASTNode{"BLOCK", "", {}};

    while (peek().type != PUNCTUATION || peek().value != "}") {
      ASTNode *stmt = parseStatement();
      if (stmt) {
        blockNode->children.push_back(stmt);
      }
      if (peek().type == END_OF_FILE) {
        std::cerr << "[PARSER ERROR] Unexpected EOF in block" << std::endl;
        return nullptr;
      }
    }

    consume(); // Consume the '}'
    return blockNode;
  } else {
    std::cerr << "[PARSER ERROR] Expected '{', found: " << token.value
              << std::endl;
    return nullptr;
  }
}

ASTNode *Parser::parseExpression() {
  ASTNode *left = parseTerm();
  if (!left)
    return nullptr;

  while (peek().type == OPERATOR &&
         (peek().value == "+" || peek().value == "-" || peek().value == ">" ||
          peek().value == "<" || peek().value == "==" || peek().value == ">=" ||
          peek().value == "<=")) {
    Token op = consume();
    ASTNode *right = parseTerm();
    if (!right)
      return nullptr;
    left = new ASTNode{"OP", op.value, {left, right}};
  }

  return left;
}

ASTNode *Parser::parseTerm() {
  ASTNode *left = parseFactor();
  if (!left)
    return nullptr;

  while (peek().type == OPERATOR &&
         (peek().value == "*" || peek().value == "/")) {
    Token op = consume();
    ASTNode *right = parseFactor();
    if (!right)
      return nullptr;
    left = new ASTNode{"OP", op.value, {left, right}};
  }

  return left;
}

ASTNode *Parser::parseFactor() {
  Token token = peek();

  // Check if the token is an integer, float, or identifier
  if (token.type == INTEGER || token.type == FLOAT ||
      token.type == IDENTIFIER) {
    consume();                                      // Consume the token
    return new ASTNode{"LITERAL", token.value, {}}; // Return a literal ASTNode
  }

  // Handle parentheses for grouping expressions
  if (token.type == PUNCTUATION && token.value == "(") {
    consume(); // Consume '('
    ASTNode *expr = parseExpression();
    if (!expr)
      return nullptr; // Return nullptr if expression parsing fails
    if (peek().value == ")") {
      consume(); // Consume ')'
    } else {
      std::cerr << "[PARSER ERROR] Expected ')', found: " << peek().value
                << std::endl;
      return nullptr;
    }
    return expr; // Return the parsed expression inside parentheses
  }

  // Panic Mode: If the token is unexpected, we skip it and try to recover
  std::cerr << "[PARSER ERROR] Unexpected token: " << token.value
            << ". Entering panic mode." << std::endl;

  int skippedTokens = 0;
  const int maxSkippedTokens =
      10; // Limit the number of tokens to skip before aborting

  // Skip invalid tokens until we find a valid one or reach the end of input
  while (peek().type != END_OF_FILE &&
         !(peek().type == INTEGER || peek().type == FLOAT ||
           peek().type == IDENTIFIER || peek().value == "(")) {
    consume(); // Skip the current token
    skippedTokens++;

    // If we've skipped too many tokens without making progress, terminate the
    // parser
    if (skippedTokens >= maxSkippedTokens) {
      std::cerr << "[PARSER ERROR] Too many unexpected tokens encountered. "
                   "Terminating parser."
                << std::endl;
      exit(EXIT_FAILURE); // Terminate the parser with an error
    }
  }

  // After skipping invalid tokens, retry parsing the factor
  return parseFactor();
}

ASTNode *Parser::parse() {
  ASTNode *root = new ASTNode{"PROGRAM", "", {}};

  while (peek().type != END_OF_FILE) {
    ASTNode *stmt = parseStatement();
    if (stmt)
      root->children.push_back(stmt);
  }

  std::cout << "[DEBUG] AST Tree:\n";
  root->print(); // Assuming `print()` exists to visualize the AST.

  return root;
}
