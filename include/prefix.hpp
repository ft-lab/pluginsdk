#ifndef PREFIX_661B1848_A460_4CF3_8913_A666522DD43F
#define PREFIX_661B1848_A460_4CF3_8913_A666522DD43F

#if __clang__
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wdeprecated-register"
	#pragma clang diagnostic ignored "-Wundef"
	#pragma clang diagnostic ignored "-Wc++98-compat"
	#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
	#pragma clang diagnostic ignored "-Wdeprecated"
	#pragma clang diagnostic ignored "-Wold-style-cast"
	#pragma clang diagnostic ignored "-Wdisabled-macro-expansion"
	#pragma clang diagnostic ignored "-Wextra-semi"
	#pragma clang diagnostic ignored "-Wdocumentation"
	#pragma clang diagnostic ignored "-Wnested-anon-types"
	#pragma clang diagnostic ignored "-Wpadded"
	#pragma clang diagnostic ignored "-Wswitch-enum"
	#pragma clang diagnostic ignored "-Wfloat-equal"
	#pragma clang diagnostic ignored "-Wglobal-constructors"
	#pragma clang diagnostic ignored "-Wweak-vtables"
	#pragma clang diagnostic ignored "-Wimplicit-fallthrough"
	#pragma clang diagnostic ignored "-Wconditional-uninitialized"
	#pragma clang diagnostic ignored "-Wmissing-noreturn"
	#pragma clang diagnostic ignored "-Wcast-align"
#endif

#include "sxcore/config.hpp"

#if SXAPI_CORE_FOUNDATION
	#include <CoreFoundation/CoreFoundation.h>
#undef	SXOS_OSX
#define	SXOS_OSX	1
#undef	SXOS_IOS
#define	SXOS_IOS	0
#undef	SXAPI_COCOA
#define	SXAPI_COCOA	1
#undef	SXAPI_COCOA_TOUCH
#define	SXAPI_COCOA_TOUCH	0
#endif

#if (SXOS_OSX || SXOS_IOS)
	#define SXAPI_GCD		1
	#define SXLIBDISPATCH	1
#endif

#ifdef _WIN32
	#include <winapifamily.h>
	#define SXDESKTOP WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
	#define SXWINRT !SXDESKTOP
	#if !SXDESKTOP
		#define WIN32_LEAN_AND_MEAN
//		#pragma region Application Family
		//#ifdef SXAPI_WIN32
		//	#undef SXAPI_WIN32
		//#endif
		//#define SXAPI_WIN32	0
		#ifdef SXAPI_MFC
			#undef SXAPI_MFC
		#endif
		#define SXAPI_MFC	0
	#endif
#endif

#ifndef SXSIMD
	#if _WIN32
		#if _WIN64
			#define SXSIMD 3
		#else
			#define SXSIMD 2
		#endif
	#elif SXOS_IOS
		#define SXSIMD 0
	#elif SXOS_LINUX
		#define SXSIMD 0
	#elif __SSE__
		#define SXSIMD 3
	#else
		#define SXSIMD 0
	#endif
#endif

#ifndef SXQUICKTIME
	#define	SXQUICKTIME 0
	#ifdef _WIN32
		#ifndef _WIN64
			#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
				#undef	SXQUICKTIME
				#define	SXQUICKTIME	1
			#endif
		#endif
	#endif
#endif

#define SX64 (__LP64__ || _WIN64)
#define SX32 (!SX64)
#if _AFXDLL || _AFX
	#include <afx.h>
	#include <winsock2.h>
	#include <stdafx.h>
#endif
#ifdef __APPLE__
	#define __ASSERTMACROS__ 1
	#define DLLEXPORT
	#define STDCALL
	#define __PROJECT_BUILDER__ 1
#endif
#if !defined(_WIN32)
    #define __stdcall
#endif

#ifdef _WIN32
	#ifndef _WINDOWS_
		#include <Windows.h>
	#endif
	#ifdef WIN32_LEAN_AND_MEAN
		#include <rpc.h>
	#endif
	#if SXDESKTOP
		#include <Gdiplus.h> // Gdiplus.h uses "min" and "max" macro symbol which shall be undefined to avoid conflict with std::max.
		#undef min
		#undef max
	#else
		#undef min
		#undef max
	#endif
	#ifdef DEBUG // quicktime header uses DEBUG macro symbol.
		#undef DEBUG
		#define DEBUG 1
	#endif
