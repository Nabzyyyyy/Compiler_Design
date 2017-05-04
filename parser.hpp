#ifndef PARSER_HPP
#define PARSER_HPP
#include <cstdlib>
#include <iostream>
#include <sstream>
#include "lexer.hpp"
#include "token.hpp"
#include "eval.hpp"
#include "stmt.hpp"
#include "context.hpp"
#include <deque>

struct Parser {
public:
	std::deque<Token*> tokens;


	Parser();
	Parser(std::deque<Token*> t, Context* cxt);

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
	Stmt* statement();
	Stmt* decl_statement();
	Stmt* expr_statement();
	Decl* declaration();
	Decl* var_declaration();
	Decl* var_redeclaration();
	Stmt* parse() { return statement(); }

	void print();
	const Type* parse_type();
	const std::string parse_id();

private:
	Token* peek();
	//Token* peek(int);
	Token* lookahead();
	Token* require(Tok_kind kind);
	Token* lookahead(int);
	Token* consume();
	Token* match(Tok_kind);
	Token* match_if(Tok_kind);
	bool match_if(Token* t, Tok_kind kind);

	Context* cxt;

};


Parser::Parser() {}

Parser::Parser(std::deque<Token*> _t, Context* _cxt) : tokens(_t), cxt(_cxt) {
	tokens = tokens;
}

bool Parser::eof() {
	return tokens.empty();
}

void Parser::print() {
	//std::cout << "beginning of parser::print \n";
	Stmt* s = parse();

	if (expr_stmt* exp = dynamic_cast<expr_stmt*>(s)) {
		std::cout << "Output: " << exp->ex->Eval() << "\n\n";

		//<< "Input: " << exp->ex->print() << "\n" 
		//		  << "Output: " << exp->ex->Eval() << "\n\n";
	}

	else if (decl_stmt* dec = dynamic_cast<decl_stmt*>(s)) {
		//std::cout << "Making decl_stmt \n";
		if (Var_Decl* vard = dynamic_cast<Var_Decl*>(dec->d)) {
			std::cout << "Output: " << vard->name << " = " << vard->init->Eval() << "\n\n";

				  	  //<< "Input: " << vard->name << " = " << vard->init_f->print() << "\n"
				  	  //<< "Output: " << vard->name << " = " << vard->init->Eval() << "\n\n";
		}		
	}
	//std::cout << "end of parser::print \n";
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
		// case Bool_tok:{
		// 	Bool_token* bt = dynamic_cast<Bool_token*>(t);
		// 	consume();
		// 	return new Bool_expr(bt->val, cxt); break;
		// }
		case True_kw: {
			consume();
			return new Bool_expr(true, cxt); break;
		}
		case False_kw: {
			consume();
			return new Bool_expr(false, cxt); break;
		}
		case Int_tok: {
			Int_token* it = dynamic_cast<Int_token*>(t);
			consume();
			return new Int_expr(it->val, cxt); break;
		}
		case Lparen_tok: {
			consume();
			Expr* e = expression();
			match(Rparen_tok);
			return e;
			break;
		}
		default: {
			throw std::runtime_error("parser.hpp: declaration error");
			break;
		}
	}

}


Token* Parser::peek() {
	if (!tokens.empty()) // If there is no more token "!tok.empty" 
		return tokens.front();
	else 
		return nullptr;
}

// Token* Parser::peek(int cnt) {
// 	if (n == 0) // peek(0) is the same as the peek function above
// 		return peek();

// 	if (cnt < )


// }
	

Token* Parser::lookahead() {
	if (Token* tok = peek())
		return tok;
	else { // add this back in //////////////////////////
		tok = new PuncOp_token(Eof_tok);
		return tok;
	}
}

