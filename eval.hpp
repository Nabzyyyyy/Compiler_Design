#ifndef AST_HPP
#define AST_HPP

#include <cassert>
#include "type.hpp"
#include <limits>
#include <stdexcept>
#include <string> 
#include <cstring>
#include "context.hpp"

//const Bool_Type Bool_; // Defining constant global objects to compare for correct types
//const Int_Type Int_;

//struct Context;

struct Expr {
	//	struct Visitor;
public:
	const Type* ExprType;
	Context* cxt;
	std::string err = "Threw exception of type invalid_type."; // defines and initializes (to most common case) a string the be passed into runetime_error when it is thrown

	virtual ~Expr() = default; // virtual destructor
	virtual int Eval() = 0; // created virtual so that it will be overriden by every inherited class
	std::string Evaluate();
	const Type* check() { // returns the type
		// if (ExprType == &(cxt->Int_))
		// 	std::cout << "check() is int type  \n";
		return ExprType;
	}

	Expr* precompute();

private:
	// virtual void accept(Visitor&) = 0;
};

// struct Expr::Visitor{
// 	virtual visit(Bool_expr*) = 0;
// 	virtual visit(And_expr*) = 0;
// 	virtual visit(Or_expr*) = 0;
// 	virtual visit(Not_expr*) = 0;
// }

struct Bool_expr : Expr {
private:
	bool val;

public:
	Bool_expr(bool b, Context* _cxt) : val(b) {		// if types are not valid, throw an exception
		cxt = _cxt;
		ExprType = &(cxt->Bool_);
	}

	int Eval() { return val; } // returns true or false as a integer of 1 or 0 respectively
};

struct Int_expr : Expr { // supports 32 bits, so (-2^31) up to ((2^31) - 1)
private:
	int val;

public:
	Int_expr(int v, Context* _cxt) : val(v) {
		cxt = _cxt;
		ExprType = &(cxt->Int_);
	}

	int Eval() { return val; }
};

struct And_expr : Expr { // returns e2 if e1 is true, false otherwise
private:
	Expr* e1;
	Expr* e2;

public:
	And_expr(Expr* e1, Expr* e2, Context* _cxt) : e1(e1), e2(e2) {
		cxt = _cxt;
		ExprType = &(cxt->Bool_);
		if (e1->check() != &(cxt->Bool_) & e2->check() != &(cxt->Bool_))		// if types are not valid, throw an exception
			throw std::runtime_error(err);
	}
	int Eval() { return e1->Eval() ? e2->Eval() : false; }	// if e1 is true, return e2, else return false;
};

struct Or_expr : Expr { // returns true if e1 is true, e2 otherwise
private:
	Expr* e1;
	Expr* e2;

public:
	Or_expr(Expr* e1, Expr* e2, Context* _cxt) : e1(e1), e2(e2) {
		cxt = _cxt;
		ExprType = &(cxt->Bool_);
		if (e1->check() != &(cxt->Bool_) & e2->check() != &(cxt->Bool_)) 	// if types are not valid, throw an exception
			throw std::runtime_error(err);
	}
	int Eval() {
		return e1->Eval() ? true : e2->Eval(); 		// if e1 is true, return true, else return e2
	}
};

// struct And_expr : Expr { // returns e1 & e2 // OLD -- SINGLE &
// private:
// 	Expr* e1;
// 	Expr* e2;

// public:
// 	And_expr(Expr* e1, Expr* e2) : e1(e1), e2(e2) {
// 		ExprType = &(cxt->Bool_);
// 		if (e1->check() != &(cxt->Bool_) | e2->check() != &(cxt->Bool_))	// if types are not valid, throw an exception
// 			throw std::runtime_error(err);
// 	}

// 	int Eval() { return e1->Eval() & e2->Eval(); }
// };

// struct Or_expr : Expr { // returns e1 | e2 // OLD -- SINGLE |
// private:
// 	Expr* e1;
// 	Expr* e2;

// public:
// 	Or_expr(Expr* e1, Expr* e2) : e1(e1), e2(e2) {		// if types are not valid, throw an exception
// 		ExprType = &(cxt->Bool_);
// 		if (e1->check() != &(cxt->Bool_) | e2->check() != &(cxt->Bool_))
// 			throw std::runtime_error(err);
// 	}

// 	int Eval() { return e1->Eval() | e2->Eval(); }

// };