#endif

#if SXAPI_COCOA
	#define SXQTKIT 1
	#ifdef __OBJC__
		#import <Cocoa/Cocoa.h>
	#endif
#endif
#if SXOS_IOS
	#include <MobileCoreServices/MobileCoreServices.h>
	#include <CoreGraphics/CoreGraphics.h>
	#ifdef __OBJC__
		#import <Foundation/Foundation.h>
		#import <UIKit/UIKit.h>
	#endif
#endif

#if SXOS_WINDOWS
	#define SXPARALLELPATTERNSLIBRARY	1
#endif

#ifdef _MSC_VER
  #define SXSDKEXPORT	__declspec(dllexport)
#else
	#ifdef __GNUG__
		#define SXSDKEXPORT	__attribute__ ((visibility("default")))
	#else
	  #define SXSDKEXPORT
	#endif
#endif

#if SXAPI_MACH
	#include <malloc/malloc.h>
#elif SXAPI_POSIX
    #include <malloc.h>
#endif

#include <assert.h>

#if _MSC_VER
	#include <stdint.h>
	#define _STDINT_H
#endif
#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable:4996)
	#pragma warning(disable:4121)
	#pragma warning(disable:4267)
	#pragma warning(disable:4244)
	#pragma warning(disable:4800)
#endif

#include <tuple>
#include <array>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <stack>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <cmath>
#include <string>
#include <cctype>
#include <algorithm>
#include <limits>
#include <memory>
#include <functional>
#include <iomanip>
#include <type_traits>
#include <mutex>
#include <atomic>
#include <thread>
#include <future>
#include <utility>
#include <chrono>
#include <bitset>
#include <cctype>
#ifdef _MSC_VER
	#pragma warning(pop)
#endif

#define BOOST_ALL_NO_LIB
#include <boost/version.hpp>
static_assert(105300 <= BOOST_VERSION, "");
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/any.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/random.hpp>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <boost/variant.hpp>
#include <boost/format.hpp>
#include <boost/assign.hpp>
#include <boost/container/static_vector.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/scoped_array.hpp>
#include <boost/functional/hash.hpp>

#if SXAPI_POSIX
	#include <pthread.h>
#endif

// fake unimplemented c++11 features
//#if __GNUG__ && !__clang__
//	namespace std {
//        template<typename T> using is_trivially_copyable = std::has_trivial_copy_assign<T>;
//        template<typename T> using is_trivially_constructible = std::has_trivial_default_constructor<T>;
//    }
//#endif

#if SXOS_IOS
	#include <pthread.h>
	#define SXTHREAD
	namespace sx {
		namespace thread {
			template<typename T> class local {
				static_assert(sizeof(T) <= sizeof(void*), "");
				static_assert(std::is_trivially_copyable<T>::value, "");
				local (const local&)			= delete;
				local& operator= (const local&)	= delete;
			public:
				local () {
					::pthread_key_create(&_key, nullptr);
				}
				local (const T& t) {
					::pthread_key_create(&_key, nullptr);
					operator()(t);
				}
				~local () = default;
				void operator () (const T& t) { // setter
					union {
						T		t;
						void*	p{};
					} u;
					u.t = t;
					::pthread_setspecific(_key, u.p);
				}
				T operator() () const { // getter
					union {
						T		t;
						void*	p{};
					} u;
					u.p = ::pthread_getspecific(_key);
					return u.t;
				}
				T operator++ () {
					T t = (*this)();
					++t;
					(*this)(t);
					return t;
				}
				T operator-- () {
					T t = (*this)();
					--t;
					(*this)(t);
					return t;
				}
				T operator++ (int) {
					const T t = (*this)();
					(*this)(t+1);
					return t;
				}
				T operator-- (int) {
					const T t = (*this)();
					(*this)(t-1);
					return t;
				}
			private:
				pthread_key_t	_key{};
			};
		}
	}
#else
	#if __GNUG__ || __clang__
		#define SXTHREAD	__thread
	#elif (1900 <= _MSC_VER)
		#define SXTHREAD	thread_local
	#else
		#define SXTHREAD	__declspec(thread)
	#endif

	namespace sx {
		namespace thread {
			template<typename T> class local {
			public:
				local () = default;
//				local (const T& t) : _data(t) { }
				~local () = default;
				void operator () (const T& t) { // setter
					_data = t;
				}
				const T& operator() () const { // getter
					return _data;
				}
				const T& operator++ () {
					return ++_data;
				}
				const T& operator-- () {
					return --_data;
				}
				T operator++ (int) {
					return _data++;
				}
				T operator-- (int) {
					return _data--;
				}
//			private: // commented out due to VC++ BUG: C2483
				T _data;
			};
		}
	}
