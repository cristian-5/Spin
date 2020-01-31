
/*!
 *
 *    + --------------------------------------- +
 *    |  AST-Statements.hpp                     |
 *    |                                         |
 *    |     Abstract Syntax Tree Statements     |
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

#ifndef SPINSTATEMENTS
#define SPINSTATEMENTS

namespace Spin {

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

	ClassStatement::ClassStatement(Token * n,
								   Array<FieldStatement *> * sF,
								   Array<FieldStatement *> * dF) {
		name = n; staticFields = sF; dynamicFields = dF;
	}
	void ClassStatement::accept(Visitor * visitor) {
		try { visitor -> visitClassStatement(this); }
		catch (Exception & e) { throw; }
	}
	ClassStatement::~ClassStatement() {
		delete name;
		// TODO: Unwrap for method implementation.
		/*for (FunctionStatement * f : * functions) delete f;
		for (ProcedureStatement * p : * procedures) delete p;
		delete functions; delete procedures;*/
	}
	
	ContinueStatement::ContinueStatement(Token * c) { continueToken = c; }
	void ContinueStatement::accept(Visitor * visitor) {
		try { visitor -> visitContinueStatement(this); }
		catch (Exception & e) { throw; }
	}
	ContinueStatement::~ContinueStatement() { delete continueToken; }

	DeleteStatement::DeleteStatement(Token * n) { name = n; }
	void DeleteStatement::accept(Visitor * visitor) {
		try { visitor -> visitDeleteStatement(this); }
		catch (Exception & e) { throw; }
	}
	DeleteStatement::~DeleteStatement() { if (name) delete name; }
	
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

	FieldStatement::FieldStatement(Statement * f, Modifier m) {
		field = f; modifier = m;
	}
	void FieldStatement::accept(Visitor * visitor) {
		try { visitor -> visitFieldStatement(this); }
		catch (Exception & e) { throw; }
	}
	FieldStatement::~FieldStatement() {
		delete field;
	}

	FileStatement::FileStatement(String * n, String * f) {
		file = f; name = n;
	}
	void FileStatement::accept(Visitor * visitor) {
		try { visitor -> visitFileStatement(this); }
		catch (Exception & e) { throw; }
	}
	
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
		
	VariableStatement::VariableStatement(Token * n, Expression * i, BasicType t, Token * e, Token * o) {
		name = n; initialiser = i; type = t; equal = e; object = o;
	}
	void VariableStatement::accept(Visitor * visitor) {
		try { visitor -> visitVariableStatement(this); }
		catch (Exception & e) { throw; }
	}
	VariableStatement::~VariableStatement() {
		delete name;
		delete initialiser;
		delete equal;
		delete object;
	}

	VectorStatement::VectorStatement(Token * v, String & n, Expression * i, Token * e) {
		vector = v; name = n; initialiser = i; equal = e;
	}
	void VectorStatement::accept(Visitor * visitor) {
		try { visitor -> visitVectorStatement(this); }
		catch (Exception & e) { throw; }
	}
	VectorStatement::~VectorStatement() {
		delete vector;
		delete initialiser;
		delete equal;
	}
	
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
