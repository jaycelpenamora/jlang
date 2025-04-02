#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Token Types
enum TokenType {
  IDENTIFIER,
  INTEGER,
  FLOAT,
  PUNCTUATION,
  STRING_LITERAL,
  OPERATOR,
  LOGICAL_OPERATOR,
  COMPARISON_OPERATOR,
  KEYWORD,
  PRINT,
  PARENTHESIS,
  SEMICOLON,
  ASSIGN,
  END_OF_FILE
};

// Token Structure
struct Token {
  TokenType type;
  string value;
};

// Lexer Class
class Lexer {
private:
  string source;
  vector<Token> tokens;
  int pos = 0;

  bool isDigit(char ch) { return ch >= '0' && ch <= '9'; }
  bool isAlpha(char ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || ch == '_';
  }

public:
  Lexer(string input) : source(input) {}

  vector<Token> tokenize();
};

#endif // LEXER_H