#endif


#if _MSC_VER && (_MSC_VER < 1900)
	#define noexcept
#endif

// lvalue reference qualifier
#if !_MSC_VER || (1900 <= _MSC_VER)
	#define SXLREF	&
#else
	#define SXLREF
#endif

#if _MSC_VER
	namespace std {
		// std::alignment_of<__m128>::value is (correctly) 16, but standard std::aligned_storage<...,16>::type does not honor 16byte alignment.
		template<size_t _Len> struct aligned_storage<_Len, 16> {
			typedef struct {
				__declspec(align(16)) char _data[_Len];
			} type;
		};
	}
#endif

// fake c++14 features
#include <boost/optional.hpp>
namespace std {
	using ::boost::optional;

	template<typename T> class shared_lock {
	public:
		explicit shared_lock () : _t(nullptr) {
		}
		explicit shared_lock (T& t) : _t(&t) {
			t.lock_shared();
		}
		shared_lock (shared_lock&& t) noexcept : _t(t._t) {
			t._t = nullptr;
		}
		shared_lock& operator= (shared_lock&& t) noexcept {
			swap(_t, t._t);
			return *this;
		}
		~shared_lock () {
			if (_t) _t->unlock();
		}
	private:
		T*	_t;
	};

	#if (__cplusplus < 201305L) && (_MSC_VER < 1900)

		#if __GNUG__ || __clang__
			template <size_t _Len, size_t _Align = __find_max_align<__all_types, _Len>::value> using aligned_storage_t = typename aligned_storage<_Len, _Align>::type;

			template<typename T> auto cbegin	(const T& t)	-> decltype(t.cbegin())	{ return t.cbegin(); }
			template<typename T> auto cend		(const T& t)	-> decltype(t.cend())	{ return t.cend(); }
			template<typename T, typename... A> std::unique_ptr<T> make_unique(A&&... a) {
				return std::unique_ptr<T>(new T(std::forward<A>(a)...));
			}
		#endif

		// see the last slide of STL's presentation on random() at goingnative 13.
		template<typename T, T... I> struct integer_sequence {
			template<T N> using Prepend = integer_sequence<T, N, I...>;
		};
		template<typename T, typename Num, typename Ret> struct MakeIntegerSequenceHelper;
		template<typename T, typename Ret> struct MakeIntegerSequenceHelper<T, integral_constant<T,0>, Ret> {
			typedef Ret type;
		};
		template<typename T, typename Num, typename Ret> struct MakeIntegerSequenceHelper : MakeIntegerSequenceHelper<T, integral_constant<T, Num::value-1>, typename Ret::template Prepend<Num::value-1>> {
		};
	//	template <typename T, T N> using make_integer_sequence = typename MakeIntegerSequenceHelper<T, integral_constant<T, N>, integer_sequence<T>>::type;

		template<size_t... I> using index_sequence = integer_sequence<size_t, I...>;
	//	template<size_t N> using make_index_sequence = make_integer_sequence<size_t, N>;
		template<size_t N> using make_index_sequence =  typename MakeIntegerSequenceHelper<size_t, integral_constant<size_t, N>, integer_sequence<size_t>>::type;
		template<typename... T> using index_sequence_for = make_index_sequence<sizeof...(T)>;
	#endif
}

// fake Swift feature
#if __clang__
	#define fallthrough [[clang::fallthrough]]
#else
	#define fallthrough
#endif

#if _MSC_VER
//	static_assert(_SECURE_SCL == 0, "");
#endif

#if __clang__
	#define SXNORETURN	__attribute__((noreturn))
#else
	#define SXNORETURN
#endif

//#define SXDEFAULTCOPYCONSTRUCTOR(T)	T (const T&) = default;
//#define SXDEFAULTCOPYASSIGNMENT(T)	//T& operator= (const T&) = default;

