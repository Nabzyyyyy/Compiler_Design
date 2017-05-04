#ifndef LEXER_HPP
#define LEXER_HPP
#include <cstdlib>
#include <string>
#include <cstring>
#include "token.hpp"
#include <sstream>
#include <stdexcept>
#include "context.hpp"

//bool debug = true;

//#include <type>

struct Lexer {
	std::string::iterator f; // iterators for string
	std::string::iterator l;

	std::string str; // used for input

	Context *cxt;

	Token* Lex_Id();

	Lexer(std::string _str) : str(_str) {
		f = str.begin();
		l = str.end();
	}

	std::string buf = ""; // buffer for numbers
	Token* next();
	bool eof() const { 
		return (f == l); 
	}
	char lookahead() const {
		//if (!eof())
		return *f;
	}

	char lookahead(int step) const {
		return *(f + step);
	}

	void consume() { // consumes to next char
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
				// case 't': {
				// 	buffer();
				// 	if (lookahead() == 'r')
				// 		buffer();
				// 	if (lookahead() == 'u')
				// 		buffer();
				// 	if (lookahead() == 'e')
				// 		buffer();
				// 	return new Bool_token(true);
				// }
				// case 'f': {
				// 	buffer(); 
				// 	if (lookahead() == 'a')
				// 		buffer();
				// 	if (lookahead() == 'l')
				// 		buffer();
				// 	if (lookahead() == 's')
				// 		buffer();
				// 	if (lookahead() == 'e')
				// 		buffer();
				// 	return new Bool_token(false);
				//}
				// case 'a' ... 'z':
				// case 'A' ... 'Z': return Lex_Id();
				case '_':
				case 'a':
				case 'b':
				case 'c':
				case 'd':
				case 'e':
				case 'f':
				case 'g':
				case 'h':
				case 'i':
				case 'j':
				case 'k':
				case 'l':
				case 'm':
				case 'n':
				case 'o':
				case 'p':
				case 'q':
				case 'r':
				case 's':
				case 't':
				case 'u':
				case 'v':
				case 'w':
				case 'x':
				case 'y':
				case 'z':
				case 'A':
				case 'B':
				case 'C':
				case 'D':
				case 'E':
				case 'F':
				case 'G':
				case 'H':
				case 'I':
				case 'J':
				case 'K':
				case 'L':
				case 'M':
				case 'N':
				case 'O':
				case 'P':
				case 'Q':
				case 'R':
				case 'S':
				case 'T':
				case 'U':
				case 'V':
				case 'W':
				case 'X':
				case 'Y':
				case 'Z': return Lex_Id();
				default: throw std::runtime_error("Received unsupported character"); break;
			} // end switch
		} //end while // return nullpt	r;
		throw std::runtime_error("Lexer: Next: Maybe this will fix it");
}

Token* Lexer::Lex_Id() {
	buffer();
	while (isdigit(lookahead()) || isalpha(lookahead()) || lookahead() == '_')
		buffer();

	const std::string str = buf;
	if(Token *kw = cxt->check_kw(str)) // checks to see if buffer has a keyword to parse as an Id_token
		return kw;
	else 
		return new Id_token(str);
}

#endif