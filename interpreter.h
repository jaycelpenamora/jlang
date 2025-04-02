#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "parser.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <string>

class Interpreter {
public:
  Interpreter() {}

  double evaluate(ASTNode *node) {
    if (node->type == "EXPR") {
      // If it's a number, return its value
      if (isNumber(node->value)) {
        return std::stod(node->value);
      }
      // If it's an identifier, retrieve its value
      if (variables.find(node->value) != variables.end()) {
        return variables[node->value];
      }
    } else if (node->type == "ASSIGN") {
      // Evaluate the expression on the right-hand side
      double value = evaluate(node->children[0]);
      variables[node->value] = value; // Store the variable value
      return value;
    } else if (node->type == "OP") {
      double leftValue = evaluate(node->children[0]);
      double rightValue = evaluate(node->children[1]);

      if (node->value == "+") {
        return leftValue + rightValue;
      } else if (node->value == "-") {
        return leftValue - rightValue;
      } else if (node->value == "*") {
        return leftValue * rightValue;
      } else if (node->value == "/") {
        return leftValue / rightValue;
      }
    }
    return 0.0; // Default return for unhandled nodes
  }

  void execute(ASTNode *node) {
    if (node->type == "PRINT") {
      double value = evaluate(node->children[0]);
      std::cout << value << std::endl;
    } else if (node->type == "ASSIGN") {
      evaluate(node); // Just evaluate assignment to store the value
    } else if (node->type == "EXPR") {
      evaluate(node); // Evaluate expressions if needed
    }
  }

private:
  std::map<std::string, double> variables; // Store variable values

  bool isNumber(const std::string &str) {
    return !str.empty() &&
           std::find_if(str.begin(), str.end(), [](unsigned char c) {
             return !std::isdigit(c);
           }) == str.end();
  }
};

#endif // INTERPRETER_H
