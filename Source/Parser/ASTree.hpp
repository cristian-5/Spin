
/*!
 *
 *    + --------------------------------------- +
 *    |  ASTree.hpp                             |
 *    |                                         |
 *    |           Abstract Syntax Tree          |
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

#ifndef STACKASTREE
#define STACKASTREE

namespace Stack {

	/* Expressions */

	Assignment::Assignment(Token * name, Expression * value) {
		this -> name = name;
		this -> value = value;
	}
	void Assignment::accept(Visitor * visitor) {
		try { visitor -> visitAssignmentExpression(this); }
		catch (Exception & e) { throw; }
	}
	Assignment::~Assignment() { delete name; delete value; }

	Binary::Binary(Expression * ls, Token * op, Expression * rs) {
		r = rs, l = ls; o = op;
	}
	void Binary::accept(Visitor * visitor) {
		try { visitor -> visitBinaryExpression(this); }
		catch (Exception & e) { throw; }
	}
	Binary::~Binary() { delete r; delete l; delete o; }

	Call::Call(Expression * c, Token * p, Array<Expression *> * a) {
		parenthesis = p; callee = c; arguments = a;
	}
	void Call::accept(Visitor * visitor) {
		try { visitor -> visitCallExpression(this); }
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
	void Comparison::accept(Visitor * visitor) {
		try { visitor -> visitComparisonExpression(this); }
		catch (Exception & e) { throw; }
	}
	Comparison::~Comparison() { delete r; delete l; delete o; }

	Get::Get(Expression * o, Token * n) {
		object = o; name = n;
	}
	void Get::accept(Visitor * visitor) {
		try { visitor -> visitGetExpression(this); }
		catch (Exception & e) { throw; }
	}
	Get::~Get() { delete object; delete name; }

	Grouping::Grouping(Expression * e) { expression = e; }
	void Grouping::accept(Visitor * visitor) {
		try { visitor -> visitGroupingExpression(this); }
		catch (Exception & e) { throw; }
	}
	Grouping::~Grouping() { delete expression; }

	List::List(Array<Expression *> * v) { values = v; }
	void List::accept(Visitor * visitor) {
		try { visitor -> visitListExpression(this); }
		catch (Exception & e) { throw; }
	}
	List::~List() {
		for (Expression * value : * values) {
			delete value;
		}
		delete values;
	}

	Literal::Literal(Token * t) { token = t; }
	void Literal::accept(Visitor * visitor) {
		try { visitor -> visitLiteralExpression(this); }
		catch (Exception & e) { throw; }
	}
	Literal::~Literal() { delete token; }

	Logical::Logical(Expression * ls, Token * op, Expression * rs) {
		r = rs, l = ls; o = op;
	}
	void Logical::accept(Visitor * visitor) {
		try { visitor -> visitLogicalExpression(this); }
		catch (Exception & e) { throw; }
	}
	Logical::~Logical() { delete r; delete l; delete o; }
		
	Set::Set(Expression * o, Token * n, Expression * v) {
		object = o; name = n; value = v;
	}
	void Set::accept(Visitor * visitor) {
		try { visitor -> visitSetExpression(this); }
		catch (Exception & e) { throw; }
	}
	Set::~Set() { delete object; delete name; delete value; }

	Subscript::Subscript(Expression * i, Token * b, Expression * e) {
		bracket = b; item = i; expression = e;
	}
	void Subscript::accept(Visitor * visitor) {
		try { visitor -> visitSubscriptExpression(this); }
		catch (Exception & e) { throw; }
	}
	Subscript::~Subscript() {
		delete bracket; delete item; delete expression;
	}
		
	Super::Super(Token * k, Token * m) { keyword = k; method = m; }
	void Super::accept(Visitor * visitor) {
		try { visitor -> visitSuperExpression(this); }
		catch (Exception & e) { throw; }
	}
	Super::~Super() { delete keyword; delete method; }
		
	This::This(Token * k) { keyword = k; }
	void This::accept(Visitor * visitor) {
		try { visitor -> visitThisExpression(this); }
		catch (Exception & e) { throw; }
	}
	This::~This() { delete keyword; }
		
	Unary::Unary(Token * op, Expression * rs) { o = op; r = rs; }
	void Unary::accept(Visitor * visitor) {
		try { visitor -> visitUnaryExpression(this); }
		catch (Exception & e) { throw; }
	}
	Unary::~Unary() { delete r; delete o; }
		
	Identifier::Identifier(Token * n) { name = n; }
	void Identifier::accept(Visitor * visitor) {
		try { visitor -> visitIdentifierExpression(this); }
		catch (Exception & e) { throw; }
	}
	Identifier::~Identifier() { delete name; }


	/* Core Unwinding */

	Parameter::Parameter(BasicType bt, Token * tt, Token * nm, Bool rf) {
		type = bt; tokenType = tt;
		name = nm; reference = rf;
	}
	Parameter * Parameter::copy() const {
		return new Parameter(
			type, new Token(* tokenType),
			new Token(* name), reference
		);
	}
	Parameter::~Parameter() {
		if (tokenType) delete tokenType;
		if (name) delete name;
	}

	/* Statements */

	BlockStatement::BlockStatement(Array<Statement *> * s) { statements = s; }
	BlockStatement::BlockStatement(Statement * s) {
		statements = new Array<Statement *>();
		statements -> push(s);
	}
	void BlockStatement::accept(Visitor * visitor) {
		try { visitor -> visitBlockStatement(this); }
		catch (Exception & e) { throw; }
	}
	BlockStatement::~BlockStatement() {
		for (Statement * statement : * statements) {
			delete statement;
		}
		delete statements;
	}

	BreakStatement::BreakStatement(Token * b) { breakToken = b; }
	void BreakStatement::accept(Visitor * visitor) {
		try { visitor -> visitBreakStatement(this); }
		catch (Exception & e) { throw; }
	}
	BreakStatement::~BreakStatement() { delete breakToken; }
	
	ContinueStatement::ContinueStatement(Token * c) { continueToken = c; }
	void ContinueStatement::accept(Visitor * visitor) {
		try { visitor -> visitContinueStatement(this); }
		catch (Exception & e) { throw; }
	}
	ContinueStatement::~ContinueStatement() { delete continueToken; }
	
	DoWhileStatement::DoWhileStatement(Statement * b, Expression * e, Token * w) {
		expression = e; body = b; whileToken = w;
	}
	void DoWhileStatement::accept(Visitor * visitor) {
		try { visitor -> visitDoWhileStatement(this); }
		catch (Exception & e) { throw; }
	}
	DoWhileStatement::~DoWhileStatement() {
		delete whileToken;
		delete expression;
		delete body;
	}
	
	ExpressionStatement::ExpressionStatement(Expression * ex) { e = ex; }
	void ExpressionStatement::accept(Visitor * visitor) {
		try { visitor -> visitExpressionStatement(this); }
		catch (Exception & e) { throw; }
	}
	ExpressionStatement::~ExpressionStatement() { delete e; }
	
	ForStatement::ForStatement(Statement * d, Expression * e,
					Expression * s, Statement * b, Token * f) {
		declaration = d; expression = e;
		stepper = s; body = b; forToken = f;
	}
	void ForStatement::accept(Visitor * visitor) {
		try { visitor -> visitForStatement(this); }
		catch (Exception & e) { throw; }
	}
	ForStatement::~ForStatement() {
		delete forToken;
		delete declaration;
		delete expression;
		delete stepper;
		delete body;
	}
	
	FunctionStatement::FunctionStatement(Token * n, Array<Parameter *> * p, BlockStatement * b, Parameter * r) {
		name = n, params = p; body = b; returnType = r;
	}
	void FunctionStatement::accept(Visitor * visitor) {
		try { visitor -> visitFunctionStatement(this); }
		catch (Exception & e) { throw; }
	}
	FunctionStatement::~FunctionStatement() {
		delete name; delete returnType; delete body;
		for (Parameter * param : * params) delete param;
		delete params;
	}
	
	IfStatement::IfStatement(Expression * x, Statement * t,
				Statement * e, Token * i) {
		expression = x; thenBranch = t;
		elseBranch = e; ifToken = i;
	}
	void IfStatement::accept(Visitor * visitor) {
		try { visitor -> visitIfStatement(this); }
		catch (Exception & e) { throw; }
	}
	IfStatement::~IfStatement() {
		delete expression;
		delete thenBranch;
		delete elseBranch;
	}
	
	LoopStatement::LoopStatement(Statement * b, Token * l) {
		body = b; loopToken = l;
	}
	void LoopStatement::accept(Visitor * visitor) {
		try { visitor -> visitLoopStatement(this); }
		catch (Exception & e) { throw; }
	}
	LoopStatement::~LoopStatement() {
		delete loopToken;
		delete body;
	}
	
	PrintStatement::PrintStatement(Expression * ex) { e = ex; }
	void PrintStatement::accept(Visitor * visitor) {
		try { visitor -> visitPrintStatement(this); }
		catch (Exception & e) { throw; }
	}
	PrintStatement::~PrintStatement() { delete e; }

	ProcedureStatement::ProcedureStatement(Token * n, Array<Parameter *> * p, BlockStatement * b) {
		name = n, params = p; body = b;
	}
	void ProcedureStatement::accept(Visitor * visitor) {
		try { visitor -> visitProcedureStatement(this); }
		catch (Exception & e) { throw; }
	}
	ProcedureStatement::~ProcedureStatement() {
		delete name; delete body;
		for (Parameter * param : * params) delete param;
		delete params;
	}
	
	RepeatUntilStatement::RepeatUntilStatement(Statement * b, Expression * e, Token * u) {
		expression = e; body = b; untilToken = u;
	}
	void RepeatUntilStatement::accept(Visitor * visitor) {
		try { visitor -> visitRepeatUntilStatement(this); }
		catch (Exception & e) { throw; }
	}
	RepeatUntilStatement::~RepeatUntilStatement() {
		delete untilToken;
		delete expression;
		delete body;
	}
		
	void RestStatement::accept(Visitor * visitor) {
		try { visitor -> visitRestStatement(this); }
		catch (Exception & e) { throw; }
	}

	ReturnStatement::ReturnStatement(Expression * ex, Token * rt) {
		e = ex; returnToken = rt;
	}
	void ReturnStatement::accept(Visitor * visitor) {
		try { visitor -> visitReturnStatement(this); }
		catch (Exception & e) { throw; }
	}
	ReturnStatement::~ReturnStatement() {
		if (e) delete e;
		if (returnToken) delete returnToken;
	}
		
	UntilStatement::UntilStatement(Expression * e, Statement * b, Token * u) {
		expression = e; body = b; untilToken = u;
	}
	void UntilStatement::accept(Visitor * visitor) {
		try { visitor -> visitUntilStatement(this); }
		catch (Exception & e) { throw; }
	}
	UntilStatement::~UntilStatement() {
		delete untilToken;
		delete expression;
		delete body;
	}
		
	VariableStatement::VariableStatement(Token * n, Expression * i, BasicType t) {
		name = n; initializer = i; type = t;
	}
	void VariableStatement::accept(Visitor * visitor) {
		try { visitor -> visitVariableStatement(this); }
		catch (Exception & e) { throw; }
	}
	VariableStatement::~VariableStatement() { delete name; }
	
	WhileStatement::WhileStatement(Expression * e, Statement * b, Token * w) {
		expression = e; body = b; whileToken = w;
	}
	void WhileStatement::accept(Visitor * visitor) {
		try { visitor -> visitWhileStatement(this); }
		catch (Exception & e) { throw; }
	}
	WhileStatement::~WhileStatement() {
		delete whileToken;
		delete expression;
		delete body;
	}

}

#endif
