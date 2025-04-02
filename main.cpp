#include "codegen.h"
#include "interpreter.h"
#include "lexer.h"
#include "parser.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

using namespace std;

int main(int argc, char *argv[]) {
  if (argc < 2) {
    cout << "Usage: ./minilang <file.ml>" << endl;
    return 1;
  }

  ifstream file(argv[1]);
  if (!file) {
    cout << "Error: Could not open file " << argv[1] << endl;
    return 1;
  }

  stringstream buffer;
  buffer << file.rdbuf();
  string code = buffer.str();

  // Running Lexer
  cout << "[INFO] Running Lexer..." << endl;
  Lexer lexer(code);
  vector<Token> tokens = lexer.tokenize();
  cout << "[INFO] Tokenization complete!" << endl;

  // Running Parser
  cout << "[INFO] Running Parser..." << endl;
  Parser parser(tokens);
  unique_ptr<ASTNode> ast(
      parser.parse()); // Use smart pointer for automatic cleanup
  cout << "[INFO] Parsing complete!" << endl;

  // Check if AST is null (in case of parsing errors)
  if (!ast) {
    cout << "[ERROR] Parsing failed! AST is empty." << endl;
    return 1;
  }

  // Running Interpreter
  cout << "[INFO] Running Interpreter..." << endl;
  Interpreter interpreter;
  for (auto &child : ast->children) {
    interpreter.execute(child); // Execute each statement in the program
  }
  cout << "[INFO] Execution complete!" << endl;

  return 0;
}
