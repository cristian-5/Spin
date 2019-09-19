
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

namespace Stack {

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
	struct FilePosition {
		UInt32 col = 0;
		UInt32 row = 0;
	};
	class Linker {
		private:
		Linker() = default;
		public:
		static FilePosition getPosition(String * input, UInt32 cursor);
		static ArrayList<String> linesFromFile(String & path);
		static String stringFromFile(String path);
		static void createNewFile(String path, String content = "");
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
		ketSymbol,

		intLiteral,
		stringLiteral,
		charLiteral,
		boolLiteral,
		imaginaryLiteral,
		realLiteral,
		colourLiteral,
		emptyLiteral,

		arrow,

		colon,
		semicolon,
		comma,
		dot,

		diamond,

		plusEqual,
		minusEqual,
		starEqual,
		slashEqual,
		tildeEqual,
		pipeEqual,
		ampersandEqual,
		modulusEqual,
		hatEqual,

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

		plus,
		minus,
		tilde,
		star,
		backslash,
		slash,
		at,
		pipe,
		hashtag,
		ampersand,
		modulus,
		dollar,
		hat,

		openParenthesis,
		closeParenthesis,

		openBracket,
		closeBracket,

		openBrace,
		closeBrace,

		tryKeyword,
		catchKeyword,
		throwKeyword,
		throwsKeyword,

		printKeyword,

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

		importKeyword,
		funcKeyword,
		procKeyword,
		staticKeyword,
		classKeyword,
		enumKeyword,
		structKeyword,
		exceptKeyword,
		privateKeyword,
		publicKeyword,
		constKeyword,
		returnKeyword,
		restKeyword,

		refKeyword,
		cpyKeyword,

		invalid,

		endFile

	};
	class Token {
		public:
		TokenType type = TokenType::empty;
		String lexeme = "";
		UInt32 position = 0;
		Token() = default;
		Token(String l, TokenType t, UInt32 p = 0);
		Bool isTypeLiteral() const;
		Bool isTypeType() const;
	};
	class TokenRule {
		public:
		TokenType type = TokenType::empty;
		String pattern = "";
		TokenRule() = default;
		TokenRule(String p, TokenType t);
	};

	/* Basic Types */

	enum BasicType {

		BoolType,
		CharacterType,

		ByteType,
		
		Int64Type,

		RealType,

		ColourType,

		ImaginaryType,
		ComplexType,

		StringType,

		ArrayListType,

		VectorType,

		FunctionType,

		ClassType,
		StructureType,
		ExceptionType,

		UnknownType

	};

	/* Converter */

	class Object;
	class Colour;

	class Converter {
		private:
		Converter() = default;
		static Bool checkBase(String base, String & s);
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
		Object * getReference(String name);
		Object * getValue(String name);
	};

	/* Base Classes */

	class Array;
	class Assignment;
	class Binary;
	class Call;
	class Comparison;
	class Get;
	class Grouping;
	class Literal;
	class Logical;
	class Set;
	class Subscript;
	class Super;
	class This;
	class Unary;
	class Identifier;

