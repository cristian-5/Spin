
#include "../Common/Header.hpp"

#ifndef SPIN_ARGUMENTS_PURE
#define SPIN_ARGUMENTS_PURE

#include <vector>
#include <unordered_map>
#include <sstream>

using StringStream = std::stringstream;

namespace CommandLine {

	class Option {
		friend class Arguments;
		public:
		String command;
		String cut;
		String usage;
		SizeType arguments = 0;
		Option(String c, String s, SizeType a);
		Option(String c, String s);
		Option(String c, SizeType a);
		Option(String c);
		private:
		Boolean found = false;
	};

	Option::Option(String c, String s, SizeType a) {
		command = c; cut = s; arguments = a;
	}
	Option::Option(String c, String s) {
		command = c; cut = s;
	}
	Option::Option(String c, SizeType a) {
		command = c; arguments = a;
	}
	Option::Option(String c) {
		command = c;
	}

	class Parameter;
	class Parameters;
	class Arguments;

	class Arguments {

		private:

		static SizeType levenstein(String source, String target);
		static String similar(String s);

		public:

		static inline Boolean strictMode = true;

		typedef void (* UnclearEvent)(String, SizeType);
		typedef void (* UndefinedEvent)(String, String);
		typedef void (* RedefinitionEvent)(String);
		typedef void (* HelpEvent)();

		static Parameters parse(Int32 argc, Character * argv[]);
		static void reset();

		static UndefinedEvent onUndefined;
		static RedefinitionEvent onRedefinition;
		static UnclearEvent onUnclear;
		static HelpEvent onHelp;

		static String helpCommandShort;
		static String helpCommandFull;

		static Array<Option> options;

		static std::reference_wrapper<std::ostream> log;

		static Boolean helpRaised;

		static String program;

	};

	class Parameter {
		public:
		String object;
		template <typename TYPE> TYPE to();
		template <typename TYPE> Array<TYPE> toVector();
	};

	class Parameters {
		private:
		friend class Arguments;
		Dictionary<String, Parameter> list;
		Boolean hasFailed = false;
		Boolean hasFailedExclusion = false;
		public:
		Array<String> freeParameters;
		Parameters() = default;
		Parameters(Boolean failed);
		typedef void (* ExclusionEvent)(String, String);
		static ExclusionEvent onExclusion;
		void add(String c, Parameter p);
		void removeOptional(String o);
		void removeOptionals(Array<String> o);
		SizeType size() const;
		SizeType parametersCount() const;
		SizeType sizeOfFree() const;
		String mutualExclusion(Array<String> c);
		Parameter operator [] (String p);
		Boolean failed();
		Boolean exclusionFailed();
	};

	class ConversionException { };

	template <> Int64 Parameter::to<Int64>() {
		try {
			return std::stol(object);
		} catch (std::exception & e) {
			throw ConversionException();
		}
	}
	template <> Int32 Parameter::to<Int32>() {
		try {
			return std::stoi(object);
		} catch (std::exception & e) {
			throw ConversionException();
		}
	}
	template <> Int16 Parameter::to<Int16>() {
		try {
			return (Int16)std::stoi(object);
		} catch (std::exception & e) {
			throw ConversionException();
		}
	}
	template <> Real Parameter::to<Real>() {
		try {
			return std::stod(object);
		} catch (std::exception & e) {
			throw ConversionException();
		}
	}
	template <> Character Parameter::to<Character>() {
		if (object.length() != 1) {
			throw ConversionException();
		}
		return object[0];
	}
	template <> Boolean Parameter::to<Boolean>() {
		return object == "true";
	}
	template <> String Parameter::to<String>() {
		return object;
	}

