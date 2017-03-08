//#include "stdafx.hpp"
#include "eval.hpp"
#include "lexer.hpp"
#include <exception>
#include <iostream>
#include <stdio.h>
#include <sstream>

int main() {	

	// Token* five = new Int_tok;
	// five->val = 5; 

	// Token* t = new Bool_tok;
	// t->val = true;






	std::string strIn; // 
	//std::stringstream strStream;

	while(std::getline(std::cin, strIn)) { 	// while there's standard input
		//strStream << strIn; // append the string to the stream

		Lexer l = Lexer(strIn);

		while(l.f != l.l) {
	 		Token * t = l.next();
			t->print();
		}

		std::cout << "\n";

	}

	//std::cout << "\n" << strStream.str() << std::endl;

	

	
	return 0;
}