#if _MSC_VER && (_MSC_VER < 1900) // default implementation of move constructor/assignment operator not supported.
	#define SXDEFAULTMOVECONSTRUCTOR(T)					noexcept : base(std::move(T)) { }
	#define SXDEFAULTMOVEASSIGNMENT(T)					noexcept { if (this != &T) { base::operator=(std::move(T)); } return *this; } 

	#define SXDEFAULTMOVECONSTRUCTOR1(T,P0)				noexcept : base(std::move(T)), P0(std::move(T.P0)) { }
	#define SXDEFAULTMOVEASSIGNMENT1(T,P0)				noexcept { if (this != &T) { base::operator=(std::move(T)); P0 = std::move(T.P0); } return *this; } 

	#define SXDEFAULTMOVECONSTRUCTOR2(T,P0,P1)			noexcept : base(std::move(T)), P0(std::move(T.P0)), P1(std::move(T.P1)) { }
	#define SXDEFAULTMOVEASSIGNMENT2(T,P0,P1)			noexcept { if (this != &T) { base::operator=(std::move(T)); P0 = std::move(T.P0); P1 = std::move(T.P1); } return *this; } 

	#define SXDEFAULTMOVECONSTRUCTOR3(T,P0,P1,P2)		noexcept : base(std::move(T)), P0(std::move(T.P0)), P1(std::move(T.P1)), P2(std::move(T.P2)) { }
	#define SXDEFAULTMOVEASSIGNMENT3(T,P0,P1,P2)		noexcept { if (this != &T) { base::operator=(std::move(T)); P0 = std::move(T.P0); P1 = std::move(T.P1); P2 = std::move(T.P2); } return *this; } 

	#define SXDEFAULTMOVECONSTRUCTOR4(T,P0,P1,P2,P3)	noexcept : base(std::move(T)), P0(std::move(T.P0)), P1(std::move(T.P1)), P2(std::move(T.P2)), P3(std::move(T.P3)) { }
	#define SXDEFAULTMOVEASSIGNMENT4(T,P0,P1,P2,P3)		noexcept { if (this != &T) { base::operator=(std::move(T)); P0 = std::move(T.P0); P1 = std::move(T.P1); P2 = std::move(T.P2); P3 = std::move(T.P3); } return *this; } 

	#define SXDEFAULTMOVECONSTRUCTOR5(T,P0,P1,P2,P3,P4)	noexcept : base(std::move(T)), P0(std::move(T.P0)), P1(std::move(T.P1)), P2(std::move(T.P2)), P3(std::move(T.P3)), P4(std::move(T.P4)) { }
	#define SXDEFAULTMOVEASSIGNMENT5(T,P0,P1,P2,P3,P4)		noexcept { if (this != &T) { base::operator=(std::move(T)); P0 = std::move(T.P0); P1 = std::move(T.P1); P2 = std::move(T.P2); P3 = std::move(T.P3); P4 = std::move(T.P4); } return *this; } 

	#define SXDEFAULTMOVECONSTRUCTOR7(T,P0,P1,P2,P3,P4,P5,P6)	noexcept : base(std::move(T)), P0(std::move(T.P0)), P1(std::move(T.P1)), P2(std::move(T.P2)), P3(std::move(T.P3)), P4(std::move(T.P4)), P5(std::move(T.P5)), P6(std::move(T.P6)) { }
	#define SXDEFAULTMOVEASSIGNMENT7(T,P0,P1,P2,P3,P4,P5,P6)		noexcept { if (this != &T) { base::operator=(std::move(T)); P0 = std::move(T.P0); P1 = std::move(T.P1); P2 = std::move(T.P2); P3 = std::move(T.P3); P4 = std::move(T.P4); P5 = std::move(T.P5); P6 = std::move(T.P6); } return *this; } 

	#define SXDEFAULTMOVECONSTRUCTOR17(T,P0,P1,P2,P3,P4,P5,P6,P7,P8,P9,P10,P11,P12,P13,P14,P15,P16)	noexcept : base(std::move(T)), P0(std::move(T.P0)), P1(std::move(T.P1)), P2(std::move(T.P2)), P3(std::move(T.P3)), P4(std::move(T.P4)), P5(std::move(T.P5)), P6(std::move(T.P6)), P7(std::move(T.P7)), P8(std::move(T.P8)), P9(std::move(T.P9)), P10(std::move(T.P10)), P11(std::move(T.P11)), P12(std::move(T.P12)), P13(std::move(T.P13)), P14(std::move(T.P14)), P15(std::move(T.P15)), P16(std::move(T.P16)) { }
	#define SXDEFAULTMOVEASSIGNMENT17(T,P0,P1,P2,P3,P4,P5,P6,P7,P8,P9,P10,P11,P12,P13,P14,P15,P16)		noexcept { if (this != &T) { base::operator=(std::move(T)); P0 = std::move(T.P0); P1 = std::move(T.P1); P2 = std::move(T.P2); P3 = std::move(T.P3); P4 = std::move(T.P4); P5 = std::move(T.P5); P6 = std::move(T.P6); P7 = std::move(T.P7); P8 = std::move(T.P8); P9 = std::move(T.P9); P10 = std::move(T.P10); P11 = std::move(T.P11); P12 = std::move(T.P12); P13 = std::move(T.P13); P14 = std::move(T.P14); P15 = std::move(T.P15); P16 = std::move(T.P16); } return *this; } 

	#define SXDEFAULTMOVECONSTRUCTOR18(T,P0,P1,P2,P3,P4,P5,P6,P7,P8,P9,P10,P11,P12,P13,P14,P15,P16,P17)	noexcept : base(std::move(T)), P0(std::move(T.P0)), P1(std::move(T.P1)), P2(std::move(T.P2)), P3(std::move(T.P3)), P4(std::move(T.P4)), P5(std::move(T.P5)), P6(std::move(T.P6)), P7(std::move(T.P7)), P8(std::move(T.P8)), P9(std::move(T.P9)), P10(std::move(T.P10)), P11(std::move(T.P11)), P12(std::move(T.P12)), P13(std::move(T.P13)), P14(std::move(T.P14)), P15(std::move(T.P15)), P16(std::move(T.P16)), P17(std::move(T.P17)) { }
	#define SXDEFAULTMOVEASSIGNMENT18(T,P0,P1,P2,P3,P4,P5,P6,P7,P8,P9,P10,P11,P12,P13,P14,P15,P16,P17)		noexcept { if (this != &T) { base::operator=(std::move(T)); P0 = std::move(T.P0); P1 = std::move(T.P1); P2 = std::move(T.P2); P3 = std::move(T.P3); P4 = std::move(T.P4); P5 = std::move(T.P5); P6 = std::move(T.P6); P7 = std::move(T.P7); P8 = std::move(T.P8); P9 = std::move(T.P9); P10 = std::move(T.P10); P11 = std::move(T.P11); P12 = std::move(T.P12); P13 = std::move(T.P13); P14 = std::move(T.P14); P15 = std::move(T.P15); P16 = std::move(T.P16); P17 = std::move(T.P17); } return *this; } 
