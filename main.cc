#include "parser.h"
#include "treeNode.h"
#include <algorithm>
#include <iostream>
#include <map>

// a helper function parsing the input into the formula string and the
// assignment string
void parseLine(const std::string &line, std::string &formulaStr,
               std::string &assignmentStr) {

  formulaStr = "";
  assignmentStr = "";

  // checking if the input has more than one semicolons
  if (std::count(line.begin(), line.end(), ';') > 1) {
    throw std::invalid_argument("Error: invalid input");
  }

  // splitting line based on the position of the semicolon
  std::string::size_type semiColonPosition = line.find(';');
  if (line.npos != semiColonPosition) {
    formulaStr = line.substr(0, semiColonPosition);
    assignmentStr = line.substr(semiColonPosition + 1);
  }
}

// The program shall continuously ask for new inputs from standard input and
// output to the standard output The program should terminate gracefully (and
// quietly) once it sees EOF
int main() {

  std::string line;          // store each input line
  std::string formulaStr;    // store the formula string
  std::string assignmentStr; // store the assignment string

  while (getline(std::cin, line)) {

    TreeNode *root = nullptr;

    try {

      // calling the parseLine function to get formula and assignment strings
      parseLine(line, formulaStr, assignmentStr);

      // checking if formula string is empty
      if (formulaStr.empty()) {
        throw std::invalid_argument("Error: invalid input");
      }

      // calling the formulaParser to generate AST
      FormulaParser formulaParser(formulaStr);

      // root of AST
      root = formulaParser.getTreeRoot();

      // check if parse tree was not formed
      if (root == nullptr) {
        throw std::invalid_argument("Error: invalid input");
      }

      // parse Assignment(s)
      AssignmentParser assignmentParser(assignmentStr);
      std::map<std::string, bool> assignmentMap =
          assignmentParser.getAssignmentMap();

      // boolean obj to store final result
      bool result = root->evaluate(assignmentMap);
      std::cout << result << std::endl;

      delete (root);
    }

    // catch errors thrown
    catch (std::invalid_argument &error) {
      std::cout << error.what() << std::endl;

      if (root != nullptr) {
        delete (root);
      }
    }
  }
}
