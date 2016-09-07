#pragma once
SXMODULE_SXCORE
#include "sxcore/memory.hpp"
#include "sxcore/simd.hpp"
#include "sxcore/vectors.hpp"
#include "sxcore/debug.hpp"
#include "sxcore/minmax.hpp"
#include "sxcore/endian.hpp"

namespace sx {

	template<typename T> class bounds {
	public: // properties
		#if ((0 < _MSC_VER) && (_MSC_VER < 1900))	// see sxcore/note.txt
		const T& origin () const {
		#else
		T origin () const {
		#endif
			SXASSERT(min < std::numeric_limits<T>::max());
			return min;
		}
		void origin (const T& origin) {
			SXASSERT(!empty());
			const T s = max - min;
			min = origin;
			max = origin + s;
		}
		T size () const {
			return (((min==std::numeric_limits<T>::max()) && (max==std::numeric_limits<T>::lowest())) ? std::numeric_limits<T>::lowest() : (max - min));
		}
		void size (const T& size) {
			SXASSERT(min < std::numeric_limits<T>::max());
			max = min + size;
		}
		T center () const {
			SXASSERT(!empty());
			return (max + min) / 2;
		}
		bool empty () const {
			return !(min <= max);
		}
	public:
		T min{std::numeric_limits<T>::max()};
		T max{std::numeric_limits<T>::lowest()};

		bounds () = default;
		bounds (const bounds&) = default;
		bounds (const T& min, const T& max) : min{min}, max{max} {
			sx::assert_invariant(*this);
		}
//		explicit bounds (const T& max) : min{}, max(max) {
//			sx::assert_invariant(*this);
//		}
//		template<typename U0, typename U1, typename U2, typename U3> explicit bounds (const U0& minx, const U1& miny, const U2& maxx, const U3& maxy) : min{minx, miny}, max{maxx, maxy} {
//			sx::assert_invariant(*this);
//		}
//		template<typename U0, typename U1, typename U2, typename U3, typename U4, typename U5> explicit bounds (const U0& minx, const U1& miny, const U2& minz, const U3& maxx, const U4& maxy, const U5& maxz) : min(minx, miny, minz), max(maxx, maxy, maxz) {
//			sx::assert_invariant(*this);
//		}
		#if SXAPI_COCOA || SXAPI_COCOA_TOUCH
			explicit bounds (const CGRect& r) : min(r.origin.x, r.origin.y), max(r.origin.x+r.size.width, r.origin.y+r.size.height) {
				sx::assert_invariant(*this);
			}
			explicit operator CGRect () const {
				return CGRectMake(min.x, min.y, max.x-min.x, max.y-min.y);
			}
		#endif
//		#if SXAPI_COCOA && !(SX64)
//			explicit bounds (const NSRect& r) : min(r.origin.x, r.origin.y), max(r.origin.x+r.size.width, r.origin.y+r.size.height) {
//				sx::assert_invariant(*this);
//			}
//		#endif
		#if	SXAPI_WIN32
			explicit bounds (const RECT& r) : min(r.left, r.top), max(r.right, r.bottom) {
				sx::assert_invariant(*this);
			}
			explicit operator RECT () const {
				return { LONG(min.x), LONG(min.y), LONG(max.x), LONG(max.y) };
			}
			explicit bounds (const Gdiplus::Rect& r) : min{r.X,r.Y}, max{r.X+r.Width,r.Y+r.Height} {
			}
			explicit operator Gdiplus::Rect () const {
				return Gdiplus::Rect(min.x, min.y, size().x, size().y);
			}
		#endif
		bounds& operator= (const bounds&) SXLREF = default;

		template<typename U> operator bounds<U> () const {
			return bounds<U>{ min == std::numeric_limits<T>::max() ? std::numeric_limits<U>::max() : sx::cast<U>(min), max == std::numeric_limits<T>::lowest() ? std::numeric_limits<U>::lowest() : sx::cast<U>(max) };
		}

		bounds operator| (const T& t) const {
			if (empty()) return bounds(t, t);
			return bounds(sx::min(min,t), sx::max(max,t));
		}
		bounds& operator|= (const T& b) {
			if (!empty()) {
				this->min = sx::min(this->min, b);
				this->max = sx::max(this->max, b);
			}
			else {
				min = max = b;
			}
			return *this;
		}
		void inset (const T& t) {
			min += t;
			max -= t;
		}
		const T& operator[] (bool i) const	{ return (i ? max : min); }
		T& operator[] (bool i)				{ return (i ? max : min); }
		void assert_invariant () const {
			sx::assert_invariant(min);
			sx::assert_invariant(max);
		}

		static bounds max_bounds () { return bounds(std::numeric_limits<T>::lowest(), std::numeric_limits<T>::max()); }
		template<typename U> bounds operator* (const U& u) const {
			// #9335 -> mingled with another "operator*" -> recalculate_bounds_with_mat
			static_assert(!(std::is_same<sx::ivec3, T>::value && std::is_same<sx::mat<float, 4>, U>::value), "U must be simpler type.");
			static_assert(!(std::is_same<sx::vec3, T>::value && std::is_same<sx::mat<float, 4>, U>::value), "U must be simpler type.");
			static_assert(!(std::is_same<sx::vec<double,3>, T>::value && std::is_same<sx::mat<float, 4>, U>::value), "U must be simpler type.");
			static_assert(std::is_same<float, U>::value ||
				std::is_same<sx::mat4, U>::value ||
				std::is_same<sx::vec2, U>::value,
				"U must be simpler type.");
			if (!empty()) return bounds(min * u, max * u);
			return *this;
		}
		template<typename U> bounds& operator*= (const U& u) {
			static_assert(!(std::is_same<sx::ivec3, T>::value && std::is_same<sx::mat<float, 4>, U>::value), "U must be simpler type.");
			static_assert(!(std::is_same<sx::vec3, T>::value && std::is_same<sx::mat<float, 4>, U>::value), "U must be simpler type.");
			static_assert(!(std::is_same<sx::vec<double,3>, T>::value && std::is_same<sx::mat<float, 4>, U>::value), "U must be simpler type.");
			static_assert(std::is_same<float, U>::value ||
				std::is_same<sx::mat4, U>::value ||
				std::is_same<sx::vec2, U>::value,
				"U must be simpler type.");
			if (!empty()) { min *= u; max *= u; }
			return *this;
		}
		template<typename U> bounds operator/ (const U& u) const {
			static_assert(!(std::is_same<sx::ivec3, T>::value && std::is_same<sx::mat<float, 4>, U>::value), "U must be simpler type.");
			static_assert(!(std::is_same<sx::vec3, T>::value && std::is_same<sx::mat<float, 4>, U>::value), "U must be simpler type.");
			static_assert(!(std::is_same<sx::vec<double,3>, T>::value && std::is_same<sx::mat<float, 4>, U>::value), "U must be simpler type.");
			static_assert(std::is_same<float, U>::value ||
				std::is_same<sx::mat4, U>::value ||
				std::is_same<sx::vec2, U>::value,
				"U must be simpler type.");
			if (!empty()) return bounds(min / u, max / u);
			return *this;
		}
		template<typename U> bounds& operator/= (const U& u) {
			static_assert(!(std::is_same<sx::ivec3, T>::value && std::is_same<sx::mat<float, 4>, U>::value), "U must be simpler type.");
			static_assert(!(std::is_same<sx::vec3, T>::value && std::is_same<sx::mat<float, 4>, U>::value), "U must be simpler type.");
			static_assert(!(std::is_same<sx::vec<double,3>, T>::value && std::is_same<sx::mat<float, 4>, U>::value), "U must be simpler type.");
			static_assert(std::is_same<float, U>::value ||
				std::is_same<sx::mat4, U>::value ||
				std::is_same<sx::vec2, U>::value,
				"U must be simpler type.");
			if (!empty()) { min /= u; max /= u; }
			return *this;
		}
		bounds operator+ (const T& t) const {
			if (!empty()) return bounds(min + t, max + t);
			return *this;
		}
		bounds& operator+= (const T& t) {
			if (!empty()) { min += t; max += t; }
			return *this;
		}
		bounds operator- (const T& t) const {
			if (!empty()) return bounds(min - t, max - t);
			return *this;
		}
		bounds& operator-= (const T& t) {
			if (!empty()) { min -= t; max -= t; }
			return *this;
		}
		bool operator== (const bounds& b) const {
			if (empty() && b.empty()) return true;
			return (min == b.min) && (max == b.max);
		}
		bool operator!= (const bounds& b) const {
			return !(*this == b);
		}
		bounds operator& (const bounds& b) const {
			if (empty() || b.empty()) return bounds();
			return bounds(sx::max(min, b.min), sx::min(max, b.max));
		}
		bounds& operator&= (const bounds& b) {
			(*this) = (*this) & b;
			return *this;
		}
		bounds operator| (const bounds& b) const {
			if (empty()) return sx::cast<bounds>(b);
			if (b.empty()) return *this;
			return bounds(sx::min(min, b.min), sx::max(max, b.max));
		}
		bounds& operator|= (const bounds& b) {
			(*this) = (*this) | b;
			return *this;
		}
	};
	template<typename T> inline T center (const bounds<T>& b) {
		SXASSERT(!b.empty());
		return ((b.min + b.max) / 2);
	}

	template<> class bounds<sx::mm::vector> : public sx::object {
	public: // properties
		const sx::mm::vector& origin () const {
			return min;
		}
		void origin (const sx::mm::vector& origin) {
			const sx::mm::vector s = (max - min);
			min = origin;
			max = origin + s;
		}
		sx::mm::vector size () const {
			return (max - min);
		}
		void size (const sx::mm::vector& size) {
			max = min + size;
		}
		sx::mm::vector center () const {
			return (max + min) * sx::mm::make_vector(0.5f);
		}
		bool empty () const {
			return !(sx::lemask(min, max) == 0x0f);
		}
	public:
		 sx::mm::vector min;
		 sx::mm::vector max;

		explicit bounds () : min(sx::mm::vmax), max(sx::mm::vlowest) { }
		bounds (const bounds&) = default;
		template<typename U> bounds (const bounds<U>& c) : min(c.min), max(c.max) { }
		explicit bounds (const sx::mm::vector& min, const sx::mm::vector& max) : min(min), max(max) {
			sx::assert_invariant(*this);
		}
		explicit bounds (const sx::mm::vector& max) : min(sx::mm::vzero), max(max) {
			sx::assert_invariant(*this);
		}
		bounds& operator= (const bounds&) SXLREF = default;

		template<typename U> operator bounds<U> () const {
			return bounds<U>{ sx::cast<U>(min), sx::cast<U>(max) };
		}

		bounds& operator|= (const sx::mm::vector& b) {
			if (!empty()) {
				this->min = sx::min(this->min, b);
				this->max = sx::max(this->max, b);
			}
			else {
				min = max = b;
			}
			return *this;
		}
		bounds& operator|= (const bounds& b) {
			if (!b.empty()) {
				if (!empty()) {
					this->min = sx::min(this->min, b.min);
					this->max = sx::max(this->max, b.max);
				}
				else {
					*this = b;
				}
			}
			return *this;
		}
		void inset (const sx::mm::vector& d) {
			min = (min + d);
			max = (max - d);
		}
		void assert_invariant () const {
			sx::assert_invariant(min);
			sx::assert_invariant(max);
		}

		static bounds empty_bounds () { return bounds(); }
		static bounds max_bounds () { return bounds(sx::mm::vlowest, sx::mm::vmax); }
	};

	template<> class bounds<sx::mm::vec3> : public sx::object {
	public: // properties
		const sx::mm::vec3& origin () const {
			return min;
		}
		void origin (const sx::mm::vec3& origin) {
			const sx::mm::vec3 s = max - min;
			min = origin;
			max = origin + s;
		}
		sx::mm::vec3 size () const {
			return (max - min);
		}
		void size (const sx::mm::vec3& size) {
			max = min + size;
		}
		sx::mm::vec3 center () const {
			return (max + min) * sx::cast<sx::mm::vec3>(0.5f);
		}
		bool empty () const {
			return !(min <= max);
		}
	public:
		 sx::mm::vec3 min;
		 sx::mm::vec3 max;

		explicit bounds () : min(std::numeric_limits<sx::mm::vec3>::max()), max(std::numeric_limits<sx::mm::vec3>::lowest()) { }
		explicit bounds (const sx::mm::vec3& min, const sx::mm::vec3& max) : min(min), max(max) {
			sx::assert_invariant(*this);
		}
		explicit bounds (const sx::mm::vec3& max) : min(sx::cast<sx::mm::vec3>(0.0f)), max(max) {
			sx::assert_invariant(*this);
		}
		template<typename U> explicit bounds (U minx, U miny, U maxx, U maxy) : min(minx, miny), max(maxx, maxy) {
			sx::assert_invariant(*this);
		}

		template<typename U> operator bounds<U> () const {
			return bounds<U>{ min == std::numeric_limits<sx::mm::vec3>::max() ? std::numeric_limits<U>::max() : sx::cast<U>(min), max == std::numeric_limits<sx::mm::vec3>::lowest() ? std::numeric_limits<U>::lowest() : sx::cast<U>(max) };
		}

		bounds& operator|= (const sx::mm::vec3& b) {
			if (!empty()) {
				this->min.x = sx::min(this->min.x, b.x);
				this->min.y = sx::min(this->min.y, b.y);
				this->min.z = sx::min(this->min.z, b.z);
				this->max.x = sx::max(this->max.x, b.x);
				this->max.y = sx::max(this->max.y, b.y);
				this->max.z = sx::max(this->max.z, b.z);
			}
			else {
				min = max = b;
			}
			return *this;
		}
		bounds& operator|= (const bounds& b) {
			if (!b.empty()) {
				if (!empty()) {
					this->min.x = sx::min(this->min.x, b.min.x);
					this->min.y = sx::min(this->min.y, b.min.y);
					this->min.z = sx::min(this->min.z, b.min.z);
					this->max.x = sx::max(this->max.x, b.max.x);
					this->max.y = sx::max(this->max.y, b.max.y);
					this->max.z = sx::max(this->max.z, b.max.z);
				}
				else {
					*this = b;
				}
			}
			return *this;
		}
		void inset (const sx::mm::vec3& d) {
			min += d;
			max -= d;
		}
		const sx::mm::vec3& operator[] (bool i) const	{ return (i ? max : min); }
		sx::mm::vec3& operator[] (bool i)				{ return (i ? max : min); }
		void assert_invariant () const {
			sx::assert_invariant(min);
			sx::assert_invariant(max);
		}

		static bounds empty_bounds () { return bounds(); }
		static bounds max_bounds () { return bounds(std::numeric_limits<sx::mm::vec3>::lowest(), std::numeric_limits<sx::mm::vec3>::max()); }
	};
}

// This function is dangerous because		B * M * M != B * (M * M)
template<typename T> sx::bounds<sx::vec<T,3>> recalculate_bounds_with_mat (const sx::bounds<sx::vec<T,3>>& b, const sx::mat4& t) {
	sx::bounds<sx::vec<T,3>> b2;
	if (!b.empty()) {
		b2 |= (sx::vec<T,3>(b.min.x, b.min.y, b.min.z) * t);
		b2 |= (sx::vec<T,3>(b.min.x, b.min.y, b.max.z) * t);
		b2 |= (sx::vec<T,3>(b.min.x, b.max.y, b.min.z) * t);
		b2 |= (sx::vec<T,3>(b.min.x, b.max.y, b.max.z) * t);
		b2 |= (sx::vec<T,3>(b.max.x, b.min.y, b.min.z) * t);
		b2 |= (sx::vec<T,3>(b.max.x, b.min.y, b.max.z) * t);
		b2 |= (sx::vec<T,3>(b.max.x, b.max.y, b.min.z) * t);
		b2 |= (sx::vec<T,3>(b.max.x, b.max.y, b.max.z) * t);
	}
	return b2;
}

namespace sx {