#else
	#define SXDEFAULTMOVECONSTRUCTOR(T)																	= default
	#define SXDEFAULTMOVEASSIGNMENT(T)																	= default 
	#define SXDEFAULTMOVECONSTRUCTOR1(T,P0)																= default
	#define SXDEFAULTMOVEASSIGNMENT1(T,P0)																= default 
	#define SXDEFAULTMOVECONSTRUCTOR2(T,P0,P1)															= default
	#define SXDEFAULTMOVEASSIGNMENT2(T,P0,P1)															= default 
	#define SXDEFAULTMOVECONSTRUCTOR3(T,P0,P1,P2)														= default
	#define SXDEFAULTMOVEASSIGNMENT3(T,P0,P1,P2)														= default 
	#define SXDEFAULTMOVECONSTRUCTOR4(T,P0,P1,P2,P3)													= default
	#define SXDEFAULTMOVEASSIGNMENT4(T,P0,P1,P2,P3)														= default 
	#define SXDEFAULTMOVECONSTRUCTOR5(T,P0,P1,P2,P3,P4)													= default
	#define SXDEFAULTMOVEASSIGNMENT5(T,P0,P1,P2,P3,P4)													= default 
	#define SXDEFAULTMOVECONSTRUCTOR7(T,P0,P1,P2,P3,P4,P5,P6)											= default
	#define SXDEFAULTMOVEASSIGNMENT7(T,P0,P1,P2,P3,P4,P5,P6)											= default 
	#define SXDEFAULTMOVECONSTRUCTOR17(T,P0,P1,P2,P3,P4,P5,P6,P7,P8,P9,P10,P11,P12,P13,P14,P15,P16)		= default 
	#define SXDEFAULTMOVEASSIGNMENT17(T,P0,P1,P2,P3,P4,P5,P6,P7,P8,P9,P10,P11,P12,P13,P14,P15,P16)		= default 
	#define SXDEFAULTMOVECONSTRUCTOR18(T,P0,P1,P2,P3,P4,P5,P6,P7,P8,P9,P10,P11,P12,P13,P14,P15,P16,P17)	= default 
	#define SXDEFAULTMOVEASSIGNMENT18(T,P0,P1,P2,P3,P4,P5,P6,P7,P8,P9,P10,P11,P12,P13,P14,P15,P16,P17)	= default 
