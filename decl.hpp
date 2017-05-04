#pragma once

//#include "symbols.hpp"
#include <vector>

struct Type;
struct Expr;
struct Stmt;
struct Context;

struct Decl {
	virtual ~Decl() = default;
	virtual const std::string getName() = 0;

	Context *cxt;
};

struct Var_Decl : Decl { // variable declaration
	//Var_Decl(symbol* n, type* t) : name(n), ty(t), init(nullptr) { } //
	Var_Decl(Context * cont, const std::string n, const Type *t) : type(t), name(n) {
		cxt = cont;
	}
	const Type *type; // type of expr
	
	Expr* init; // evaluated expr
	Expr* init_f; // expr for print

	const std::string name;
	const std::string getName() { 
		return name;
	}
	// symbol* name; // name of decl

	// type* ty; // type of decl

	// expr* init; // initial value of decl
};

// struct Func_Decl : Decl { // preparing for function declaration
// 	Func_Decl(symbol* n, type* t) : name(n), ty(t), init(nullptr) { }

// 	symbol* name;

// 	type* ty;

// 	expr* init;
	
// };