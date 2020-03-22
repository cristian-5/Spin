
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
		Boolean isInControlFlow = false;
		Boolean isInFunction = false;
		Boolean isInProcedure = false;
		Boolean isInClass = false;
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
		Expression * completeCall(Expression * callee, Boolean isConstructor);
		Expression * primary();
		String * typeString(Boolean current = false);
		Statement * declaration();
		Statement * variableDeclaration(String stringType, Boolean isClass = false);
		Statement * vectorDeclaration();
		Statement * statement();
		Statement * ifStatement();
		Statement * blockStatement();
		Statement * breakStatement();
		Statement * continueStatement();
		Statement * expressionStatement();
		Statement * functionStatement();
		Statement * procedureStatement(Boolean allowType = false);
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
		inline Boolean match(TokenType type);
		inline Boolean matchRange(TokenType from, TokenType to);
		inline Boolean check(TokenType type);
		inline Boolean isOutOfRange();
		inline Boolean isAtEnd();
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
