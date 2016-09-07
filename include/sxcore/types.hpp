#pragma once
SXMODULE_SXCORE

namespace sx {
	using float32_t		= float;	static_assert(sizeof(float32_t) == 4, "");
	using float64_t		= double;	static_assert(sizeof(float64_t) == 8, "");
	using size_t		= std::make_signed<std::size_t>::type;
	using byte_t		= char;

	template<typename T> struct type {
		inline static const T& cast (const T& t) { return t; }
		inline static T& cast (T& t) { return t; }
		template<typename U> inline static T cast (U&& u) { return static_cast<T>(std::forward<U>(u)); }
	};
	
	template<typename T, std::size_t N> struct type<std::array<T,N>> {
		inline static std::array<T,N> cast (const T& t) { std::array<T,N> r; r.fill(t); return r; }
	};

	template<typename T, typename... A> inline T cast (const A&... a) {
		return sx::type<T>::cast(a...);
	}

	#if SXAPI_CORE_WIN32
		template<> struct type<LARGE_INTEGER> {
			inline static LARGE_INTEGER cast (LONGLONG l) {
				LARGE_INTEGER large_integer{};
				large_integer.QuadPart = l;
				return large_integer;
			}
		};
	#endif
}

