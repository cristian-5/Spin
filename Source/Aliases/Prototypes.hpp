
/*!
 *
 *    + --------------------------------------- +
 *    |  Prototypes.hpp                         |
 *    |                                         |
 *    |                Prototypes               |
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

#include "Aliases.hpp"

#ifndef PROTOTYPES
#define PROTOTYPES

namespace Spin {

	/* Exception */

	class Exception { };

	/* Linker */

	class BadFileException: public Exception {
		private:
		const String path;
		public:
		const String & getPath() const { return path; }
		BadFileException(const String & p):
		Exception(), path(p) { }
	};
	class BadAccessException: public Exception {
		private:
		const String path;
		public:
		const String & getPath() const { return path; }
		BadAccessException(const String & p):
		Exception(), path(p) { }
	};
	class Linker {
		private:
		Linker() = default;
		public:
		static UInt64 getLine(String * input, SizeType cursor);
		static String * stringFromFile(String path);
		static void createNewFile(String path, String content = String());
	};

	/* Token */

    enum TokenType {

		beginFile,

		empty,
		comment,

		basicType,
		customType,
		symbol,
		identifier,

		braketSymbol,
		braSymbol,
		measureSymbol,
		ketSymbol,
		ketbraSymbol,

		intLiteral,
		imaginaryLiteral,
		basisBraLiteral,
		basisKetLiteral,
		realLiteral,
		stringLiteral,
		charLiteral,
		boolLiteral,
		colourLiteral,
		emptyLiteral,

		arrow,
		doublecolon,
		semicolon,
		colon,
		comma,
		dot,

		virtualOperator,

		/* Low Priority */

		plusEqual,
		minusEqual,
		pipeEqual,
		plus,
		minus,
		pipe,

		/* Medium Priority */

		starEqual,
		slashEqual,
		ampersandEqual,
		daggerEqual,
		modulusEqual,
		star,
		slash,
		ampersand,
		dagger,
		modulus,

		minor,
		minorEqual,
		major,
		majorEqual,
		equal,
		equality,
		inequality,
		questionMark,
		exclamationMark,

		AND,
		OR,

		tilde,
		backslash,

		openParenthesis,
		closeParenthesis,

		openBracket,
		closeBracket,

		openBrace,
		closeBrace,

		ifKeyword,
		elseKeyword,
		switchKeyword,
		caseKeyword,
		defaultKeyword,
		whileKeyword,
		doKeyword,
		loopKeyword,
		forKeyword,
		repeatKeyword,
		untilKeyword,
		breakKeyword,
		continueKeyword,

		printKeyword,

		importKeyword,
		funcKeyword,
		procKeyword,
		staticKeyword,
		classKeyword,
		structKeyword,
		enumKeyword,
		privateKeyword,
		publicKeyword,
		constKeyword,
		returnKeyword,
		restKeyword,

		refKeyword,
		cpyKeyword,

		newKeyword,
		deleteKeyword,

		invalid,

		endFile

	};
	class Token {
		public:
		TokenType type = TokenType::empty;
		String lexeme;
		SizeType position = 0;
		Token() = default;
		Token(String l, TokenType t, SizeType p = 0);
		Bool isTypeLiteral() const;
		Bool isTypeNumeral() const;
		Bool isTypeBasicType() const;
		Bool isTypeType() const;
		Bool isTypeBraKet() const;
	};
	class TokenRule {
		public:
		TokenType type = TokenType::empty;
		Regex pattern;
		TokenRule() = default;
		TokenRule(Regex p, TokenType t);
	};

	/* Basic Types */

	enum BasicType: UInt8 {

		BoolType,
		CharacterType,

		ByteType,
		
		Int64Type,

		RealType,

		ColourType,

		ImaginaryType,
		ComplexType,

		StringType,

		ArrayType,

		VectorType,

		FunctionType,

		ClassType,
		InstanceType,
		StructureType,
		ExceptionType,

		UnknownType

	};

	using BasicTypes = UInt16;

	/* Converter */

	class Object;
	class Colour;
	class CallProtocol;

	class Converter {
		private:
		Converter() = default;
		static inline Bool checkBase(Regex base, String s);
		static inline Bool test(Regex r, String s);
		static Bool isHexChar(Character c);
		static Character hexToChar(String & s);
		static Int64 decToInt64(String & s);
		static UInt8 charToHex(Character & c);
		static Int64 hexToInt64(String & s);
		static UInt32 hexToUInt32(String & s);
		static Int64 octToInt64(String & s);
		static Int64 binToInt64(String & s);
		public:
		static Object * literalToObject(Token * t);
		static Bool stringToBool(String & s);
		static Int64 stringToInt64(String & s);
		static Real stringToReal(String & s);
		static Real stringToImaginary(String & s);
		static String escapeString(String & s);
		static Colour stringToColour(String & s);
		static Character escapeChar(String & s);
		static BasicType typeFromString(String s);
	};

	/* Environment */

	class VariableRedefinitionException: public Exception {
		private:
		const String _type;
		public:
		const String & getType() const { return _type; }
		VariableRedefinitionException(String type):
		Exception(), _type(type) { }
	};
	class VariableNotFoundException: public Exception {
		public: VariableNotFoundException(): Exception() { }
	};
	class Environment {
		private:
		Environment * enclosing = nullptr;
		Dictionary<String, Object *> values = Dictionary<String, Object *>();
		public:
		Environment() = default;
		Environment(Environment * enclosing);
		~Environment();
		void define(String name, Object * value);
		void forget(String name);
		Object * getReference(String name);
		Object * getValue(String name);
	};

	/* Base Classes */

	class Assignment;
	class Binary;
	class Bra;
	class Call;
	class Comparison;
	class Get;
	class Grouping;
	class Ket;
	class List;
	class Literal;
	class Logical;
	class Mutable;
	class Set;
	class Subscript;
	class Super;
	class This;
	class Unary;
	class Identifier;

	class BlockStatement;
	class BreakStatement;
	class ClassStatement;
	class ContinueStatement;
	class DeleteStatement;
	class DoWhileStatement;
	class ExpressionStatement;
	class FileStatement;
	class ForStatement;
	class FunctionStatement;
	class IfStatement;
	class LoopStatement;	
	class PrintStatement;
	class ProcedureStatement;
	class RepeatUntilStatement;
	class RestStatement;
	class ReturnStatement;
	class UntilStatement;
	class VariableStatement;
	class WhileStatement;

	class Expression {
		public:
		virtual ~Expression() = default;
		class Visitor {
			public:
			virtual Object * visitAssignmentExpression(Assignment * e) = 0;
			virtual Object * visitBinaryExpression(Binary * e) = 0;
			virtual Object * visitBraExpression(Bra * e) = 0;
			virtual Object * visitCallExpression(Call * e) = 0;
			virtual Object * visitComparisonExpression(Comparison * e) = 0;
			virtual Object * visitGetExpression(Get * e) = 0;
			virtual Object * visitGroupingExpression(Grouping * e) = 0;
			virtual Object * visitKetExpression(Ket * e) = 0;
			virtual Object * visitListExpression(List * e) = 0;
			virtual Object * visitLiteralExpression(Literal * e) = 0;
			virtual Object * visitLogicalExpression(Logical * e) = 0;
			virtual Object * visitMutableExpression(Mutable * e) = 0;
			virtual Object * visitSetExpression(Set * e) = 0;
			virtual Object * visitSubscriptExpression(Subscript * e) = 0;
			virtual Object * visitSuperExpression(Super * e) = 0;
			virtual Object * visitThisExpression(This * e) = 0;
			virtual Object * visitUnaryExpression(Unary * e) = 0;
			virtual Object * visitIdentifierExpression(Identifier * e) = 0;
		};
		virtual Object * accept(Visitor *) { return nullptr; }
		template<typename t>
		Bool isInstanceOf() {
			return (dynamic<t *>(this));
		}
	};
	class Statement {
		public:
		virtual ~Statement() = default;
		class Visitor {
			public:
			virtual void visitBlockStatement(BlockStatement * e) = 0;
			virtual void visitBreakStatement(BreakStatement * e) = 0;
			virtual void visitClassStatement(ClassStatement * e) = 0;
			virtual void visitContinueStatement(ContinueStatement * e) = 0;
			virtual void visitDeleteStatement(DeleteStatement * e) = 0;
			virtual void visitDoWhileStatement(DoWhileStatement * e) = 0;
			virtual void visitExpressionStatement(ExpressionStatement * e) = 0;
			virtual void visitFileStatement(FileStatement * e) = 0;
			virtual void visitForStatement(ForStatement * e) = 0;
			virtual void visitFunctionStatement(FunctionStatement * e) = 0;
			virtual void visitIfStatement(IfStatement * e) = 0;
			virtual void visitLoopStatement(LoopStatement * e) = 0;
			virtual void visitPrintStatement(PrintStatement * e) = 0;
			virtual void visitProcedureStatement(ProcedureStatement * e) = 0;
			virtual void visitRepeatUntilStatement(RepeatUntilStatement * e) = 0;
			virtual void visitRestStatement(RestStatement * e) = 0;
			virtual void visitReturnStatement(ReturnStatement * e) = 0;
			virtual void visitUntilStatement(UntilStatement * e) = 0;
			virtual void visitVariableStatement(VariableStatement * e) = 0;
			virtual void visitWhileStatement(WhileStatement * e) = 0;
		};
		virtual void accept(Visitor *) { }
		template<typename t>
		Bool isInstanceOf() {
			return (dynamic<t *>(this));
		}
	};

	/* ASTree */

	class Assignment: public Expression {
		public:
		Token * name = nullptr;
		Expression * value = nullptr;
		Token * o = nullptr;
		Assignment(Token * name, Expression * value, Token * o);
		Object * accept(Visitor * visitor) override;
		~Assignment();
	};
	class Binary: public Expression {
		public:
		Expression * r = nullptr;
		Expression * l = nullptr;
		Token * o = nullptr;
		Binary(Expression * ls, Token * op, Expression * rs);
		Object * accept(Visitor * visitor) override;
		~Binary();
	};
	class Bra: public Expression {
		public:
		String * n = nullptr;
		Token * o = nullptr;
		Bra(String * nm, Token * op);
		Object * accept(Visitor * visitor) override;
		~Bra();
	};
	class Call: public Expression {
		public:
		Token * parenthesis = nullptr;
		Expression * callee = nullptr;
		Array<Expression *> * arguments = nullptr;
		Bool isConstructor = false;
		Call(Expression * c, Token * p, Array<Expression *> * a, Bool i);
		Object * accept(Visitor * visitor) override;
		~Call();
	};
	class Comparison: public Expression {
		public:
		Expression * r = nullptr;
		Expression * l = nullptr;
		Token * o = nullptr;
		Comparison(Expression * ls, Token * op, Expression * rs);
		Object * accept(Visitor * visitor) override;
		~Comparison();
	};
	class Get: public Expression {
		public:
		Expression * object = nullptr;
		Token * name = nullptr;
		Get(Expression * o, Token * n);
		Object * accept(Visitor * visitor) override;
		~Get();
	};
	class Grouping: public Expression {
		public:
		Expression * expression = nullptr;
		Grouping(Expression * e);
		Object * accept(Visitor * visitor) override;
		~Grouping();
	};
	class Ket: public Expression {
		public:
		String * n = nullptr;
		Token * o = nullptr;
		Ket(String * nm, Token * op);
		Object * accept(Visitor * visitor) override;
		~Ket();
	};
	class List: public Expression {
		public:
		Array<Expression *> * values = nullptr;
		List(Array<Expression *> * v);
		Object * accept(Visitor * visitor) override;
		~List();
	};
	class Literal: public Expression {
		public:
		Token * token = nullptr;
		Object * object = nullptr;
		Literal(Token * t);
		Object * accept(Visitor * visitor) override;
		~Literal();
	};
	class Logical: public Expression {
		public:
		Expression * r = nullptr;
		Expression * l = nullptr;
		Token * o = nullptr;
		Logical(Expression * ls, Token * op, Expression * rs);
		Object * accept(Visitor * visitor) override;
		~Logical();
	};
	class Mutable: public Expression {
		public:
		Token * name = nullptr;
		Expression * value = nullptr;
		Token * o = nullptr;
		Mutable(Token * name, Expression * value, Token * o);
		Object * accept(Visitor * visitor) override;
		~Mutable();
	};
	class Set: public Expression {
		public:
		Expression * object = nullptr;
		Token * name = nullptr;
		Expression * value = nullptr;
		Set(Expression * o, Token * n, Expression * v);
		Object * accept(Visitor * visitor) override;
		~Set();
	};
	class Subscript: public Expression {
		public:
		Token * bracket = nullptr;
		Expression * item = nullptr;
		Expression * expression = nullptr;
		Subscript(Expression * i, Token * b, Expression * e);
		Object * accept(Visitor * visitor) override;
		~Subscript();
	};
	class Super: public Expression {
		public:
		Token * keyword = nullptr;
		Token * method = nullptr;
		Super(Token * k, Token * m);
		Object * accept(Visitor * visitor) override;
		~Super();
	};
	class This: public Expression {
		public:
		Token * keyword = nullptr;
		This(Token * k);
		Object * accept(Visitor * visitor) override;
		~This();
	};
	class Unary: public Expression {
		public:
		Expression * r = nullptr;
		Token * o = nullptr;
		Unary(Token * op, Expression * rs);
		Object * accept(Visitor * visitor) override;
		~Unary();
	};
	class Identifier: public Expression {
		public:
		Token * name = nullptr;
		Identifier(Token * n);
		Object * accept(Visitor * visitor) override;
		~Identifier();
	};

	class Parameter {
		public:
		Bool reference = false;
		BasicType type = BasicType::UnknownType;
		Token * tokenType = nullptr;
		Token * name = nullptr;
		Parameter() = default;
		Parameter(BasicType bt, Token * tt, Token * nm, Bool rf = false) {
			type = bt; tokenType = tt; name = nm; reference = rf;
		}
		Parameter * copy() const {
			return new Parameter(
				type, new Token(* tokenType),
				new Token(* name), reference
			);
		}
		~Parameter() {
			if (tokenType) delete tokenType;
			if (name) delete name;
		}
	};

	class BlockStatement: public Statement {
		public:
		Array<Statement *> * statements = nullptr;
		BlockStatement(Array<Statement *> * s);
		BlockStatement(Statement * s);
		void accept(Visitor * visitor) override;
		~BlockStatement();
	};
	class BreakStatement: public Statement {
		public:
		Token * breakToken = nullptr;
		BreakStatement(Token * b);
		void accept(Visitor * visitor) override;
		~BreakStatement();
	};
	class ClassStatement: public Statement {
		public:
		Token * name = nullptr;
		Array<CallProtocol *> * methods = nullptr;
		ClassStatement(Token * n, Array<CallProtocol *> * m);
		void accept(Visitor * visitor) override;
		~ClassStatement();
	};
	class ContinueStatement: public Statement {
		public:
		Token * continueToken = nullptr;
		ContinueStatement(Token * c);
		void accept(Visitor * visitor) override;
		~ContinueStatement();
	};
	class DeleteStatement: public Statement {
		public:
		Token * name = nullptr;
		DeleteStatement(Token * n);
		void accept(Visitor * visitor) override;
		~DeleteStatement();
	};
	class DoWhileStatement: public Statement {
		public:
		Token * whileToken = nullptr;
		Expression * expression = nullptr;
		Statement * body = nullptr;
		DoWhileStatement(Statement * b, Expression * e, Token * w);
		void accept(Visitor * visitor) override;
		~DoWhileStatement();
	};
	class ExpressionStatement: public Statement {
		public:
		Expression * e = nullptr;
		ExpressionStatement(Expression * ex);
		void accept(Visitor * visitor) override;
		~ExpressionStatement();
	};
	class FileStatement: public Statement {
		public:
		String * file = nullptr;
		String * name = nullptr;
		FileStatement(String * n, String * f);
		void accept(Visitor * visitor) override;
	};
	class ForStatement: public Statement {
		public:
		Token * forToken = nullptr;
		Statement * declaration = nullptr;
		Expression * expression = nullptr;
		Expression * stepper = nullptr;
		Statement * body = nullptr;
		ForStatement(Statement * d, Expression * e, Expression * s, Statement * b, Token * f);
		void accept(Visitor * visitor) override;
		~ForStatement();
	};
	class FunctionStatement: public Statement {
		public:
		Token * name = nullptr;
		Array<Parameter *> * params = nullptr;
		BlockStatement * body = nullptr;
		Parameter * returnType = nullptr;
		FunctionStatement(Token * n, Array<Parameter *> * p, BlockStatement * b, Parameter * r);
		void accept(Visitor * visitor) override;
		~FunctionStatement();
	};
	class IfStatement: public Statement {
		public:
		Token * ifToken = nullptr;
		Expression * expression = nullptr;
		Statement * thenBranch = nullptr;
		Statement * elseBranch = nullptr;
		IfStatement(Expression * x, Statement * t, Statement * e, Token * i);
		void accept(Visitor * visitor) override;
		~IfStatement();
	};
	class LoopStatement: public Statement {
		public:
		Token * loopToken = nullptr;
		Statement * body = nullptr;
		LoopStatement(Statement * b, Token * l);
		void accept(Visitor * visitor) override;
		~LoopStatement();
	};
	class PrintStatement: public Statement {
		public:
		Expression * e = nullptr;
		PrintStatement(Expression * ex);
		void accept(Visitor * visitor) override;
		~PrintStatement();
	};
	class ProcedureStatement: public Statement {
		public:
		Token * name = nullptr;
		Array<Parameter *> * params = nullptr;
		BlockStatement * body = nullptr;
		ProcedureStatement(Token * n, Array<Parameter *> * p, BlockStatement * b);
		void accept(Visitor * visitor) override;
		~ProcedureStatement();
	};
	class RepeatUntilStatement: public Statement {
		public:
		Token * untilToken = nullptr;
		Statement * body = nullptr;
		Expression * expression = nullptr;
		RepeatUntilStatement(Statement * b, Expression * e, Token * u);
		void accept(Visitor * visitor) override;
		~RepeatUntilStatement();
	};
	class RestStatement: public Statement {
		public:
		RestStatement() = default;
		void accept(Visitor * visitor) override;
	};
	class ReturnStatement: public Statement {
		public:
		Token * returnToken = nullptr;
		Expression * e = nullptr;
		ReturnStatement(Expression * ex, Token * rt);
		void accept(Visitor * visitor) override;
		~ReturnStatement();
	};
	class UntilStatement: public Statement {
		public:
		Token * untilToken = nullptr;
		Expression * expression = nullptr;
		Statement * body = nullptr;
		UntilStatement(Expression * e, Statement * b, Token * u);
		void accept(Visitor * visitor) override;
		~UntilStatement();
	};
	class VariableStatement: public Statement {
		public:
		Token * name = nullptr;
		BasicType type = BasicType::UnknownType;
		Expression * initializer = nullptr;
		Token * object = nullptr;
		VariableStatement(Token * n, Expression * i, BasicType t, Token * o = nullptr);
		void accept(Visitor * visitor) override;
		~VariableStatement();
	};
	class WhileStatement: public Statement {
		public:
		Token * whileToken = nullptr;
		Expression * expression = nullptr;
		Statement * body = nullptr;
		WhileStatement(Expression * e, Statement * b, Token * w);
		void accept(Visitor * visitor) override;
		~WhileStatement();
	};

	/* Object */

	class Object {
		public:
		void * value = nullptr;
		BasicType type = BasicType::UnknownType;
		Object() = default;
		Object(BasicType t);
		Object(BasicType t, void * v);
		~Object();
		inline Bool isByte() const;
		inline Bool isInteger() const;
		inline Bool isReal() const;
		inline Bool isComplexType() const;
		inline Bool isNumericType() const;
		inline Bool isBool() const;
		inline Bool isString() const;
		inline Bool isColour() const;
		inline Bool isCharacter() const;
		String getObjectName() const;
		Object * copy() const;
		String getObjectStringValue() const;
		Bool isUnknown() const;
		Bool isFunction() const;
		Bool isCallable() const;
		Bool isArray() const;
		Bool isSubscriptable() const;
		Bool getBoolValue() const;
	};

	/* Colour */

	class Colour {
		private:
		static String valueToHex(UInt8 value);
		public:
		UInt8 r = 0;
		UInt8 g = 0;
		UInt8 b = 0;
		UInt8 a = 255;
		Colour(UInt8 r, UInt8 g, UInt8 b, UInt8 a = 255);
		Colour(UInt32 rgba);
		Colour() = default;
		UInt32 colorValue();
		Colour operator ~ () const;
		Bool operator == (Colour c) const;
		Bool operator != (Colour c) const;
		Colour operator + (Colour r) const;
		Colour operator - (Colour r) const;
		Colour operator * (Colour r) const;
		Colour operator / (Colour r) const;
		Colour operator % (Colour r) const;
		String stringValue() const;
	};

	/* Complex */

	class ComplexDBZException: public Exception {
		public: ComplexDBZException(): Exception() { }
	};
	class Complex {
		public:
		Real a = 0.0;
		Real b = 0.0;
		Complex(Real n, Real i);
		Complex() = default;
		static Complex fromPolar(Real m, Real a);
		void setRealPart(Real val);
		void setImaginaryPart(Real val);
		Complex getConjugate() const;
		void conjugate();
		Real getNormalized() const;
		Real getMagnitude() const;
		inline Real getModulus() const;
		Real getPhase() const;
		inline Real getAngle() const;
		void operator = (Real r);
		Bool operator == (Complex r) const;
		Bool operator == (Real r) const;
		Bool operator != (Complex r) const;
		Bool operator != (Real r) const;
		Complex operator - () const;
		Complex operator + (Complex r) const;
		void operator += (Complex r);
		Complex operator - (Complex r) const;
		void operator -= (Complex r);
		Complex operator * (Complex r) const;
		void operator *= (Complex r);
		Complex operator / (Complex r) const;
		void operator /= (Complex r);
		String stringValue() const;
	};

	/* Function */

	class Interpreter;

	class CallProtocol {
		public:
		virtual ~CallProtocol() = default;
		virtual Object * call(Interpreter * i, Array<Object *> a, Token * c) = 0;
		virtual String stringValue() const = 0;
		virtual UInt32 arity() const = 0;
		virtual CallProtocol * copy() const = 0;
		template<typename t>
		Bool isInstanceOf() {
			return (dynamic<t *>(this));
		}
	};
	class Function: public CallProtocol {
		private:
		FunctionStatement * declaration = nullptr;
		Environment * closure = nullptr;
		public:
		Function() = default;
		Function(FunctionStatement * d, Environment * c);
		~Function() = default;
		Object * call(Interpreter * i, Array<Object *> a, Token * c) override;
		String stringValue() const override;
		UInt32 arity() const override;
		CallProtocol * copy() const override;
    };
	class Procedure: public CallProtocol {
		private:
		ProcedureStatement * declaration = nullptr;
		Environment * closure = nullptr;
		public:
		Procedure() = default;
		Procedure(ProcedureStatement * d, Environment * c);
		~Procedure() = default;
		Object * call(Interpreter * i, Array<Object *> a, Token * c) override;
		String stringValue() const override;
		UInt32 arity() const override;
		CallProtocol * copy() const override;
    };
	typedef Lambda<Object * (Interpreter * i, Array<Object *> a, Token * t)> NativeLambda;
	class NativeFunction: public CallProtocol {
		private:
		Array<Parameter *> * params = nullptr;
		NativeLambda lambda = nullptr;
		String name = "<native>";
		public:
		NativeFunction(NativeLambda l, Array<Parameter *> * p);
		NativeFunction(NativeLambda l, Array<Parameter *> * p, String n);
		~NativeFunction() = default;
		Object * call(Interpreter * i, Array<Object *> a, Token * c) override;
		String stringValue() const override;
		UInt32 arity() const override;
		CallProtocol * copy() const override;
	};

	/* Class */

	class Class: public CallProtocol {
		public:
		String name;
		Class(String n);
		Object * call(Interpreter * i, Array<Object *> a, Token * c) override;
		String stringValue() const override;
		UInt32 arity() const override;
		CallProtocol * copy() const override;
	};
	class Instance {
		private:
		Class * type = nullptr;
		public:
		Instance(Class * t);
		String stringValue() const;
		Instance * copy() const;
	};

	/* ArrayList */

	class ArrayList {
		public:
		Array<Object *> * elements = nullptr;
		ArrayList(Array<Object *> * e);
		ArrayList(Object * o);
		ArrayList();
		~ArrayList();
		Object * copyAt(SizeType i) const;
		Object * referenceAt(SizeType i) const;
		ArrayList * copy() const;
		String stringValue() const;
	};

	/* Vector */

	class InvalidIndexException: public Exception {
		public: InvalidIndexException(): Exception() { }
	};
	class InvalidOperationException: public Exception {
		public: InvalidOperationException(): Exception() { }
	};
	class Vector {
		private:
		const Bool braDirection = false;
		const Bool ketDirection = false;
		Complex * space = nullptr;
		SizeType size = 0;
		Bool direction = ketDirection;
		public:
		Vector(SizeType s);
		~Vector();
		SizeType getSize() const;
		Bool isEmpty() const;
		Bool isBra() const;
		Bool isKet() const;
		inline void negate();
		void invert();
		inline Vector * getInverse() const;
		Vector * getAdditiveInverse() const;
		Complex & operator [] (SizeType i);
		Bool operator == (Vector r) const;
		Bool operator != (Vector r) const;
		Vector operator + (Vector r) const;
		Vector operator - () const;
		Vector operator - (Vector r) const;
		Vector operator * (Complex z) const;
		static inline Vector * kroneckerProduct(Vector * a, Vector * b);
		static Vector * tensorProduct(Vector * a, Vector * b);
		Complex * copyAt(SizeType i) const;
		Complex * referenceAt(SizeType i) const;
		Vector * copy() const;
		String stringValue() const;
	};

	/* Processor */

	class EvaluationError: public Exception {
		private:
		const String message;
		const Token token;
		public:
		const String & getMessage() const { return message; }
		const Token & getToken() const { return token; }
		EvaluationError(String m, Token t):
		Exception(), message(m), token(t) { }
	};
	class Processor {
		private:
		Processor() = default;
		~Processor() = default;
		typedef Lambda<Object * (Object *)> UnaryHandler;
		typedef Lambda<Object * (Object *, Object *)> BinaryHandler;
		typedef Lambda<void (Object *, Object *)> AssignmentHandler;
		Dictionary<BasicType, UnaryHandler> unaryNegation = {
			{
				BasicType::Int64Type,
				[] (Object * o) -> Object * {
					Int64 * i = (Int64 *) o -> value;
					return new Object(o -> type, new Int64(-(* i)));
				}
			},
			{
				BasicType::RealType,
				[] (Object * o) -> Object * {
					Real * i = (Real *) o -> value;
					return new Object(o -> type, new Real(-(* i)));
				}
			},
			{
				BasicType::ImaginaryType,
				[] (Object * o) -> Object * {
					Real * i = (Real *) o -> value;
					return new Object(o -> type, new Real(-(* i)));
				}
			},
			{
				BasicType::ComplexType,
				[] (Object * o) -> Object * {
					Complex * c = (Complex *) o -> value;
					return new Object(o -> type, new Complex(-(* c)));
				}
			}
		};
		Dictionary<BasicType, UnaryHandler> unaryInversion = {
			{
				BasicType::ByteType,
				[] (Object * o) -> Object * {
					UInt8 * b = (UInt8 *) o -> value;
					b = new UInt8(~(* b));
					return new Object(o -> type, b);
				}
			},
			{
				BasicType::ColourType,
				[] (Object * o) -> Object * {
					Colour * c = (Colour *) o -> value;
					c = new Colour(~(* c));
					return new Object(o -> type, c);
				}
			},
			{
				BasicType::Int64Type,
				[] (Object * o) -> Object * {
					Int64 * i = (Int64 *) o -> value;
					i = new Int64(~(* i));
					return new Object(o -> type, i);
				}
			}
		};
		Dictionary<BasicTypes, BinaryHandler> binaryAddition = {
			{
				compose(BasicType::Int64Type, BasicType::Int64Type),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Int64 * c = new Int64((* a) + (* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				compose(BasicType::RealType, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Real * c = new Real((* b) + (* a));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				compose(BasicType::Int64Type, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Int64 * c = new Int64((* a) + (Int64)(* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				compose(BasicType::Int64Type, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					Real * c = new Real((Real)(* a) + (* b));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				compose(BasicType::Int64Type, BasicType::ImaginaryType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					Complex * c = new Complex((Real)(* a), (* b));
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				compose(BasicType::Int64Type, BasicType::ComplexType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Complex * b = (Complex *) r -> value;
					Complex * c = new Complex((* a) + b -> a, b -> b);
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				compose(BasicType::ComplexType, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Complex * a = (Complex *) l -> value;
					Real * b = (Real *) r -> value;
					Complex * c = new Complex((* b) + a -> a, a -> b);
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				compose(BasicType::ImaginaryType, BasicType::ImaginaryType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Real * c = new Real((* a) + (* b));
					return new Object(BasicType::ImaginaryType, c);
				}
			},
			{
				compose(BasicType::ComplexType, BasicType::ImaginaryType),
				[] (Object * l, Object * r) -> Object * {
					Complex * a = (Complex *) l -> value;
					Real * b = (Real *) r -> value;
					Complex * c = new Complex(a -> a, a -> b + (* b));
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				compose(BasicType::ImaginaryType, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Complex * c = new Complex((* b), (* a));
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				compose(BasicType::ColourType, BasicType::ColourType),
				[] (Object * l, Object * r) -> Object * {
					Colour * a = (Colour *) l -> value;
					Colour * b = (Colour *) r -> value;
					Colour * c = new Colour((* a) + (* b));
					return new Object(BasicType::ColourType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::CharacterType),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					Character * c = new Character((* a) + (* b));
					return new Object(BasicType::CharacterType, c);
				}
			},
			{
				compose(BasicType::ByteType, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					UInt8 * c = new UInt8((* a) + (* b));
					return new Object(BasicType::ByteType, c);
				}
			}
		};
		Dictionary<BasicTypes, BinaryHandler> stringAddition = {
			{
				compose(BasicType::StringType, BasicType::StringType),
				[] (Object * l, Object * r) -> Object * {
					String * a = (String *) l -> value;
					String * b = (String *) r -> value;
					String * c = new String((* a) + (* b));
					return new Object(BasicType::StringType, c);
				}
			},
			{
				compose(BasicType::StringType, BasicType::CharacterType),
				[] (Object * l, Object * r) -> Object * {
					String * a = (String *) l -> value;
					Character * b = (Character *) r -> value;
					StringStream s = StringStream();
					s << (* a) << (* b);
					String * c = new String(s.str());
					return new Object(BasicType::StringType, c);
				}
			},
			{
				compose(BasicType::StringType, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					String * a = (String *) l -> value;
					Real * b = (Real *) r -> value;
					String * c = new String((* a) + realToString(* b));
					return new Object(BasicType::StringType, c);
				}
			},
			{
				compose(BasicType::StringType, BasicType::ImaginaryType),
				[] (Object * l, Object * r) -> Object * {
					String * a = (String *) l -> value;
					Real * b = (Real *) r -> value;
					String * c = new String((* a) + realToString(* b) + "i");
					return new Object(BasicType::StringType, c);
				}
			},
			{
				compose(BasicType::StringType, BasicType::Int64Type),
				[] (Object * l, Object * r) -> Object * {
					String * a = (String *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					String * c = new String((* a) + intToString(* b));
					return new Object(BasicType::StringType, c);
				}
			},
			{
				compose(BasicType::StringType, BasicType::ColourType),
				[] (Object * l, Object * r) -> Object * {
					String * a = (String *) l -> value;
					Colour * b = (Colour *) r -> value;
					String * c = new String((* a) + b -> stringValue());
					return new Object(BasicType::StringType, c);
				}
			},
			{
				compose(BasicType::StringType, BasicType::ComplexType),
				[] (Object * l, Object * r) -> Object * {
					String * a = (String *) l -> value;
					Complex * b = (Complex *) r -> value;
					String * c = new String((* a) + b -> stringValue());
					return new Object(BasicType::StringType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::StringType),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					String * b = (String *) r -> value;
					StringStream s = StringStream();
					s << (* a) << (* b);
					String * c = new String(s.str());
					return new Object(BasicType::StringType, c);
				}
			},
			{
				compose(BasicType::RealType, BasicType::StringType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					String * b = (String *) r -> value;
					String * c = new String(realToString(* a) + (* b));
					return new Object(BasicType::StringType, c);
				}
			},
			{
				compose(BasicType::ImaginaryType, BasicType::StringType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					String * b = (String *) r -> value;
					String * c = new String(realToString(* a) + "i" + (* b));
					return new Object(BasicType::StringType, c);
				}
			},
			{
				compose(BasicType::Int64Type, BasicType::StringType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					String * b = (String *) r -> value;
					String * c = new String(intToString(* a) + (* b));
					return new Object(BasicType::StringType, c);
				}
			},
			{
				compose(BasicType::ColourType, BasicType::StringType),
				[] (Object * l, Object * r) -> Object * {
					Colour * a = (Colour *) l -> value;
					String * b = (String *) r -> value;
					String * c = new String(a -> stringValue() + (* b));
					return new Object(BasicType::StringType, c);
				}
			},
			{
				compose(BasicType::ComplexType, BasicType::StringType),
				[] (Object * l, Object * r) -> Object * {
					Complex * a = (Complex *) l -> value;
					String * b = (String *) r -> value;
					String * c = new String(a -> stringValue() + (* b));
					return new Object(BasicType::StringType, c);
				}
			}
		};
		Dictionary<BasicTypes, BinaryHandler> binarySubtraction = {
			{
				compose(BasicType::Int64Type, BasicType::Int64Type),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Int64 * c = new Int64((* a) - (* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				compose(BasicType::RealType, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Real * c = new Real((* a) - (* b));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				compose(BasicType::Int64Type, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					Real * c = new Real((Real)(* a) - (* b));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				compose(BasicType::RealType, BasicType::Int64Type),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Real * c = new Real((* a) - (Real)(* b));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				compose(BasicType::Int64Type, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Int64 * c = new Int64((* a) - (* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				compose(BasicType::ByteType, BasicType::Int64Type),
				[] (Object * l, Object * r) -> Object * {
					UInt8 * a = (UInt8 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Int64 * c = new Int64((Int64)(* a) - (* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				compose(BasicType::ComplexType, BasicType::ComplexType),
				[] (Object * l, Object * r) -> Object * {
					Complex * a = (Complex *) l -> value;
					Complex * b = (Complex *) r -> value;
					Complex * c = new Complex((* a) - (* b));
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				compose(BasicType::ImaginaryType, BasicType::Int64Type),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Complex * c = new Complex(-(Real)(* b), (* a));
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				compose(BasicType::Int64Type, BasicType::ImaginaryType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					Complex * c = new Complex((Real)(* b), -(* a));
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				compose(BasicType::ImaginaryType, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Complex * c = new Complex(-(* b), (* a));
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				compose(BasicType::RealType, BasicType::ImaginaryType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Complex * c = new Complex((* a), -(* b));
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				compose(BasicType::ComplexType, BasicType::ImaginaryType),
				[] (Object * l, Object * r) -> Object * {
					Complex * a = (Complex *) l -> value;
					Real * b = (Real *) r -> value;
					Complex * c = new Complex((* a).a, (* a).b - (* b));
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				compose(BasicType::ImaginaryType, BasicType::ComplexType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Complex * b = (Complex *) r -> value;
					Complex * c = new Complex(-(* b).a, (* a) - (* b).b);
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				compose(BasicType::ComplexType, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Complex * a = (Complex *) l -> value;
					Real * b = (Real *) r -> value;
					Complex * c = new Complex((* a).a - (* b), (* a).b);
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				compose(BasicType::RealType, BasicType::ComplexType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Complex * b = (Complex *) r -> value;
					Complex * c = new Complex((* a) - (* b).a, -(* b).b);
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				compose(BasicType::ComplexType, BasicType::Int64Type),
				[] (Object * l, Object * r) -> Object * {
					Complex * a = (Complex *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Complex * c = new Complex((* a).a - (Real)(* b), (* a).b);
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				compose(BasicType::Int64Type, BasicType::ComplexType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Complex * b = (Complex *) r -> value;
					Complex * c = new Complex((Real)(* a) - (* b).a, -(* b).b);
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::CharacterType),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					Character * c = new Character((* a) - (* b));
					return new Object(BasicType::CharacterType, c);
				}
			}
		};
		Dictionary<BasicTypes, BinaryHandler> binaryMultiplication = {
			{
				compose(BasicType::Int64Type, BasicType::Int64Type),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Int64 * c = new Int64((* a) * (* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				compose(BasicType::Int64Type, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					Real * c = new Real((Real)(* a) * (* b));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				compose(BasicType::RealType, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Real * c = new Real((* a) * (* b));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				compose(BasicType::ByteType, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					UInt8 * c = new UInt8((* a) * (* b));
					return new Object(BasicType::ByteType, c);
				}
			},
			{
				compose(BasicType::ByteType, BasicType::Int64Type),
				[] (Object * l, Object * r) -> Object * {
					UInt8 * a = (UInt8 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Int64 * c = new Int64((Int64)(* a) * (* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::CharacterType),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					Character * c = new Character((* a) * (* b));
					return new Object(BasicType::CharacterType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::Int64Type),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Int64 * c = new Int64((Int64)(* a) * (* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				compose(BasicType::ColourType, BasicType::ColourType),
				[] (Object * l, Object * r) -> Object * {
					Colour * a = (Colour *) l -> value;
					Colour * b = (Colour *) r -> value;
					Colour * c = new Colour((* a) * (* b));
					return new Object(BasicType::ColourType, c);
				}
			},
			{
				compose(BasicType::ImaginaryType, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Real * c = new Real((* a) * (* b));
					return new Object(BasicType::ImaginaryType, c);
				}
			},
			{
				compose(BasicType::ImaginaryType, BasicType::Int64Type),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Real * c = new Real((* a) * (Real)(* b));
					return new Object(BasicType::ImaginaryType, c);
				}
			},
			{
				compose(BasicType::ImaginaryType, BasicType::ImaginaryType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Real * c = new Real(-((* a) * (* b)));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				compose(BasicType::ImaginaryType, BasicType::ComplexType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Complex * b = (Complex *) r -> value;
					Complex * c = new Complex(-(* a) * b -> b, (* a) * b -> a);
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				compose(BasicType::ComplexType, BasicType::ComplexType),
				[] (Object * l, Object * r) -> Object * {
					Complex * a = (Complex *) l -> value;
					Complex * b = (Complex *) r -> value;
					Complex * c = new Complex((* a) * (* b));
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				compose(BasicType::ComplexType, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Complex * a = (Complex *) l -> value;
					Real * b = (Real *) r -> value;
					Complex * c = new Complex(a -> a * (* b), a -> b * (* b));
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				compose(BasicType::ComplexType, BasicType::Int64Type),
				[] (Object * l, Object * r) -> Object * {
					Complex * a = (Complex *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Complex * c = new Complex(a -> a * (Real)(* b), a -> b * (Real)(* b));
					return new Object(BasicType::ComplexType, c);
				}
			}
		};
		Dictionary<BasicTypes, BinaryHandler> binaryDivision = {
			{
				compose(BasicType::Int64Type, BasicType::Int64Type),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					if ((* b) == 0) return nullptr;
					Int64 * c = new Int64((* a) / (* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				compose(BasicType::Int64Type, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					if ((* b) == 0.0) return nullptr;
					Real * c = new Real((Real)(* a) / (* b));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				compose(BasicType::RealType, BasicType::Int64Type),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					if ((* b) == 0) return nullptr;
					Real * c = new Real((* a) / (Real)(* b));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				compose(BasicType::RealType, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					if ((* b) == 0.0) return nullptr;
					Real * c = new Real((* a) / (* b));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				compose(BasicType::ByteType, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					if ((* b) == 0) return nullptr;
					UInt8 * c = new UInt8((* a) / (* b));
					return new Object(BasicType::ByteType, c);
				}
			},
			{
				compose(BasicType::ByteType, BasicType::Int64Type),
				[] (Object * l, Object * r) -> Object * {
					UInt8 * a = (UInt8 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					if ((* b) == 0) return nullptr;
					Int64 * c = new Int64((Int64)(* a) / (* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				compose(BasicType::Int64Type, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					if ((* b) == 0) return nullptr;
					Int64 * c = new Int64((* a) / (Int64)(* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::CharacterType),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					if ((* b) == 0) return nullptr;
					Character * c = new Character((* a) / (* b));
					return new Object(BasicType::CharacterType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::Int64Type),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					if ((* b) == 0) return nullptr;
					Int64 * c = new Int64((Int64)(* a) / (* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					if ((* b) == 0) return nullptr;
					Character * c = new Character((* a) / (UInt8)(* b));
					return new Object(BasicType::CharacterType, c);
				}
			},
			{
				compose(BasicType::ColourType, BasicType::ColourType),
				[] (Object * l, Object * r) -> Object * {
					Colour * a = (Colour *) l -> value;
					Colour * b = (Colour *) r -> value;
					Colour * c = new Colour((* a) / (* b));
					return new Object(BasicType::ColourType, c);
				}
			},
			{
				compose(BasicType::ImaginaryType, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					if ((* b) == 0.0) return nullptr;
					Real * c = new Real((* a) / (* b));
					return new Object(BasicType::ImaginaryType, c);
				}
			},
			{
				compose(BasicType::ImaginaryType, BasicType::Int64Type),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					if ((* b) == 0) return nullptr;
					Real * c = new Real((* a) / (Real)(* b));
					return new Object(BasicType::ImaginaryType, c);
				}
			},
			{
				compose(BasicType::ImaginaryType, BasicType::ImaginaryType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					if ((* b) == 0) return nullptr;
					Real * c = new Real((* a) / (* b));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				compose(BasicType::ComplexType, BasicType::ComplexType),
				[] (Object * l, Object * r) -> Object * {
					Complex * a = (Complex *) l -> value;
					Complex * b = (Complex *) r -> value;
					Complex * c = nullptr;
					try { c = new Complex((* a) / (* b)); }
					catch (ComplexDBZException & e) { return nullptr; }
					return new Object(BasicType::ComplexType, c);
				}
			}
		};
		Dictionary<BasicTypes, BinaryHandler> binaryAND = {
			{
				compose(BasicType::Int64Type, BasicType::Int64Type),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Int64 * c = new Int64((* a) & (* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				compose(BasicType::ByteType, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					UInt8 * c = new UInt8((* a) & (* b));
					return new Object(BasicType::ByteType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::CharacterType),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					Character * c = new Character((* a) & (* b));
					return new Object(BasicType::CharacterType, c);
				}
			},
			{
				compose(BasicType::BoolType, BasicType::BoolType),
				[] (Object * l, Object * r) -> Object * {
					Bool * a = (Bool *) l -> value;
					Bool * b = (Bool *) r -> value;
					Bool * c = new Bool((* a) && (* b));
					return new Object(BasicType::BoolType, c);
				}
			}
		};
		Dictionary<BasicTypes, BinaryHandler> binaryXOR = {
			{
				compose(BasicType::Int64Type, BasicType::Int64Type),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Int64 * c = new Int64((* a) ^ (* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				compose(BasicType::ByteType, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					UInt8 * c = new UInt8((* a) ^ (* b));
					return new Object(BasicType::ByteType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::CharacterType),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					Character * c = new Character((* a) ^ (* b));
					return new Object(BasicType::CharacterType, c);
				}
			},
			{
				compose(BasicType::BoolType, BasicType::BoolType),
				[] (Object * l, Object * r) -> Object * {
					Bool * a = (Bool *) l -> value;
					Bool * b = (Bool *) r -> value;
					Bool * c = new Bool((* a) != (* b));
					return new Object(BasicType::BoolType, c);
				}
			}
		};
		Dictionary<BasicTypes, BinaryHandler> binaryOR = {
			{
				compose(BasicType::Int64Type, BasicType::Int64Type),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Int64 * c = new Int64((* a) | (* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				compose(BasicType::ByteType, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					UInt8 * c = new UInt8((* a) | (* b));
					return new Object(BasicType::ByteType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::CharacterType),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					Character * c = new Character((* a) | (* b));
					return new Object(BasicType::CharacterType, c);
				}
			},
			{
				compose(BasicType::BoolType, BasicType::BoolType),
				[] (Object * l, Object * r) -> Object * {
					Bool * a = (Bool *) l -> value;
					Bool * b = (Bool *) r -> value;
					Bool * c = new Bool((* a) || (* b));
					return new Object(BasicType::BoolType, c);
				}
			}
		};
		void applyAdditionAssignment(Token * t, Object * l, Object * r);
		void applySubtractionAssignment(Token * t, Object * l, Object * r);
		void applyMultiplicationAssignment(Token * t, Object * l, Object * r);
		void applyDivisionAssignment(Token * t, Object * l, Object * r);
		void applyModulusAssignment(Token * t, Object * l, Object * r);
		void applyORAssignment(Token * t, Object * l, Object * r);
		void applyANDAssignment(Token * t, Object * l, Object * r);
		void applyXORAssignment(Token * t, Object * l, Object * r);
		Object * applyAddition(Token * t, Object * l, Object * r);
		Object * applySubtraction(Token * t, Object * l, Object * r);
		Object * applyMultiplication(Token * t, Object * l, Object * r);
		Object * applyDivision(Token * t, Object * l, Object * r);
		Object * applyModulus(Token * t, Object * l, Object * r);
		Object * applyAND(Token * t, Object * l, Object * r);
		Object * applyXOR(Token * t, Object * l, Object * r);
		Object * applyOR(Token * t, Object * l, Object * r);
		Dictionary<BasicTypes, BinaryHandler> binaryStrictEquality = {
			{
				compose(BasicType::Int64Type, BasicType::Int64Type),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((* a) == (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::RealType, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((* a) == (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::StringType, BasicType::StringType),
				[] (Object * l, Object * r) -> Object * {
					String * a = (String *) l -> value;
					String * b = (String *) r -> value;
					Bool * c = new Bool((* a) == (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::BoolType, BasicType::BoolType),
				[] (Object * l, Object * r) -> Object * {
					Bool * a = (Bool *) l -> value;
					Bool * b = (Bool *) r -> value;
					Bool * c = new Bool((* a) == (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::ColourType, BasicType::ColourType),
				[] (Object * l, Object * r) -> Object * {
					Colour * a = (Colour *) l -> value;
					Colour * b = (Colour *) r -> value;
					Bool * c = new Bool((* a) == (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::ComplexType, BasicType::ComplexType),
				[] (Object * l, Object * r) -> Object * {
					Complex * a = (Complex *) l -> value;
					Complex * b = (Complex *) r -> value;
					Bool * c = new Bool((* a) == (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::ImaginaryType, BasicType::ImaginaryType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((* a) == (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::ByteType, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((* a) == (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::CharacterType),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) == (* b));
					return new Object(BasicType::BoolType, c);
				}
			}
		};
		Dictionary<BasicTypes, BinaryHandler> binaryMixedEquality = {
			{
				compose(BasicType::Int64Type, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((Real)(* a) == (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::Int64Type, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((* a) == (Int64)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((* a) == (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::Int64Type, BasicType::CharacterType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) == (Int64)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::ImaginaryType, BasicType::ComplexType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Complex * b = (Complex *) r -> value;
					Bool * c = new Bool(((* a) == b -> b) && (b -> a == 0));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::RealType, BasicType::ComplexType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Complex * b = (Complex *) r -> value;
					Bool * c = new Bool(((* a) == b -> a) && (b -> b == 0));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::StringType, BasicType::CharacterType),
				[] (Object * l, Object * r) -> Object * {
					String * a = (String *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool(false);
					if (a -> length() != 1) {
						return new Object(BasicType::BoolType, c);
					}
					* c = (a -> at(0) == (* b));
					return new Object(BasicType::BoolType, c);
				}
			}
		};
		Dictionary<BasicTypes, BinaryHandler> binaryMajor = {
			{
				compose(BasicType::Int64Type, BasicType::Int64Type),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((* a) > (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::Int64Type, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((Real)(* a) > (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::Int64Type, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((* a) > (Int64)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::Int64Type, BasicType::CharacterType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) > (Int64)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::RealType, BasicType::Int64Type),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((* a) > (Real)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::CharacterType),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) > (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::ByteType, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((* a) > (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::RealType, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((* a) > (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::ImaginaryType, BasicType::ImaginaryType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((* a) > (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::ByteType, BasicType::Int64Type),
				[] (Object * l, Object * r) -> Object * {
					UInt8 * a = (UInt8 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((Int64)(* a) > (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::Int64Type),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((Int64)(* a) > (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((UInt8)(* a) > (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::ByteType, BasicType::CharacterType),
				[] (Object * l, Object * r) -> Object * {
					UInt8 * a = (UInt8 *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) > (UInt8)(* b));
					return new Object(BasicType::BoolType, c);
				}
			}
		};
		Dictionary<BasicTypes, BinaryHandler> binaryMajorEqual = {
			{
				compose(BasicType::Int64Type, BasicType::Int64Type),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((* a) >= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::Int64Type, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((Real)(* a) >= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::Int64Type, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((* a) >= (Int64)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::Int64Type, BasicType::CharacterType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) >= (Int64)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::RealType, BasicType::Int64Type),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((* a) >= (Real)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::CharacterType),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) >= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::ByteType, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((* a) >= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::RealType, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((* a) >= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::ImaginaryType, BasicType::ImaginaryType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((* a) >= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::ByteType, BasicType::Int64Type),
				[] (Object * l, Object * r) -> Object * {
					UInt8 * a = (UInt8 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((Int64)(* a) >= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::Int64Type),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((Int64)(* a) >= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((UInt8)(* a) >= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::ByteType, BasicType::CharacterType),
				[] (Object * l, Object * r) -> Object * {
					UInt8 * a = (UInt8 *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) >= (UInt8)(* b));
					return new Object(BasicType::BoolType, c);
				}
			}
		};
		Dictionary<BasicTypes, BinaryHandler> binaryMinor = {
			{
				compose(BasicType::Int64Type, BasicType::Int64Type),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((* a) < (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::Int64Type, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((Real)(* a) < (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::Int64Type, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((* a) < (Int64)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::Int64Type, BasicType::CharacterType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) < (Int64)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::RealType, BasicType::Int64Type),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((* a) < (Real)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::CharacterType),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) < (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::ByteType, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((* a) < (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::RealType, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((* a) < (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::ImaginaryType, BasicType::ImaginaryType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((* a) < (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::ByteType, BasicType::Int64Type),
				[] (Object * l, Object * r) -> Object * {
					UInt8 * a = (UInt8 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((Int64)(* a) < (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::Int64Type),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((Int64)(* a) < (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((UInt8)(* a) < (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::ByteType, BasicType::CharacterType),
				[] (Object * l, Object * r) -> Object * {
					UInt8 * a = (UInt8 *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) < (UInt8)(* b));
					return new Object(BasicType::BoolType, c);
				}
			}
		};
		Dictionary<BasicTypes, BinaryHandler> binaryMinorEqual = {
			{
				compose(BasicType::Int64Type, BasicType::Int64Type),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((* a) <= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::Int64Type, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((Real)(* a) <= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::Int64Type, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((* a) <= (Int64)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::Int64Type, BasicType::CharacterType),
				[] (Object * l, Object * r) -> Object * {
					Int64 * a = (Int64 *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) <= (Int64)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::RealType, BasicType::Int64Type),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((* a) <= (Real)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::CharacterType),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) <= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::ByteType, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((* a) <= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::RealType, BasicType::RealType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((* a) <= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::ImaginaryType, BasicType::ImaginaryType),
				[] (Object * l, Object * r) -> Object * {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((* a) <= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::ByteType, BasicType::Int64Type),
				[] (Object * l, Object * r) -> Object * {
					UInt8 * a = (UInt8 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((Int64)(* a) <= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::Int64Type),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((Int64)(* a) <= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::ByteType),
				[] (Object * l, Object * r) -> Object * {
					Character * a = (Character *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((UInt8)(* a) <= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				compose(BasicType::ByteType, BasicType::CharacterType),
				[] (Object * l, Object * r) -> Object * {
					UInt8 * a = (UInt8 *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) <= (UInt8)(* b));
					return new Object(BasicType::BoolType, c);
				}
			}
		};
		Object * applyEquality(Token * t, Object * l, Object * r);
		Object * applyInequality(Token * t, Object * l, Object * r);
		Object * applyMajor(Token * t, Object * l, Object * r);
		Object * applyMajorEqual(Token * t, Object * l, Object * r);
		Object * applyMinor(Token * t, Object * l, Object * r);
		Object * applyMinorEqual(Token * t, Object * l, Object * r);
		Dictionary<BasicTypes, AssignmentHandler> pureAssignment = {
			{
				compose(BasicType::InstanceType, BasicType::InstanceType),
				[] (Object * l, Object * r) {
					Instance * a = (Instance *) l -> value;
					Instance * b = (Instance *) r -> value;
					Instance * c = b -> copy();
					* a = * c; delete c;
				}
			},
			{
				compose(BasicType::Int64Type, BasicType::Int64Type),
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					* a = * b;
				}
			},
			{
				compose(BasicType::BoolType, BasicType::BoolType),
				[] (Object * l, Object * r) {
					Bool * a = (Bool *) l -> value;
					Bool * b = (Bool *) r -> value;
					* a = * b;
				}
			},
			{
				compose(BasicType::ByteType, BasicType::ByteType),
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					* a = * b;
				}
			},
			{
				compose(BasicType::StringType, BasicType::StringType),
				[] (Object * l, Object * r) {
					String * a = (String *) l -> value;
					String * b = (String *) r -> value;
					* a = String(* b);
				}
			},
			{
				compose(BasicType::RealType, BasicType::RealType),
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					* a = * b;
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::CharacterType),
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					* a = * b;
				}
			},
			{
				compose(BasicType::ImaginaryType, BasicType::ImaginaryType),
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					* a = * b;
				}
			},
			{
				compose(BasicType::ComplexType, BasicType::ComplexType),
				[] (Object * l, Object * r) {
					Complex * a = (Complex *) l -> value;
					Complex * b = (Complex *) r -> value;
					* a = * b;
				}
			},
			{
				compose(BasicType::ColourType, BasicType::ColourType),
				[] (Object * l, Object * r) {
					Colour * a = (Colour *) l -> value;
					Colour * b = (Colour *) r -> value;
					* a = * b;
				}
			},
		};
		Dictionary<BasicTypes, AssignmentHandler> mixedAssignment = {
			{
				compose(BasicType::Int64Type, BasicType::RealType),
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					* a = (Int64)(* b);
				}
			},
			{
				compose(BasicType::RealType, BasicType::Int64Type),
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					* a = (Real)(* b);
				}
			},
			{
				compose(BasicType::ComplexType, BasicType::Int64Type),
				[] (Object * l, Object * r) {
					Complex * a = (Complex *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					* a = Complex(* b, 0.0);
				}
			},
			{
				compose(BasicType::ComplexType, BasicType::RealType),
				[] (Object * l, Object * r) {
					Complex * a = (Complex *) l -> value;
					Real * b = (Real *) r -> value;
					* a = Complex(* b, 0.0);
				}
			},
			{
				compose(BasicType::ComplexType, BasicType::ImaginaryType),
				[] (Object * l, Object * r) {
					Complex * a = (Complex *) l -> value;
					Real * b = (Real *) r -> value;
					* a = Complex(0.0, * b);
				}
			},
			{
				compose(BasicType::Int64Type, BasicType::CharacterType),
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Character * b = (Character *) r -> value;
					* a = (Int64)(* b);
				}
			},
			{
				compose(BasicType::Int64Type, BasicType::ByteType),
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					* a = (Int64)(* b);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::Int64Type),
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					* a = (UInt8)(* b);
				}
			},
			{
				compose(BasicType::CharacterType, BasicType::ByteType),
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					* a = * b;
				}
			},
			{
				compose(BasicType::ByteType, BasicType::Int64Type),
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					* a = (UInt8)(* b);
				}
			},
			{
				compose(BasicType::ByteType, BasicType::CharacterType),
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					Character * b = (Character *) r -> value;
					* a = (UInt8)(* b);
				}
			},
			{
				compose(BasicType::StringType, BasicType::CharacterType),
				[] (Object * l, Object * r) {
					String * a = (String *) l -> value;
					Character * b = (Character *) r -> value;
					StringStream s = StringStream();
					s << (* b);
					* a = String(s.str());
				}
			},
			{
				compose(BasicType::StringType, BasicType::Int64Type),
				[] (Object * l, Object * r) {
					String * a = (String *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					* a = String(intToString(* b));
				}
			},
			{
				compose(BasicType::StringType, BasicType::RealType),
				[] (Object * l, Object * r) {
					String * a = (String *) l -> value;
					Real * b = (Real *) r -> value;
					* a = String(realToString(* b));
				}
			},
			{
				compose(BasicType::StringType, BasicType::ImaginaryType),
				[] (Object * l, Object * r) {
					String * a = (String *) l -> value;
					Real * b = (Real *) r -> value;
					* a = String(realToString(* b));
				}
			},
			{
				compose(BasicType::StringType, BasicType::ComplexType),
				[] (Object * l, Object * r) {
					String * a = (String *) l -> value;
					Complex * b = (Complex *) r -> value;
					* a = b -> stringValue();
				}
			},
			{
				compose(BasicType::StringType, BasicType::ColourType),
				[] (Object * l, Object * r) {
					String * a = (String *) l -> value;
					Colour * b = (Colour *) r -> value;
					* a = b -> stringValue();
				}
			},
			{
				compose(BasicType::StringType, BasicType::BoolType),
				[] (Object * l, Object * r) {
					String * a = (String *) l -> value;
					Bool * b = (Bool *) r -> value;
					* a = (* b) ? "true" : "false";
				}
			}
		};
		inline BasicTypes compose(BasicType a, BasicType b);
		public:
		Processor(const Processor &) = delete;
		Processor(Processor &&) = delete;
		Processor & operator = (const Processor &) = delete;
		Processor & operator = (Processor &&) = delete;
		static Processor * self() {
			static Processor instance;
			return & instance;
		}
		Object * applyComparisonOperator(Token * t, Object * l, Object * r);
		Object * applyBinaryOperator(Token * t, Object * l, Object * r);
		Object * applySubscriptOperator(Token * t, Object * l, Object * r);
		Object * applyUnaryOperator(Token * t, Object * o);
		void applyAssignment(Token * t, Object * l, Object * r);
		void applyMutableAssignment(Token * t, Object * l, Object * r);
	};

	/* Libraries */

	class Kronos {
		private:
		Kronos() = default;
		public:
		static void defineLibrary(Environment * global);
	};
	class Maths {
		private:
		Maths() = default;
		public:
		static void defineLibrary(Environment * global);
	};
	class Standard {
		private:
		Standard() = default;
		public:
		static void defineLibrary(Environment * global);
	};

	/* Interpreter */

	class SyntaxTree {
		public:
		Bool standardLibrary = false;
		Bool mathsLibrary = false;
		Bool kronosLibrary = false;
		Array<Statement *> * statements = nullptr;
		SyntaxTree() = default;
		SyntaxTree(Array<Statement *> * s);
		~SyntaxTree() {
			if (!statements) return;
			for (Statement * s : * statements) delete s;
			delete statements;
		}
	};
	class InterpreterErrorException: public Exception {
		private:
		const String message;
		const UInt64 line;
		const String fileName;
		public:
		const String & getMessage() const { return message; }
		const UInt64 & getLine() const { return line; }
		const String & getFileName() const { return fileName; }
		InterpreterErrorException(String m, UInt64 l, String n):
		Exception(),  message(m), line(l), fileName(n) { }
	};
	class InterpreterReturn: public Exception {
		private:
		Object * value = nullptr;
		Token * returnToken = nullptr;
		public:
		Object * getReturnValue() const { return value; }
		Token * getReturnToken() const { return returnToken; }
		InterpreterReturn(Object * v, Token * t): 
		value(v), returnToken(t), Exception() { }
		~InterpreterReturn() { if (returnToken) delete returnToken; }
	};
	class Interpreter: public Expression::Visitor, public Statement::Visitor {
		private:
		String * fileName = nullptr;
		String * fileContents = nullptr;
		Processor * CPU = Processor::self();
		Environment * memory = nullptr;
		Bool broken = false;
		Bool continued = false;
		Object * visitAssignmentExpression(Assignment * e) override;
		Object * visitBinaryExpression(Binary * e) override;
		Object * visitBraExpression(Bra * e) override;
		Object * visitCallExpression(Call * e) override;
		Object * visitComparisonExpression(Comparison * e) override;
		Object * visitGetExpression(Get * e) override;
		Object * visitGroupingExpression(Grouping * e) override;
		Object * visitKetExpression(Ket * e) override;
		Object * visitListExpression(List * e) override;
		Object * visitLiteralExpression(Literal * e) override;
		Object * visitLogicalExpression(Logical * e) override;
		Object * visitMutableExpression(Mutable * e) override;
		Object * visitSetExpression(Set * e) override;
		Object * visitSubscriptExpression(Subscript * e) override;
		Object * visitSuperExpression(Super * e) override;
		Object * visitThisExpression(This * e) override;
		Object * visitUnaryExpression(Unary * e) override;
		Object * visitIdentifierExpression(Identifier * e) override;
		Object * evaluateExpression(Expression * e);
		void visitBlockStatement(BlockStatement * e) override;
		void visitBreakStatement(BreakStatement * e) override;
		void visitClassStatement(ClassStatement * e) override;
		void visitContinueStatement(ContinueStatement * e) override;
		void visitDeleteStatement(DeleteStatement * e) override;
		void visitDoWhileStatement(DoWhileStatement * e) override;
		void visitExpressionStatement(ExpressionStatement * e) override;
		void visitFileStatement(FileStatement * e) override;
		void visitForStatement(ForStatement * e) override;
		void visitFunctionStatement(FunctionStatement * e) override;
		void visitIfStatement(IfStatement * e) override;
		void visitLoopStatement(LoopStatement * e) override;
		void visitPrintStatement(PrintStatement * e) override;
		void visitProcedureStatement(ProcedureStatement * e) override;
		void visitRepeatUntilStatement(RepeatUntilStatement * e) override;
		void visitRestStatement(RestStatement * e) override;
		void visitReturnStatement(ReturnStatement * e) override;
		void visitUntilStatement(UntilStatement * e) override;
		void visitVariableStatement(VariableStatement * e) override;
		void visitWhileStatement(WhileStatement * e) override;
		void executeStatement(Statement * statement);
		void executeBlock(Array<Statement *> * statements, Environment * environment);
		Object * evaluate(Expression * expression, String * input = nullptr, String fileName = "Unknown File");
		Interpreter();
		~Interpreter() = default;
		public:
		Interpreter(const Interpreter &) = delete;
		Interpreter(Interpreter &&) = delete;
		Interpreter & operator = (const Interpreter &) = delete;
		Interpreter & operator = (Interpreter &&) = delete;
		static Interpreter * self() {
			static Interpreter instance;
			return & instance;
		}
		Environment * globals = nullptr;
		void executeFunction(BlockStatement * block, Environment * environment);
		void evaluate(SyntaxTree * syntaxTree);
	};

	/* Lexer */

	class CodeUnit {
		public:
		Array<Token> * tokens = nullptr;
		String * name = nullptr;
		String * contents = nullptr;
		CodeUnit() = default;
		CodeUnit(Array<Token> * t, String * n, String * c) {
			tokens = t; name = n; contents = c;
		}
		~CodeUnit() {
			if (tokens) delete tokens;
			if (name) delete name;
			if (contents) delete contents;
		}
	};
	class Lexer {
		private:
		Array<TokenRule> grammar = {

			{ Regex("^([ \\t\\n]+)"), TokenType::empty },

			{ Regex("^(\\/\\*+[^*]*\\*+(?:[^/*][^*]*\\*+)*\\/)"), TokenType::comment },

			{ Regex("^([0-9]+(?:\\.[0-9]+(?:[eE][-]?[0-9]+)?)?i)"), TokenType::imaginaryLiteral },
			{ Regex("^([0-9]+\\.[0-9]+(?:[eE][-]?[0-9]+)?)"), TokenType::realLiteral },
			{ Regex("^((?:0[x][0-9A-Fa-f]+)|(?:0b[01]+)|(?:0o[0-7]+)|(?:0d[0-9]+)|(?:[0-9]+))"), TokenType::intLiteral },
			{ Regex("^(\"(?:[^\\\\\"]|\\\\[\"\\\\0abfnrtv]|\\\\0x[0-9A-Fa-f]{2})*\")"), TokenType::stringLiteral },
			{ Regex("^('(?:[^\\\\]|\\\\0x[0-9A-Fa-f]{2}|\\\\['\\\\0abfnrtv])')"), TokenType::charLiteral },
			{ Regex("^(#[A-Fa-f0-9]{6}(?:[A-Fa-f0-9][A-Fa-f0-9])?|#[A-Fa-f0-9]{3,4})\\b"), TokenType::colourLiteral },
			{ Regex("^(false|true)\\b"), TokenType::boolLiteral },

			{ Regex("^(<[01]\\|)"), TokenType::basisBraLiteral },
			{ Regex("^(\\|[01]>)"), TokenType::basisKetLiteral },

			{ Regex("^(<[A-Za-z_][A-Za-z0-9_]*>)"), TokenType::measureSymbol },
			{ Regex("^(<[A-Za-z_][A-Za-z0-9_]*\\|[A-Za-z_][A-Za-z0-9_]*>)"), TokenType::braketSymbol },
			{ Regex("^(<[A-Za-z_][A-Za-z0-9_]*\\|)"), TokenType::braSymbol },
			{ Regex("^(\\|[A-Za-z_][A-Za-z0-9_]*><[A-Za-z_][A-Za-z0-9_]*\\|)"), TokenType::ketbraSymbol },
			{ Regex("^(\\|[A-Za-z_][A-Za-z0-9_]*>)"), TokenType::ketSymbol },

			{ Regex("^(\\->)"), TokenType::arrow },
			{ Regex("^(\\:)"), TokenType::colon },
			{ Regex("^(\\;)"), TokenType::semicolon },
			{ Regex("^(\\,)"), TokenType::comma },
			{ Regex("^(\\.)"), TokenType::dot },
			{ Regex("^(<=)"), TokenType::minorEqual },
			{ Regex("^(<)"), TokenType::minor },
			{ Regex("^(>=)"), TokenType::majorEqual },
			{ Regex("^(>)"), TokenType::major },
			{ Regex("^(==)"), TokenType::equality },
			{ Regex("^(=)"), TokenType::equal },
			{ Regex("^(\\?)"), TokenType::questionMark },
			{ Regex("^(\\!=)"), TokenType::inequality },
			{ Regex("^(\\!)"), TokenType::exclamationMark },

			{ Regex("^(\\+=)"), TokenType::plusEqual },
			{ Regex("^(\\+)"), TokenType::plus },
			{ Regex("^(\\-=)"), TokenType::minusEqual },
			{ Regex("^(\\-)"), TokenType::minus },
			{ Regex("^(\\~)"), TokenType::tilde },
			{ Regex("^(\\*=)"), TokenType::starEqual },
			{ Regex("^(\\*)"), TokenType::star },
			{ Regex("^(\\\\)"), TokenType::backslash },
			{ Regex("^(\\/=)"), TokenType::slashEqual },
			{ Regex("^(\\/)"), TokenType::slash },
			{ Regex("^(\\|=)"), TokenType::pipeEqual },
			{ Regex("^(\\|\\|)"), TokenType::OR },
			{ Regex("^(\\|)"), TokenType::pipe },
			{ Regex("^(\\&=)"), TokenType::ampersandEqual },
			{ Regex("^(\\&\\&)"), TokenType::AND },
			{ Regex("^(\\&)"), TokenType::ampersand },
			{ Regex("^(\\%=)"), TokenType::modulusEqual },
			{ Regex("^(\\%)"), TokenType::modulus },
			{ Regex("^(\\^=)"), TokenType::daggerEqual },
			{ Regex("^(\\^)"), TokenType::dagger },

			{ Regex("^(\\()"), TokenType::openParenthesis },
			{ Regex("^(\\))"), TokenType::closeParenthesis },
			{ Regex("^(\\[)"), TokenType::openBracket },
			{ Regex("^(\\])"), TokenType::closeBracket },
			{ Regex("^(\\{)"), TokenType::openBrace },
			{ Regex("^(\\})"), TokenType::closeBrace },

			{ Regex("^(print)\\b"), TokenType::printKeyword },

			{ Regex("^(if)\\b"), TokenType::ifKeyword },
			{ Regex("^(else)\\b"), TokenType::elseKeyword },
			{ Regex("^(switch)\\b"), TokenType::switchKeyword },
			{ Regex("^(case)\\b"), TokenType::caseKeyword },
			{ Regex("^(default)\\b"), TokenType::defaultKeyword },
			{ Regex("^(while)\\b"), TokenType::whileKeyword },
			{ Regex("^(do)\\b"), TokenType::doKeyword },
			{ Regex("^(loop)\\b"), TokenType::loopKeyword },
			{ Regex("^(for)\\b"), TokenType::forKeyword },
			{ Regex("^(repeat)\\b"), TokenType::repeatKeyword },
			{ Regex("^(until)\\b"), TokenType::untilKeyword },
			{ Regex("^(break)\\b"), TokenType::breakKeyword },
			{ Regex("^(continue)\\b"), TokenType::continueKeyword },

			{ Regex("^(delete)\\b"), TokenType::deleteKeyword },

			{ Regex("^(import)\\b"), TokenType::importKeyword },
			{ Regex("^(func)\\b"), TokenType::funcKeyword },
			{ Regex("^(proc)\\b"), TokenType::procKeyword },
			{ Regex("^(static)\\b"), TokenType::staticKeyword },
			{ Regex("^(class)\\b"), TokenType::classKeyword },
			{ Regex("^(enumerator)\\b"), TokenType::enumKeyword },
			{ Regex("^(structure)\\b"), TokenType::structKeyword },
			{ Regex("^(private)\\b"), TokenType::privateKeyword },
			{ Regex("^(public)\\b"), TokenType::publicKeyword },
			{ Regex("^(ref)\\b"), TokenType::refKeyword },
			{ Regex("^(cpy)\\b"), TokenType::cpyKeyword },
			{ Regex("^(const)\\b"), TokenType::constKeyword },
			{ Regex("^(empty)\\b"), TokenType::emptyLiteral },
			{ Regex("^(rest)\\b"), TokenType::restKeyword },
			{ Regex("^(return)\\b"), TokenType::returnKeyword },

			{ Regex("^(Bool|Byte|Character|Colour|Complex|Imaginary|Integer|Measurement|Real|String|Vector)\\b"), TokenType::basicType },

			{ Regex("^([A-Za-z_][A-Za-z0-9_]*)\\b"), TokenType::symbol },

		};
		static void removeComments(String & input);
		Lexer() = default;
		~Lexer() = default;
		public:
		Lexer(const Lexer &) = delete;
		Lexer(Lexer &&) = delete;
		Lexer & operator = (const Lexer &) = delete;
		Lexer & operator = (Lexer &&) = delete;
		static Lexer * self() {
			static Lexer instance;
			return & instance;
		}
		Array<Token> * tokenise(String * input) const;
	};

	/* Regex Tools */

	class RegexTools {
		private:
		RegexTools() = default;
		public:
		static Bool test(Regex & regex, const String & input);
		static String match(Regex & regex, const String & input);
		static String findFirstGroup(Regex & regex, const String & input);
		static Array<String> findAllGroups(Regex & regex, const String & input);
		static void replaceMatches(const String & mtc, String & input, const String & rpl);
	};

	/* Parser */

	class SyntaxError: public Exception {
		private:
		const String message;
		const UInt64 line;
		public:
		const UInt64 & getLine() const { return line; }
		const String & getMessage() const { return message; }
		SyntaxError(String m, UInt64 l):
		Exception(), message(m), line(l) { }
	};
	class UnitError {
		private:
		const Array<SyntaxError> * const errors;
		const String name;
		public:
		const Array<SyntaxError> * const getErrors() const { return errors; }
		const String & getName() const { return name; }
		UnitError(Array<SyntaxError> * e, String n): errors(e), name(n) { }
		~UnitError() { delete errors; }
	};
	class ParserErrorException: public Exception {
		private:
		const Array<UnitError *> * const units;
		public:
		const Array<UnitError *> * const getUnitErrors() const { return units; }
		ParserErrorException(Array<UnitError *> * u): Exception(), units(u) { }
		ParserErrorException(UnitError * u):
		Exception(), units(new Array<UnitError *>({ u })) { }
		~ParserErrorException() {
			if (!units) return;
			for (UnitError * u : * units) delete u;
			delete units;
		}
	};
	class Parser {
		private:
		CodeUnit * currentUnit = nullptr;
		Array<Token> * tokens = nullptr;
		Array<SyntaxError> * errors = new Array<SyntaxError>();
		SizeType index = 0;
		Bool isInControlFlow = false;
		Bool isInFunction = false;
		Bool isInProcedure = false;
		Expression * expression();
		Expression * assignment();
		Expression * shortOR();
		Expression * shortAND();
		Expression * equality();
		Expression * comparison();
		Expression * lowPriorityOperator();
		Expression * mediumPriorityOperator();
		Expression * highPriorityOperator();
		Expression * subscription();
		Expression * completeSubscript(Expression * item);
		Expression * call();
		Expression * completeCall(Expression * callee, Bool isConstructor);
		Expression * primary();
		String * typeString();
		Statement * declaration();
		Statement * variableDeclaration(String stringType, Bool isClass = false);
		Statement * vectorDeclaration();
		Statement * statement();
		Statement * ifStatement();
		Statement * blockStatement();
		Statement * breakStatement();
		Statement * continueStatement();
		Statement * expressionStatement();
		Statement * functionStatement();
		Statement * procedureStatement();
		Statement * classDeclaration();
		Statement * forStatement();
		Statement * printStatement();
		Statement * whileStatement();
		Statement * doWhileStatement();
		Statement * untilStatement();
		Statement * repeatUntilStatement();
		Statement * loopStatement();
		Statement * restStatement();
		Statement * returnStatement();
		Statement * deleteStatement();
		Statement * fileStatement();
		void replace(TokenType type, String lexeme, TokenType newType);
		void runTypeClassification();
		String parseImport(SizeType & i);
		SyntaxTree * runImportClassification(SyntaxTree * syntaxTree);
		void runCastClassification();
		void runVectorClassification();
		void cleanEmptyTokens();
		inline Bool match(TokenType type);
		inline Bool matchRange(TokenType from, TokenType to);
		inline Bool check(TokenType type);
		inline Bool isOutOfRange();
		inline Bool isAtEnd();
		inline Token peek();
		inline Token peekAdvance();
		inline Token previous();
		inline Token advance();
		inline Token consume(TokenType type, String lexeme = String());
		inline void resetState();
		void synchronise();
		Parser() = default;
		~Parser() = default;
		public:
		Parser(const Parser &) = delete;
		Parser(Parser &&) = delete;
		Parser & operator = (const Parser &) = delete;
		Parser & operator = (Parser &&) = delete;
		static Parser * self() {
			static Parser instance;
			return & instance;
		}
		SyntaxTree * parse(Array<CodeUnit *> * units);
		SyntaxTree * parse(CodeUnit * unit);
	};

}

#endif
