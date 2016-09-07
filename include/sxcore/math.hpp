#pragma once
SXMODULE_SXCORE
#include "sxcore/unittest.hpp"

#if !SXOS_IOS
	#include <xmmintrin.h>
#endif

namespace sx {
	static const float pi = 3.14159265358979323846f;
	static const float epsilon = 0.0002f;
	inline int iround (float x) {
		return (0.0f <= x) ? int(x+0.5f) : int(x-0.5f);
	}
	inline double pow (double a, double b) { // approximation. see http://martin.ankerl.com/2007/10/04/optimized-pow-approximation-for-java-and-c-c/
		// use union to avoid strict aliasing violation
		static_assert(sizeof(double) == sizeof(std::uint64_t), "");
		union {
			double			d;
			std::uint64_t	i;
		} u;
		u.d = a;
		const int x = int(u.i >> 32);
		const int y = int(b * (x - 1072632447) + 1072632447);
		u.i = static_cast<std::int64_t>(y) << 32;
		return u.d;
	}

//	/// operation with overflow check.
//	template<typename T, typename EXCEPTION = std::exception> class safe_math_template {
//		T a;
//	public:
//		explicit safe_math_template (const T &a) : a(a) { }
//
//		bool is_positive () const	{ return !(std::numeric_limits<T>::is_signed && a < T(0)); }
//		bool less_one () const		{ return (!std::numeric_limits<T>::is_integer && (!std::numeric_limits<T>::is_signed || T(-1) < a) && a < T(1)); }
//
//		const T &get () const	{ return a; }
//		const T abs () const	{ return (is_positive() ? a : (T(0) - a)); }
//
//		const safe_math_template operator+ (const safe_math_template &t) const {
//			const T &b = t.get();
//			if ((t.is_positive() && std::numeric_limits<T>::max() - b < a) || (!t.is_positive() && a < std::numeric_limits<T>::min() - b)) throw EXCEPTION();
//			return safe_math_template(a + b);
//		}
//		const safe_math_template operator- (const safe_math_template &t) const {
//			const T &b = t.get();
//			if ((t.is_positive() && a < std::numeric_limits<T>::min() + b) || (!t.is_positive() && std::numeric_limits<T>::max() + b < a)) throw EXCEPTION();
//			return safe_math_template(a - b);
//		}
//		const safe_math_template operator* (const safe_math_template &t) const {
//			if (!t.less_one() && (std::numeric_limits<T>::max() / t.abs() < abs())) throw EXCEPTION();
//			return safe_math_template(a * t.get());
//		}
//		const safe_math_template operator/ (const safe_math_template &t) const {
//			if (t.less_one() && (std::numeric_limits<T>::max() * t.abs() < abs())) throw EXCEPTION();
//			return safe_math_template(a / t.get());
//		}
//	};
	
	// std::isnan on ARM seems to be broken. VC++ doesn't even have std::isnan. comparing to itself may not work when IEEE adherence is relaxed.
//	inline bool isnan (const float &f) {
//		static_assert(sizeof(float) == sizeof(unsigned), "");
//		return (0x7f800000 < (*(unsigned *)(&f) & 0x7fffffff));
//	}
//	inline bool isnan (const double &d) {
//		static_assert(sizeof(double) == sizeof(long long unsigned), "");
//		return (0x7ff0000000000000uLL < (*(unsigned long long *)(&d) & 0x7fffffffffffffffuLL));
//	}
//	template<typename T> bool is_finite (const T &t) {
//		return ((std::numeric_limits<T>::lowest() <= t) && (t <= std::numeric_limits<T>::max()));
//	}
	namespace math {
		inline void initialize () {
			#if !SXOS_IOS
				_mm_setcsr( _mm_getcsr() | 0x8040 );	// disable denormal floats. see http://en.wikipedia.org/wiki/Denormal_number
			#endif
		}
		inline void finalize () {
		}
	}
	template<typename T> bool isnormal (const T t) {
		return ((std::numeric_limits<T>::min() <= std::abs(t)) || (t == T(0.0)));
	}
}
