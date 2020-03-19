
/*!
 *
 *    + --------------------------------------- +
 *    |  Chaos.hpp                              |
 *    |                                         |
 *    |        Chaos Cycle Bit Generator        |
 *    |                                         |
 *    |  Created by Richard C., Cristian A.     |
 *    |  Copyright © MIT. All rights reserved.  |
 *    + --------------------------------------- +
 *
 *    Note: This software is licensed under
 *          the (MIT) Massachusetts Institute
 *          of Technology License.
 *
 *          The Chaos algorithm is a modified
 *          version of the cycle bit generator,
 *          originally developed and maintained
 *          by Richard Cookman.
 *
 *          The hash function is based on the
 *          One At Time hashing algorithm.
 *
!*/

#include "../Aliases.hpp"

#ifndef SPIN_CHAOS_HPP
#define SPIN_CHAOS_HPP

#include <type_traits>
#include <vector>

namespace Spin {

	// Prototypes:

	template <typename Type>
	class Chaos {
		static_assert(std::is_integral<Type>::value);
		private:
		class CBRNG {

			private:

			class CycleCount {

				private:

				Byte dat[8];

				public:

				CycleCount() {
					for (UInt32 i = 0; i < 8; i += 1) dat[i] = 0;
				}

				Bool increment() {
					for (UInt32 i = 0; i < 8; i += 1) {
						if (dat[i] == 0xFF) dat[i] = 0;
						else {
							++dat[i];
							return true;
						}
					}
					return false;
				}

				Bool good() const {
					for (UInt32 i = 0; i < 8; i += 1) {
						if (dat[i] != 0xFF) return true;
					}
					return false;
				}

				operator UInt64() const {
					UInt64 rtn = 0;
					UInt32 j = 0;
					for (UInt32 i = 0; i < 8; i += 1) {
						rtn ^= ((UInt64) dat[i]) << (8 * j);
						j += 1; if (j == 8) j = 0;
					}
					return rtn;
				}

			};

			template <typename ValueType>
			static inline Bool getBit(ValueType val, UInt32 pos) {
				return val & ((ValueType) 1 << pos);
			}
			template <typename ValueType>
			static inline ValueType setBit(ValueType val, UInt32 pos, Bool to) {
				return (val & ~((ValueType) 1 << pos)) | ((ValueType) to << pos);
			}

			UInt64 val;
			UInt64 last;

			CycleCount cnt;

			UInt8 flags;

			inline static UInt64 shiftTransform(UInt64 val, Int32 i, Bool & startBit) {
				val ^= (UInt64)startBit << i;
				startBit ^= getBit(val, i);
				return val;
			}

			static UInt64 generate(UInt64 val, Bool left, Bool startBit) {
				if (left) {
					for (Int32 i = 0; i < 64; i += 1) {
						val = shiftTransform(val, i, startBit);
					}
				} else {
					for (Int32 i = 63; i >= 0; i -= 1) {
						val = shiftTransform(val, i, startBit);
					}
				}
				return val;
			}

			UInt64 generate(UInt64 inval, UInt32 BP) {
				Bool left = (getBit(val, (22 + BP) % 64) ^ getBit(flags, 1) ^ true);
				Bool startBit = (getBit(val, (44 + BP) % 64) ^ getBit(flags, 0) ^ true);
				flags = setBit(flags, 1, left);
				flags = setBit(flags, 0, startBit);
				return last = generate(inval, left, startBit) ^ inval ^ ~last;
			}

			UInt64 generateOuter(UInt64 tmp) {
				return val = ((generate(val, 0) << 1) * (generate(tmp, 1) << 1)) ^ generate(val, 2);
			}

			void setSeed(UInt64 rnd, UInt64 offset, Bool reseed) {
				flags = 0;
				val = rnd + offset;
				last = ~(rnd - offset);
				flags = setBit(flags, 2, reseed);
			}

			public:

			CBRNG(UInt64 seed = 0) {
				setSeed(seed ^ 0xC215714300A3D2E4, 10, false);
			}

