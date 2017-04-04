#ifndef LEXER_HPP
#define LEXER_HPP
#include <string>
#include <cstring>
#include "token.hpp"
#include <sstream>
#include <stdexcept>

//#include <type>

struct Lexer {
	std::string::iterator f;
	std::string::iterator l;

	std::string str;

	Lexer(std::string _str) : str(_str) {
		f = str.begin();
		l = str.end();
	}

	std::string buf = "";
	Token* next();
	bool eof() const { 
		return (f == l); 
	}
	char lookahead() const {
		if (!eof())
			return *f;
	}

	void consume() {
		f++;
	}
	void buffer() {
		// if (eof())
		// 	return;

		buf += *f;
		f++;
		//buf.back();
	}

};

Token * Lexer::next() {
	
		while(!eof()) {
			buf = "";
			//std::cout << "lookahead : " <<  lookahead() << " : " << (int)lookahead() << std::endl;
			switch(lookahead()) {
				case ' ': consume(); continue;
				case '\n':consume(); continue;
				case '\t':consume(); continue;
				case '\v':consume(); continue;
				case '+': consume(); return new PuncOp_token(Plus_tok);
				case '-': consume(); return new PuncOp_token(Minus_tok);
				case '*': consume(); return new PuncOp_token(Star_tok);
				case '/': consume(); return new PuncOp_token(Slash_tok);
				case '%': consume(); return new PuncOp_token(Percent_tok);
				case '?': consume(); return new PuncOp_token(Query_tok);
				case ':': consume(); return new PuncOp_token(Colon_tok);
				case '(': consume(); return new PuncOp_token(Lparen_tok);
				case ')': consume(); return new PuncOp_token(Rparen_tok);
				case ';': consume(); return new PuncOp_token(Semi_tok);
				case '&': { 
					consume();
					if (lookahead() == '&') {
						consume();
						return new PuncOp_token(AmpAmp_tok);
					}
					else {
						std::cout << "Received unsupported character";
						throw std::runtime_error(""); 
						break;
						}
				}
				case '|': { 
					consume();
					if (lookahead() == '|') {
						consume();
						return new PuncOp_token(PipePipe_tok);
					}
					else {
						std::cout << "Received unsupported character";
						throw std::runtime_error("");
						break;
					}
				}
				case '!': { 
					consume();
					if (lookahead() == '=') {
						consume();
						return new PuncOp_token(BangEq_tok);
					}
					else return new PuncOp_token(Bang_tok);
				}
				case '=': { 
					consume();
					if (lookahead() == '=') {
						consume();
						return new PuncOp_token(EqEq_tok);
					}
					else return new PuncOp_token(Eq_tok);
				}
				case '<': { 
					consume();
					if (lookahead() == '=') {
						consume();
						return new PuncOp_token(LtEq_tok);
					}
					else return new PuncOp_token(Lt_tok);
				}
				case '>': { 
					consume();
					if (lookahead() == '=') {
						consume();
						return new PuncOp_token(GtEq_tok);
					}
					else return new PuncOp_token(Gt_tok);
				}
				case '0':
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				case '6':
				case '7':
				case '8':
				case '9': {
					buffer();
					while(!eof() && ((lookahead()) != '\n') && isdigit(lookahead())) 
						buffer();
					return new Int_token(std::stoi(buf));
				}
				case 't': {
					buffer();
					if (lookahead() == 'r')
						buffer();
					if (lookahead() == 'u')
						buffer();
					if (lookahead() == 'e')
						buffer();
					return new Bool_token(true);
				}
				case 'f': {
					buffer(); 
					if (lookahead() == 'a')
						buffer();
					if (lookahead() == 'l')
						buffer();
					if (lookahead() == 's')
						buffer();
					if (lookahead() == 'e')
						buffer();
					return new Bool_token(false);
				}
				default: std::cout << "Received unsupported character" << std::endl; throw std::runtime_error(""); break;
			} // end switch
		} //end while // return nullpt	r;
}

#endif