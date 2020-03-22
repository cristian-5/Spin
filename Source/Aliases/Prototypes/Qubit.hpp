
#include "../Aliases.hpp"

#ifndef SPIN_QUBIT_HPP
#define SPIN_QUBIT_HPP

#include "Exception.hpp"

namespace Spin {

	class Complex;
	class Vector;

	class Qubit {

		private:

		Vector * data;

		Boolean isValid();
		Int64 collapse(Int64 i);

		static const Real error = 0.005;

		public:

		class InavlidStateException: public Exception {
			public: InavlidStateException() = default;
		};

		Qubit(Complex a, Complex b);
		Qubit(Vector * v);

		SizeType getSize();

		Vector * getData();
		void setData(Vector * data);

		Boolean measure(Int64 qubit);

	};

}

#endif
