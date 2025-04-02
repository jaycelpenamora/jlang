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
      if (node->value == ">") {
        double leftValue = evaluate(node->children[0]);
        double rightValue = evaluate(node->children[1]);

        std::cout << "[DEBUG] Evaluating: " << leftValue << " > " << rightValue
                  << std::endl;

        return (leftValue > rightValue) ? 1.0
                                        : 0.0; // Correctly handle > operator
      }
      // Handle other operators like *, +, etc. here
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
      } else if (node->value == ">") {
        return (leftValue > rightValue) ? 1.0 : 0.0;
      } else if (node->value == "<") {
        return (leftValue < rightValue) ? 1.0 : 0.0;
      }
    }

    return 0.0;
  }

  // Execute the program by evaluating nodes and performing actions
  void execute(ASTNode *node) {
    if (node->type == "PRINT") {
      // Evaluate and print the result of a PRINT node
      double value = evaluate(node->children[0]);
      std::cout << "Print: " << value << std::endl;
    } else if (node->type == "ASSIGN") {
      // Evaluate right-hand side of assignment and store the value in variables
      double value = evaluate(node->children[0]);
      variables[node->value] = value; // Assign the value to the variable
      std::cout << "[DEBUG] Variable " << node->value
                << " assigned value: " << value << std::endl;
    } else if (node->type == "EXPR") {
      // Evaluate an expression if it's an expression node
      evaluate(node);
    } else if (node->type == "IF_ELSE") {
      // Execute the IF statement (conditional execution)
      double condition = evaluate(node->children[0]);
      std::cout << "If condition: " << condition << std::endl;

      if (condition != 0) {
        std::cout << "Executing IF block" << std::endl;
        // Condition is true, execute the "if" block
        execute(node->children[1]);
      } else if (node->children.size() > 2) {
        std::cout << "Executing ELSE block" << std::endl;
        // Condition is false and "else" block is present, execute the "else"
        // block
        execute(node->children[2]);
      }
    } else if (node->type == "FOR") {
      // Execute a FOR loop (iterate based on the evaluated condition)
      for (int i = 0; i < evaluate(node->children[0]); i++) {
        execute(node->children[1]); // Execute the loop body
      }
    } else if (node->type == "WHILE") {
      // Execute a WHILE loop (repeat while condition is true)
      while (evaluate(node->children[0]) != 0) {
        execute(node->children[1]); // Execute the loop body
      }
    } else if (node->type == "FUNC_CALL") {
      // Handle function calls (not implemented yet)
      std::string funcName = node->value;
      if (functions.find(funcName) != functions.end()) {
        functions[funcName](node->children); // Call the function
      }
    } else if (node->type == "BLOCK") {
      // Execute a block of statements (e.g., inside an "if" or "else" block)
      for (auto &child : node->children) {
        execute(child); // Execute each statement inside the block
      }
    }
  }

  // Function to define a new function that can be called from the AST
  void defineFunction(const std::string &name,
                      std::function<void(std::vector<ASTNode *>)> func) {
    functions[name] = func;
  }

private:
  std::map<std::string, double> variables; // Store variable values
  std::map<std::string, std::function<void(std::vector<ASTNode *>)>>
      functions; // Store function definitions

  // Check if the string is a valid number (integer or floating point)
  bool isNumber(const std::string &str) {
    return !str.empty() &&
           std::find_if(str.begin(), str.end(), [](unsigned char c) {
             return !std::isdigit(c) && c != '.';
           }) == str.end();
  }

  // Check if the string is a valid string literal (currently only checks for
  // quotes)
  bool isString(const std::string &str) {
    return !str.empty() && str[0] == '"' && str[str.length() - 1] == '"';
  }
};

#endif // INTERPRETER_H
