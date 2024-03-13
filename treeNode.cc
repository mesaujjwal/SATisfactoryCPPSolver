#include "treeNode.h"

TreeNode::TreeNode(std::string cntt): content {cntt} {}

std::string TreeNode::getContent() const { return content; }

TreeNode *TreeNode::getLeftChild() const { return leftChild; }
TreeNode *TreeNode::getRightChild() const { return rightChild; }

void TreeNode::updateLeftChild(TreeNode *lChild) { leftChild = lChild; }

void TreeNode::updateChildren(TreeNode *lChild, TreeNode *rChild) {
  leftChild = lChild;
  rightChild = rChild;
}

std::string TreeNode::printStr() const {
  if (!leftChild && !rightChild) return content;
  else if (content == "-") return ("(" + content + "(" + leftChild->printStr() + ")" + ")");
  else return ("(" + leftChild->printStr() + content + rightChild->printStr() + ")");
}

OperatorNode::OperatorNode(std::string cntt): TreeNode{cntt} {}

bool OperatorNode::evaluate(const std::map<std::string, bool> &assignment) const {
  bool tvLeft = getLeftChild()->evaluate(assignment);
  if (getContent() == "-") return (!tvLeft);
  bool tvRight = getRightChild()->evaluate(assignment);
  if (getContent() == "+") return tvLeft || tvRight;
  else if (getContent() == "*") return tvLeft && tvRight;
  else throw "Error: invalid input";
}

ConstantNode::ConstantNode(std::string cntt): TreeNode{cntt} {}

bool ConstantNode::evaluate(const std::map<std::string, bool> &assignment) const {
  return stoi(getContent());
}

VariableNode::VariableNode(std::string cntt): TreeNode{cntt} {}

bool VariableNode::evaluate(const std::map<std::string, bool> &assignment) const {
  std::string variable = getContent();
  if (assignment.find(variable) == assignment.end()) {
    throw "Error: incomplete assignment";
  }
  return assignment.at(variable);
}

TreeNode::~TreeNode() {
  delete leftChild;
  delete rightChild;
}
