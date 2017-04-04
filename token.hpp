#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <iostream>

enum Tok_kind {
	Eof_tok,
	Int_tok,
	Bool_tok,
	Plus_tok,
	Minus_tok,
	Star_tok,
	Slash_tok,
	Percent_tok,
	AmpAmp_tok,
	PipePipe_tok,
	Bang_tok,
	Eq_tok,
	EqEq_tok,
	BangEq_tok,
	Lt_tok,
	Gt_tok,
	LtEq_tok,
	GtEq_tok,
	Query_tok,
	Colon_tok,
	Lparen_tok,
	Rparen_tok,
	Semi_tok
};

struct Token {
	Tok_kind kind;
	virtual void print() = 0;
};

struct PuncOp_token : Token { // make constructor
	
	PuncOp_token(Tok_kind _kind) {
		kind = _kind;
	}

	void print() {
		switch (kind) {
			// case Int_tok :  std::cout << "Int_token" << std::endl; break;
			// case Bool_tok : std::cout << "Bool_token" << std::endl; break;
			case Eof_tok : std::cout << "End of file" << std::endl; break;
			case Plus_tok : std::cout << "Plus_token" << std::endl; break;
			case Minus_tok : std::cout << "Minus_token" << std::endl; break;
			case Star_tok : std::cout << "Star_token" << std::endl; break;
			case Slash_tok : std::cout << "Slash_token" << std::endl; break;
			case Percent_tok : std::cout << "Percent_token" << std::endl; break;
			case AmpAmp_tok : std::cout << "AmpAmp_token" << std::endl; break;
			case PipePipe_tok : std::cout << "PipePipe_token" << std::endl; break;
			case Bang_tok : std::cout << "Bang_token" << std::endl; break;
			case Eq_tok : std::cout << "Eq_token" << std::endl; break;
			case EqEq_tok : std::cout << "EqEq_token" << std::endl; break;
			case BangEq_tok : std::cout << "BangEq_token" << std::endl; break;
			case Lt_tok : std::cout << "Lt_token" << std::endl; break;
			case Gt_tok: std::cout << "Gt_token" << std::endl; break;
			case LtEq_tok : std::cout << "LtEq_token" << std::endl; break;
			case GtEq_tok : std::cout << "GtEq_token" << std::endl; break;
			case Query_tok : std::cout << "Query_token" << std::endl; break;
			case Colon_tok : std::cout << "Colon_token" << std::endl; break;
			case Lparen_tok : std::cout << "Lparen_token" << std::endl; break;
			case Rparen_tok : std::cout << "Rparen_token" << std::endl; break;
			case Semi_tok : std::cout << "Semi_token" << std::endl; break;
		};
	}
};

struct Bool_token : Token {
	bool val;

	Bool_token(bool value) {
		kind = Bool_tok;
		val = value;
	}
	
	void print() {
		std::cout << "Bool_token : " <<  (val ? "true" : "false") << std::endl;
	}
};

struct Int_token : Token {
	int val;

	Int_token(int buffer) {
		kind = Int_tok;
		val = buffer;
	}

	void print() { 
		std::cout << "Int_Token : " << val << std::endl;
	}
};



#endif