#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "parser.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

class Interpreter {
public:
  Interpreter() {}

  // Evaluate an ASTNode and return its value
  double evaluate(ASTNode *node) {
    if (node->type == "LITERAL") {
      if (isNumber(node->value)) {
        return std::stod(node->value);
      }
      if (isString(node->value)) {
        return static_cast<double>(node->value.length());
      }
      if (variables.find(node->value) != variables.end()) {
        std::cout << "[DEBUG] Variable " << node->value
                  << " found with value: " << variables[node->value]
                  << std::endl;
        return variables[node->value];
      }
      std::cerr << "[ERROR] Undefined variable: " << node->value << std::endl;
      return 0.0;
    }

    if (node->type == "ASSIGN") {
      double value = evaluate(node->children[0]);
      variables[node->value] = value;
      return value;
    }

    if (node->type == "OP") {
      double leftValue = evaluate(node->children[0]);
      double rightValue = evaluate(node->children[1]);

      if (node->value == "+") {
        return leftValue + rightValue;
      } else if (node->value == "-") {
        return leftValue - rightValue;
      } else if (node->value == "*") {
        return leftValue * rightValue;
      } else if (node->value == "/") {
        if (rightValue != 0) {
          return leftValue / rightValue;
        } else {
          std::cerr << "[ERROR] Division by zero." << std::endl;
          return 0.0;
        }
      } else if (node->value == ">") {
        return (leftValue > rightValue) ? 1.0 : 0.0;
      }
    }

    if (node->type == "LOGICAL_OP") {
      double leftValue = evaluate(node->children[0]);
      double rightValue = evaluate(node->children[1]);

      if (node->value == "and") {
        return (leftValue != 0 && rightValue != 0) ? 1.0 : 0.0;
      } else if (node->value == "or") {
        return (leftValue != 0 || rightValue != 0) ? 1.0 : 0.0;
      } else if (node->value == "==") {
        return (leftValue == rightValue) ? 1.0 : 0.0;
      } else if (node->value == "!=") {
        return (leftValue != rightValue) ? 1.0 : 0.0;
      } else if (node->value == "<") {
        return (leftValue < rightValue) ? 1.0 : 0.0;
      }
    }

    return 0.0;
  }

  // Execute the program by evaluating nodes and performing actions
  void execute(ASTNode *node) {
    if (node->type == "PRINT") {
      double value = evaluate(node->children[0]);
      std::cout << "Print: " << value << std::endl;
    } else if (node->type == "ASSIGN") {
      double value = evaluate(node->children[0]);
      variables[node->value] = value;
      std::cout << "[DEBUG] Variable " << node->value
                << " assigned value: " << value << std::endl;
    } else if (node->type == "EXPR") {
      evaluate(node);
    } else if (node->type == "IF_ELSE") {
      double condition = evaluate(node->children[0]);
      std::cout << "If condition: " << condition << std::endl;

      if (condition != 0) {
        std::cout << "Executing IF block" << std::endl;
        execute(node->children[1]);
      } else if (node->children.size() > 2) {
        std::cout << "Executing ELSE block" << std::endl;
        execute(node->children[2]);
      }
    } else if (node->type == "FOR") {
      for (int i = 0; i < evaluate(node->children[0]); i++) {
        execute(node->children[1]);
      }
    } else if (node->type == "WHILE") {
      while (evaluate(node->children[0]) != 0) {
        execute(node->children[1]);
      }
    } else if (node->type == "BLOCK") {
      for (auto &child : node->children) {
        execute(child);
      }
    }
  }

private:
  std::map<std::string, double> variables;

  bool isNumber(const std::string &str) {
    return !str.empty() &&
           std::find_if(str.begin(), str.end(), [](unsigned char c) {
             return !std::isdigit(c) && c != '.';
           }) == str.end();
  }

  bool isString(const std::string &str) {
    return !str.empty() && str[0] == '"' && str[str.length() - 1] == '"';
  }
};

#endif // INTERPRETER_H