struct Xor_expr : Expr { // returns e1 ^ e2
private:
	Expr* e1;
	Expr* e2;

public:
	Xor_expr(Expr* e1, Expr* e2, Context* _cxt) : e1(e1), e2(e2) {		// if types are not valid, throw an exception
		cxt = _cxt;
		ExprType = &(cxt->Bool_);
		if (e1->check() != &(cxt->Bool_) | e2->check() != &(cxt->Bool_))
			throw std::runtime_error(err);
	}

	int Eval() { return e1->Eval() ^ e2->Eval(); }
};

struct Not_expr : Expr { // returns !e1
private:
	Expr* e1;

public:
	Not_expr(Expr* e1, Context* _cxt) : e1(e1) {		// if types are not valid, throw an exception
		cxt = _cxt;
		ExprType = &(cxt->Bool_);
		if (e1->check() != &(cxt->Bool_))
			throw std::runtime_error(err);
	}
	int Eval() { return !e1->Eval(); }
};

struct If_expr : Expr { // returns e2 if e1 is true, e3 otherwise
private:
	Expr* e1;
	Expr* e2;
	Expr* e3;

public:
	If_expr(Expr* e1, Expr* e2, Expr* e3, Context* _cxt) : e1(e1), e2(e2), e3(e3) {		// if types are not valid, throw an exception
		cxt = _cxt;
		ExprType = &(cxt->Bool_);
		if (e1->check() != &(cxt->Bool_) & e2->check() != &(cxt->Bool_) & e3->check() != &(cxt->Bool_))
			throw std::runtime_error(err);
	}

	int Eval() {
		if (e1->Eval())
			return e2->Eval();
		else
			return e3->Eval();
	}
};

struct Eq_expr : Expr { // returns true if e1 == e2, false otherwise
private:
	Expr* e1;
	Expr* e2;

public:
	Eq_expr(Expr* e1, Expr* e2, Context* _cxt) : e1(e1), e2(e2) {
		cxt = _cxt;
		ExprType = &(cxt->Bool_);
		if ((e1->check() == &(cxt->Bool_) & e2->check() == &(cxt->Int_)) | (e1->check() == &(cxt->Int_) & e2->check() == &(cxt->Bool_))) {		// if types are not valid, throw an exception
			throw std::runtime_error(err);
		}
	}


	int Eval() {
		return e1->Eval() == e2->Eval();
	}
};

struct NotEq_expr : Expr { // returns true if e1 != e2, false otherwise
private:
	Expr* e1;
	Expr* e2;

public:
	NotEq_expr(Expr* e1, Expr* e2, Context* _cxt) : e1(e1), e2(e2) {
		cxt = _cxt;
		ExprType = &(cxt->Bool_);
		if ((e1->check() == &(cxt->Bool_) & e2->check() == &(cxt->Int_)) | (e1->check() == &(cxt->Int_) & e2->check() == &(cxt->Bool_)))		// if types are not valid, throw an exception
			throw std::runtime_error(err);
	}

	int Eval() {
		return e1->Eval() ^ e2->Eval();
	}
};

struct Less_expr : Expr { // returns true if e1 < e2, false otherwise
private:
	Expr* e1;
	Expr* e2;

public:
	Less_expr(Expr* e1, Expr* e2, Context* _cxt) : e1(e1), e2(e2) {
		cxt = _cxt;
		ExprType = &(cxt->Bool_);
		if (e1->check() != &(cxt->Int_) || e2->check() != &(cxt->Int_))		// if types are not valid, throw an exception
			throw std::runtime_error(err);
	}
	int Eval() {
		return (e1->Eval() < e2->Eval());
	}
};

struct Greater_expr : Expr { // returns true if e1 > e2, false otherwise
private:
	Expr* e1;
	Expr* e2;

public:
	Greater_expr(Expr* e1, Expr* e2, Context* _cxt) : e1(e1), e2(e2) {
		cxt = _cxt;
		ExprType = &(cxt->Bool_);
		if (e1->check() != &(cxt->Int_) || e2->check() != &(cxt->Int_))	// if types are not valid, throw an exception
			throw std::runtime_error(err);
	}
	int Eval() {
		return (e1->Eval() > e2->Eval());
	}
};