#endif

#if SXAPI_COCOA
	#define SXAUTORELEASEPOOL	@autoreleasepool	
#else
	#define SXAUTORELEASEPOOL
#endif

#ifndef __OBJC__
	#define	__unsafe_unretained
#endif

#ifdef __MAC_10_8	// os x 10.8 header shamelessly include gl.h. nothing can be done about that...
	#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED
#else
	#ifdef GL_VERSION_1_1
		#error
	#endif
	#define glCullFace (0/0)
#endif

#if __clang__
	#pragma clang diagnostic pop
#endif

#if __clang__
	#pragma clang diagnostic error "-Woverloaded-virtual"
	#pragma clang diagnostic error "-Wnon-virtual-dtor"
	#pragma clang diagnostic error "-Wstrict-aliasing-2"
	#pragma clang diagnostic error "-Wdeprecated"
	#pragma clang diagnostic error "-Wempty-body"
	#pragma clang diagnostic error "-Wover-aligned"
	#pragma clang diagnostic error "-Winvalid-offsetof"
	#pragma clang diagnostic error "-Wold-style-cast"
	#pragma clang diagnostic error "-Wundefined-reinterpret-cast"
	#pragma clang diagnostic error "-Wlogical-not-parentheses"
	#pragma clang diagnostic error "-Wobjc-interface-ivars"
	#pragma clang diagnostic error "-Wshift-sign-overflow"
	#pragma clang diagnostic error "-Wimplicit-fallthrough"
//	#pragma clang diagnostic error "-Wconditional-uninitialized"

	#pragma clang diagnostic ignored "-Wtautological-compare"
	#pragma clang diagnostic ignored "-Wmissing-noreturn"
	#pragma clang diagnostic ignored "-Wfloat-equal"
	#pragma clang diagnostic ignored "-Wglobal-constructors"
	#pragma clang diagnostic ignored "-Wreorder"
	#pragma clang diagnostic ignored "-Wcast-align"
	#pragma clang diagnostic ignored "-Wpadded"
	#pragma clang diagnostic ignored "-Wunreachable-code"
	#pragma clang diagnostic ignored "-Wunused-member-function"
	#pragma clang diagnostic ignored "-Wunused-exception-parameter"
	#pragma clang diagnostic ignored "-Wswitch-enum"
	#pragma clang diagnostic ignored "-Wcovered-switch-default"
	#pragma clang diagnostic ignored "-Wsign-compare"
	#pragma clang diagnostic ignored "-Wdocumentation"
	#pragma clang diagnostic ignored "-Wunused-private-field"
	#pragma clang diagnostic ignored "-Wmissing-braces"
	#pragma clang diagnostic ignored "-Wc++98-compat"
	#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
	#pragma clang diagnostic ignored "-Wundef"
	#pragma clang diagnostic ignored "-Wdisabled-macro-expansion"
	#pragma clang diagnostic ignored "-Wweak-vtables"
	#pragma clang diagnostic ignored "-Wshorten-64-to-32"
	#pragma clang diagnostic ignored "-Wformat-nonliteral"
	#pragma clang diagnostic ignored "-Wswitch"
	#pragma clang diagnostic ignored "-Wdirect-ivar-access"
	#pragma clang diagnostic ignored "-Wunused-value"
	#pragma clang diagnostic ignored "-Wabsolute-value"

	#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif
#ifdef _MSC_VER
	#pragma warning(push, 4)

	// off by default
	#pragma warning(default:4263)	// 'function' : member function does not override any base class virtual member function
//	#pragma warning(default:4264)	// 'virtual_function' : no override available for virtual member function from base 'class'; function is hidden

