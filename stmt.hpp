#pragma once

struct Expr;
struct Decl;

// base class for all statements in language
struct Stmt {
	virtual ~Stmt() = default;
};

// statement that evalutes an expr. discard result
struct expr_stmt : Stmt {
	expr_stmt(Expr* e) : ex(e) { }

	Expr* ex; 

};

// a statement that containts a decl

struct decl_stmt : Stmt {
	decl_stmt(Decl* d_) : d(d_) { }

	// declared entity
	Decl* d;
};