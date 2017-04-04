#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <sstream>
#include "lexer.hpp"
#include "token.hpp"
#include "eval.hpp"
#include <deque>

struct Parser {
public:
	std::deque<Token*> tokens;

	Parser();
	Parser(std::deque<Token*> t);

	bool eof();

	Expr* expression();
	Expr* conditional_expression();
	Expr* logicalOR_expression();
	Expr* logicalAND_expression();
	Expr* equality_expression();
	Expr* relational_expression();
	Expr* additive_expression();
	Expr* multiplicative_expression();
	Expr* unary_expression();
	Expr* primary_expression();

private:
	Token* peek();
	//Token* peek(int);
	Token* lookahead();
	//Tok_kind lookahead(int);
	Token* consume();
	//Token* require(Tok_kind);
	Token* match(Tok_kind);
	Token* match_if(Tok_kind);

};


Parser::Parser() {}

Parser::Parser(std::deque<Token*> t) {
	tokens = t;
}

bool Parser::eof() {
	return tokens.empty();
}


Expr* Parser::expression() {
	return conditional_expression();
}

Expr* Parser::conditional_expression() {
	Expr* e1 = logicalOR_expression();
	Expr* e2;
	Expr* e3;
	while(true) {
		if (match_if(Query_tok)) {
			e2 = logicalOR_expression();
		}
		else if (match_if(Colon_tok)) {
			e3 = logicalOR_expression();
			e1 = new If_expr(e1, e2, e3);
		}
		else 
			break;
	}
	return e1;
}

Expr* Parser::logicalOR_expression() {
	Expr* e1 = logicalAND_expression();
	while(true) {
		if (match_if(PipePipe_tok)) {
			Expr* e2 = logicalAND_expression();
			e1 = new Or_expr(e1, e2);
		}
		else 
			break;
	}
	return e1;
}

Expr* Parser::logicalAND_expression() {
	Expr* e1 = equality_expression();
	while(true) {
		if (match_if(AmpAmp_tok)) {
			Expr* e2 = equality_expression();
			e1 = new And_expr(e1, e2);
		}
		else 
			break;
	}
	return e1;
}

Expr* Parser::equality_expression() {
	Expr* e1 = relational_expression();
	while(true) {
		if (match_if(EqEq_tok)) {
			Expr* e2 = relational_expression();
			e1 = new Eq_expr(e1, e2);
		}
		else if (match_if(BangEq_tok)) {
			Expr* e2 = relational_expression();
			e1 = new NotEq_expr(e1, e2);
		}
		else
			break;
	}
	return e1;
}

Expr* Parser::relational_expression() {
	Expr* e1 = additive_expression();
	while(true) {
		if (match_if(Lt_tok)) {
			Expr* e2 = additive_expression();
			e1 = new Less_expr(e1, e2);
		}
		else if (match_if(Gt_tok)) {
			Expr* e2 = additive_expression();
			e1 = new Greater_expr(e1, e2);
		}
		else if (match_if(LtEq_tok)) {
			Expr* e2 = additive_expression();
			e1 = new LessOrEq_expr(e1, e2);
		}
		else if (match_if(GtEq_tok)) {
			Expr* e2 = additive_expression();
			e1 = new GreaterOrEq_expr(e1, e2);
		}
		else
			break;
	}
	return e1;
}

Expr* Parser::additive_expression() {
	Expr* e1 = multiplicative_expression();
	while(true) {
		if (match_if(Plus_tok)) {
			Expr* e2 = multiplicative_expression();
			e1 = new Add_expr(e1, e2);
		}
		else if (match_if(Minus_tok)) {
			Expr* e2 = multiplicative_expression();
			e1 = new Sub_expr(e1, e2);
		}
		else
			break;
	}
	return e1;
}

Expr* Parser::multiplicative_expression() {
	Expr* e1 = unary_expression();
	while(true) {
		if (match_if(Star_tok)) {
			Expr* e2 = unary_expression();
			e1 = new Mult_expr(e1, e2);
		}
		else if (match_if(Slash_tok)) {
			Expr* e2 = unary_expression();
			e1 = new Div_expr(e1, e2);
		}
		else if (match_if(Percent_tok)) {
			Expr* e2 = unary_expression();
			e1 = new Mod_expr(e1, e2);
		}
		else
			break;
	}
	return e1;
}

Expr* Parser::unary_expression() { // !, -
	Expr* e;
	if (match_if(Minus_tok)) {
		e = unary_expression();
		e = new Neg_expr(e);
	}
	else if (match_if(Bang_tok)) {
		e = unary_expression();
		e = new Not_expr(e);
	}
	else {
		return primary_expression();
	}
	return e;
}

Expr* Parser::primary_expression() {
	Token* t = lookahead();
	switch(t->kind) {
		case Bool_tok:{
			Bool_token* bt = dynamic_cast<Bool_token*>(t);
			consume();
			return new Bool_expr(bt->val); break;
		}
		case Int_tok: {
			Int_token* it = dynamic_cast<Int_token*>(t);
			consume();
			return new Int_expr(it->val); break;
		}
		case Lparen_tok: {
			consume();
			Expr* e = expression();
			match(Rparen_tok);
			return e;
			break;
		}
		default:
			break;
		}
	}


Token* Parser::peek() {
	if (!tokens.empty()) // If there is no more token "!tok.empty" 
		return tokens.front();
	else 
		return nullptr;
}
	

Token* Parser::lookahead() {
	if (Token* tok = peek())
		return tok;
	else { // add this back in //////////////////////////
		tok = new PuncOp_token(Eof_tok);
		return tok;
	}
}


Token* Parser::consume() {
	// make sure there is a token to consume before
	assert(!tokens.empty());

	Token* tok = tokens.front();
	tokens.pop_front();
	return tok;
}

	
Token* Parser::match(Tok_kind k) {
	Token* t = lookahead();
	if (t->kind == k)
		return consume();
	else {
		throw std::runtime_error("Expected different token.");
	}
}
	
Token* Parser::match_if(Tok_kind k) { //
	Token* t = lookahead();
	if (t->kind == k)
		return consume();
	else 
		return nullptr;
}

#endif 