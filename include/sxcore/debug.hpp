#pragma once
SXMODULE_SXCORE
#include "sxcore/incrementer.hpp"

namespace sx {
	class object;

	namespace debug {
		namespace settings {
			extern bool debug;
			extern int verbose_level;
		}

		class tracer_class {
		public:
			explicit tracer_class (const char *name);
			~tracer_class ();
		private:
			const char *const _name;
		};
		extern double time;

		void print_backtrace ();
		void print_lasterror (const char* file, int line);
		void check_lasterror (const char* file, int line);
	}


	extern int debug_level;
	void debugbreak (const char *file, int line) SXNORETURN;
	
	namespace debug {
		inline void donothing () {
		}
		inline void breakonfail (const bool b, const char* const file, const int line) {
			if (!b) sx::debugbreak(file, line);
		}
	}
}

#if __GNUG__
	#define SXTRACE sx::debug::tracer_class sxdebugtracer(__PRETTY_FUNCTION__);
#else
	#define SXTRACE
#endif

#ifndef sxassert
	#ifdef NDEBUG
		#define sxassert(b)	sx::debug::donothing()
	#else
		#define sxassert(b) sx::debug::breakonfail(bool(b), __FILE__, __LINE__)
	#endif
#endif

#if SXASSERTDEFINED
	#define SXASSERT(b) sx::debug::breakonfail(bool(b), __FILE__, __LINE__)
#else
	#define SXASSERT(b) sxassert(b)
#endif

#if SXTESTDEFINED
	#define SXTEST(b)	(b)
#else
	#define SXTEST(b)	sx::debug::donothing()
#endif

#if SXBACKTRACEDEFINED
	#define SXBACKTRACE sx::debug::print_backtrace()
#else
	#define SXBACKTRACE
#endif

#if SXLASTERRORDEFINED
	#define SXPRINTLASTERROR sx::debug::print_lasterror(__FILE__, __LINE__)
	#define SXCHECKLASTERROR sx::debug::check_lasterror(__FILE__, __LINE__)
#else
	#define SXPRINTLASTERROR
	#define SXCHECKLASTERROR
#endif

#define  SXDEBUG (SXASSERTDEFINED || !defined(NDEBUG))

namespace sx {
	extern SXTHREAD sx::thread::local<int> _asserting_invariant;

	// uses expression SFINAE. see http://stackoverflow.com/questions/257288/is-it-possible-to-write-a-c-template-to-check-for-a-functions-existence
	#if SXINVARIANT
	template<typename T> inline auto _assert_invariant (const T &t, int) -> decltype(t.T::assert_invariant(), void()) {
		SXASSERT(([&t]{ if (_asserting_invariant() < 4) { auto _ = sx::make_incrementer(_asserting_invariant); t.T::assert_invariant(); } }(), true));
	}
	#endif
	template<typename T> inline auto _assert_invariant (const T &t, long) -> void {
	}

	template<typename T> inline void assert_invariant (const T &t) {
		_assert_invariant(t, 0);
	}
	template<typename T> inline void assert_invariant (const std::reference_wrapper<T> &t) {
		assert_invariant(t.get());
	}
	template<typename T, typename U> inline auto assert_invariant (const U& u) -> typename std::enable_if<!std::is_base_of<T,U>::value,void>::type {
	}
	
	template<typename T, typename... U> inline void assert_invariant (const T& t, const U&... u);
	template<typename U> inline void _assert_invariants (const U& u) {
		assert_invariant(u);
	}
	template<typename... U> inline void _assert_invariants (const U&... u) {
		assert_invariant(u...);
	}
	template<typename T, typename... U> inline void assert_invariant (const T& t, const U&... u) {
		assert_invariant(t);
		_assert_invariants(u...);
	}
	
	template<typename T> class _invariant_scope {
		_invariant_scope (const _invariant_scope&)				= delete;
		_invariant_scope& operator= (const _invariant_scope)	= delete;
	public:
		explicit _invariant_scope (const T& t) : _t(&t) {
			sx::assert_invariant(t);
		}
		_invariant_scope (_invariant_scope&& i) : _t(i._t) {
			i._t = nullptr;
		}
		_invariant_scope& operator= (_invariant_scope&& i) {
			_t		= i._t;
			i._t	= nullptr;
		}
		~_invariant_scope () {
			if (_t) sx::assert_invariant(*_t);	// self-destructing objects should be handled properly...
		}
	private:
		const T* _t;
	};
	
	template<typename T> _invariant_scope<T> make_invariant_scope (const T& t){
		return _invariant_scope<T>{t};
	}
}
