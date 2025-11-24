#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include "Token.h"
#include "Lexer.h"
#include "Parser.h"
#include "SymbolTable.h"
#include "EvaluationVisitor.h"
#include "Exception.h"   


int main(int argc, char* argv[]) {
    
    if (argc < 2) {
        std::cerr << "error: usage " << argv[0] << " <file_input>" << std::endl;
        return EXIT_FAILURE;
    }

    //fase lettura file
    std::ifstream inputFile;
    inputFile.open(argv[1]);
    if (!inputFile) {
        std::cerr << "ERROR: cannot open " << argv[1] << std::endl;
        return EXIT_FAILURE;
    }

    // traduzione dei caratteri letti in token
    std::vector<Token> inputTokens;
    try {
        Lexer lx;
        lx.tokenizeInputFile(inputFile, inputTokens);
    } catch (const LexicalError& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (const std::exception& e) {
        std::cerr << "ERROR: Lexing failed: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    // Parsing dei token, costruzione albero sintattico
    Program* ptrprogram = nullptr;
    try {
        Parser parse;
        ptrprogram = parse(inputTokens);
    } catch (const SyntaxError& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (const std::exception& e) {
        std::cerr << "ERROR: Parsing failed: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    // Valutazione dei nodi costrutiti dal parser
    SymbolTable symbols;
    EvaluationVisitor evaluator{ symbols, std::cout };

    try {
        if (ptrprogram) { 
            ptrprogram->accept(evaluator);
        }
    } catch (const RuntimeError& e) {
        std::cerr << "ERROR : " << e.what() << std::endl;
        delete ptrprogram;
        return EXIT_FAILURE;
    } catch (const std::exception& e) {
        std::cerr << "ERROR: Evaluation failed: " << e.what() << std::endl;
        delete ptrprogram;
        return EXIT_FAILURE;
    }

    delete ptrprogram;
    return EXIT_SUCCESS;
}
