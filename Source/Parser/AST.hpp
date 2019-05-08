
/*!
 *
 *    + --------------------------------------- +
 *    |  AST.hpp                                |
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

#ifndef STACKAST
#define STACKAST

#include <iostream>
#include <string>

#include "../Collection/Collection.hpp"

#include "../Token/Token.hpp"
#include "../Syntax/SyntaxRule.hpp"

using String = std::string;
using UInt32 = std::uint32_t;

using namespace Collection;

/*! @brief Namespace Stack */
namespace Stack {

	class TreeNode {
		
		public:

		StrongList<TreeNode *> children = StrongList<TreeNode *>();

		Token * token = nullptr;

		TreeNode() { }

		TreeNode(StrongList<TreeNode *> & c, Token * t) {
			children = c;
			token = t;
		}

		TreeNode(TreeNode * k, Token * t) {
			children.link(k);
			token = t;
		}

		void addKid(TreeNode * k) {
			children.link(k);
		}

		void addChildren(StrongList<TreeNode *> & c) {
			for (UInt32 i = 0; i < c.count(); i++) {
				children.link(c[i]);
			}
		}

	};

	class AbstractSyntaxTree {

		public:

		StrongList<TreeNode *> children = StrongList<TreeNode *>();

		AbstractSyntaxTree() { }

		void addNode(TreeNode * node) {
			children.link(node);
		}

		void setCursorNode() {

		}

	};

}

#endif
