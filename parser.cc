#include "parser.h"
#include "tokenizer.h"
#include "treeNode.h"
#include <iostream>

FormulaParser::FormulaParser(std::string ln) : tknzr{new Tokenizer{ln}} {}

// returns root of the AST
TreeNode *FormulaParser::getTreeRoot() {

  TreeNode *root = this->parseFormula();

  // if tonkenizer has tokens then string does not fit the CFG
  if (tknzr->hasToken()) {

    return nullptr;
  }

  return root;
}

// recursive parseFormula function
TreeNode *FormulaParser::parseFormula() {

  TreeNode *conjTerm = this->parseConjTerm();

  if (tknzr->getToken().content == "+") {

    TreeNode *operatorNode = new OperatorNode(tknzr->getToken().content);

    tknzr->advanceToken();
    TreeNode *formula = this->parseFormula();

    operatorNode->updateChildren(conjTerm, formula);

    return operatorNode;
  }

  return conjTerm;
}

// recursive parseConjTerm function
TreeNode *FormulaParser::parseConjTerm() {

  TreeNode *lTerm = this->parseTerm();

  if (tknzr->getToken().content == "*") {

    TreeNode *operatorNode = new OperatorNode(tknzr->getToken().content);

    tknzr->advanceToken();
    TreeNode *rTerm = this->parseConjTerm();

    operatorNode->updateChildren(lTerm, rTerm);

    return operatorNode;
  }
  return lTerm;
}

// recursive parseTerm function
TreeNode *FormulaParser::parseTerm() {

  if (tknzr->getToken().content == "(") {

    tknzr->advanceToken();
    TreeNode *formula = this->parseFormula();

    // check for balanced paranthesis
    if (tknzr->getToken().content != ")") {

      throw std::invalid_argument("Error: invalid input");
    }

    tknzr->advanceToken();

    return formula;
  }

  if (tknzr->getToken().content == "-") {

    TreeNode *operatorNode = new OperatorNode(tknzr->getToken().content);

    tknzr->advanceToken();
    TreeNode *constantNode = this->parseTerm();

    operatorNode->updateLeftChild(constantNode);

    return operatorNode;
  }

  if (tknzr->getToken().type == "Variable") {

    TreeNode *variableNode = new VariableNode(tknzr->getToken().content);

    tknzr->advanceToken();

    return variableNode;
  }

  if (tknzr->getToken().content == "0") {

    TreeNode *constantNode = new ConstantNode(tknzr->getToken().content);
    tknzr->advanceToken();

    return constantNode;
  } else if (tknzr->getToken().content == "1") {

    TreeNode *constantNode = new ConstantNode(tknzr->getToken().content);
    tknzr->advanceToken();

    return constantNode;
  }

  // some invalid input has been entered
  throw std::invalid_argument("Error: invalid input");
}

// destructor for FormulaParser
FormulaParser::~FormulaParser() { delete (tknzr); }

// AssignmentParser starts here

// Constructor
AssignmentParser::AssignmentParser(std::string ln) : tknzr{new Tokenizer{ln}} {

  this->parseAssignments();
}

// recursive parseAssignments
void AssignmentParser::parseAssignments() {

  this->parseAssignment();

  if (tknzr->getToken().type == "Comma") {

    tknzr->advanceToken();
    this->parseAssignments();
  }

  return;
}

// recursive parseAssignment
void AssignmentParser::parseAssignment() {

  if (tknzr->getToken().type == "Variable") {

    std::string varName = tknzr->getToken().content;

    tknzr->advanceToken();
    if (tknzr->getToken().type == "Colon") {

      tknzr->advanceToken();
      if (tknzr->getToken().type == "Zero" || tknzr->getToken().type == "One") {

        bool boolValue = (tknzr->getToken().type == "Zero") ? false : true;

        // condition for contradicting assignments
        if (assignmentMap.find(varName) != assignmentMap.end() &&
            boolValue != assignmentMap.find(varName)->second) {

          throw std::invalid_argument("Error: contradicting assignment");
        }
        this->assignmentMap.insert(
            std::pair<std::string, bool>(varName, boolValue));
        tknzr->advanceToken();

        return;
      }
    }
  } else if (!tknzr->hasToken()) {
    return;
  }

  throw std::invalid_argument("Error: invalid input");
}

std::map<std::string, bool> AssignmentParser::getAssignmentMap() {

  return this->assignmentMap;
}

AssignmentParser::~AssignmentParser() { delete (tknzr); }