	class BlockStatement;
	class BreakStatement;
	class ContinueStatement;
	class DoWhileStatement;
	class ExpressionStatement;
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
			virtual void visitArrayExpression(Array * e) = 0;
			virtual void visitAssignmentExpression(Assignment * e) = 0;
			virtual void visitBinaryExpression(Binary * e) = 0;
			virtual void visitCallExpression(Call * e) = 0;
			virtual void visitComparisonExpression(Comparison * e) = 0;
			virtual void visitGetExpression(Get * e) = 0;
			virtual void visitGroupingExpression(Grouping * e) = 0;
			virtual void visitLiteralExpression(Literal * e) = 0;
			virtual void visitLogicalExpression(Logical * e) = 0;
			virtual void visitSetExpression(Set * e) = 0;
			virtual void visitSubscriptExpression(Subscript * e) = 0;
			virtual void visitSuperExpression(Super * e) = 0;
			virtual void visitThisExpression(This * e) = 0;
			virtual void visitUnaryExpression(Unary * e) = 0;
			virtual void visitIdentifierExpression(Identifier * e) = 0;
		};
		virtual void accept(Visitor *) { }
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
			virtual void visitContinueStatement(ContinueStatement * e) = 0;
			virtual void visitDoWhileStatement(DoWhileStatement * e) = 0;
			virtual void visitExpressionStatement(ExpressionStatement * e) = 0;
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

	class Array: public Expression {
		public:
		ArrayList<Expression *> values = ArrayList<Expression *>();
		Array(ArrayList<Expression *> v);
		void accept(Visitor * visitor) override;
		~Array();
	};
	class Assignment: public Expression {
		public:
		Token * name = nullptr;
		Expression * value = nullptr;
		Assignment(Token * name, Expression * value);
		void accept(Visitor * visitor) override;
		~Assignment();
	};
	class Binary: public Expression {
		public:
		Expression * r = nullptr;
		Expression * l = nullptr;
		Token * o = nullptr;
		Binary(Expression * ls, Token * op, Expression * rs);
		void accept(Visitor * visitor) override;
		~Binary();
	};
	class Call: public Expression {
		public:
		Token * parenthesis = nullptr;
		Expression * callee = nullptr;
		ArrayList<Expression *> arguments = ArrayList<Expression *>();
		Call(Expression * c, Token * p, ArrayList<Expression *> a);
		void accept(Visitor * visitor) override;
		~Call();
	};
	class Comparison: public Expression {
		public:
		Expression * r = nullptr;
		Expression * l = nullptr;
		Token * o = nullptr;
		Comparison(Expression * ls, Token * op, Expression * rs);
		void accept(Visitor * visitor) override;
		~Comparison();
	};
	class Get: public Expression {
		public:
		Expression * object = nullptr;
		Token * name = nullptr;
		Get(Expression * o, Token * n);
		void accept(Visitor * visitor) override;
		~Get();
	};
	class Grouping: public Expression {
		public:
		Expression * expression = nullptr;
		Grouping(Expression * e);
		void accept(Visitor * visitor) override;
		~Grouping();
	};
	class Literal: public Expression {
		public:
		Token * token = nullptr;
		Object * object = nullptr;
		Literal(Token * t);
		void accept(Visitor * visitor) override;
		~Literal();
	};
	class Logical: public Expression {
		public:
		Expression * r = nullptr;
		Expression * l = nullptr;
		Token * o = nullptr;
		Logical(Expression * ls, Token * op, Expression * rs);
		void accept(Visitor * visitor) override;
		~Logical();
	};
	class Set: public Expression {
		public:
		Expression * object = nullptr;
		Token * name = nullptr;
		Expression * value = nullptr;
		Set(Expression * o, Token * n, Expression * v);
		void accept(Visitor * visitor) override;
		~Set();
	};
	class Subscript: public Expression {
		public:
		Token * bracket = nullptr;
		Expression * item = nullptr;
		Expression * expression = nullptr;
		Subscript(Expression * i, Token * b, Expression * e);
		void accept(Visitor * visitor) override;
		~Subscript();
	};
	class Super: public Expression {
		public:
		Token * keyword = nullptr;
		Token * method = nullptr;
		Super(Token * k, Token * m);
		void accept(Visitor * visitor) override;
		~Super();
	};
	class This: public Expression {
		public:
		Token * keyword = nullptr;
		This(Token * k);
		void accept(Visitor * visitor) override;
		~This();
	};
	class Unary: public Expression {
		public:
		Expression * r = nullptr;
		Token * o = nullptr;
		Unary(Token * op, Expression * rs);
		void accept(Visitor * visitor) override;
		~Unary();
	};
	class Identifier: public Expression {
		public:
		Token * name = nullptr;
		Identifier(Token * n);
		void accept(Visitor * visitor) override;
		~Identifier();
	};

	class Parameter {
		public:
		Bool reference = false;
		BasicType type = BasicType::UnknownType;
		Token * tokenType = nullptr;
		Token * name = nullptr;
		Parameter() = default;
		Parameter(BasicType bt, Token * tt, Token * nm, Bool rf = false);
		Parameter * copy() const;
		~Parameter();
	};

	class BlockStatement: public Statement {
		public:
		ArrayList<Statement *> statements = ArrayList<Statement *>();
		BlockStatement(ArrayList<Statement *> s);
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
	class ContinueStatement: public Statement {
		public:
		Token * continueToken = nullptr;
		ContinueStatement(Token * c);
		void accept(Visitor * visitor) override;
		~ContinueStatement();
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
		ArrayList<Parameter *> params = ArrayList<Parameter *>();
		BlockStatement * body = nullptr;
		Parameter * returnType = nullptr;
		FunctionStatement(Token * n, ArrayList<Parameter *> p, BlockStatement * b, Parameter * r);
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
		ArrayList<Parameter *> params = ArrayList<Parameter *>();
		BlockStatement * body = nullptr;
		ProcedureStatement(Token * n, ArrayList<Parameter *> p, BlockStatement * b);
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
		VariableStatement(Token * n, Expression * i, BasicType t);
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
		Complex getConjugate();
		void conjugate();
		Real getNormalized();
		Real getMagnitude();
		inline Real getModulus();
		Real getPhase();
		inline Real getAngle();
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
		virtual Object * call(Interpreter * i, ArrayList<Object *> a, Token * c) = 0;
		virtual String stringValue() const = 0;
		virtual UInt32 arity() const = 0;
		virtual CallProtocol * copy() const = 0;
	};
	class Function: CallProtocol {
		private:
		FunctionStatement * declaration = nullptr;
		Environment * closure = nullptr;
		public:
		Function() = default;
		Function(FunctionStatement * d, Environment * c);
		~Function() = default;
		Object * call(Interpreter * i, ArrayList<Object *> a, Token * c) override;
		String stringValue() const override;
		UInt32 arity() const override;
		CallProtocol * copy() const override;
    };
	class Procedure: CallProtocol {
		private:
		ProcedureStatement * declaration = nullptr;
		Environment * closure = nullptr;
		public:
		Procedure() = default;
		Procedure(ProcedureStatement * d, Environment * c);
		~Procedure() = default;
		Object * call(Interpreter * i, ArrayList<Object *> a, Token * c) override;
		String stringValue() const override;
		UInt32 arity() const override;
		CallProtocol * copy() const override;
    };
	typedef Lambda<Object * (Interpreter * i, ArrayList<Object *> a)> NativeLambda;
	class NativeFunction: CallProtocol {
		private:
		UInt32 _arity = 0;
		NativeLambda _lambda = nullptr;
		public:
		NativeFunction(NativeLambda l, UInt32 a = 0);
		~NativeFunction() = default;
		Object * call(Interpreter * i, ArrayList<Object *> a, Token * c) override;
		String stringValue() const override;
		UInt32 arity() const override;
		CallProtocol * copy() const override;
	};

	/* Processor */

	class EvaluationError: public Exception {
		private:
		const String _message;
		const Token _token;
		public:
		const String & getMessage() const { return _message; }
		const Token & getToken() const { return _token; }
		EvaluationError(String message, Token token):
		Exception(), _message(message), _token(token) { }
	};
	class Processor {
		private:
		Processor() = default;
		~Processor() = default;
		typedef Lambda<Object * (Object *)> UnaryHandler;
		typedef Lambda<Object * (Object *, Object *)> BinaryHandler;
		typedef Lambda<void (Object *, Object *)> AssignmentHandler;
		typedef Pair<BasicType, BasicType> BasicTypes;
		Map<BasicType, UnaryHandler> unaryNegation = {
			{
				BasicType::Int64Type,
				[] (Object * o) {
					Int64 * i = (Int64 *) o -> value;
					return new Object(o -> type, new Int64(-(* i)));
				}
			},
			{
				BasicType::RealType,
				[] (Object * o) {
					Real * i = (Real *) o -> value;
					return new Object(o -> type, new Real(-(* i)));
				}
			},
			{
				BasicType::ImaginaryType,
				[] (Object * o) {
					Real * i = (Real *) o -> value;
					return new Object(o -> type, new Real(-(* i)));
				}
			},
			{
				BasicType::ComplexType,
				[] (Object * o) {
					Complex * c = (Complex *) o -> value;
					return new Object(o -> type, new Complex(-(* c)));
				}
			}
		};
		Map<BasicType, UnaryHandler> unaryInversion = {
			{
				BasicType::ByteType,
				[] (Object * o) {
					UInt8 * b = (UInt8 *) o -> value;
					b = new UInt8(~(* b));
					return new Object(o -> type, b);
				}
			},
			{
				BasicType::ColourType,
				[] (Object * o) {
					Colour * c = (Colour *) o -> value;
					c = new Colour(~(* c));
					return new Object(o -> type, c);
				}
			},
			{
				BasicType::Int64Type,
				[] (Object * o) {
					Int64 * i = (Int64 *) o -> value;
					i = new Int64(~(* i));
					return new Object(o -> type, i);
				}
			}
		};
		Map<BasicTypes, BinaryHandler> binaryAddition = {
			{
				{ BasicType::Int64Type, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Int64 * c = new Int64((* a) + (* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::RealType, BasicType::RealType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Real * c = new Real((* b) + (* a));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::ByteType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Int64 * c = new Int64((* a) + (Int64)(* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::RealType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					Real * c = new Real((Real)(* a) + (* b));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::ImaginaryType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					Complex * c = new Complex((Real)(* a), (* b));
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::ComplexType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Complex * b = (Complex *) r -> value;
					Complex * c = new Complex((* a) + b -> a, b -> b);
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				{ BasicType::ComplexType, BasicType::RealType },
				[] (Object * l, Object * r) {
					Complex * a = (Complex *) l -> value;
					Real * b = (Real *) r -> value;
					Complex * c = new Complex((* b) + a -> a, a -> b);
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				{ BasicType::ImaginaryType, BasicType::ImaginaryType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Real * c = new Real((* a) + (* b));
					return new Object(BasicType::ImaginaryType, c);
				}
			},
			{
				{ BasicType::ComplexType, BasicType::ImaginaryType },
				[] (Object * l, Object * r) {
					Complex * a = (Complex *) l -> value;
					Real * b = (Real *) r -> value;
					Complex * c = new Complex(a -> a, a -> b + (* b));
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				{ BasicType::ImaginaryType, BasicType::RealType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Complex * c = new Complex((* b), (* a));
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				{ BasicType::ColourType, BasicType::ColourType },
				[] (Object * l, Object * r) {
					Colour * a = (Colour *) l -> value;
					Colour * b = (Colour *) r -> value;
					Colour * c = new Colour((* a) + (* b));
					return new Object(BasicType::ColourType, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					Character * c = new Character((* a) + (* b));
					return new Object(BasicType::CharacterType, c);
				}
			},
			{
				{ BasicType::ByteType, BasicType::ByteType },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					UInt8 * c = new UInt8((* a) + (* b));
					return new Object(BasicType::ByteType, c);
				}
			}
		};
		Map<BasicTypes, BinaryHandler> stringAddition = {
			{
				{ BasicType::StringType, BasicType::StringType },
				[] (Object * l, Object * r) {
					String * a = (String *) l -> value;
					String * b = (String *) r -> value;
					String * c = new String((* a) + (* b));
					return new Object(BasicType::StringType, c);
				}
			},
			{
				{ BasicType::StringType, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					String * a = (String *) l -> value;
					Character * b = (Character *) r -> value;
					StringStream s = StringStream();
					s << (* a) << (* b);
					String * c = new String(s.str());
					return new Object(BasicType::StringType, c);
				}
			},
			{
				{ BasicType::StringType, BasicType::RealType },
				[] (Object * l, Object * r) {
					String * a = (String *) l -> value;
					Real * b = (Real *) r -> value;
					String * c = new String((* a) + realToString(* b));
					return new Object(BasicType::StringType, c);
				}
			},
			{
				{ BasicType::StringType, BasicType::ImaginaryType },
				[] (Object * l, Object * r) {
					String * a = (String *) l -> value;
					Real * b = (Real *) r -> value;
					String * c = new String((* a) + realToString(* b) + "i");
					return new Object(BasicType::StringType, c);
				}
			},
			{
				{ BasicType::StringType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					String * a = (String *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					String * c = new String((* a) + intToString(* b));
					return new Object(BasicType::StringType, c);
				}
			},
			{
				{ BasicType::StringType, BasicType::ColourType },
				[] (Object * l, Object * r) {
					String * a = (String *) l -> value;
					Colour * b = (Colour *) r -> value;
					String * c = new String((* a) + b -> stringValue());
					return new Object(BasicType::StringType, c);
				}
			},
			{
				{ BasicType::StringType, BasicType::ComplexType },
				[] (Object * l, Object * r) {
					String * a = (String *) l -> value;
					Complex * b = (Complex *) r -> value;
					String * c = new String((* a) + b -> stringValue());
					return new Object(BasicType::StringType, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::StringType },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					String * b = (String *) r -> value;
					StringStream s = StringStream();
					s << (* a) << (* b);
					String * c = new String(s.str());
					return new Object(BasicType::StringType, c);
				}
			},
			{
				{ BasicType::RealType, BasicType::StringType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					String * b = (String *) r -> value;
					String * c = new String(realToString(* a) + (* b));
					return new Object(BasicType::StringType, c);
				}
			},
			{
				{ BasicType::ImaginaryType, BasicType::StringType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					String * b = (String *) r -> value;
					String * c = new String(realToString(* a) + "i" + (* b));
					return new Object(BasicType::StringType, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::StringType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					String * b = (String *) r -> value;
					String * c = new String(intToString(* a) + (* b));
					return new Object(BasicType::StringType, c);
				}
			},
			{
				{ BasicType::ColourType, BasicType::StringType },
				[] (Object * l, Object * r) {
					Colour * a = (Colour *) l -> value;
					String * b = (String *) r -> value;
					String * c = new String(a -> stringValue() + (* b));
					return new Object(BasicType::StringType, c);
				}
			},
			{
				{ BasicType::ComplexType,  BasicType::StringType },
				[] (Object * l, Object * r) {
					Complex * a = (Complex *) l -> value;
					String * b = (String *) r -> value;
					String * c = new String(a -> stringValue() + (* b));
					return new Object(BasicType::StringType, c);
				}
			}
		};
		Map<BasicTypes, BinaryHandler> binarySubtraction = {
			{
				{ BasicType::Int64Type, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Int64 * c = new Int64((* a) - (* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::RealType, BasicType::RealType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Real * c = new Real((* a) - (* b));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::RealType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					Real * c = new Real((Real)(* a) - (* b));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				{ BasicType::RealType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Real * c = new Real((* a) - (Real)(* b));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::ByteType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Int64 * c = new Int64((* a) - (* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::ByteType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Int64 * c = new Int64((Int64)(* a) - (* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::ComplexType, BasicType::ComplexType },
				[] (Object * l, Object * r) {
					Complex * a = (Complex *) l -> value;
					Complex * b = (Complex *) r -> value;
					Complex * c = new Complex((* a) - (* b));
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				{ BasicType::ImaginaryType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Complex * c = new Complex(-(Real)(* b), (* a));
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::ImaginaryType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					Complex * c = new Complex((Real)(* b), -(* a));
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				{ BasicType::ImaginaryType, BasicType::RealType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Complex * c = new Complex(-(* b), (* a));
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				{ BasicType::RealType, BasicType::ImaginaryType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Complex * c = new Complex((* a), -(* b));
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				{ BasicType::ComplexType, BasicType::ImaginaryType },
				[] (Object * l, Object * r) {
					Complex * a = (Complex *) l -> value;
					Real * b = (Real *) r -> value;
					Complex * c = new Complex((* a).a, (* a).b - (* b));
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				{ BasicType::ImaginaryType, BasicType::ComplexType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Complex * b = (Complex *) r -> value;
					Complex * c = new Complex(-(* b).a, (* a) - (* b).b);
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				{ BasicType::ComplexType, BasicType::RealType },
				[] (Object * l, Object * r) {
					Complex * a = (Complex *) l -> value;
					Real * b = (Real *) r -> value;
					Complex * c = new Complex((* a).a - (* b), (* a).b);
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				{ BasicType::RealType, BasicType::ComplexType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Complex * b = (Complex *) r -> value;
					Complex * c = new Complex((* a) - (* b).a, -(* b).b);
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				{ BasicType::ComplexType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Complex * a = (Complex *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Complex * c = new Complex((* a).a - (Real)(* b), (* a).b);
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::ComplexType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Complex * b = (Complex *) r -> value;
					Complex * c = new Complex((Real)(* a) - (* b).a, -(* b).b);
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					Character * c = new Character((* a) - (* b));
					return new Object(BasicType::CharacterType, c);
				}
			}
		};
		Map<BasicTypes, BinaryHandler> binaryMultiplication = {
			{
				{ BasicType::Int64Type, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Int64 * c = new Int64((* a) * (* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::RealType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					Real * c = new Real((Real)(* a) * (* b));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				{ BasicType::RealType, BasicType::RealType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Real * c = new Real((* a) * (* b));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				{ BasicType::ByteType, BasicType::ByteType },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					UInt8 * c = new UInt8((* a) * (* b));
					return new Object(BasicType::ByteType, c);
				}
			},
			{
				{ BasicType::ByteType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Int64 * c = new Int64((Int64)(* a) * (* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					Character * c = new Character((* a) * (* b));
					return new Object(BasicType::CharacterType, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Int64 * c = new Int64((Int64)(* a) * (* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::ColourType, BasicType::ColourType },
				[] (Object * l, Object * r) {
					Colour * a = (Colour *) l -> value;
					Colour * b = (Colour *) r -> value;
					Colour * c = new Colour((* a) * (* b));
					return new Object(BasicType::ColourType, c);
				}
			},
			{
				{ BasicType::ImaginaryType, BasicType::RealType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Real * c = new Real((* a) * (* b));
					return new Object(BasicType::ImaginaryType, c);
				}
			},
			{
				{ BasicType::ImaginaryType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Real * c = new Real((* a) * (Real)(* b));
					return new Object(BasicType::ImaginaryType, c);
				}
			},
			{
				{ BasicType::ImaginaryType, BasicType::ImaginaryType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Real * c = new Real(-((* a) * (* b)));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				{ BasicType::ImaginaryType, BasicType::ComplexType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Complex * b = (Complex *) r -> value;
					Complex * c = new Complex(-(* a) * b -> b, (* a) * b -> a);
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				{ BasicType::ComplexType, BasicType::ComplexType },
				[] (Object * l, Object * r) {
					Complex * a = (Complex *) l -> value;
					Complex * b = (Complex *) r -> value;
					Complex * c = new Complex((* a) * (* b));
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				{ BasicType::ComplexType, BasicType::RealType },
				[] (Object * l, Object * r) {
					Complex * a = (Complex *) l -> value;
					Real * b = (Real *) r -> value;
					Complex * c = new Complex(a -> a * (* b), a -> b * (* b));
					return new Object(BasicType::ComplexType, c);
				}
			},
			{
				{ BasicType::ComplexType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Complex * a = (Complex *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Complex * c = new Complex(a -> a * (Real)(* b), a -> b * (Real)(* b));
					return new Object(BasicType::ComplexType, c);
				}
			}
		};
		Map<BasicTypes, BinaryHandler> binaryDivision = {
			{
				{ BasicType::Int64Type, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					if ((* b) == 0) return new Object();
					Int64 * c = new Int64((* a) / (* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::RealType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					if ((* b) == 0.0) return new Object();
					Real * c = new Real((Real)(* a) / (* b));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				{ BasicType::RealType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					if ((* b) == 0) return new Object();
					Real * c = new Real((* a) / (Real)(* b));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				{ BasicType::RealType, BasicType::RealType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					if ((* b) == 0.0) return new Object();
					Real * c = new Real((* a) / (* b));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				{ BasicType::ByteType, BasicType::ByteType },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					if ((* b) == 0) return new Object();
					UInt8 * c = new UInt8((* a) / (* b));
					return new Object(BasicType::ByteType, c);
				}
			},
			{
				{ BasicType::ByteType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					if ((* b) == 0) return new Object();
					Int64 * c = new Int64((Int64)(* a) / (* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::ByteType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					if ((* b) == 0) return new Object();
					Int64 * c = new Int64((* a) / (Int64)(* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					if ((* b) == 0) return new Object();
					Character * c = new Character((* a) / (* b));
					return new Object(BasicType::CharacterType, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					if ((* b) == 0) return new Object();
					Int64 * c = new Int64((Int64)(* a) / (* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::ByteType },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					if ((* b) == 0) return new Object();
					Character * c = new Character((* a) / (UInt8)(* b));
					return new Object(BasicType::CharacterType, c);
				}
			},
			{
				{ BasicType::ColourType, BasicType::ColourType },
				[] (Object * l, Object * r) {
					Colour * a = (Colour *) l -> value;
					Colour * b = (Colour *) r -> value;
					Colour * c = new Colour((* a) / (* b));
					return new Object(BasicType::ColourType, c);
				}
			},
			{
				{ BasicType::ImaginaryType, BasicType::RealType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					if ((* b) == 0.0) return new Object();
					Real * c = new Real((* a) / (* b));
					return new Object(BasicType::ImaginaryType, c);
				}
			},
			{
				{ BasicType::ImaginaryType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					if ((* b) == 0) return new Object();
					Real * c = new Real((* a) / (Real)(* b));
					return new Object(BasicType::ImaginaryType, c);
				}
			},
			{
				{ BasicType::ImaginaryType, BasicType::ImaginaryType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					if ((* b) == 0) return new Object();
					Real * c = new Real((* a) / (* b));
					return new Object(BasicType::RealType, c);
				}
			},
			{
				{ BasicType::ComplexType, BasicType::ComplexType },
				[] (Object * l, Object * r) {
					Complex * a = (Complex *) l -> value;
					Complex * b = (Complex *) r -> value;
					Complex * c = nullptr;
					try { c = new Complex((* a) / (* b)); }
					catch (ComplexDBZException & e) { return new Object(); }
					return new Object(BasicType::ComplexType, c);
				}
			}
		};
		Map<BasicTypes, BinaryHandler> binaryAND = {
			{
				{ BasicType::Int64Type, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Int64 * c = new Int64((* a) & (* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::ByteType, BasicType::ByteType },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					UInt8 * c = new UInt8((* a) & (* b));
					return new Object(BasicType::ByteType, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					Character * c = new Character((* a) & (* b));
					return new Object(BasicType::CharacterType, c);
				}
			},
			{
				{ BasicType::BoolType, BasicType::BoolType },
				[] (Object * l, Object * r) {
					Bool * a = (Bool *) l -> value;
					Bool * b = (Bool *) r -> value;
					Bool * c = new Bool((* a) && (* b));
					return new Object(BasicType::BoolType, c);
				}
			}
		};
		Map<BasicTypes, BinaryHandler> binaryXOR = {
			{
				{ BasicType::Int64Type, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Int64 * c = new Int64((* a) ^ (* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::ByteType, BasicType::ByteType },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					UInt8 * c = new UInt8((* a) ^ (* b));
					return new Object(BasicType::ByteType, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					Character * c = new Character((* a) ^ (* b));
					return new Object(BasicType::CharacterType, c);
				}
			},
			{
				{ BasicType::BoolType, BasicType::BoolType },
				[] (Object * l, Object * r) {
					Bool * a = (Bool *) l -> value;
					Bool * b = (Bool *) r -> value;
					Bool * c = new Bool((* a) != (* b));
					return new Object(BasicType::BoolType, c);
				}
			}
		};
		Map<BasicTypes, BinaryHandler> binaryOR = {
			{
				{ BasicType::Int64Type, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Int64 * c = new Int64((* a) | (* b));
					return new Object(BasicType::Int64Type, c);
				}
			},
			{
				{ BasicType::ByteType, BasicType::ByteType },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					UInt8 * c = new UInt8((* a) | (* b));
					return new Object(BasicType::ByteType, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					Character * c = new Character((* a) | (* b));
					return new Object(BasicType::CharacterType, c);
				}
			},
			{
				{ BasicType::BoolType, BasicType::BoolType },
				[] (Object * l, Object * r) {
					Bool * a = (Bool *) l -> value;
					Bool * b = (Bool *) r -> value;
					Bool * c = new Bool((* a) || (* b));
					return new Object(BasicType::BoolType, c);
				}
			}
		};
		Object * applyAddition(Token * t, Object * l, Object * r);
		Object * applySubtraction(Token * t, Object * l, Object * r);
		Object * applyMultiplication(Token * t, Object * l, Object * r);
		Object * applyDivision(Token * t, Object * l, Object * r);
		Object * applyModulus(Token * t, Object * l, Object * r);
		Object * applyAND(Token * t, Object * l, Object * r);
		Object * applyXOR(Token * t, Object * l, Object * r);
		Object * applyOR(Token * t, Object * l, Object * r);
		Map<BasicTypes, BinaryHandler> binaryStrictEquality = {
			{
				{ BasicType::Int64Type, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((* a) == (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::RealType, BasicType::RealType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((* a) == (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::StringType, BasicType::StringType },
				[] (Object * l, Object * r) {
					String * a = (String *) l -> value;
					String * b = (String *) r -> value;
					Bool * c = new Bool((* a) == (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::BoolType, BasicType::BoolType },
				[] (Object * l, Object * r) {
					Bool * a = (Bool *) l -> value;
					Bool * b = (Bool *) r -> value;
					Bool * c = new Bool((* a) == (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::ColourType, BasicType::ColourType },
				[] (Object * l, Object * r) {
					Colour * a = (Colour *) l -> value;
					Colour * b = (Colour *) r -> value;
					Bool * c = new Bool((* a) == (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::ComplexType, BasicType::ComplexType },
				[] (Object * l, Object * r) {
					Complex * a = (Complex *) l -> value;
					Complex * b = (Complex *) r -> value;
					Bool * c = new Bool((* a) == (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::ImaginaryType, BasicType::ImaginaryType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((* a) == (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::ByteType, BasicType::ByteType },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((* a) == (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) == (* b));
					return new Object(BasicType::BoolType, c);
				}
			}
		};
		Map<BasicTypes, BinaryHandler> binaryMixedEquality = {
			{
				{ BasicType::Int64Type, BasicType::RealType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((Real)(* a) == (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::ByteType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((* a) == (Int64)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::ByteType },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((* a) == (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) == (Int64)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::ImaginaryType, BasicType::ComplexType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Complex * b = (Complex *) r -> value;
					Bool * c = new Bool(((* a) == b -> b) && (b -> a == 0));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::RealType, BasicType::ComplexType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Complex * b = (Complex *) r -> value;
					Bool * c = new Bool(((* a) == b -> a) && (b -> b == 0));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::StringType, BasicType::CharacterType },
				[] (Object * l, Object * r) {
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
		Map<BasicTypes, BinaryHandler> binaryMajor = {
			{
				{ BasicType::Int64Type, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((* a) > (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::RealType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((Real)(* a) > (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::ByteType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((* a) > (Int64)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) > (Int64)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::RealType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((* a) > (Real)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) > (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::ByteType, BasicType::ByteType },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((* a) > (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::RealType, BasicType::RealType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((* a) > (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::ImaginaryType, BasicType::ImaginaryType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((* a) > (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::ByteType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((Int64)(* a) > (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((Int64)(* a) > (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::ByteType },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((UInt8)(* a) > (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::ByteType, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) > (UInt8)(* b));
					return new Object(BasicType::BoolType, c);
				}
			}
		};
		Map<BasicTypes, BinaryHandler> binaryMajorEqual = {
			{
				{ BasicType::Int64Type, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((* a) >= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::RealType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((Real)(* a) >= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::ByteType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((* a) >= (Int64)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) >= (Int64)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::RealType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((* a) >= (Real)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) >= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::ByteType, BasicType::ByteType },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((* a) >= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::RealType, BasicType::RealType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((* a) >= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::ImaginaryType, BasicType::ImaginaryType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((* a) >= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::ByteType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((Int64)(* a) >= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((Int64)(* a) >= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::ByteType },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((UInt8)(* a) >= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::ByteType, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) >= (UInt8)(* b));
					return new Object(BasicType::BoolType, c);
				}
			}
		};
		Map<BasicTypes, BinaryHandler> binaryMinor = {
			{
				{ BasicType::Int64Type, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((* a) < (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::RealType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((Real)(* a) < (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::ByteType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((* a) < (Int64)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) < (Int64)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::RealType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((* a) < (Real)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) < (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::ByteType, BasicType::ByteType },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((* a) < (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::RealType, BasicType::RealType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((* a) < (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::ImaginaryType, BasicType::ImaginaryType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((* a) < (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::ByteType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((Int64)(* a) < (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((Int64)(* a) < (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::ByteType },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((UInt8)(* a) < (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::ByteType, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) < (UInt8)(* b));
					return new Object(BasicType::BoolType, c);
				}
			}
		};
		Map<BasicTypes, BinaryHandler> binaryMinorEqual = {
			{
				{ BasicType::Int64Type, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((* a) <= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::RealType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((Real)(* a) <= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::ByteType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((* a) <= (Int64)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) <= (Int64)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::RealType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((* a) <= (Real)(* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					Bool * c = new Bool((* a) <= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::ByteType, BasicType::ByteType },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((* a) <= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::RealType, BasicType::RealType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((* a) <= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::ImaginaryType, BasicType::ImaginaryType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					Bool * c = new Bool((* a) <= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::ByteType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((Int64)(* a) <= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					Bool * c = new Bool((Int64)(* a) <= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::ByteType },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					Bool * c = new Bool((UInt8)(* a) <= (* b));
					return new Object(BasicType::BoolType, c);
				}
			},
			{
				{ BasicType::ByteType, BasicType::CharacterType },
				[] (Object * l, Object * r) {
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
		Map<BasicTypes, AssignmentHandler> pureAssignment = {
			{
				{ BasicType::Int64Type, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					* a = * b;
				}
			},
			{
				{ BasicType::BoolType, BasicType::BoolType },
				[] (Object * l, Object * r) {
					Bool * a = (Bool *) l -> value;
					Bool * b = (Bool *) r -> value;
					* a = * b;
				}
			},
			{
				{ BasicType::ByteType, BasicType::ByteType },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					* a = * b;
				}
			},
			{
				{ BasicType::StringType, BasicType::StringType },
				[] (Object * l, Object * r) {
					String * a = (String *) l -> value;
					String * b = (String *) r -> value;
					* a = String(* b);
				}
			},
			{
				{ BasicType::RealType, BasicType::RealType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					* a = * b;
				}
			},
			{
				{ BasicType::CharacterType, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					Character * b = (Character *) r -> value;
					* a = * b;
				}
			},
			{
				{ BasicType::ImaginaryType, BasicType::ImaginaryType },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Real * b = (Real *) r -> value;
					* a = * b;
				}
			},
			{
				{ BasicType::ComplexType, BasicType::ComplexType },
				[] (Object * l, Object * r) {
					Complex * a = (Complex *) l -> value;
					Complex * b = (Complex *) r -> value;
					* a = * b;
				}
			},
			{
				{ BasicType::ColourType, BasicType::ColourType },
				[] (Object * l, Object * r) {
					Colour * a = (Colour *) l -> value;
					Colour * b = (Colour *) r -> value;
					* a = * b;
				}
			},
		};
		Map<BasicTypes, AssignmentHandler> mixedAssignment = {
			{
				{ BasicType::Int64Type, BasicType::RealType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Real * b = (Real *) r -> value;
					* a = (Int64)(* b);
				}
			},
			{
				{ BasicType::RealType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Real * a = (Real *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					* a = (Real)(* b);
				}
			},
			{
				{ BasicType::ComplexType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Complex * a = (Complex *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					* a = Complex(* b, 0.0);
				}
			},
			{
				{ BasicType::ComplexType, BasicType::RealType },
				[] (Object * l, Object * r) {
					Complex * a = (Complex *) l -> value;
					Real * b = (Real *) r -> value;
					* a = Complex(* b, 0.0);
				}
			},
			{
				{ BasicType::ComplexType, BasicType::ImaginaryType },
				[] (Object * l, Object * r) {
					Complex * a = (Complex *) l -> value;
					Real * b = (Real *) r -> value;
					* a = Complex(0.0, * b);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					Character * b = (Character *) r -> value;
					* a = (Int64)(* b);
				}
			},
			{
				{ BasicType::Int64Type, BasicType::ByteType },
				[] (Object * l, Object * r) {
					Int64 * a = (Int64 *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					* a = (Int64)(* b);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					* a = (UInt8)(* b);
				}
			},
			{
				{ BasicType::CharacterType, BasicType::ByteType },
				[] (Object * l, Object * r) {
					Character * a = (Character *) l -> value;
					UInt8 * b = (UInt8 *) r -> value;
					* a = * b;
				}
			},
			{
				{ BasicType::ByteType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					* a = (UInt8)(* b);
				}
			},
			{
				{ BasicType::ByteType, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					UInt8 * a = (UInt8 *) l -> value;
					Character * b = (Character *) r -> value;
					* a = (UInt8)(* b);
				}
			},
			{
				{ BasicType::StringType, BasicType::CharacterType },
				[] (Object * l, Object * r) {
					String * a = (String *) l -> value;
					Character * b = (Character *) r -> value;
					StringStream s = StringStream();
					s << (* b);
					* a = String(s.str());
				}
			},
			{
				{ BasicType::StringType, BasicType::Int64Type },
				[] (Object * l, Object * r) {
					String * a = (String *) l -> value;
					Int64 * b = (Int64 *) r -> value;
					* a = String(intToString(* b));
				}
			},
			{
				{ BasicType::StringType, BasicType::RealType },
				[] (Object * l, Object * r) {
					String * a = (String *) l -> value;
					Real * b = (Real *) r -> value;
					* a = String(realToString(* b));
				}
			},
			{
				{ BasicType::StringType, BasicType::ImaginaryType },
				[] (Object * l, Object * r) {
					String * a = (String *) l -> value;
					Real * b = (Real *) r -> value;
					* a = String(realToString(* b));
				}
			},
			{
				{ BasicType::StringType, BasicType::ComplexType },
				[] (Object * l, Object * r) {
					String * a = (String *) l -> value;
					Complex * b = (Complex *) r -> value;
					* a = b -> stringValue();
				}
			},
			{
				{ BasicType::StringType, BasicType::ColourType },
				[] (Object * l, Object * r) {
					String * a = (String *) l -> value;
					Colour * b = (Colour *) r -> value;
					* a = b -> stringValue();
				}
			},
			{
				{ BasicType::StringType, BasicType::BoolType },
				[] (Object * l, Object * r) {
					String * a = (String *) l -> value;
					Bool * b = (Bool *) r -> value;
					* a = (* b) ? "true" : "false";
				}
			}
		};
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
	};

	/* Interpreter */

	class InterpreterErrorException: public Exception {
		private:
		const String _message;
		const FilePosition _position;
		const String _fileName;
		public:
		const String & getMessage() const { return _message; }
		const FilePosition & getPosition() const { return _position; }
		const String & getFileName() const { return _fileName; }
		InterpreterErrorException(String message, FilePosition position, String name):
		Exception(),  _message(message), _position(position), _fileName(name) { }
	};
	class InterpreterReturn: public Exception {
		public: InterpreterReturn(): Exception() { }
	};
	class Interpreter: public Expression::Visitor, public Statement::Visitor {
		private:
		Object * value = nullptr;
		Processor * CPU = Processor::self();
		Environment * memory = nullptr;
		Bool broken = false;
		Bool continued = false;
		void deleteValue();
		void safeDeleteValue();
		void resetValue();
		void setValue(Object * o);
		void visitArrayExpression(Array * e) override;
		void visitAssignmentExpression(Assignment * e) override;
		void visitBinaryExpression(Binary * e) override;
		void visitCallExpression(Call * e) override;
		void visitComparisonExpression(Comparison * e) override;
		void visitGetExpression(Get * e) override;
		void visitGroupingExpression(Grouping * e) override;
		void visitLiteralExpression(Literal * e) override;
		void visitLogicalExpression(Logical * e) override;
		void visitSetExpression(Set * e) override;
		void visitSubscriptExpression(Subscript * e) override;
		void visitSuperExpression(Super * e) override;
		void visitThisExpression(This * e) override;
		void visitUnaryExpression(Unary * e) override;
		void visitIdentifierExpression(Identifier * e) override;
		void evaluateExpression(Expression * e);
		void visitBlockStatement(BlockStatement * e) override;
		void visitBreakStatement(BreakStatement * e) override;
		void visitContinueStatement(ContinueStatement * e) override;
		void visitDoWhileStatement(DoWhileStatement * e) override;
		void visitExpressionStatement(ExpressionStatement * e) override;
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
		void executeBlock(ArrayList<Statement *> statements, Environment * environment);
		public:
		Environment * globals = nullptr;
		Object * getCurrentValue() const;
		Interpreter();
		void executeFunction(BlockStatement * block, Environment * environment);
		void evaluate(ArrayList<Statement *> * statements, String * input = nullptr, String fileName = "Unknown File");
		Object * evaluate(Expression * expression, String * input = nullptr, String fileName = "Unknown File");
	};

	/* Lexer */

	class Lexer {
		private:
		String handleComments(String input) const;
		ArrayList<TokenRule> grammar = {

			{ "([ \\t\\n]+)", TokenType::empty },

			{ "(\\/\\*+[^*]*\\*+(?:[^/*][^*]*\\*+)*\\/)", TokenType::comment },

			{ "([0-9]+(?:\\.[0-9]+(?:[eE][-]?[0-9]+)?)?i)", TokenType::imaginaryLiteral },
			{ "([0-9]+\\.[0-9]+(?:[eE][-]?[0-9]+)?)", TokenType::realLiteral },
			{ "((?:0[x][0-9A-Fa-f]+)|(?:0b[01]+)|(?:0o[0-7]+)|(?:0d[0-9]+)|(?:[0-9]+))", TokenType::intLiteral },
			{ "(\"(?:[^\\\\\"]|\\\\[\"\\\\0abfnrtv]|\\\\0x[0-9A-Fa-f]{2})*\")", TokenType::stringLiteral },
			{ "('(?:[^\\\\]|\\\\0x[0-9A-Fa-f]{2}|\\\\['\\\\0abfnrtv])')", TokenType::charLiteral },
			{ "(#[A-Fa-f0-9]{6}(?:[A-Fa-f0-9][A-Fa-f0-9])?|#[A-Fa-f0-9]{3,4})\\b", TokenType::colourLiteral },
			{ "(false|true)\\b", TokenType::boolLiteral },

			{ "(<[ \\t\\n]*[A-Za-z_][A-Za-z0-9_]*[ \\t\\n]*\\|[ \\t\\n]*[A-Za-z_][A-Za-z0-9_]*[ \\t\\n]*>)", TokenType::braketSymbol },
			{ "(<[ \\t\\n]*[A-Za-z_][A-Za-z0-9_]*[ \\t\\n]*\\|)", TokenType::braSymbol },
			{ "(\\|[ \\t\\n]*[A-Za-z_][A-Za-z0-9_]*[ \\t\\n]*>)", TokenType::ketSymbol },

			{ "(\\->)", TokenType::arrow },
			{ "(\\:)", TokenType::colon },
			{ "(\\;)", TokenType::semicolon },
			{ "(\\,)", TokenType::comma },
			{ "(\\.)", TokenType::dot },
			{ "(<>)", TokenType::diamond },
			{ "(<=)", TokenType::minorEqual },
			{ "(<)", TokenType::minor },
			{ "(>=)", TokenType::majorEqual },
			{ "(>)", TokenType::major },
			{ "(==)", TokenType::equality },
			{ "(=)", TokenType::equal },
			{ "(\\?)", TokenType::questionMark },
			{ "(\\!=)", TokenType::inequality },
			{ "(\\!)", TokenType::exclamationMark },

			{ "(\\+=)", TokenType::plusEqual },
			{ "(\\+)", TokenType::plus },
			{ "(\\-=)", TokenType::minusEqual },
			{ "(\\-)", TokenType::minus },
			{ "(\\~=)", TokenType::tildeEqual },
			{ "(\\~)", TokenType::tilde },
			{ "(\\*=)", TokenType::starEqual },
			{ "(\\*)", TokenType::star },
			{ "(\\\\)", TokenType::backslash },
			{ "(\\/=)", TokenType::slashEqual },
			{ "(\\/)", TokenType::slash },
			{ "(\\@)", TokenType::at },
			{ "(\\|=)", TokenType::pipeEqual },
			{ "(\\|\\|)", TokenType::OR },
			{ "(\\|)", TokenType::pipe },
			{ "(\\#)", TokenType::hashtag },
			{ "(\\&=)", TokenType::ampersandEqual },
			{ "(\\&\\&)", TokenType::AND },
			{ "(\\&)", TokenType::ampersand },
			{ "(\\%=)", TokenType::modulusEqual },
			{ "(\\%)", TokenType::modulus },
			{ "(\\$)", TokenType::dollar },
			{ "(\\^=)", TokenType::hatEqual },
			{ "(\\^)", TokenType::hat },

			{ "(\\()", TokenType::openParenthesis },
			{ "(\\))", TokenType::closeParenthesis },
			{ "(\\[)", TokenType::openBracket },
			{ "(\\])", TokenType::closeBracket },
			{ "(\\{)", TokenType::openBrace },
			{ "(\\})", TokenType::closeBrace },

			{ "(try)\\b", TokenType::tryKeyword },
			{ "(catch)\\b", TokenType::catchKeyword },
			{ "(throw)\\b", TokenType::throwKeyword },
			{ "(throws)\\b", TokenType::throwsKeyword },

			{ "(print)\\b", TokenType::printKeyword },

			{ "(if)\\b", TokenType::ifKeyword },
			{ "(else)\\b", TokenType::elseKeyword },
			{ "(switch)\\b", TokenType::switchKeyword },
			{ "(case)\\b", TokenType::caseKeyword },
			{ "(default)\\b", TokenType::defaultKeyword },
			{ "(while)\\b", TokenType::whileKeyword },
			{ "(do)\\b", TokenType::doKeyword },
			{ "(loop)\\b", TokenType::loopKeyword },
			{ "(for)\\b", TokenType::forKeyword },
			{ "(repeat)\\b", TokenType::repeatKeyword },
			{ "(until)\\b", TokenType::untilKeyword },
			{ "(break)\\b", TokenType::breakKeyword },
			{ "(continue)\\b", TokenType::continueKeyword },

			{ "(import)\\b", TokenType::importKeyword },
			{ "(func)\\b", TokenType::funcKeyword },
			{ "(proc)\\b", TokenType::procKeyword },
			{ "(static)\\b", TokenType::staticKeyword },
			{ "(class)\\b", TokenType::classKeyword },
			{ "(enumerator)\\b", TokenType::enumKeyword },
			{ "(structure)\\b", TokenType::structKeyword },
			{ "(exception)\\b", TokenType::exceptKeyword },
			{ "(private)\\b", TokenType::privateKeyword },
			{ "(public)\\b", TokenType::publicKeyword },
			{ "(ref)\\b", TokenType::refKeyword },
			{ "(cpy)\\b", TokenType::cpyKeyword },
			{ "(const)\\b", TokenType::constKeyword },
			{ "(empty)\\b", TokenType::emptyLiteral },
			{ "(rest)\\b", TokenType::restKeyword },
			{ "(return)\\b", TokenType::returnKeyword },

			{ "(Bool|Byte|Character|Colour|Complex|Imaginary|Integer|Real|String)\\b", TokenType::basicType },

			{ "([A-Za-z_][A-Za-z0-9_]*)\\b", TokenType::symbol },

		};
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
		ArrayList<Token> * tokenise(String * input, String fileName = "Unknown File") const;
	};

	/* Regex Tools */

	class RegexTools {
		private:
		RegexTools() = default;
		public:
		static Bool test(String rgx, String & input);
		static String match(String rgx, String & input);
		static String matchClose(String rgx, String & input);
		static ArrayList<String> matchGroupClose(String rgx, String & input);
		inline static String matchStart(String rgx, String & input);
		inline static String matchCloseStart(String rgx, String & input);
		static String replaceMatches(String mtc, String & input, String rpl);
	};

	/* Parser */

	class SyntaxError: public Exception {
		private:
		const String _message;
		const FilePosition _position;
		public:
		const FilePosition & getPosition() const { return _position; }
		const String & getMessage() const { return _message; }
		SyntaxError(String message, FilePosition position):
		Exception(), _message(message), _position(position) { }
	};

	class ParserErrorException: public Exception {
		private:
		const ArrayList<SyntaxError> * const _errors;
		const String _fileName;
		public:
		const ArrayList<SyntaxError> * const getErrors() const { return _errors; }
		const String & getFileName() const { return _fileName; }
		ParserErrorException(ArrayList<SyntaxError> * errors, String name):
		Exception(), _errors(errors), _fileName(name) { }
		~ParserErrorException() { delete _errors; }
	};

	class Parser {
		private:
		String * input = nullptr;
		ArrayList<Token> * tokens = nullptr;
		ArrayList<SyntaxError> * errors = new ArrayList<SyntaxError>();
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
		Expression * completeCall(Expression * callee);
		Expression * primary();
		String * typeString();
		Statement * declaration();
		Statement * variableDeclaration(String stringType);
		Statement * statement();
		Statement * ifStatement();
		Statement * blockStatement();
		Statement * breakStatement();
		Statement * continueStatement();
		Statement * expressionStatement();
		Statement * functionStatement();
		Statement * procedureStatement();
		Statement * forStatement();
		Statement * printStatement();
		Statement * whileStatement();
		Statement * doWhileStatement();
		Statement * untilStatement();
		Statement * repeatUntilStatement();
		Statement * loopStatement();
		Statement * restStatement();
		Statement * returnStatement();
		void runTypeClassification();
		Bool match(TokenType type);
		Bool match(ArrayList<TokenType> * types);
		Bool check(TokenType type);
		Bool isOutOfRange();
		Bool isAtEnd();
		Token peek();
		Token peekAdvance();
		Token previous();
		Token advance();
		Token consume(TokenType type, String lexeme = "");
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
		ArrayList<Statement *> * parse(ArrayList<Token> * tokens, String * input = nullptr, String fileName = "Unknown File");
	};

}

#endif
