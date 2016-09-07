#include "sxcore/_dependency.hpp"

bool	sx::debug::settings::debug			{ false };
int		sx::debug::settings::verbose_level	{ 0 };
double	sx::debug::time						{ 0.0 };
int		sx::debug_level						{ 0 };

SXTHREAD sx::thread::local<int>	sx::_asserting_invariant;

void sx::debugbreak (const char *file, int line) {
	sx::debug::print_backtrace();
	std::cerr << "debugbreak " << file << " " << line << std::endl << std::flush;
	#if __GNUC__ || __clang__
		__builtin_trap();
	#elif SXAPI_WIN32
		::DebugBreak(); // for some reason DebugBreak() seems to destroy call stack information on 32-bit windows release build. then yet, *(char *)(0) = 0; doesn't quite work well......
	#else
		#error
	#endif
	::exit(-1);
}

sx::debug::tracer_class::tracer_class (const char *const name) : _name(name) {
	std::cerr << ">>>> " << _name << std::endl;
}
sx::debug::tracer_class::~tracer_class () {
	try { std::cerr << "<<<< " << _name << std::endl; } catch (...) { }
}

#if SXAPI_MACH
	#include <execinfo.h>
	void sx::debug::print_backtrace () {
		if (sx::core::settings::backtrace()) {
			void* callstack[128];
			int frames = ::backtrace(callstack, 128);
			char** strs = ::backtrace_symbols(callstack, frames);
			for (int i = 0; i < frames; ++i) {
				::fprintf(stderr, "%s\n", strs[i]);
			}
			::free(strs);
		}
	}
#elif SXAPI_WIN32
	#include <DbgHelp.h>
	#pragma comment(lib, "dbghelp.lib")
	void sx::debug::print_backtrace () {
		if (sx::core::settings::backtrace()) {
			HANDLE process = ::GetCurrentProcess();
			::SymInitialize(process, NULL, TRUE);

			void* stack[100];
			unsigned short frames = ::CaptureStackBackTrace(0, 100, stack, NULL);
			SYMBOL_INFO* symbol = (SYMBOL_INFO*)calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
			symbol->MaxNameLen   = 255;
			symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

			for (int i = 0; i < frames; ++i) {
				::SymFromAddr(process, (DWORD64)(stack[i]), 0, symbol);
				::fprintf(stderr, "%i: %s - 0x%0X\n", frames - i - 1, symbol->Name, symbol->Address);
			}
			::free(symbol);
		}
	}
#else
	void sx::debug::print_backtrace () {
	}
#endif

void sx::debug::print_lasterror (const char *file, int line) {
	#if SXLASTERRORDEFINED && SXAPI_CORE_WIN32
		if (::GetLastError() != 0) {
			std::ios::fmtflags f = std::cerr.flags();
			std::cerr << file << " " << std::dec << line << " LastError: " << std::hex << ::GetLastError() << " " << sx::format_message(::GetLastError());
			std::cerr.flags(f);
		}
	#endif
}

void sx::debug::check_lasterror (const char *file, int line) {
	#if SXLASTERRORDEFINED && SXAPI_CORE_WIN32
		if (::GetLastError() != 0) {
			print_lasterror(file, line);
			if (0 < sx::debug::settings::verbose_level) print_backtrace();
			//SXASSERT(false);
		}
	#endif
}
