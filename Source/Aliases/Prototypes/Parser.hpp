
#include "../Aliases.hpp"

#ifndef SPIN_PARSER_HPP
#define SPIN_PARSER_HPP

#include <vector>

#include "Exception.hpp"
#include "Token.hpp"

namespace Spin {

	class Lexer;
	class CodeUnit;
	class Expression;
	class Statement;
	class SyntaxTree;

	class SyntaxError: public Exception {
		private:
		const String message;
		const UInt64 line;
		public:
		SyntaxError(String m, UInt64 l);
		const UInt64 & getLine() const;
		const String & getMessage() const;
	};

	class UnitError {
		private:
		const Array<SyntaxError> * const errors;
		const String name;
		public:
		const Array<SyntaxError> * const getErrors() const;
		const String & getName() const;
		UnitError(Array<SyntaxError> * e, String n);
		~UnitError();
	};

	class ParserErrorException: public Exception {
		private:
		const Array<UnitError *> * const units;
		public:
		ParserErrorException(Array<UnitError *> * u);
		ParserErrorException(UnitError * u);
		~ParserErrorException();
		const Array<UnitError *> * const getUnitErrors() const;
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
		Bool isInClass = false;
		Expression * expression();
		Expression * assignment();
		Expression * shortOR();
		Expression * shortAND();
		Expression * equality();
		Expression * comparison();
		Expression * lowPriorityOperator();
		Expression * mediumPriorityOperator();
		Expression * postfixOperator();
		Expression * prefixOperator();
		Expression * subscription();
		Expression * completeSubscript(Expression * item);
		Expression * call();
		Expression * completeCall(Expression * callee, Bool isConstructor);
		Expression * primary();
		String * typeString(Bool current = false);
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
		Statement * procedureStatement(Bool allowType = false);
		Statement * classDeclaration();
		Statement * fieldStatement();
		Statement * forStatement();
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
		inline Token recede();
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
