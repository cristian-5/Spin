
/*!
 *
 *    + --------------------------------------- +
 *    |  ASTPrint.hpp                           |
 *    |                                         |
 *    |       Abstract Syntax Tree Printer      |
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

#ifndef STACKASTPRINT
#define STACKASTPRINT

#include "../Aliases/Aliases.hpp"

#include "ASTree.hpp"

namespace Stack {

	class ASTPrinter: public Expression::Visitor {

		private:

		UInt32 indentLevel = 0;
		String indent = "";


		void parenthesise(const String & name, StrongList<Expression *> e) {
			String s = "(" + name;
			for (UInt32 i = 0; i < e.count(); i++) {
				if (e[i] == nullptr) continue;
				s += " ";
				e[i] -> accept(* this);
			}
			s += ")";
		}

		/*void visitBinaryExpression(const Binary & e) {
			paranthesise(e.op.lexeme(), { e.left.get(), e.right.get() });
		}*/


		public:

		template <typename T>
		String print(StrongList<T> * statements) {
			String s = "";
			for (UInt32 i = 0; i < statements.count(); i++) {
				s += indent;
				// This might not work because of reference:
				statements[i] -> accept(* this);
				s += "\n";
			}
			return s;
		}

		

	};

}

#endif
