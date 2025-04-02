#include "lexer.h"

vector<Token> Lexer::tokenize() {
  while (pos < source.length()) {
    char ch = source[pos];

    if (isspace(ch)) {
      pos++;
      continue;
    }

    if (isDigit(ch)) { // Numbers
      string num;
      while (pos < source.length() &&
             (isDigit(source[pos]) || source[pos] == '.')) {
        num += source[pos++];
      }
      TokenType type = num.find('.') != string::npos ? FLOAT : INTEGER;
      tokens.push_back({type, num});
      cout << "[LEXER] Token: " << num << endl; // Debug Print
      continue;
    }

    if (isAlpha(ch)) { // Identifiers & Keywords
      string word;
      while (pos < source.length() &&
             (isAlpha(source[pos]) || isDigit(source[pos]))) {
        word += source[pos++];
      }
      TokenType type =
          (word == "if" || word == "else" || word == "for" || word == "print")
              ? KEYWORD
              : IDENTIFIER;
      tokens.push_back({type, word});
      cout << "[LEXER] Token: " << word << endl; // Debug Print
      continue;
    }

    if (string("+-*/=<>!").find(ch) != string::npos) { // Operators
      tokens.push_back({OPERATOR, string(1, ch)});
      cout << "[LEXER] Token: " << ch << endl; // Debug Print
      pos++;
      continue;
    }

    if (string("();{}").find(ch) != string::npos) { // Punctuation
      tokens.push_back({PUNCTUATION, string(1, ch)});
      cout << "[LEXER] Token: " << ch << endl; // Debug Print
      pos++;
      continue;
    }

    cout << "[LEXER] Skipping Unrecognized Character: " << ch << endl;
    pos++; // Skip unrecognized characters
  }

  tokens.push_back({END_OF_FILE, ""});
  cout << "[LEXER] Tokenization Complete!" << endl;
  return tokens;
}
