
#include "../Aliases.hpp"

#ifndef SPIN_PARSER_HPP
#define SPIN_PARSER_HPP

#include <vector>

#include "Exception.hpp"
#include "Token.hpp"

namespace Spin {

	class CodeUnit;
	class Program;
	class Expression;
	class Statement;
	class SyntaxTree;

	class Parser {
		private:
		CodeUnit * currentUnit = nullptr;
		Array<Token> * tokens = nullptr;
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
		Statement * swapStatement();
		Statement * deleteStatement();
		Statement * fileStatement();
		void replace(TokenType type, String lexeme, TokenType newType);
		void runTypeClassification();
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
		SyntaxTree * parse(Program * code);
	};

}

#endif
