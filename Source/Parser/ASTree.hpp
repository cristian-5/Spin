
/*!
 *
 *    + --------------------------------------- +
 *    |  ASTree.hpp                             |
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

#include "../Aliases/Aliases.hpp"
#include "../Collection/Collection.hpp"
#include "../Token/Token.hpp"
#include "../Syntax/SRule.hpp"

using namespace Collection;

/*! @brief Namespace Stack. */
namespace Stack {

	/*! @brief Class TreeNode. */
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

	/*! @brief Class AbstractSyntaxTree. */
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