Token* Parser::lookahead(int cnt) {
	// if (Token* tok = peek(cnt))
	// 	return tok->kind;
	// else
	// 	return Eof_tok;

	if(this->eof())
		return new PuncOp_token(Eof_tok);

	std::deque<Token*> temp = tokens;

	for (int i = 0; i < cnt; ++i) {
		temp.pop_front();
		if(temp.begin() == tokens.end())
			return nullptr;
	}
	
	return temp.front();
}


Token* Parser::consume() {
	// make sure there is a token to consume before
	assert(!tokens.empty());

	Token* tok = tokens.front();
	tokens.pop_front();
	return tok;
}

Token* Parser::require(Tok_kind k) {
	assert(lookahead()->kind == k);
	return consume();
}

Stmt* Parser::statement() {
	//std::cout << "inside parser::statement \n";
	switch(lookahead()->kind){
		case Var_kw: return decl_statement(); 
		case Id_tok:
			if(lookahead(1) && match_if(lookahead(1), Eq_tok))
				return decl_statement();
		default: 
			return expr_statement();
		}
}

Stmt* Parser::decl_statement() {
	Decl* d = declaration();
	return new decl_stmt(d);
}

Stmt* Parser::expr_statement() {
		Expr* e = expression();
		match(Semi_tok); 
		return new expr_stmt(e);

}

Decl* Parser::declaration() {
	switch(lookahead()->kind) {
		case Var_kw: return var_declaration();
		case Id_tok: return var_redeclaration();
		default: throw std::runtime_error("parser.hpp: declaration error"); break;
	}
}

Decl* Parser::var_declaration() {

	require(Var_kw);

	const Type* t = parse_type(); // to get the type of the var
	const std::string n = parse_id(); // to get the identifier for the variable

	if(cxt->find_sym(n)) // this checks for an existing variable
		throw std::runtime_error("Cannot declare two variables with the same name");

	Var_Decl* var = new Var_Decl(cxt, n, t);
	require(Eq_tok);

	Expr* e = expression();


	if(e->check() != t) { // make sure the expression and variable are of the same type.
		//std::cout << "After check \n";
		std::string ss = "parser.hpp: Cannot assign that expression to a variable of this type";
		throw std::runtime_error(ss);
	}
	//std::cout << "4  \n";
	var->init_f = e;
	var->init = e->precompute();  // store expression
	//std::cout << " 5 \n";
	match(Semi_tok); // allow matching of a semicolon
	cxt->insert_sym(var); // add the variable to the symbol table

	return var;
}

Decl* Parser::var_redeclaration() {
	Id_token* t = dynamic_cast<Id_token*>(require(Id_tok)); // get id

	if(Var_Decl* var = dynamic_cast<Var_Decl*>(cxt->find_sym(t->name))) {
		require(Eq_tok); // require an equals sign

		Expr* e = expression();

		if(e->check() != var->type) { // make sure the expression and variable are of the same type.
			std::string ss = "parser.hpp: Cannot assign that expression to a variable of this type";
			throw std::runtime_error(ss);
		}

		var->init_f = e; // store expression (print)
		var->init = e->precompute(); // store expression (calc)

		match(Semi_tok); // allow semicolon
		cxt->update_sym(var->getName(), var); // update variable on symbol table

		return var;
	}

	throw std::runtime_error("parser.hpp: variable redeclaration error");
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
	else {
		return nullptr;
	}
}

bool Parser::match_if(Token* t, Tok_kind k) {
	return t->kind == k;
}

const Type* Parser::parse_type() {
	switch(lookahead()->kind) {
		case Bool_kw: {
			consume();
			//std::cout << "parse_type returned bool type  \n";
			return &(cxt->Bool_);
		}
		case Int_kw: {
			consume();
			//std::cout << "parse_type returned int type  \n";
			return &(cxt->Int_);
		}
	}
//	std::cout << "parse_type didn't return either type \n";
	throw std::runtime_error("parser.hpp: Variable type definition failed");
}

const std::string Parser::parse_id() {
	return dynamic_cast<Id_token*>(require(Id_tok))->name;
}
#endif 