//	#pragma warning(disable:4063)	// case 'identifier' is not a valid value for switch of enum 'enumeration'
//	#pragma warning(disable:4390)	// ';' : empty controlled statement found; is this the intent?
//	#pragma warning(disable:4700)	// uninitialized local variable 'name' used
//	#pragma warning(disable:4213)	/// nonstandard extension used : cast on l-value
//	#pragma warning(disable:4239)	/// nonstandard extension used : 'token' : conversion from 'type' to 'type'
//	#pragma warning(disable:4273)	/// 'function' : inconsistent DLL linkage
//	#pragma warning(disable:4627)	/// '<identifier>': skipped when looking for precompiled header use

	#pragma warning(disable:4121)	// 'symbol' : alignment of a member was sensitive to packing
	#pragma warning(disable:4702)	// unreachable code
	#pragma warning(disable:4510)	// 'class' : default constructor could not be generated
	#pragma warning(disable:4512)	// 'class' : assignment operator could not be generated
	#pragma warning(disable:4610)	// object 'class' can never be instantiated - user-defined constructor required
	#pragma warning(disable:4258)	// 'variable' : definition from the for loop is ignored; the definition from the enclosing scope is used"
	#pragma warning(disable:4065)	// switch statement contains 'default' but no 'case' labels
	#pragma warning(disable:4005)	// 'identifier' : macro redefinition
	#pragma warning(disable:4068)	// unknown pragma
	#pragma warning(disable:4100)	// 'identifier' : unreferenced formal parameter
	#pragma warning(disable:4101)	// 'identifier' : unreferenced local variable
	#pragma warning(disable:4127)	// conditional expression is constant
	#pragma warning(disable:4189)	// 'identifier' : local variable is initialized but not referenced
	#pragma warning(disable:4250)	// 'class1' : inherits 'class2::member' via dominance
	#pragma warning(disable:4324)	// 'struct_name' : structure was padded due to __declspec(align())
	#pragma warning(disable:4505)	// 'function' : unreferenced local function has been removed
	#pragma warning(disable:4310)	// cast truncates constant value
	#pragma warning(disable:4805)	// 'operation' : unsafe mix of type 'type' and type 'type' in operation
	#pragma warning(disable:4389)	// 'operator' : signed/unsigned mismatch
	#pragma warning(disable:4018)	// 'expression' : signed/unsigned mismatch
	#pragma warning(disable:4244)	// 'argument' : conversion from 'type1' to 'type2', possible loss of data
	#pragma warning(disable:4245)	// 'conversion' : conversion from 'type1' to 'type2', signed/unsigned mismatch
	#pragma warning(disable:4267)	// 'var' : conversion from 'size_t' to 'type', possible loss of data
	#pragma warning(disable:4305)	// 'identifier' : truncation from 'type1' to 'type2'
	#pragma warning(disable:4334)	// 'operator' : result of 32-bit shift implicitly converted to 64 bits (was 64-bit shift intended?)
	#pragma warning(disable:4800)	// 'type' : forcing value to bool 'true' or 'false' (performance warning)
	#pragma warning(disable:4611)	// interaction between 'function' and C++ object destruction is non-portable
	#pragma warning(disable:4723)	// potential divide by 0

	#pragma warning(disable:4996)	/// 'function': was declared deprecated
	#pragma warning(disable:4819)	/// The file contains a character that cannot be represented in the current code page (936). Save the file in Unicode format to prevent data loss

	#pragma warning(disable:4458)	// declaration of 'name' hides class member
	#pragma warning(disable:4456)	// declaration of 'i' hides previous local declaration
	#pragma warning(disable:4456)	// declaration of 'i' hides previous local declaration
	#pragma warning(disable:4459)	// declaration of 'queue' hides global declaration
	#pragma warning(disable:4091)	// 'typedef ': ignored on left of '' when no variable is declared
	#pragma warning(disable:4457)	// declaration of 'color' hides function parameter
	#pragma warning(disable:4312)	// 'reinterpret_cast': conversion from 'const int' to 'void *' of greater size
	#pragma warning(disable:4838)	// conversion from 'float' to 'LONG' requires a narrowing conversion
	#pragma warning(disable:4297)	// '`anonymous-namespace'::save_exr_class::~save_exr_class': function assumed not to throw an exception but does
	#pragma warning(disable:4311)	// '<function-style-cast>': pointer truncation from 'sxsdk::shade_interface *' to 'long'
	#pragma warning(disable:4302)	// '<function-style-cast>': truncation from 'sxsdk::shade_interface *' to 'long'
#endif

#endif