	template <> Array<Int64> Parameter::toVector<Int64>() {
		StringStream ss(object);
		String s;
		Array<Int64> v;
		while (std::getline(ss, s, ' ')) {
			try { v.push_back(std::stol(s)); }
			catch (std::exception & e) {
				throw ConversionException();
			}
		}
		return v;
	}
	template <> Array<Int32> Parameter::toVector<Int32>() {
		StringStream ss(object);
		String s;
		Array<Int32> v;
		while (std::getline(ss, s, ' ')) {
			try { v.push_back(std::stoi(s)); }
			catch (std::exception & e) {
				throw ConversionException();
			}
		}
		return v;
	}
	template <> Array<Int16> Parameter::toVector<Int16>() {
		StringStream ss(object);
		String s;
		Array<Int16> v;
		while (std::getline(ss, s, ' ')) {
			try { v.push_back((Int16)std::stoi(s)); }
			catch (std::exception & e) {
				throw ConversionException();
			}
		}
		return v;
	}
	template <> Array<Real> Parameter::toVector<Real>() {
		StringStream ss(object);
		String s;
		Array<Real> v;
		while (std::getline(ss, s, ' ')) {
			try { v.push_back(std::stod(s)); }
			catch (std::exception & e) {
				throw ConversionException();
			}
		}
		return v;
	}
	template <> Array<Character> Parameter::toVector<Character>() {
		StringStream ss(object);
		String s;
		Array<Character> v;
		while (std::getline(ss, s, ' ')) {
			try {
				if (s.length() != 1) {
					throw ConversionException();
				}
				v.push_back(s[0]);
			} catch (std::exception & e) {
				throw ConversionException();
			}
		}
		return v;
	}
	template <> Array<Boolean> Parameter::toVector<Boolean>() {
		StringStream ss(object);
		String s;
		Array<Boolean> v;
		while (std::getline(ss, s, ' ')) {
			try { v.push_back(s == "true"); }
			catch (std::exception & e) {
				throw ConversionException();
			}
		}
		return v;
	}
	template <> Array<String> Parameter::toVector<String>() {
		StringStream ss(object);
		String s;
		Array<String> v;
		while (std::getline(ss, s, ' ')) {
			try { v.push_back(s); }
			catch (std::exception & e) {
				throw ConversionException();
			}
		}
		return v;
	}

	std::reference_wrapper<std::ostream> Arguments::log = std::ref(std::cout);

	Array<Option> Arguments::options;
	String Arguments::helpCommandShort = "-h";
	String Arguments::helpCommandFull = "-help";
	Arguments::UndefinedEvent Arguments::onUndefined = [] (String c, String s) {
		log.get() << program << ": Command '" << c << "' is undefined!\n"
				<< "Did you mean '" << s << "' instead?\n"
				<< "Type " << helpCommandShort
				<< " or " << helpCommandFull
				<< " for a list of valid commands.\n";
	};
	Arguments::RedefinitionEvent Arguments::onRedefinition = [] (String c) {
		log.get() << program << ": Command '" << c << "' has already been defined!\n";
	};
	Arguments::UnclearEvent Arguments::onUnclear = [] (String c, SizeType s) {
		log.get() << program << ": Command '" << c << "' needs at least "
				<< s << " subsequent parameters!\n";
	};
	Arguments::HelpEvent Arguments::onHelp = [] () {
		log.get() << "Usage: " << program << " [<argument>]\n";
	};

	Boolean Arguments::helpRaised = false;

	String Arguments::program = "program";

	SizeType Arguments::levenstein(String source, String target) {
		std::transform(
			source.begin(), source.end(), source.begin(),
			[] (Character c) { return std::tolower(c); }
		);
		std::transform(
			target.begin(), target.end(), target.begin(),
			[] (Character c) { return std::tolower(c); }
		);
		if (source.length() > target.length()) {
			std::swap(target, source);
		}
		const SizeType min = source.length();
		const SizeType max = target.length();
		Array<SizeType> distance(min + 1);
		for (SizeType i = 0; i <= min; i += 1) {
			distance[i] = i;
		}
		for (SizeType j = 1; j <= max; j += 1) {
			SizeType previousDiagonal = distance[0];
			SizeType previousDiagonalSave;
			distance[0] += 1;
			for (SizeType i = 1; i <= min; i += 1) {
				previousDiagonalSave = distance[i];
				if (source[i - 1] == target[j - 1]) {
					distance[i] = previousDiagonal;
				} else {
					distance[i] = std::min(
						std::min(
							distance[i - 1], distance[i]
						),
						previousDiagonal
					) + 1;
				}
				previousDiagonal = previousDiagonalSave;
			}
		}
		return distance[min];
	}
	String Arguments::similar(String s) {
		if (!options.size() || s.empty()) return String();
		SizeType p = 0;
		SizeType c = levenstein(s, options[p].command);
		for (SizeType i = 1; i < options.size(); i += 1) {
			SizeType t = levenstein(s, options[i].command);
			if (t < c) {
				c = t;
				p = i;
			}
		}
		return options[p].command;
	}

