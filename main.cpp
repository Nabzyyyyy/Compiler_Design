//#include "stdafx.hpp" // g++ -std=c++11 main.cpp
#include "eval.hpp"
#include "lexer.hpp"
#include <exception>
#include <iostream>
#include <stdio.h>
#include <sstream>
#include "parser.hpp"
#include <deque>

int main() {	

	// Token* five = new Int_tok;
	// five->val = 5; 

	// Token* t = new Bool_tok;
	// t->val = true;


	std::string strIn; // 
	std::deque<Token*> tokens;
	//std::stringstream strStream;

	Context* cxt = new Context();

	while(std::getline(std::cin, strIn)) { 	// while there's standard input
		//strStream << strIn; // append the string to the stream

		Lexer l = Lexer(strIn);
		Parser p;// = new Parser(tokens, cxt);
		while(l.f != l.l) {
	 		Token * t = l.next();
	 		//t->print();
	 		tokens.push_back(t);
			// push to deque instead of print //t->print();
		}

		p = Parser(tokens, cxt);


		 while (!p.eof()) {
		 	//Expr* e = p.expression();
		 	
		 	p.print();
		 	
		 	//std::cout << e->Evaluate();
		 }

		tokens.clear();

		std::cout << "\n";

	}

	//std::cout << "\n" << strStream.str() << std::endl;

	

	
	return 0;
}