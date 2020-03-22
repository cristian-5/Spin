
#include "../Aliases.hpp"

#ifndef SPIN_LIBRARIES_HPP
#define SPIN_LIBRARIES_HPP

#include <unordered_map>

namespace Spin {

	class Environment;
	class Object;

	class Library {
		public:
		Library() = delete;
		typedef Lambda<void (Environment *)> LibraryHandler;
		static const Dictionary<String, LibraryHandler> libs;
		static void defineVirual(Environment * memory);
		static void define(String name, Environment * memory);
		static Boolean isKnown(String name);
	};

	class Console {
		public:
		Console() = default;
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
		public:
		Kronos() = default;
		static void defineLibrary(Environment * global);
	};

	class Maths {
		public:
		Maths() = default;
		static void defineLibrary(Environment * global);
	};

	// Basic Types:

	class VirtualBoolean {
		public:
		VirtualBoolean() = default;
		static const String name;
		static Object * description();
		static Object * from();
		static void defineLibrary(Environment * global);
	};

}

#endif
