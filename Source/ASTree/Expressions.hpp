
/*!
 *
 *    + --------------------------------------- +
 *    |  AST-Expressions.hpp                    |
 *    |                                         |
 *    |     Abstract Syntax Tree Expressions    |
 *    |                                         |
 *    |  Created by Cristian A.                 |
 *    |  Copyright © MIT. All rights reserved.  |
 *    + --------------------------------------- +
 *
 *    Note: This software is licensed under
 *          the (MIT) Massachusetts Institute
 *          of Technology License.
 *
 */

#include "../Aliases/Prototypes.hpp"

#ifndef SPINEXPRESSIONS
#define SPINEXPRESSIONS

namespace Spin {

	Assignment::Assignment(Token * name, Expression * value, Token * o) {
		this -> name = name; this -> value = value; this -> o = o;
	}
	Object * Assignment::accept(Visitor * visitor) {
		try { return visitor -> visitAssignmentExpression(this); }
		catch (Exception & e) { throw; }
	}
	Assignment::~Assignment() { delete name; delete value; delete o; }

	Binary::Binary(Expression * ls, Token * op, Expression * rs) {
		r = rs, l = ls; o = op;
	}
	Object * Binary::accept(Visitor * visitor) {
		try { return visitor -> visitBinaryExpression(this); }
		catch (Exception & e) { throw; }
	}
	Binary::~Binary() { delete r; delete l; delete o; }

	Bra::Bra(Token * b, String & n) {
		bra = b; name = n;
	}
	Object * Bra::accept(Visitor * visitor) {
		try { return visitor -> visitBraExpression(this); }
		catch (Exception & e) { throw; }
	}
	Bra::~Bra() { delete bra; }

	Call::Call(Expression * c, Token * p, Array<Expression *> * a, Bool i) {
		parenthesis = p; callee = c; arguments = a; isConstructor = i;
	}
	Object * Call::accept(Visitor * visitor) {
		try { return visitor -> visitCallExpression(this); }
		catch (Exception & e) { throw; }
	}
	Call::~Call() {
		delete parenthesis; delete callee;
		for (Expression * argument : * arguments) {
			delete argument;
		}
		delete arguments;
	}

	Comparison::Comparison(Expression * ls, Token * op, Expression * rs) {
		r = rs, l = ls; o = op;
	}
	Object * Comparison::accept(Visitor * visitor) {
		try { return visitor -> visitComparisonExpression(this); }
		catch (Exception & e) { throw; }
	}
	Comparison::~Comparison() { delete r; delete l; delete o; }

	Get::Get(Expression * o, Token * n) {
		object = o; name = n;
	}
	Object * Get::accept(Visitor * visitor) {
		try { return visitor -> visitGetExpression(this); }
		catch (Exception & e) { throw; }
	}
	Get::~Get() { delete object; delete name; }

	Grouping::Grouping(Expression * e) { expression = e; }
	Object * Grouping::accept(Visitor * visitor) {
		try { return visitor -> visitGroupingExpression(this); }
		catch (Exception & e) { throw; }
	}
	Grouping::~Grouping() { delete expression; }

	Inner::Inner(Token * i, String & b, String & k) {
		inner = i; bra = b; ket = k;
	}
	Object * Inner::accept(Visitor * visitor) {
		try { return visitor -> visitInnerExpression(this); }
		catch (Exception & e) { throw; }
	}
	Inner::~Inner() { delete inner; }

	Ket::Ket(Token * k, String & n) {
		ket = k; name = n;
	}
	Object * Ket::accept(Visitor * visitor) {
		try { return visitor -> visitKetExpression(this); }
		catch (Exception & e) { throw; }
	}
	Ket::~Ket() { delete ket; }

	List::List(Array<Expression *> * v) { values = v; }
	Object * List::accept(Visitor * visitor) {
		try { return visitor -> visitListExpression(this); }
		catch (Exception & e) { throw; }
	}
	List::~List() {
		for (Expression * value : * values) {
			delete value;
		}
		delete values;
	}

	Literal::Literal(Token * t) { token = t; }
	Object * Literal::accept(Visitor * visitor) {
		try { return visitor -> visitLiteralExpression(this); }
		catch (Exception & e) { throw; }
	}
	Literal::~Literal() { delete token; }

	Logical::Logical(Expression * ls, Token * op, Expression * rs) {
		r = rs, l = ls; o = op;
	}
	Object * Logical::accept(Visitor * visitor) {
		try { return visitor -> visitLogicalExpression(this); }
		catch (Exception & e) { throw; }
	}
	Logical::~Logical() { delete r; delete l; delete o; }

	Mutable::Mutable(Token * name, Expression * value, Token * o) {
		this -> name = name; this -> value = value; this -> o = o;
	}
	Object * Mutable::accept(Visitor * visitor) {
		try { return visitor -> visitMutableExpression(this); }
		catch (Exception & e) { throw; }
	}
	Mutable::~Mutable() { delete name; delete value; delete o; }

	Outher::Outher(Token * o, String & k, String & b) {
		outher = o; ket = k; bra = b;
	}
	Object * Outher::accept(Visitor * visitor) {
		try { return visitor -> visitOutherExpression(this); }
		catch (Exception & e) { throw; }
	}
	Outher::~Outher() { delete outher; }
		
	Set::Set(Expression * o, Token * n, Expression * v) {
		object = o; name = n; value = v;
	}
	Object * Set::accept(Visitor * visitor) {
		try { return visitor -> visitSetExpression(this); }
		catch (Exception & e) { throw; }
	}
	Set::~Set() { delete object; delete name; delete value; }

	Subscript::Subscript(Expression * i, Token * b, Expression * e) {
		bracket = b; item = i; expression = e;
	}
	Object * Subscript::accept(Visitor * visitor) {
		try { return visitor -> visitSubscriptExpression(this); }
		catch (Exception & e) { throw; }
	}
	Subscript::~Subscript() {
		delete bracket; delete item; delete expression;
	}
		
	Super::Super(Token * k, Token * m) { keyword = k; method = m; }
	Object * Super::accept(Visitor * visitor) {
		try { return visitor -> visitSuperExpression(this); }
		catch (Exception & e) { throw; }
	}
	Super::~Super() { delete keyword; delete method; }
		
	This::This(Token * k) { keyword = k; }
	Object * This::accept(Visitor * visitor) {
		try { return visitor -> visitThisExpression(this); }
		catch (Exception & e) { throw; }
	}
	This::~This() { delete keyword; }
		
	Unary::Unary(Token * op, Expression * rs) { o = op; r = rs; }
	Object * Unary::accept(Visitor * visitor) {
		try { return visitor -> visitUnaryExpression(this); }
		catch (Exception & e) { throw; }
	}
	Unary::~Unary() { delete r; delete o; }
		
	Identifier::Identifier(Token * n) { name = n; }
	Object * Identifier::accept(Visitor * visitor) {
		try { return visitor -> visitIdentifierExpression(this); }
		catch (Exception & e) { throw; }
	}
	Identifier::~Identifier() { delete name; }

}

#endif
