
#include "../Common/Header.hpp"

#ifndef SPIN_COMPILER_HPP
#define SPIN_COMPILER_HPP

#include <unordered_map>

#include "../Token/Token.hpp"

#include "Program.hpp"

#include "../Utility/Stack.hpp"

using Unary = UInt16;
using Binary = UInt32;

namespace Spin {

	enum Precedence : UInt8 {
		none,
		assignment,
		logicOR,
		logicAND,
		bitwiseOR,
		bitwiseXOR,
		bitwiseAND,
		equality,
		comparison,
		term,
		factor,
		unary,
		call,
		primary,
	};

	class Compiler {

		private:

		typedef void (Compiler::*ParseFunction) ();

		struct ParseRule {
			ParseFunction prefix = nullptr;
			ParseFunction infix = nullptr;
			Precedence precedence = Precedence::none;
		};

		struct Local {
			String name;
			SizeType depth = 0;
			Type type;
			Boolean ready = false;
		};
		struct Global {
			SizeType index;
			Type type;
			Boolean ready = false;
		};

		struct Jump {
			SizeType line = 0;
			SizeType scope = 0;
		};

		Array<Local> locals;
		SizeType scopeDepth = 0;

		static const Dictionary<Token::Type, ParseRule> rules;

		Program * program = nullptr;
		Token current;
		Token previous;
		SizeType index = 0;
		CodeUnit * currentUnit = nullptr;
		Array<Token> * tokens = nullptr;

		Dictionary<String, Global> globals;
		UInt32 globalIndex = 0;

		Stack<Type> typeStack;
		Stack<Boolean> assignmentStack;
		Stack<SizeType> cycleScopes;
		Stack<Jump> breakStack;
		Stack<Jump> continueStack;

		static const Dictionary<Unary, Type> prefix;
		static const Dictionary<Binary, Type> infix;

		static const Dictionary<Types, Boolean> implicitCast;

		static consteval Unary compose(Token::Type token, Type type) {
			return (Unary)(((Unary) token << 8) | type);
		}
		static consteval Binary compose(Token::Type token, Type a, Type b) {
			return (Binary)(((Binary) a << 16) | ((Binary) token << 8) | b);
		}
		static consteval Types compose(Type a, Type b) {
			return (Types)(((Types) a << 8) | b);
		}

		ParseRule getRule(Token::Type token);

		static inline Unary runtimeCompose(Token::Type token, Type type) {
			return (Unary)(((Unary) token << 8) | type);
		}
		static inline Binary runtimeCompose(Token::Type token, Type a, Type b) {
			return (Binary)(((Binary) a << 16) | ((Binary) token << 8) | b);
		}
		static inline Types runtimeCompose(Type a, Type b) {
			return (Types)(((Types) a << 8) | b);
		}

		void booleanLiteral();
		void characterLiteral();
		void stringLiteral();
		void imaginaryLiteral();
		void realLiteral();
		void realIdioms();
		void integerLiteral();

		void expression();
		void statement();
		void declaration();
		void variable();
		void local();
		void global();
		void identifier();
		void block();

		void logicAND();
		void logicOR();

		void grouping();
		void ternary();
		void binary();
		void unary();

		void expressionStatement();
		void printStatement();
		void ifStatement();
		void whileStatement();
		void untilStatement();
		void doWhileStatement();
		void repeatUntilStatement();
		void loopStatement();
		void breakStatement();
		void continueStatement();
		void swapStatement();

		SizeType resolve(String & name, Local & local);

		void parsePrecedence(Precedence precedence);

		inline Boolean match(Token::Type type);
		inline Boolean check(Token::Type type);
		inline void advance();
		inline void consume(Token::Type type, String lexeme);
		inline SizeType sourcePosition();
		inline void emitException(Program::Error error);
		inline void emitOperation(ByteCode code);
		inline void emitOperation(OPCode code);
		inline void emitObject(Pointer ptr, Type type);
		inline void emitGlobal(Value value = { .integer = 0 });
		inline void emitJMB(SizeType jmb);
		inline SizeType emitJMP(OPCode code);
		inline void patchJMP(SizeType jmp);
		inline void patchJMB(SizeType pos, SizeType jmb);
		inline void patchOP(SizeType op, OPCode code);
		inline void beginScope();
		inline void endScope();

		inline SizeType emitRST();
		inline void emitRET();
		inline void emitHLT();

		void reset();

		Compiler() = default; ~Compiler() = default;

		public:

		Compiler(const Compiler &) = delete;
		Compiler(Compiler &&) = delete;
		Compiler & operator = (const Compiler &) = delete;
		Compiler & operator = (Compiler &&) = delete;
		inline static Compiler * self() {
			static Compiler instance;
			return & instance;
		}
		Program * compile(SourceCode * source);

	};

}

#endif
