
#include "../Common/Interface.hpp"

#ifndef SPIN_DECOMPILER_HPP
#define SPIN_DECOMPILER_HPP

#include "Program.hpp"

namespace Spin {

	class Decompiler {

		private:

		static const String colours[];

		enum Colour {
			red,
			yellow,
			white,
			black,
			orange,
			blue,
			green,
			purple,
			pink,
			grey,
			sky,
			acqua,
			peach,
			dark,
		};

		static void tableOP(String o, String s);
		static void aloneOP(String o, Colour c, String h);
		static void constOP(String o, Int64 i, Colour c);
		static void typesOP(String o, Types x, Colour c, String h);
		static void unaryOP(String o, Type x, Colour c, String h);
		static void jmptoOP(String o, SizeType x, String h);
		static void smallOP(String o, Byte b, Colour c, String h);

		static void rest_OP();
		
		static String resolve(Type type);

		static inline Boolean na = false;

		public:

		Decompiler() = delete;

		static void decompile(Program * program, SizeType index);
		static void decompile(Program * program, Boolean noAnsi = false);

	};

}

#endif
