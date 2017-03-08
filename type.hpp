#ifndef TYPE_HPP
#define TYPE_HPP

// virtual struct ~Type {}
// struct Bool_Type: Type {}
// struct Int_Type: Type {}

struct Type {
//	struct Visitor;
	virtual ~Type() = default;
//	virtual void accept(Visitor&) {}
};

struct Bool_Type : Type {};
struct Int_Type : Type {};

#endif

// bool needs_parens(Expr* e)
// {
// 	struct V : Visitor {
// 		bool r;
// 		void visit(Bool_expr* e) { r = false; }
// 		void visit(And_expr* e) { r = true; }
// 		void visit(Or_expr* e) { r = true; }
// 		void visit(Nor_expr* e) { r = true; }
// 	}
// }


// #include "ast.hpp"

// #include <iostream>



// check(Context* cxt, Expr* e)
// {
// 	struct V : Expr::visitor {
// 		Context& cxt;
// 		Type* r;
// 		V(Context& c) : cxt(c) {}
		
// 		void visit(Bool_expr* e) {
// 			r = &cxt.bool_type;
// 		}
// 		void visit(And_expr* e) {
// 			// check that e->e1 is bool (check stuff by refernce. Check reference of context like below this)
// 			// check that e->e2 is bool
// 			r = &cxt.bool_type;
// 		}
// 		void visit(Or_expr* e) {
// 			// check that e->e1 is bool
// 			// check that e->e2 is bool
// 			r = &cxt.bool_type;
// 		}
// 		void visit(Not_expr* e) {
// 			// check that e->e1 is bool
// 			r = &cxt.bool_type;
// 		}
	
// 	}
	
// 	V vis(cxt);
// 	e->accept(vis);
// 	return vis.r;
	
// }

//include guards

// virtual ~type destructor
//struct BOOL TYPE
//struct INT TYPE