	Parameters Arguments::parse(Int32 argc, Character * argv[]) {
		if (!options.size()) return { true };
		Parameters params;
		for (Option & option : options) {
			option.found = false;
		}
		for (SizeType i = 1; i < argc; i += 1) {
			const String t(argv[i]);
			if (t == helpCommandFull || t == helpCommandShort) {
				onHelp();
				helpRaised = true;
				return { true };
			}
			Boolean done = false;
			for (Option & o : options) {
				if (t == o.command || t == o.cut) {
					done = true;
					if (o.found) {
						onRedefinition(o.command);
						return { true };
					}
					if (!o.arguments) {
						params.add(o.command, { "true" });
						o.found = true;
					} else {
						const SizeType k = i + o.arguments;
						if (k < argc) {
							String value;
							for (SizeType j = i + 1; j <= k; j += 1) {
								value.append(argv[j]);
								value.push_back(' ');
							}
							value.pop_back();
							i = k;
							params.add(o.command, { value });
						} else {
							onUnclear(o.command, o.arguments);
							return { true };
						}
					}
					break;
				}
			}
			if (!done) {
				if (strictMode) {
					onUndefined(t, similar(t));
					return { true };
				} else if (!t.empty() && t[0] != '-') {
					params.freeParameters.push_back(t);
				} else {
					onUndefined(t, similar(t));
					return { true };
				}
			}
		}
		for (Option & o : options) {
			if (!o.found && !o.arguments) {
				o.found = true;
				params.add(o.command, { "false" });
			}
		}
		return params;
	}

	void Arguments::reset() {
		onUndefined = [] (String c, String s) {
			log.get() << program << ": Command '" << c << "' is undefined!\n"
					<< "Did you mean '" << s << "' instead?\n"
					<< "Type " << helpCommandShort
					<< " or " << helpCommandFull
					<< " for a list of valid commands.\n";
		};
		onRedefinition = [] (String c) {
			log.get() << program << ": Command '" << c << "' has already been defined!\n";
		};
		onUnclear = [] (String c, SizeType s) {
			log.get() << program << ": Command '" << c << "' needs at least "
					<< s << " subsequent parameters!\n";
		};
		onHelp = [] () {
			log.get() << "Usage: " << program << " [<argument>]\n";
		};
		log = std::ref(std::cout);
		options = Array<Option>();
		helpCommandShort = "-h";
		helpCommandFull = "-help";
	}

	Parameters::Parameters(Boolean failed) {
		hasFailed = failed;
	}

	Parameters::ExclusionEvent
	Parameters::onExclusion = [] (String a, String b) {
		Arguments::log.get() << "Argument Error: Command '" << a
							<< "' is conflicting with command '" << b
							<< "'!\nType " << Arguments::helpCommandShort
							<< " or " << Arguments::helpCommandFull
							<< " for a list of valid commands.\n";
	};

	void Parameters::add(String c, Parameter p) {
		list[c] = p;
	}
	void Parameters::removeOptional(String o) {
		list.erase(o);
	}
	void Parameters::removeOptionals(Array<String> o) {
		for (String & s : o) list.erase(s);
	}
	SizeType Parameters::size() const {
		return list.size();
	}
	SizeType Parameters::parametersCount() const {
		SizeType count = 0;
		for (auto & i : list) {
			if (i.second.object == "true" ||
				i.second.object == "false") {
				continue;
			}
			count += 1;
		}
		return count;
	}
	SizeType Parameters::sizeOfFree() const {
		return freeParameters.size();
	}

	String Parameters::mutualExclusion(Array<String> c) {
		if (c.empty() || list.empty()) return String();
		String a;
		for (String & b : c) {
			if (list.find(b) != list.end()) {
				if (!a.empty()) {
					onExclusion(a, b);
					hasFailedExclusion = true;
					return String();
				}
				a = b;
			}
		}
		return a;
	}

	Parameter Parameters::operator [] (String p) {
		if (list.find(p) != list.end()) {
			return list[p];
		}
		return Parameter();
	}

	Boolean Parameters::failed() {
		return hasFailed;
	}
	Boolean Parameters::exclusionFailed() {
		return hasFailedExclusion;
	}
	
}

#endif