	template<typename T> inline bool has_area (const sx::bounds<sx::vec<T,2>>& b) {
		return (!b.empty() && b.size().x > 0 && b.size().y > 0);
		// !b.empty() is need to avoid special cases. (min -> max : MAX_INT -> MIN_INT)
	}

	//template<int N> inline bool intersect (const sx::bounds<sx::vec<float,N>>& a, const sx::bounds<sx::vec<float,N>>& b) {
	//	if (a.empty() || b.empty()) return false;
	//	for (int i = 0; i < N; ++i) {
	//		if (a.max[i] < b.min[i]) return false;
	//		if (b.max[i] < a.min[i]) return false;
	//	}
	//	return true;
	//}
	//template<int N> inline bool intersect (const sx::bounds<sx::vec<int,N>>& a, const sx::bounds<sx::vec<int,N>>& b) {
	//	if (a.empty() || b.empty()) return false;
	//	for (int i = 0; i < N; ++i) {
	//		if (a.max[i] <= b.min[i]) return false;
	//		if (b.max[i] <= a.min[i]) return false;
	//	}
	//	return true;
	//}
	template<typename T, typename U, int N> inline bool intersect (const sx::bounds<sx::vec<T,N>>& a, const sx::bounds<sx::vec<U,N>>& b) {
		if (a.empty() || b.empty()) return false;
		for (int i = 0; i < N; ++i) {
			if (a.max[i] < b.min[i]) return false;
			if (b.max[i] < a.min[i]) return false;
		}
		return true;
	}
	template<typename T, int N> inline bool intersect (const sx::bounds<sx::vec<float,N>>& a, const sx::vec<T,N>& b) {
		if (a.empty()) return false;
		return (a.min <= b) && (b <= a.max);
	}
	template<typename T, int N> inline bool intersect (const sx::vec<T,N>& a, const sx::bounds<sx::vec<float,N>>& b) {
		if (b.empty()) return false;
		return (b.min <= a) && (a <= b.max);
	}
	template<typename T, int N> inline bool intersect (const sx::bounds<sx::vec<int,N>>& a, const sx::vec<T,N>& b) {
		if (a.empty()) return false;
		return (a.min <= b) && (b < (a.max));
	}
	template<typename T, typename U> inline bool is_in (const sx::bounds<T>& a, const sx::bounds<U>& b) {
		if (a.empty()) return true;
		if (b.empty()) return false;
		return ((a.min <= b.min) && (b.max <= a.max));
	}

//	#if SXAPI_COCOA || SXAPI_COCOA_TOUCH
//		template<typename T> CGRect CGRectt (const sx::bounds<sx::vec<T,2>>& b) {
//			return CGRectMake(b.min.x, b.min.y, b.size().x, b.size().y);
//		}
//	#endif
//	#if SXAPI_COCOA
//		template<typename T> NSRect NSRectt (const sx::bounds<sx::vec<T,2>>& b) {
//			return NSMakeRect(b.min.x, b.min.y, b.size().x, b.size().y);
//		}
//	#endif
	//#if SXAPI_WIN32
	//	template<typename T> RECT RECTt (const sx::bounds<sx::vec<T,2>>& b) {
	//		RECT r; r.left = b.min.x; r.top = b.min.y; r.right = b.max.x; r.bottom = b.max.y;
	//		return r;
	//	}
	//#endif
	template<typename T> const sx::bounds<T> inset (const sx::bounds<T>& b, const T& d) {
		return sx::bounds<T>(b.min + d, b.max - d);
	}