struct LessOrEq_expr : Expr { // returns true if e1 <= e2, false otherwise
private:
	Expr* e1;
	Expr* e2;

public:
	LessOrEq_expr(Expr* e1, Expr* e2, Context* _cxt) : e1(e1), e2(e2) {
		cxt = _cxt;
		ExprType = &(cxt->Bool_);
		if (e1->check() != &(cxt->Int_) || e2->check() != &(cxt->Int_))	// if types are not valid, throw an exception
			throw std::runtime_error(err);
	}
	int Eval() {
		return (e1->Eval() <= e2->Eval());
	}
};

struct GreaterOrEq_expr : Expr { // returns true if e1 >= e2, false otherwise
private:
	Expr* e1;
	Expr* e2;

public:
	GreaterOrEq_expr(Expr* e1, Expr* e2, Context* _cxt) : e1(e1), e2(e2) {
		cxt = _cxt;
		ExprType = &(cxt->Bool_);
		if (e1->check() != &(cxt->Int_) || e2->check() != &(cxt->Int_))	// if types are not valid, throw an exception
			throw std::runtime_error(err);
	}
	int Eval() {
		return (e1->Eval() >= e2->Eval());
	}
};

struct Add_expr : Expr { // returns e1 + e2
private:
	Expr* e1;
	Expr* e2;

public:
	Add_expr(Expr* e1, Expr* e2, Context* _cxt) : e1(e1), e2(e2) {
		cxt = _cxt;
		ExprType = &(cxt->Int_);
		if (e1->check() != &(cxt->Int_) || e2->check() != &(cxt->Int_))		// if types are not valid, throw an exception
			throw std::runtime_error(err);
	}
	int Eval() {
		if (e1->Eval() > 0 & e2->Eval() > 0 & (e1->Eval() > std::numeric_limits<int>::max() - e2->Eval())) {	// if result is out of the range supported, throw exception.
			err = "Threw exception of type overflow.";
			throw std::runtime_error(err);
		}
		else if (e1->Eval() < 0 & e2->Eval() < 0 & e1->Eval() < std::numeric_limits<int>::min() - e2->Eval()) {		// if result is out of the range supported, throw exception.
			err = "Threw exception of type overflow.";
			throw std::runtime_error(err); // std::numeric_limits<int>::max() or min
		}
		else
			return (e1->Eval() + e2->Eval());
	}
};

struct Sub_expr : Expr { // returns e1 - e2
private:
	Expr* e1;
	Expr* e2;

public:
	Sub_expr(Expr* e1, Expr* e2, Context* _cxt) : e1(e1), e2(e2) {
		cxt = _cxt;
		ExprType = &(cxt->Int_);
		if (e1->check() != &(cxt->Int_) || e2->check() != &(cxt->Int_)) 	// if types are not valid, throw an exception
			 throw std::runtime_error(err);
	}
	int Eval() {
		if (e1->Eval() > 0 & e2->Eval() < 0 & e1->Eval() > std::numeric_limits<int>::max() + e2->Eval()) {	// if result is out of the range supported, throw exception.
			err = "Threw exception of type overflow.";
			throw std::runtime_error(err);
		}
		else if (e1->Eval() < 0 & e2->Eval() > 0 & e1->Eval() < std::numeric_limits<int>::min() + e2->Eval()) {	// if result is out of the range supported, throw exception.
			err = "Threw exception of type overflow.";
			throw std::runtime_error(err);
		}
		else
			return (e1->Eval() - e2->Eval());
	}

};

struct Mult_expr : Expr { // returns e1 * e2
private:
	Expr* e1;
	Expr* e2;

public:
	Mult_expr(Expr* e1, Expr* e2, Context* _cxt) : e1(e1), e2(e2) {
		cxt = _cxt;
		ExprType = &(cxt->Int_);
		if (e1->check() != &(cxt->Int_) || e2->check() != &(cxt->Int_)) 		// if types are not valid, throw an exception
			throw std::runtime_error(err);
	}

