#include "tokenizer.h"
#include <iostream>
#include <map>

Tokenizer::Tokenizer(std::string ln) {

  // global iterator to access tokens from vector
  this->iterator = 0;

  // map with list of valid tokens
  std::map<char, std::string> validTokens;
  validTokens = {{'0', "Zero"},       {'1', "One"},   {'+', "OR"},
                 {'*', "AND"},        {'-', "NOT"},   {'(', "LeftParan"},
                 {')', "RightParan"}, {':', "Colon"}, {',', "Comma"}};

  // looping through the line
  for (size_t lineIndex = 0; lineIndex < ln.length(); lineIndex++) {

    // skip spaces
    if (ln[lineIndex] == ' ') {
      continue;
    }

    // if variable name starts with digit
    if (std::isdigit((int)ln[lineIndex]) && std::isalnum(ln[lineIndex + 1])) {

      throw std::invalid_argument("Error: invalid input");
    }

    // if token matches value in map
    auto tokenInList = validTokens.find(ln[lineIndex]);
    if (tokenInList != validTokens.end()) {

      // add to vector
      std::string token(1, ln[lineIndex]);
      this->tokens.push_back({token, tokenInList->second});
      continue;
    }

    // form variable names
    if (isalpha(ln[lineIndex])) {
      std::string var;
      while (isalnum(ln[lineIndex])) {
        var += ln[lineIndex];
        lineIndex++;
      }

      // check if variable name length is greater than 10
      if (var.length() > 10) {

        // this->tokens.clear();
        throw std::invalid_argument("Error: invalid input");
      }

      // to offset increment above where condition fails
      lineIndex--;

      // push back variable token to vector
      this->tokens.push_back({var, "Variable"});
    } else {

      // invalid token entered
      throw std::invalid_argument("Error: invalid input");
    }
  }
}

void Tokenizer::advanceToken() {
  this->iterator++;
  return;
}

bool Tokenizer::hasToken() const {
  if (this->iterator < this->tokens.size()) {
    return true;
  }
  return false;
}

Token Tokenizer::getToken() {

  Token resultToken;
  if (this->hasToken()) {
    resultToken = this->tokens.at(this->iterator);
  }
  return resultToken;
}