	template<typename T> sx::bounds<T>& revert_endian (bounds<T>& b) {
		sx::revert_endian(b.min);
		sx::revert_endian(b.max);
		return b;
	}

	template<typename T> bool isnan (const bounds<T>& b) {
		return (sx::isnan(b.min) || sx::isnan(b.max));
	}
	template<typename T> bool is_finite (const bounds<T>& b) {
		return (sx::isfinite(b.min) && sx::isfinite(b.max));
	}
	template<typename T> sx::bounds<T> inset (const sx::bounds<T>& b, const T& min, const T& max) {
		return sx::bounds<T>(b.min + min, b.max - max);
	}
	
	template<typename T> sx::bounds<T> round (const sx::bounds<T>& t) {
		return sx::bounds<T>(sx::round(t.min), sx::round(t.max));
	}
	template<typename T> sx::bounds<T> floor (const sx::bounds<T>& t) {
		return sx::bounds<T>(sx::floor(t.min), sx::floor(t.max));
	}
	template<typename T> sx::bounds<T> ceil (const sx::bounds<T>& t) {
		return sx::bounds<T>(sx::ceil(t.min), sx::ceil(t.max));
	}
	
	template<typename T> sx::bounds<T> fat_scale (const sx::bounds<T>& t, float fat_scale) {
		const T newMin(t.center() + (t.min - t.center()) * fat_scale);
		const T newMax(t.center() + (t.max - t.center()) * fat_scale);
		return sx::bounds<T>(newMin, newMax);
	}

	template<int N> sx::bounds<sx::vec<float,N>> operator+ (const sx::bounds<sx::vec<int,N>>& b, const sx::vec<float,N> &v) {
		return (sx::bounds<sx::vec<float,N>>(b) + v);
	}

	template<typename T> sx::bounds<T> centerize (const sx::bounds<T>& t, const sx::bounds<T>& r) {
		return t - t.min + r.min + (r.size()-t.size())/2;
	}

	using ibounds2			= bounds<ivec2>;
	using ibounds3			= bounds<ivec3>;
	using ibounds4			= bounds<ivec4>;
	using bounds2			= bounds<vec2>;
	using bounds3			= bounds<vec3>;
	using bounds4			= bounds<vec4>;
	using rectangle_class	= ibounds2;
}
template<typename T> std::ostream& operator<< (std::ostream &stream, const sx::bounds<T>& b) {
	if (!b.empty())	stream << "min:" << b.min << " " << "max:" << b.max;
	else			stream << "void";
	return stream;
}

namespace sx {
	namespace core {
		namespace unittest {
			void test_bounds ();
		}
	}
}
