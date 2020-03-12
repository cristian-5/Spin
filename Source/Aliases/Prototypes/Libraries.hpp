
#include "../Aliases.hpp"

#ifndef SPIN_LIBRARIES_HPP
#define SPIN_LIBRARIES_HPP

#include <unordered_map>

namespace Spin {

	class Environment;
	class Object;

	class Library {
		private: Library() = delete;
		public:
		typedef Lambda<void (Environment *)> LibraryHandler;
		static const Dictionary<Hash, LibraryHandler> libs;
		static void define(Hash id, Environment * memory);
		static Bool isKnown(Hash id);
	};

	class Console {
		private: Console() = default;
		public:
		static const String name;
		static Object * write();
		static Object * writeLine();
		static Object * read();
		static Object * readLine();
		static Object * setBackground();
		static Object * setForeground();
		static Object * setBlink();
		static Object * setBold();
		static Object * setInverse();
		static Object * reset();
		static Object * clean();
		static void defineLibrary(Environment * global);
	};

	class Kronos {
		private: Kronos() = default;
		public: static void defineLibrary(Environment * global);
	};

	class Maths {
		private: Maths() = default;
		public: static void defineLibrary(Environment * global);
	};

}

#endif
