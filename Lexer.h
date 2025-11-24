#if !defined(LEXER_H)
#define LEXER_H

#include <vector>
#include <fstream>

#include "Token.h"
#include "Exception.h"

//dichiarazioni del lexer e dei metodi per produrre token

class Lexer {
	public:
	Lexer() = default;
	~Lexer() = default;
	Lexer(Lexer const&) = delete;
	Lexer& operator=(Lexer const&) = delete;

	std::vector<Token> operator()(std::ifstream& inputFile) {
		std::vector<Token> inputTokens;
		tokenizeInputFile(inputFile, inputTokens);
		return inputTokens;
	}
	void tokenizeInputFile(std::ifstream& inputFile, std::vector<Token>& inputTokens);
};


#endif