	int Eval() {
		if (e1->Eval() == 0 | e2->Eval() == 0)
			return 0;
		else if (((e1->Eval() < 0 & e2->Eval() < 0) | (e1->Eval() > 0 & e2->Eval() > 0)) & e1->Eval() > (std::numeric_limits<int>::max() / e2->Eval())) {	// if result is out of the range supported, throw exception.
			err = "Threw exception of type overflow.";
			throw std::runtime_error(err);
		}
		else if ((e1->Eval() < 0 & e2->Eval() > 0) & e1->Eval() < std::numeric_limits<int>::min() / e2->Eval()) {	// if result is out of the range supported, throw exception.
			err = "Threw exception of type overflow.";
			throw std::runtime_error(err);
		}
		else if ((e1->Eval() > 0 & e2->Eval() < 0) & e2->Eval() < std::numeric_limits<int>::min() / e1->Eval()) {	// if result is out of the range supported, throw exception.
			err = "Threw exception of type overflow.";
			throw std::runtime_error(err);
		}
		else if (e1->Eval() == -1) {
			if (e2->Eval() == std::numeric_limits<int>::min()) {
				err = "Threw exception of type overflow.";
				throw std::runtime_error(err);
			}
			else
				return (-e2->Eval());
		}
		else if (e2->Eval() == -1) {
			if (e1->Eval() == std::numeric_limits<int>::min()) {
				err = "Threw exception of type overflow.";
				throw std::runtime_error(err);
			}
			else
				return (-e1->Eval());
			
		}
		else
			return (e1->Eval() * e2->Eval());
	}
};

struct Div_expr : Expr { // returns e1 / e2
private:
	Expr* e1;
	Expr* e2;

public:
	Div_expr(Expr* e1, Expr* e2, Context* _cxt) : e1(e1), e2(e2) {
		cxt = _cxt;
		ExprType = &(cxt->Int_);
		if (e1->check() != &(cxt->Int_) || e2->check() != &(cxt->Int_))	// if types are not valid, throw an exception
			throw std::runtime_error(err);
	}
	int Eval() {
		if (e2->Eval() == 0) {
			err = "Threw exception of type Div_by_zero.";
			throw std::runtime_error(err);
		}
		else if (e2->Eval() == std::numeric_limits<int>::min()) {
			err = "Threw exception of type Div_by_min.";
			throw std::runtime_error(err);
		}
		// if (e1 / e2 > 4294967296) 				// check for divide by zero and such
		// 	throw std::runtime_error(err);				// 
		// else if (e1 / e2 < -4294967295)
		// 	throw std::runtime_error(err);
		else
			return (e1->Eval() / e2->Eval());
	}
};

struct Mod_expr : Expr { // returns e1 % e2
private:
	Expr* e1;
	Expr* e2;

public:
	Mod_expr(Expr* e1, Expr* e2, Context* _cxt) : e1(e1), e2(e2) {
		cxt = _cxt;
		ExprType = &(cxt->Int_);
		if (e1->check() != &(cxt->Int_) || e2->check() != &(cxt->Int_))		// if types are not valid, throw an exception
			throw std::runtime_error(err);
	}
	int Eval() {
		if (e2->Eval() == 0) {
			err = "Threw exception of type Div_by_zero.";
			throw std::runtime_error(err);
		}
		else if (e2->Eval() == std::numeric_limits<int>::min()) {
			err = "Threw exception of type Div_by_min.";
			throw std::runtime_error(err);
		}
		else
			return (e1->Eval() % e2->Eval());
	}
};

struct Neg_expr : Expr { // returns -(e1)
private:
	Expr* e1;

public:
	Neg_expr(Expr* e1, Context* _cxt) : e1(e1) {
		cxt = _cxt;
		ExprType = &(cxt->Int_);
		if (e1->check() != &(cxt->Int_))		// if types are not valid, throw an exception
			throw std::runtime_error(err);
	}
	int Eval() {
		if (e1->Eval() == std::numeric_limits<int>::min()) {		// this checks to see if 2^32 is the value being negated. That is the only positive number supported that could be
			err = "Threw exception of type overflow.";
			throw std::runtime_error(err);			// negated that is out of the range supported by this compiler
		}
		else
			return (0 - e1->Eval());
	}
};

std::string Expr::Evaluate() {
	if (check() == &(cxt->Bool_)) {
		return Eval() ? "true" : "false";
	}
	else 
		return std::to_string(Eval());
	// if (check() == &(cxt->Bool_))
	// 	return Eval() ? "true" : "false";
	// else if (check() == &(cxt->Int_))
	// 	return std::to_string(Eval());
	// else
	// 	throw std::runtime_error("Evaluate error");
}

Expr* Expr::precompute() {
	Expr* e;

	if(check() == &(cxt->Bool_))
		e = new Bool_expr(Eval(), cxt);
	else if(check() == &cxt->Int_)
		e = new Int_expr(Eval(), cxt);
	else
		throw std::runtime_error("eval.hpp: precompute error");

	return e;
}


#endif

// struct Context
// {
// 	Bool_Type bool_type;
// 	Int_Type int_type;
// };
