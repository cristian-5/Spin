
#include "../Common/Header.hpp"

#ifndef SPIN_SERIALISER_PURE
#define SPIN_SERIALISER_PURE

#include <vector>

namespace Spin {

	class Serialiser {

		private:

		static inline SizeType index = 0;

		public:

		class ReadingError: Exception { };

		static inline void seek(SizeType i);
		static inline void prepare();

		template <typename Type>
		static inline void write(Buffer * buffer, Type object);

		template <typename Type>
		static inline Type read(Buffer * buffer);

	};

	inline void Serialiser::seek(SizeType i) {
		index = i;
	}
	inline void Serialiser::prepare() {
		index = 0;
	}

	template <>
	inline void Serialiser::write<Byte>(Buffer * buffer, Byte object) {
		buffer -> push_back(object);
	}
	template <>
	inline void Serialiser::write<Boolean>(Buffer * buffer, Boolean object) {
		buffer -> push_back(object ? 0xFF : 0x00);
	}
	template <>
	inline void Serialiser::write<UInt64>(Buffer * buffer, UInt64 object) {
		static Byte bytes[8];
		for (Byte i = 0; i < 8; i += 1) {
			bytes[7 - i] = (object >> (i * 8));
		}
		for (Byte i = 0; i < 8; i += 1) {
			buffer -> push_back(bytes[i]);
		}
	}
	template <>
	inline void Serialiser::write<UInt32>(Buffer * buffer, UInt32 object) {
		static Byte bytes[4];
		for (Byte i = 0; i < 4; i += 1) {
			bytes[3 - i] = (object >> (i * 8));
		}
		for (Byte i = 0; i < 4; i += 1) {
			buffer -> push_back(bytes[i]);
		}
	}
	template <>
	inline void Serialiser::write<UInt16>(Buffer * buffer, UInt16 object) {
		static Byte bytes[2];
		for (Byte i = 0; i < 2; i += 1) {
			bytes[1 - i] = (object >> (i * 8));
		}
		for (Byte i = 0; i < 2; i += 1) {
			buffer -> push_back(bytes[i]);
		}
	}
	template <>
	inline void Serialiser::write<String>(Buffer * buffer, String object) {
		const SizeType size = object.length();
		for (SizeType i = 0; i < size; i += 1) {
			buffer -> push_back(object[i] ^ 0b01011010);
		}
		buffer -> push_back(0x00 ^ 0b01011010);
	}

	template <>
	inline Byte Serialiser::read<Byte>(Buffer * buffer) {
		if (index >= buffer -> size()) {
			throw ReadingError();
		}
		return buffer -> at(index++);
	}
	template <>
	inline Boolean Serialiser::read<Boolean>(Buffer * buffer) {
		if (index >= buffer -> size()) {
			throw ReadingError();
		}
		return (buffer -> at(index++) != 0);
	}
	template <>
	inline UInt64 Serialiser::read<UInt64>(Buffer * buffer) {
		const SizeType size = index + 8;
		if (size >= buffer -> size()) {
			throw ReadingError();
		}
		UInt64 x = 0;
		while (index < size) {
			x = x << 8;
			x |= (UInt64)(buffer -> at(index++));
		}
		return x;
	}
	template <>
	inline UInt32 Serialiser::read<UInt32>(Buffer * buffer) {
		const SizeType size = index + 4;
		if (size >= buffer -> size()) {
			throw ReadingError();
		}
		UInt64 x = 0;
		while (index < size) {
			x = x << 8;
			x |= (UInt32)(buffer -> at(index++));
		}
		return x;
	}
	template <>
	inline UInt16 Serialiser::read<UInt16>(Buffer * buffer) {
		const SizeType size = index + 2;
		if (size >= buffer -> size()) {
			throw ReadingError();
		}
		UInt64 x = 0;
		while (index < size) {
			x = x << 8;
			x |= (UInt32)(buffer -> at(index++));
		}
		return x;
	}
	template <>
	inline String Serialiser::read<String>(Buffer * buffer) {
		const SizeType size = buffer -> size();
		String s;
		while (index < size) {
			const Character c = (buffer -> at(index++)) ^ 0b01011010;
			if (c == 0x00) break;
			s.push_back(c);
		}
		return s;
	}

}

#endif
