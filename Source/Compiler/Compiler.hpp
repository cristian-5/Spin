
#include "../Common/Header.hpp"

#ifndef SPIN_COMPILER_HPP
#define SPIN_COMPILER_HPP

#include <unordered_map>

#include "../Token/Token.hpp"

#include "Program.hpp"

#include "../Utility/Stack.hpp"
#include "../Utility/Converter.hpp"

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
		shift,
		term,
		factor,
		unary,
		call,
		primary,
	};

	class Compiler {

		private:

		typedef void (Compiler::*ParseFunction)();

		struct ParseRule {
			ParseFunction prefix = nullptr;
			ParseFunction infix = nullptr;
			Precedence precedence = Precedence::none;
		};

		class TypeNode {
			public:
			Type type = Type::VoidType;
			void * data = nullptr;
			TypeNode * next = nullptr;
			TypeNode() = default;
			TypeNode(Type t) { type = t; }
			~TypeNode() { if (next) delete next; }
			inline Boolean isContainer() { return next; }
			inline String description() {
				if (!next) return Converter::typeToString(type);
				String desc = Converter::typeToString(type) + "<";
				if (next) desc += next -> description() + ">";
				return desc;
			}
		};

		struct Local {
			String name;
			SizeType depth = 0;
			TypeNode * type = nullptr;
			Boolean ready = false;
			Boolean isConstant = false;
			Boolean isStack = false;
		};

		struct Jump {
			SizeType line = 0;
			SizeType scope = 0;
		};

		struct Routine {
			String name;
			TypeNode * returnType = nullptr;
			Array<TypeNode *> parameters;
			Array<ByteCode> code;
			Boolean returns = false;
			SizeType scope;
			SizeType prototypeIndex = - 1;
		};

		struct Parameter {
			String name;
			TypeNode * type = nullptr;
		};

		struct Prototype {
			String name;
			Array<Parameter> parameters;
			TypeNode * returnType = nullptr;
			SizeType address = - 1;
		};

		Array<Routine> routines;

		Array<Local> locals;
		SizeType scopeDepth = 0;

		static const Dictionary<Token::Type, ParseRule> rules;

		Program * program = nullptr;
		Token current;
		Token previous;
		SizeType index = 0;
		CodeUnit * currentUnit = nullptr;
		Array<Token> * tokens = nullptr;

		Array<Prototype> prototypes;
		Stack<TypeNode *> typeStack;
		// Stack<Type> lamdaReturn;
		Stack<Boolean> assignmentStack;
		Stack<SizeType> cycleScopes;
		Stack<SizeType> routineIndexes;
		Stack<Jump> breakStack;
		Stack<Jump> continueStack;

		// Dictionary<SizeType, SizeType> lamdas;
		Dictionary<String, SizeType> strings;

		static const Dictionary<Binary, Type> infixTable;
		static const Dictionary<Unary, Type> prefixTable;
		static const Dictionary<Unary, Type> postfixTable;
		static const Dictionary<Types, Boolean> castTable;

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

		TypeNode * type();

		void produceInitialiser(TypeNode * type);

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
		void constant();
		void identifier();
		void block();

		void logicAND();
		void logicOR();

		void grouping();
		void functional();
		void subscription();
		void cast();
		void call();
		void lamda();
		void ternary();
		void postfix();
		void binary();
		void prefix();

		void expressionStatement();
		void printStatement();
		void ifStatement();
		void whileStatement();
		void untilStatement();
		void doWhileStatement();
		void repeatUntilStatement();
		void loopStatement();
		void forStatement();
		void breakStatement();
		void continueStatement();
		void procStatement();
		void funcStatement();
		void returnStatement();
		void swapStatement();

		SizeType locate(String & name, Array<TypeNode *> & types);
		SizeType resolve(String & name, Local & local);

		void parsePrecedence(Precedence precedence);

		void foldUnary(Token token);
		void foldBinary(Token token);

		inline void pushType(Type type);
		inline void pushType(TypeNode * node);
		inline Type popAbsoluteType();
		inline void decreaseType();
		inline TypeNode * popType();
		inline TypeNode * topType();
		inline Boolean match(TypeNode * a, TypeNode * b);
		inline Boolean match(Token::Type type);
		inline Boolean matchAssignment();
		inline Boolean check(Token::Type type);
		inline void advance();
		inline void consume(Token::Type type, String lexeme);
		inline void prototypeRoutine(Boolean function);
		inline void preparePrototypes();
		inline void resolveRoutines();
		inline void resolveCalls();
		inline SizeType countLocals(SizeType scope);
		inline SizeType sourcePosition();
		inline void emitOperation(ByteCode code);
		inline void emitOperation(OPCode code);
		inline void emitString(String s);
		inline void emitJMB(SizeType jmb);
		inline SizeType emitJMP(OPCode code);
		inline void patchJMP(SizeType jmp);
		inline void patchJMB(SizeType pos, SizeType jmb);
		inline void patchOP(SizeType op, OPCode code);
		inline Array<ByteCode> cutCodes(SizeType cut);
		inline void pasteCodes(Array<ByteCode> codes);
		inline void beginScope();
		inline void beginVirtualScope();
		inline void endScope();
		inline void endVirtualScope();
		inline SizeType emitRST();
		inline void emitCAL(SizeType i);
		inline void emitPOP(SizeType n);
		inline void emitRET();
		inline void emitHLT();

		void reset();

		Compiler() = default; ~Compiler() = default;

		public:

		// Settings:

		Boolean folding = true;

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
