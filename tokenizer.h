#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <map>
#include <string>
#include <vector>

struct Token {
  std::string content;
  std::string type;
};

// a tokenzier breaks a stream of string into tokens
class Tokenizer {
private:
  size_t iterator;
  std::vector<Token> tokens;

public:
  Tokenizer(std::string ln);
  void advanceToken();   // advance to the next token; return false if no more
                         // token exist
  bool hasToken() const; // check whether there is a current token
  Token getToken();      // returns the token being processed
};
#endif
