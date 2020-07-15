
#include "../Common/Header.hpp"

#ifndef SPIN_STACK_PURE
#define SPIN_STACK_PURE

namespace Spin {

	template <typename Type>
	class Stack {

		private:

		Type * stack = nullptr;

		SizeType count = 0;
		SizeType maxCount = 16;

		void increase();

		public:

		Stack();
		~Stack();

		Type at(SizeType index);
		void edit(SizeType index, Type node);

		void push(Type node);

		Type top();
		Type pop();

		void decrease();

		Boolean isEmpty();
		SizeType size();

		void clear();

	};

	template <typename Type>
	Stack<Type>::Stack() {
		maxCount = 16;
		stack = new Type[maxCount];
	}

	template <typename Type>
	Stack<Type>::~Stack() {
		delete [] stack;
	}

	template <typename Type>
	void Stack<Type>::increase() {
		maxCount *= 1.5;
		Type * pointer = new Type[maxCount];
		std::copy(stack, stack + count, pointer);
		delete [] stack;
		stack = pointer;
	}

	template <typename Type>
	Type Stack<Type>::at(SizeType index) {
		return stack[index];
	}

	template <typename Type>
	void Stack<Type>::edit(SizeType index, Type node) {
		stack[index] = node;
	}

	template <typename Type>
	void Stack<Type>::push(Type node) {
		if (count == maxCount) increase();
		stack[count] = node;
		count += 1;
	}

	template <typename Type>
	Type Stack<Type>::top() {
		return stack[count - 1];
	}

	template <typename Type>
	Type Stack<Type>::pop() {
		return stack[--count];
	}

	template <typename Type>
	void Stack<Type>::decrease() {
		count -= 1;
	}

	template <typename Type>
	Boolean Stack<Type>::isEmpty() {
		return count == 0;
	}

	template <typename Type>
	SizeType Stack<Type>::size() {
		return count;
	}

	template <typename Type>
	void Stack<Type>::clear() {
		count = 0;
	}

}

#endif
