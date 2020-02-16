
#include "../Aliases.hpp"

#ifndef SPIN_VECTOR_HPP
#define SPIN_VECTOR_HPP

#include <sstream>

#include "Exception.hpp"

using StringStream = std::stringstream;

namespace Spin {

	class Complex;

	class Vector {

		private:

		Complex * space = nullptr;
		SizeType size = 0;
		Bool direction = ketDirection;

		public:

		class InvalidIndexException: public Exception {
			public: InvalidIndexException() = default;
		};
		class InvalidOperationException: public Exception {
			public: InvalidOperationException() = default;
		};

		static const Bool braDirection = true;
		static const Bool ketDirection = false;

		Vector(SizeType s, Bool d);
		Vector(Bool d);
		Vector() = default;
		~Vector();

		Bool getDirection() const;
		SizeType getSize() const;
		Bool isEmpty() const;

		Bool isBra() const;
		Bool isKet() const;
		void setDirection(Bool d);

		inline void negate();
		void invert();
		inline Vector * getInverse() const;
		Vector * getAdditiveInverse() const;

		void conjugate();
		Vector * getConjugate() const;

		void transpose();
		Vector * getTransposed() const;

		inline void dagger();
		inline void adjoint();
		void conjugateTranspose();
		inline Vector * getDagger() const;
		inline Vector * getAdjoint() const;
		Vector * getConjugateTranspose() const;

		void inBraForm();
		void inKetForm();

		Complex & operator [] (SizeType i);
		Complex & at(SizeType i);

		Bool operator == (Vector r) const;
		Bool operator != (Vector r) const;
		Vector operator + (Vector r) const;
		Vector operator - () const;
		Vector operator - (Vector r) const;
		Vector operator * (Complex z) const;

		void multiplyByScalarComplex(Complex & z);
		void multiplyByScalarImaginary(Real & i);
		void multiplyByScalarReal(Real & r);
		void multiplyByScalarInteger(Int64 & i);

		static Vector * basis(Bool d, Bool s);
		Complex * copyAt(SizeType i) const;
		Complex * referenceAt(SizeType i) const;
		Vector * copy() const;
		String stringValue() const;

	};

}

#endif
