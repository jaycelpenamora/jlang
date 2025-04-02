#include "lexer.h"

vector<Token> Lexer::tokenize() {
  while (pos < source.length()) {
    char ch = source[pos];

    if (isspace(ch)) {
      pos++;
      continue;
    }

    // Handle Numbers (Integers & Floats)
    if (isDigit(ch)) {
      string num;
      bool hasDot = false; // Track if the number has a decimal point
      while (pos < source.length() &&
             (isDigit(source[pos]) || source[pos] == '.')) {
        if (source[pos] == '.') {
          // If we already have a dot, it's not a valid number anymore
          if (hasDot)
            break;
          hasDot = true;
        }
        num += source[pos++];
      }

      // Classify numbers as either integer or float based on presence of dot
      if (hasDot) {
        tokens.push_back({FLOAT, num}); // Treat as a float
      } else {
        tokens.push_back({INTEGER, num}); // Treat as an integer
      }

      cout << "[LEXER] Token: " << num << endl; // Debug Print
      continue;
    }

    // Handle Identifiers & Keywords
    if (isAlpha(ch)) {
      string word;
      while (pos < source.length() &&
             (isAlpha(source[pos]) || isDigit(source[pos]))) {
        word += source[pos++];
      }
      TokenType type = (word == "if" || word == "else" || word == "for" ||
                        word == "print" || word == "def")
                           ? KEYWORD
                           : IDENTIFIER;
      tokens.push_back({type, word});
      cout << "[LEXER] Token: " << word << endl; // Debug Print
      continue;
    }

    // Handle Operators
    if (string("+-*/=<>!").find(ch) != string::npos) {
      tokens.push_back({OPERATOR, string(1, ch)});
      cout << "[LEXER] Token: " << ch << endl; // Debug Print
      pos++;
      continue;
    }

    // Handle Comparisons explicitly (==, >, <)
    if (ch == '=' && pos + 1 < source.length() &&
        source[pos + 1] == '=') { // "=="
      tokens.push_back({COMPARISON_OPERATOR, "=="});
      pos += 2;
      continue;
    }
    if (ch == '>' || ch == '<') { // Handle ">" and "<"
      tokens.push_back({COMPARISON_OPERATOR, string(1, ch)});
      pos++;
      continue;
    }

    // Handle Punctuation
    if (string("();{}").find(ch) != string::npos) {
      tokens.push_back({PUNCTUATION, string(1, ch)});
      cout << "[LEXER] Token: " << ch << endl; // Debug Print
      pos++;
      continue;
    }

    // Handle commas
    if (ch == ',') {
      tokens.push_back({PUNCTUATION, ","});
      cout << "[LEXER] Token: ," << endl; // Debug Print
      pos++;
      continue;
    }

    // Handle String Literals (with escape handling)
    if (ch == '"') {
      string str;
      pos++; // Skip the opening quote
      while (pos < source.length() && source[pos] != '"') {
        if (source[pos] == '\\' && pos + 1 < source.length()) {
          // Handle escaped characters (e.g., \", \\)
          str += source[pos + 1];
          pos += 2; // Skip the escape character and the next character
        } else {
          str += source[pos++];
        }
      }
      pos++; // Skip the closing quote
      tokens.push_back({STRING_LITERAL, str});
      continue;
    }

    // Handle Logical Operators (&&, ||)
    if (ch == '&' && pos + 1 < source.length() &&
        source[pos + 1] == '&') { // "and"
      tokens.push_back({LOGICAL_OPERATOR, "and"});
      pos += 2;
      continue;
    }
    if (ch == '|' && pos + 1 < source.length() &&
        source[pos + 1] == '|') { // "or"
      tokens.push_back({LOGICAL_OPERATOR, "or"});
      pos += 2;
      continue;
    }

    // Handle unrecognized characters
    if (string("+-*/=<>!();{}").find(ch) == string::npos && !isAlpha(ch) &&
        !isDigit(ch) && ch != '"') {
      std::cerr << "[LEXER ERROR] Unrecognized character: " << ch << std::endl;
      pos++; // Skip the unrecognized character but log the error
      continue;
    }

    // If character is not recognized at all, log and skip
    cout << "[LEXER] Skipping Unrecognized Character: " << ch << endl;
    pos++; // Skip unrecognized characters
  }

  // Push the end-of-file token
  tokens.push_back({END_OF_FILE, ""});
  cout << "[LEXER] Tokenization Complete!" << endl;
  return tokens;
}