			UInt64 next() {
				Bool gd = cnt.increment();
				UInt64 tmp = cnt;
				if (tmp == 0) tmp += 1;
				UInt64 rtn = generateOuter(tmp);
				if(!gd && reseeds()) {
					UInt64 a = generateOuter(++tmp);
					UInt64 b = generateOuter(++tmp);
					setSeed(a, b, true);
				}
				return rtn;
			}

			Bool good() const {
				return cnt.good();
			}

			Bool reseeds() const {
				return getBit(flags, 2);
			}

		};
		static CBRNG generator;
		public:
		static class Seed {
			UInt64 value = 0;
			public:
			UInt64 & operator = (const UInt64 & i) {
				generator = CBRNG(i);
				return value = i;
			}
			operator UInt64 () const {
				return value;
			}
		} seed;
		static Type next();
		static Type next(Type max);
		static Type next(Type min, Type max);
		static Type hash(String val);
		static Type hash(Array<Byte> val);
	};

	template <typename Type>
	typename Chaos<Type>::CBRNG Chaos<Type>::generator = CBRNG();

	template <typename Type>
	/// Seed of the random function.
	typename Chaos<Type>::Seed Chaos<Type>::seed;

	template <typename Type>
	/// Generates a number between n ... N included,
	/// where n is Type::minValue and N Type::maxValue.
	Type Chaos<Type>::next() {
		return generator.next();
	}

	template <typename Type>
	/// Generates a number between 0 ... max included.
	Type Chaos<Type>::next(Type max) {
		if (max < 0) return - (generator.next() % (- max + 1));
		else return generator.next() % (max + 1);
	}

	template <typename Type>
	/// Generates a number between min ... max included.
	Type Chaos<Type>::next(Type min, Type max) {
		if (max < min) std::swap(max, min);
		return (generator.next() % (max - min + 1)) + min;
	}

	template <>
	/// Generates a Bool between false ... true included.
	inline Bool Chaos<Bool>::next() {
		// N % 2 === N & 1 but faster.
		return generator.next() & 1;
	}
	template <>
	/// Generates a Bool between false ... max included.
	inline Bool Chaos<Bool>::next(Bool max) {
		if (!max) return false;
		// N % 2 === N & 1 but faster.
		return generator.next() & 1;
	}
	template <>
	/// Generates a Bool between min ... max included.
	inline Bool Chaos<Bool>::next(Bool min, Bool max) {
		if (min == max) return min;
		// N % 2 === N & 1 but faster.
		return generator.next() & 1;
	}

	template <typename Type>
	/// Generates a hash of the input.
	Type Chaos<Type>::hash(String val) {
		Array<Byte> bytes;
		for (SizeType i = 0; i < val.length(); i += 1) {
			bytes.push_back(val[i]);
		}
		return Chaos<Type>::hash(bytes);
	}
	template <typename Type>
	/// Generates a hash of the input.
	Type Chaos<Type>::hash(Array<Byte> val) {
		const UInt64 mask = 0xC215714300A3D2E4;
		if (val.empty()) return mask;
		CBRNG gen;
		for (SizeType i = 0; i < val.size(); i += 1) {
			if (i & 1) val[i] = ~val[i];
			SizeType p = (i - 1 < 0 ? val.size() - 1 : i - 1);
			val[i] += val[p] ^ val[i];
			gen = CBRNG(val[i]);
			val[i] ^= gen.next();
			if (val[i] & 1) val[i] = (val[i] << 7) | (val[i] >> 1);
			else val[i] = (val[i] >> 7) | (val[i] << 1);
		}
		Type hash = 0;
		for (SizeType i = 0; i < val.size(); i += 1) {
			if (i & 1) val[i] = (val[i] << 7) | (val[i] >> 1);
			else val[i] = (val[i] >> 7) | (val[i] << 1);
			hash = hash + val[i];
			hash ^= mask;
			if (val[i] & 1) hash = (hash << 7) | (hash >> 1);
			else hash = (hash >> 7) | (hash >> 1);
		}
		gen = CBRNG(hash ^ mask);
		return gen.next();
	}

}

#endif
