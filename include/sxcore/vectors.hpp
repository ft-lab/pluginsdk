#ifdef OBSOLETE_USE_DETECTOR

	#pragma once
	#include  "sxcore/product_ope_detector.hpp"
	#pragma message("Warning: UNDEFINE OBSOLETE_USE_DETECTOR BEFORE BUILDING RELEASE.")

#else

#pragma once
SXMODULE_SXCORE
#include <cmath>
#include <limits>
#include "sxcore/debug.hpp"
#include "sxcore/math.hpp"
#include "sxcore/endian.hpp"
#include "sxcore/minmax.hpp"
#include "sxcore/float16.hpp"
#include "sxcore/vector.hpp"

#if SXSIMD
	#if __ARM_NEON__
		#include "arm_neon.h"
		namespace sx {
			namespace mm {
				using vector	= ::float32x4_t;
			}
		}
	#else // only NEON and SSE are supported now, and if not NEON, SSE is assumed.
		#include <xmmintrin.h>
		#include <pmmintrin.h>
		namespace sx {
			namespace mm {
				using vector	= ::__m128;
			}
		}
	#endif
#else
	namespace sx {
		namespace mm {
			using vector	= std::array<float,4>;
		}
	}
#endif

namespace sx {
	namespace mm {
		using scalar	= vector;
		using matrix	= std::array<vector,4>;
		
		inline bool is_aligned (const void *const p) {
			#if SXSIMD && !__ARM_NEON__
				return (std::uint64_t(p) & 0x0000000f) == 0;
			#else
				return true;
			#endif
		}
		
		union vector_union {	// use union to avoid stritct aliasing violation
			vector							m;
			std::array<sx::float32_t, 4>		f;
			std::array<std::int32_t, 4>		i;
			std::array<std::uint32_t, 4>	u;
		};

		namespace {
			const sx::mm::vector vzero{};
			const sx::mm::scalar szero{};
		}
	}

	#if SXSIMD
		static_assert(std::alignment_of<sx::mm::vector>::value == 16, "");

		#if __ARM_NEON__
			inline sx::mm::vector _mm_set_ps1 (const float f) {
				return vdupq_n_f32(f);
			}
			inline sx::mm::vector _mm_setr_ps (const float x, const float y, const float z, const float w) {
				sx::mm::vector v;
				return vsetq_lane_f32(w, vsetq_lane_f32(z, vsetq_lane_f32(y, vsetq_lane_f32(x, v, 0), 1), 2), 3);
			}
			inline sx::mm::vector _mm_set_ss (const float f) {
				return vsetq_lane_f32(f, vdupq_n_f32(0.0f), 0);
			}
			inline sx::mm::vector _mm_loadu_ps (const float *const p) {
				return _mm_setr_ps(p[0], p[1], p[2], p[3]);
			}
			inline sx::mm::vector _mm_mul_ps (const sx::mm::vector& a, const sx::mm::vector& b) {
				return vmulq_f32(a, b);
			}
			inline sx::mm::vector _mm_mul_ss (const sx::mm::vector& a, const sx::mm::vector& b) {
				return vsetq_lane_f32(vgetq_lane_f32(a,0)*vgetq_lane_f32(b,0), a, 0);
			}
			inline sx::mm::vector _mm_add_ps (const sx::mm::vector& a, const sx::mm::vector& b) {
				return vaddq_f32(a, b);
			}
			inline sx::mm::vector _mm_add_ss (const sx::mm::vector& a, const sx::mm::vector& b) {
				return vsetq_lane_f32(vgetq_lane_f32(a,0)+vgetq_lane_f32(b,0), a, 0);
			}
			inline sx::mm::vector _mm_sub_ps (const sx::mm::vector& a, const sx::mm::vector& b) {
				return vsubq_f32(a, b);
			}
			inline sx::mm::vector _mm_sub_ss (const sx::mm::vector& a, const sx::mm::vector& b) {
				return vsetq_lane_f32(vgetq_lane_f32(a,0)-vgetq_lane_f32(b,0), a, 0);
			}
			sx::mm::vector rsqrt (const sx::mm::vector& v);
			sx::mm::vector rsqrts (const sx::mm::scalar& v);
			sx::mm::vector rcp (const sx::mm::vector& a);
			sx::mm::vector rcps (const sx::mm::scalar& a);
			inline sx::mm::vector _mm_sqrt_ps (const sx::mm::vector& v_) {
				return rcp(rsqrt(v_));
			}
			inline sx::mm::vector _mm_sqrt_ss (const sx::mm::vector& v_) {
				return rcps(rsqrts(v_));
			}
			inline sx::mm::vector _mm_min_ps (const sx::mm::vector& a, const sx::mm::vector& b) {
				return vminq_f32(a, b);
			}
			inline sx::mm::vector _mm_min_ss (const sx::mm::vector& a, const sx::mm::vector& b) {
				return (vgetq_lane_f32(a,0) < vgetq_lane_f32(b,0)) ? vsetq_lane_f32(vgetq_lane_f32(a,0), a, 0) : vsetq_lane_f32(vgetq_lane_f32(b,0), a, 0);
			}

			inline sx::mm::vector _mm_max_ps (const sx::mm::vector& a, const sx::mm::vector& b) {
				return vmaxq_f32(a, b);
			}
			inline sx::mm::vector _mm_max_ss (const sx::mm::vector& a, const sx::mm::vector& b) {
				return (vgetq_lane_f32(a,0) < vgetq_lane_f32(b,0)) ? vsetq_lane_f32(vgetq_lane_f32(b,0), a, 0) : vsetq_lane_f32(vgetq_lane_f32(a,0), a, 0);
			}

			inline sx::mm::vector _mm_cmplt_ps (const sx::mm::vector& a, const sx::mm::vector& b) {
				return sx::mm::vector(vcltq_f32(a, b));
			}
			inline sx::mm::vector _mm_cmple_ps (const sx::mm::vector& a, const sx::mm::vector& b) {
				return sx::mm::vector(vcleq_f32(a, b));
			}
			inline sx::mm::vector _mm_cmpeq_ps (const sx::mm::vector& a, const sx::mm::vector& b) {
				return sx::mm::vector(vceqq_f32(a, b));
			}
			inline sx::mm::vector _mm_and_ps (const sx::mm::vector& a, const sx::mm::vector& b) {
				return sx::mm::vector(vandq_u32(uint32x4_t(a), uint32x4_t(b)));
			}
			inline sx::mm::vector _mm_or_ps (const sx::mm::vector& a, const sx::mm::vector& b) {
				return sx::mm::vector(vorrq_u32(uint32x4_t(a), uint32x4_t(b)));
			}
			inline sx::mm::vector _mm_andnot_ps (const sx::mm::vector& a, const sx::mm::vector& b) {
				return sx::mm::vector(vandq_u32(vmvnq_u32(uint32x4_t(a)), uint32x4_t(b)));
			}
			inline sx::mm::vector _mm_xor_ps (const sx::mm::vector& a, const sx::mm::vector& b) {
				return sx::mm::vector(veorq_u32(uint32x4_t(a), uint32x4_t(b)));
			}
			inline int _mm_movemask_ps (const sx::mm::vector& a) {
				return (((vgetq_lane_s32(int32x4_t(a),0) & 0x80000000) >> 31) | ((vgetq_lane_s32(int32x4_t(a),1) & 0x80000000) >> 30) | ((vgetq_lane_s32(int32x4_t(a),2) & 0x80000000) >> 29) | ((vgetq_lane_s32(int32x4_t(a),3) & 0x80000000) >> 28));
			}

			inline sx::mm::vector _mm_cmplt_ss (const sx::mm::vector& a, const sx::mm::vector& b) {
				return sx::mm::vector(vsetq_lane_u32((vgetq_lane_f32(a,0) < vgetq_lane_f32(b,0)) ? 0xffffffff : 0x00000000, int32x4_t(a), 0));
			}
			inline sx::mm::vector _mm_cmple_ss (const sx::mm::vector& a, const sx::mm::vector& b) {
				return sx::mm::vector(vsetq_lane_u32((vgetq_lane_f32(a,0) <= vgetq_lane_f32(b,0)) ? 0xffffffff : 0x00000000, int32x4_t(a), 0));
			}
			inline sx::mm::vector _mm_cmpeq_ss (const sx::mm::vector& a, const sx::mm::vector& b) {
				return sx::mm::vector(vsetq_lane_u32((vgetq_lane_f32(a,0) == vgetq_lane_f32(b,0)) ? 0xffffffff : 0x00000000, int32x4_t(a), 0));
			}
		#endif
	#else
		inline sx::mm::vector _mm_set_ps1 (const float f) {
			sx::mm::vector v; v[0] = v[1] = v[2] = v[3] = f; return v;
		}
		inline sx::mm::vector _mm_setr_ps (const float x, const float y, const float z, const float w) {
			sx::mm::vector v; v[0] = x; v[1] = y; v[2] = z; v[3] = w; return v;
		}
		inline sx::mm::vector _mm_set_ss (const float f) {
			sx::mm::vector v; v[0] = f; v[1] = v[2] = v[3] = 0.0f; return v;
		}
		inline sx::mm::vector _mm_loadu_ps (const float *const p) {
			sx::mm::vector v; v[0] = p[0]; v[1] = p[1]; v[2] = p[2]; v[3] = p[3]; return v;
		}
		inline sx::mm::vector _mm_mul_ps (const sx::mm::vector& a, const sx::mm::vector& b) {
			sx::mm::vector v; for (int i = 0; i < 4; ++i) v[i] = a[i] * b[i]; return v;
		}
		inline sx::mm::vector _mm_mul_ss (const sx::mm::vector& a, const sx::mm::vector& b) {
			sx::mm::scalar v; v[0] = a[0] * b[0]; for (int i = 1; i < 4; ++i) v[i] = a[i]; return v;
		}
		inline sx::mm::vector _mm_div_ps (const sx::mm::vector& a, const sx::mm::vector& b) {
			sx::mm::vector v; for (int i = 0; i < 4; ++i) v[i] = a[i] / b[i]; return v;
		}
		inline sx::mm::vector _mm_div_ss (const sx::mm::vector& a, const sx::mm::vector& b) {
			sx::mm::scalar v; v[0] = a[0] / b[0]; for (int i = 1; i < 4; ++i) v[i] = a[i]; return v;
		}
		inline sx::mm::vector _mm_add_ps (const sx::mm::vector& a, const sx::mm::vector& b) {
			sx::mm::vector v; for (int i = 0; i < 4; ++i) v[i] = a[i] + b[i]; return v;
		}
		inline sx::mm::vector _mm_add_ss (const sx::mm::vector& a, const sx::mm::vector& b) {
			sx::mm::scalar v; v[0] = a[0] + b[0]; for (int i = 1; i < 4; ++i) v[i] = a[i]; return v;
		}
		inline sx::mm::vector _mm_sub_ps (const sx::mm::vector& a, const sx::mm::vector& b) {
			sx::mm::vector v; for (int i = 0; i < 4; ++i) v[i] = a[i] - b[i]; return v;
		}
		inline sx::mm::vector _mm_sub_ss (const sx::mm::vector& a, const sx::mm::vector& b) {
			sx::mm::scalar v; v[0] = a[0] - b[0]; for (int i = 1; i < 4; ++i) v[i] = a[i]; return v;
		}
		inline sx::mm::vector _mm_sqrt_ps (const sx::mm::vector& v) {
			sx::mm::vector u; for (int i = 0; i < 4; ++i) u[i] = std::sqrt(v[i]); return u;
		}
		inline sx::mm::vector _mm_sqrt_ss (const sx::mm::vector& v) {
			sx::mm::vector u; u[0] = std::sqrt(v[0]); for (int i = 1; i < 4; ++i) u[i] = v[i]; return u;
		}
		inline sx::mm::vector _mm_min_ps (const sx::mm::vector& a, const sx::mm::vector& b) {
			sx::mm::vector u; for (int i = 0; i < 4; ++i) u[i] = std::min(a[i], b[i]); return u;
		}
		inline sx::mm::vector _mm_min_ss (const sx::mm::vector& a, const sx::mm::vector& b) {
			sx::mm::vector u; u[0] = std::min(a[0], b[0]); for (int i = 1; i < 4; ++i) u[i] = a[i]; return u;
		}
		inline sx::mm::vector _mm_max_ps (const sx::mm::vector& a, const sx::mm::vector& b) {
			sx::mm::vector u; for (int i = 0; i < 4; ++i) u[i] = std::max(a[i], b[i]); return u;
		}
		inline sx::mm::vector _mm_max_ss (const sx::mm::vector& a, const sx::mm::vector& b) {
			sx::mm::vector u; u[0] = std::max(a[0], b[0]); for (int i = 1; i < 4; ++i) u[i] = a[i]; return u;
		}
		inline sx::mm::vector _mm_cmplt_ps (const sx::mm::vector& a, const sx::mm::vector& b) {
			sx::mm::vector_union u;
			for (int i = 0; i < 4; ++i) u.i[i] = ((a[i]<b[i]) ? 0xffffffff : 0x0);
			return u.m;
		}
		inline sx::mm::vector _mm_cmple_ps (const sx::mm::vector& a, const sx::mm::vector& b) {
			sx::mm::vector_union u;
			for (int i = 0; i < 4; ++i) u.i[i] = ((a[i]<=b[i]) ? 0xffffffff : 0x0);
			return u.m;
		}
		inline sx::mm::vector _mm_cmpeq_ps (const sx::mm::vector& a, const sx::mm::vector& b) {
			sx::mm::vector_union u;
			for (int i = 0; i < 4; ++i) u.i[i] = ((a[i]==b[i]) ? 0xffffffff : 0x0);
			return u.m;
		}
		inline sx::mm::vector _mm_and_ps (const sx::mm::vector& a_, const sx::mm::vector& b_) {
			sx::mm::vector_union a; a.m = a_;
			sx::mm::vector_union b; b.m = b_;
			sx::mm::vector_union u;
			for (int i = 0; i < 4; ++i) u.i[i] = (a.i[i] & b.i[i]);
			return u.m;
		}
		inline sx::mm::vector _mm_or_ps (const sx::mm::vector& a_, const sx::mm::vector& b_) {
			sx::mm::vector_union a; a.m = a_;
			sx::mm::vector_union b; b.m = b_;
			sx::mm::vector_union u;
			for (int i = 0; i < 4; ++i) u.i[i] = (a.i[i] | b.i[i]);
			return u.m;
		}
		inline sx::mm::vector _mm_andnot_ps (const sx::mm::vector& a_, const sx::mm::vector& b_) {
			sx::mm::vector_union a; a.m = a_;
			sx::mm::vector_union b; b.m = b_;
			sx::mm::vector_union u;
			for (int i = 0; i < 4; ++i) u.i[i] = (~(a.i[i]) & b.i[i]);
			return u.m;
		}
		inline sx::mm::vector _mm_xor_ps (const sx::mm::vector& a_, const sx::mm::vector& b_) {
			sx::mm::vector_union a; a.m = a_;
			sx::mm::vector_union b; b.m = b_;
			sx::mm::vector_union u;
			for (int i = 0; i < 4; ++i) u.i[i] = (a.i[i] ^ b.i[i]);
			return u.m;
		}
		inline int _mm_movemask_ps (const sx::mm::vector& a) {
			sx::mm::vector_union u;
			u.m = a;
			return (((u.i[0] & 0x80000000) >> 31) | ((u.i[1] & 0x80000000) >> 30) | ((u.i[2] & 0x80000000) >> 29) | ((u.i[3] & 0x80000000) >> 28));
		}

		inline sx::mm::vector _mm_cmplt_ss (const sx::mm::vector& a, const sx::mm::vector& b) {
			sx::mm::vector_union u;
			u.m = a;
			u.i[0] = ((a[0]<b[0]) ? 0xffffffff : 0x0);
			return u.m;
		}
		inline sx::mm::vector _mm_cmple_ss (const sx::mm::vector& a, const sx::mm::vector& b) {
			sx::mm::vector_union u;
			u.m = a;
			u.i[0] = ((a[0]<=b[0]) ? 0xffffffff : 0x0);
			return u.m;
		}
		inline sx::mm::vector _mm_cmpeq_ss (const sx::mm::vector& a, const sx::mm::vector& b) {
			sx::mm::vector_union u;
			u.m = a;
			u.i[0] = ((a[0]==b[0]) ? 0xffffffff : 0x0);
			return u.m;
		}
		inline bool lts (const sx::mm::scalar& a, const sx::mm::scalar& b) {
			return (a[0] < b[0]);
		}
	#endif

	inline float* floatp (sx::mm::vector& v) {
		return reinterpret_cast<float*>(&v);
	}
	inline const float* floatp (const sx::mm::vector& v) {
		return reinterpret_cast<const float*>(&v);
	}

	inline float* floatp (sx::mm::matrix& m) {
		return reinterpret_cast<float*>(&m);
	}
	inline const float* floatp (const sx::mm::matrix& m) {
		return reinterpret_cast<const float*>(&m);
	}

	inline void check_invariant (const sx::mm::vector& v) { SXASSERT(sx::mm::is_aligned(&v)); }
	inline bool check2 (const sx::mm::vector& v) { return (sx::mm::is_aligned(&v) && (reinterpret_cast<const float*>(&v)[2] == 0.0f) && (reinterpret_cast<const float*>(&v)[3] == 0.0f)); }
	inline bool check3 (const sx::mm::vector& v) { return (sx::mm::is_aligned(&v) && reinterpret_cast<const float*>(&v)[3] == 0.0f); }
}

#if (__clang__ && SXSIMD)
	// operators are supported as language extension
#else
	inline sx::mm::vector operator+ (const sx::mm::vector& a, const sx::mm::vector& b) {
		using namespace sx;
		return _mm_add_ps(a, b);
	}
	inline sx::mm::vector& operator+= (sx::mm::vector& a, const sx::mm::vector& b) {
		using namespace sx;
		return (a = _mm_add_ps(a, b));
	}
	inline sx::mm::vector operator- (const sx::mm::vector& a, const sx::mm::vector& b) {
		using namespace sx;
		return _mm_sub_ps(a, b);
	}
	inline sx::mm::vector& operator-= (sx::mm::vector& a, const sx::mm::vector& b) {
		using namespace sx;
		return (a = _mm_sub_ps(a, b));
	}
	inline sx::mm::vector operator- (const sx::mm::vector& a) {
		using namespace sx;
		return _mm_sub_ps(sx::mm::vector{}, a);
	}
	inline sx::mm::vector operator* (const sx::mm::vector& a, const sx::mm::vector& b) {
		using namespace sx;
		return _mm_mul_ps(a, b);
	}
#if __clang__
namespace sx {
	inline sx::mm::vector operator* (const sx::mm::vector& a, const sx::mm::vector& b) {
		using namespace sx;
		return _mm_mul_ps(a, b);
	}
}
#endif
	inline sx::mm::vector& operator*= (sx::mm::vector& a, const sx::mm::vector& b) {
		using namespace sx;
		return (a = _mm_mul_ps(a, b));
	}
	inline sx::mm::vector operator/ (const sx::mm::vector& a, const sx::mm::vector& b) {
		using namespace sx;
		return _mm_div_ps(a, b);
	}
	inline sx::mm::vector& operator/= (sx::mm::vector& a, const sx::mm::vector& b) {
		using namespace sx;
		return (a = _mm_div_ps(a, b));
	}
#endif

namespace sx {

	enum { RGB, BGR, RGBA, BGRA, ARGB };

	template<typename T> class default_rgb_component_ordering {
	public:
		enum { ordering = RGB };
	};
	template<> class default_rgb_component_ordering<std::uint8_t> {
	public:
		#if SXOS_OSX
			enum { ordering = RGB };
		#elif SXOS_WINDOWS
			enum { ordering = BGR };
		#else
			enum { ordering = RGB };
		#endif
	};
	template<typename T> class default_rgba_component_ordering {
	public:
		enum { ordering = RGBA };
	};
	template<> class default_rgba_component_ordering<std::uint8_t> {
	public:
		#if SXOS_OSX
			enum { ordering = ARGB };
		#elif SXOS_WINDOWS
			enum { ordering = BGRA };
		#else
			enum { ordering = RGBA };
		#endif
	};

	template<typename T = float, int K = default_rgb_component_ordering<T>::ordering>	class rgb_color;
	template<typename T = float, int K = default_rgba_component_ordering<T>::ordering>	class rgba_color;

	namespace imp {
		template<typename T> T			one ();
		template<> inline std::uint8_t	one ()	{ return 255; }
		template<> inline sx::float16_t	one ()	{ return sx::float16_t(1.0f); }
		template<> inline float			one ()	{ return 1.0f; }
		template<> inline double		one ()	{ return 1.0; }

		template<typename T, int K> class color;
		template<typename T> class color<T,RGB> {
		public:
			static const std::size_t N = 3;
			T red{}, green{}, blue{};
			color ()								= default;
			color (const color&)					= default;
			color& operator= (const color&) SXLREF	= default;
			explicit color (const T& red, const T& green, const T& blue) : red(red), green(green), blue(blue) { }
		};
		template<typename T> class color<T,BGR> {
		public:
			static const std::size_t N = 3;
			T blue{}, green{}, red{};
			color ()								= default;
			color (const color&)					= default;
			color& operator= (const color&) SXLREF	= default;
			explicit color (const T& red, const T& green, const T& blue) : red(red), green(green), blue(blue) { }
		};
		template<typename T> class color<T,RGBA> {
		public:
			static const std::size_t N = 4;
			T red{}, green{}, blue{}, alpha{};
			color ()								= default;
			color (const color&)					= default;
			color& operator= (const color&) SXLREF	= default;
			explicit color (const T& red, const T& green, const T& blue, const T& alpha) : red(red), green(green), blue(blue), alpha(alpha) { }
			operator const rgb_color<T,RGB>& () const { return *reinterpret_cast<const rgb_color<T,RGB>*>(this); }
		};
		template<typename T> class color<T,BGRA> {
		public:
			static const std::size_t N = 4;
			T blue{}, green{}, red{}, alpha{};
			color ()								= default;
			color (const color&)					= default;
			color& operator= (const color&) SXLREF	= default;
			explicit color (const T& red, const T& green, const T& blue, const T& alpha) : red(red), green(green), blue(blue), alpha(alpha) { }
		};
		template<typename T> class color<T,ARGB> {
		public:
			static const std::size_t N = 4;
			T alpha{}, red{}, green{}, blue{};
			color ()								= default;
			color (const color&)					= default;
			color& operator= (const color&) SXLREF	= default;
			explicit color (const T& red, const T& green, const T& blue, const T& alpha) : red(red), green(green), blue(blue), alpha(alpha) { }
		};
	}

	template<typename T, int K_> class rgb_color : public imp::color<T,K_> { using base = imp::color<T,K_>;
		static_assert(K_ == RGB || K_ == BGR, "");
	public:
		rgb_color ()									= default;
		rgb_color (const rgb_color&)					= default;
		rgb_color& operator= (const rgb_color&) SXLREF	= default;

		rgb_color (const T& red, const T& green, const T& blue) : base(red, green, blue) { }

		rgb_color		operator- () const				{ return rgb_color(-base::red, -base::green, -base::blue); }
		// rgb_color		operator+ (const T& t) const	{ return rgb_color(base::red+t, base::green+t, base::blue+t); }
		// rgb_color		operator- (const T& t) const	{ return rgb_color(base::red-t, base::green-t, base::blue-t); }
		rgb_color		operator* (const T& t) const	{ return rgb_color(base::red*t, base::green*t, base::blue*t); }
		rgb_color		operator/ (const T& t) const	{ return rgb_color(base::red/t, base::green/t, base::blue/t); }
		// rgb_color&	operator+= (const T& t)			{ base::red+=t; base::green+=t; base::blue+=t; return *this; }
		// rgb_color&	operator-= (const T& t)			{ base::red-=t; base::green-=t; base::blue-=t; return *this; }
		rgb_color&	operator*= (const T& t)			{ base::red*=t; base::green*=t; base::blue*=t; return *this; }
		rgb_color&	operator/= (const T& t)			{ base::red/=t; base::green/=t; base::blue/=t; return *this; }

		template<int K> rgb_color		operator+ (const rgb_color<T,K>& c) const		{ return rgb_color(base::red+c.red, base::green+c.green, base::blue+c.blue); }
		template<int K> rgb_color		operator- (const rgb_color<T,K>& c) const		{ return rgb_color(base::red-c.red, base::green-c.green, base::blue-c.blue); }
		template<int K> rgb_color		operator* (const rgb_color<T,K>& c) const		{ return rgb_color(base::red*c.red, base::green*c.green, base::blue*c.blue); }
		template<int K> rgb_color		operator/ (const rgb_color<T,K>& c) const		{ return rgb_color(base::red/c.red, base::green/c.green, base::blue/c.blue); }
		template<int K> rgb_color&	operator+= (const rgb_color<T,K>& c)			{ base::red+=c.red; base::green+=c.green; base::blue+=c.blue; return *this; }
		template<int K> rgb_color&	operator-= (const rgb_color<T,K>& c)			{ base::red-=c.red; base::green-=c.green; base::blue-=c.blue; return *this; }
		template<int K> rgb_color&	operator*= (const rgb_color<T,K>& c)			{ base::red*=c.red; base::green*=c.green; base::blue*=c.blue; return *this; }
		template<int K> rgb_color&	operator/= (const rgb_color<T,K>& c)			{ base::red/=c.red; base::green/=c.green; base::blue/=c.blue; return *this; }
		template<int K> bool	operator== (const rgb_color<T,K>& c) const	{ return ((base::red==c.red) && (base::green==c.green) && (base::blue==c.blue)); }
		template<int K> bool	operator!= (const rgb_color<T,K>& c) const	{ return !((*this) == c); }
		template<int K> bool	operator< (const rgb_color<T,K>& c) const		{ return ((base::red<c.red) && (base::green<c.green) && (base::blue<c.blue)); }
		template<int K> bool	operator<= (const rgb_color<T,K>& c) const	{ return ((base::red<=c.red) && (base::green<=c.green) && (base::blue<=c.blue)); }
		template<int K> bool	operator> (const rgb_color<T,K>& c) const		{ return ((base::red>c.red) && (base::green>c.green) && (base::blue>c.blue)); }
		template<int K> bool	operator>= (const rgb_color<T,K>& c) const	{ return ((base::red>=c.red) && (base::green>=c.green) && (base::blue>=c.blue)); }

		void assert_invariant () const {
			sx::assert_invariant(base::red, base::green,base::blue);
		}
	};

	template<typename T, int K_> class rgba_color : public imp::color<T,K_> { using base = imp::color<T,K_>;
		static_assert(K_ == RGBA || K_ == BGRA || K_ == ARGB, "");
	public:
		rgba_color ()									= default;
		rgba_color (const rgba_color&)						= default;
		rgba_color& operator= (const rgba_color&) SXLREF	= default;

		rgba_color (const T& red, const T& green, const T& blue, const T& alpha = imp::one<T>()) : base(red, green, blue, alpha) { }
		template<int L> rgba_color (const rgb_color<T,L>& c) : base(c.red, c.green, c.blue, sx::imp::one<T>()) { }
		template<int L> rgba_color (const rgb_color<T,L>& c, const T& alpha) : base(c.red, c.green, c.blue, alpha) { }
		template<int L> rgba_color (const rgba_color<T,L>& c, const T& alpha) : base(c.red, c.green, c.blue, alpha) { }

		rgba_color	operator- () const				{ return rgba_color(-base::red, -base::green, -base::blue, -base::alpha); }
		// rgba_color	operator+ (const T& t) const	{ return rgba_color(base::red+t, base::green+t, base::blue+t, base::alpha+t); }
		// rgba_color	operator- (const T& t) const	{ return rgba_color(base::red-t, base::green-t, base::blue-t, base::alpha-t); }
		rgba_color	operator* (const T& t) const	{ return rgba_color(base::red*t, base::green*t, base::blue*t, base::alpha*t); }
		rgba_color	operator/ (const T& t) const	{ return rgba_color(base::red/t, base::green/t, base::blue/t, base::alpha/t); }
		// rgba_color&	operator+= (const T& t)			{ base::red+=t; base::green+=t; base::blue+=t; base::alpha+=t; return *this; }
		// rgba_color&	operator-= (const T& t)			{ base::red-=t; base::green-=t; base::blue-=t; base::alpha-=t; return *this; }
		rgba_color&	operator*= (const T& t)			{ base::red*=t; base::green*=t; base::blue*=t; base::alpha*=t; return *this; }
		rgba_color&	operator/= (const T& t)			{ base::red/=t; base::green/=t; base::blue/=t; base::alpha/=t; return *this; }

		template<int K> rgba_color	operator+ (const rgba_color<T,K>& c) const	{ return rgba_color(base::red+c.red, base::green+c.green, base::blue+c.blue, base::alpha+c.alpha); }
		template<int K> rgba_color	operator- (const rgba_color<T,K>& c) const	{ return rgba_color(base::red-c.red, base::green-c.green, base::blue-c.blue, base::alpha-c.alpha); }
		template<int K> rgba_color	operator* (const rgba_color<T,K>& c) const	{ return rgba_color(base::red*c.red, base::green*c.green, base::blue*c.blue, base::alpha*c.alpha); }
		template<int K> rgba_color	operator/ (const rgba_color<T,K>& c) const	{ return rgba_color(base::red/c.red, base::green/c.green, base::blue/c.blue, base::alpha/c.alpha); }
		template<int K> rgba_color&	operator+= (const rgba_color<T,K>& c)			{ base::red+=c.red; base::green+=c.green; base::blue+=c.blue; base::alpha+=c.alpha; return *this; }
		template<int K> rgba_color&	operator-= (const rgba_color<T,K>& c)			{ base::red-=c.red; base::green-=c.green; base::blue-=c.blue; base::alpha-=c.alpha; return *this; }
		template<int K> rgba_color&	operator*= (const rgba_color<T,K>& c)			{ base::red*=c.red; base::green*=c.green; base::blue*=c.blue; base::alpha*=c.alpha; return *this; }
		template<int K> rgba_color&	operator/= (const rgba_color<T,K>& c)			{ base::red/=c.red; base::green/=c.green; base::blue/=c.blue; base::alpha/=c.alpha; return *this; }
		template<int K> bool	operator== (const rgba_color<T,K>& c) const	{ return ((base::red==c.red) && (base::green==c.green) && (base::blue==c.blue) && (base::alpha==c.alpha)); }
		template<int K> bool	operator!= (const rgba_color<T,K>& c) const	{ return !((*this) == c); }
		template<int K> bool	operator< (const rgba_color<T,K>& c) const	{ return ((base::red<c.red) && (base::green<c.green) && (base::blue<c.blue) && (base::alpha<c.alpha)); }
		template<int K> bool	operator<= (const rgba_color<T,K>& c) const	{ return ((base::red<=c.red) && (base::green<=c.green) && (base::blue<=c.blue) && (base::alpha<=c.alpha)); }
		template<int K> bool	operator> (const rgba_color<T,K>& c) const	{ return ((base::red>c.red) && (base::green>c.green) && (base::blue>c.blue) && (base::alpha>c.alpha)); }
		template<int K> bool	operator>= (const rgba_color<T,K>& c) const	{ return ((base::red>=c.red) && (base::green>=c.green) && (base::blue>=c.blue) && (base::alpha>=c.alpha)); }

		void assert_invariant () const {
			sx::assert_invariant(base::red, base::green, base::blue, base::alpha);
		}
	};

	template<typename T, int K_> struct type<rgb_color<T,K_>> {
		inline static rgb_color<T,K_> cast (const T v[]) { if (v) return { v[0], v[1], v[2] }; return {}; }
		inline static rgb_color<T,K_> cast (const T& t) { return { t, t, t }; }
	};
	template<typename T, int K_> struct type<rgba_color<T,K_>> {
		inline static rgba_color<T,K_> cast (const T v[]) { if (v) return { v[0], v[1], v[2], v[3] }; return {}; }
		inline static rgba_color<T,K_> cast (const T& t) { return { t, t, t, t }; }
	};

	#if SXAPI_WIN32
		template<> struct type<Gdiplus::Color> {
			template<int K_>				inline static Gdiplus::Color cast (const rgba_color<std::uint8_t,K_>& c)	{ return { c.alpha, c.red, c.green, c.blue };  }
			template<typename T, int K_>	inline static Gdiplus::Color cast (const rgba_color<T,K_>& c)				{ return { BYTE(c.alpha/255.0f+0.5f), BYTE(c.red/255.0f+0.5f), BYTE(c.green/255.0f+0.5f), BYTE(c.blue/255.0f+0.5f) };  }
			template<int K_>				inline static Gdiplus::Color cast (const rgb_color<std::uint8_t,K_>& c)		{ return { BYTE(255), c.red, c.green, c.blue };  }
			template<typename T, int K_>	inline static Gdiplus::Color cast (const rgb_color<T,K_>& c)				{ return { BYTE(255), BYTE(c.red/255.0f+0.5f), BYTE(c.green/255.0f+0.5f), BYTE(c.blue/255.0f+0.5f) };  }
		};
	#endif
	
	using rgb	= rgb_color<>;
	using rgba	= rgba_color<>;
}

namespace sx {
	template<typename T, int K>			inline sx::rgb_color<T,K>		abs (const sx::rgb_color<T,K>& c)								{ return sx::rgb_color<T,K>(std::abs(c.red), std::abs(c.green), std::abs(c.blue)); }
	template<typename T, int K>			inline sx::rgba_color<T,K>	abs (const sx::rgba_color<T,K>& c)							{ return sx::rgba_color<T,K>(std::abs(c.red), std::abs(c.green), std::abs(c.blue), std::abs(c.alpha)); }
	template<typename T, int K, int L>	inline sx::rgb_color<T,K>		min (const sx::rgb_color<T,K>& a, const sx::rgb_color<T,L>& b)		{ return sx::rgb_color<T,K>(std::min(a.red,b.red), std::min(a.green,b.green), std::min(a.blue,b.blue)); }
	template<typename T, int K, int L>	inline sx::rgba_color<T,K>	min (const sx::rgba_color<T,K>& a, const sx::rgba_color<T,L>& b)	{ return sx::rgba_color<T,K>(std::min(a.red,b.red), std::min(a.green,b.green), std::min(a.blue,b.blue), std::min(a.alpha,b.alpha)); }
	template<typename T, int K, int L>	inline sx::rgb_color<T,K>		max (const sx::rgb_color<T,K>& a, const sx::rgb_color<T,L>& b)		{ return sx::rgb_color<T,K>(std::max(a.red,b.red), std::max(a.green,b.green), std::max(a.blue,b.blue)); }
	template<typename T, int K, int L>	inline sx::rgba_color<T,K>	max (const sx::rgba_color<T,K>& a, const sx::rgba_color<T,L>& b)	{ return sx::rgba_color<T,K>(std::max(a.red,b.red), std::max(a.green,b.green), std::max(a.blue,b.blue), std::max(a.alpha,b.alpha)); }
	template<typename T, int K>			inline rgb_color<T,K>&		revert_endian (rgb_color<T,K>& c)								{ sx::revert_endian(c.red); sx::revert_endian(c.green); sx::revert_endian(c.blue); return c; }
	template<typename T, int K>			inline rgba_color<T,K>&		revert_endian (rgba_color<T,K>& c)							{ sx::revert_endian(c.red); sx::revert_endian(c.green); sx::revert_endian(c.blue); sx::revert_endian(c.alpha); return c; }
	template<int K>						inline float			luminance (const sx::rgb_color<float,K>& c)					{ return (c.red * 0.299f + c.green * 0.587f + c.blue * 0.114f); }
	template<int K>						inline float			luminance (const sx::rgba_color<float,K>& c)					{ return (c.red * 0.299f + c.green * 0.587f + c.blue * 0.114f); }
	template<int K>						inline float			average (const sx::rgb_color<float,K>& c)						{ return (c.red + c.green + c.blue) * (1.0f / 3.0f); }
	template<int K>						inline float			average (const sx::rgba_color<float,K>& c)					{ return (c.red + c.green + c.blue) * (1.0f / 3.0f); }
	template<int K>						inline bool				zero (const sx::rgb_color<float,K>& c)						{ return -epsilon<c.red && c.red<epsilon && -epsilon<c.green && c.green<epsilon && -epsilon<c.blue && c.blue<epsilon; }
	template<int K>						inline bool				zero (const sx::rgba_color<float,K>& c)						{ return -epsilon<c.red && c.red<epsilon && -epsilon<c.green && c.green<epsilon && -epsilon<c.blue && c.blue<epsilon && -epsilon<c.alpha && c.alpha<epsilon; }
	template<typename T, int K>			inline rgb_color<T,K>			sqr (const rgb_color<T,K>& c)									{ return rgb_color<T,K>(c.red*c.red, c.green*c.green, c.blue*c.blue); }
	template<typename T, int K>			inline rgba_color<T,K>		sqr (const rgba_color<T,K>& c)								{ return rgba_color<T,K>(c.red*c.red, c.green*c.green, c.blue*c.blue, c.alpha*c.alpha); }
	template<typename T, int K>			inline rgb_color<T,K>			sqrt (const rgb_color<T,K>& c)								{ return rgb_color<T,K>(std::sqrt(c.red), std::sqrt(c.green), std::sqrt(c.blue)); }
	template<typename T, int K>			inline rgba_color<T,K>		sqrt (const rgba_color<T,K>& c)								{ return rgba_color<T,K>(std::sqrt(c.red), std::sqrt(c.green), std::sqrt(c.blue), std::sqrt(c.alpha)); }
	template<typename T, int K>			inline T				minimum3 (const rgb_color<T,K>& c)							{ return std::min(std::min(c.red, c.green), c.blue); }
	template<typename T, int K>			inline T				minimum3  (const rgba_color<T,K>& c)							{ return std::min(std::min(c.red, c.green), c.blue); }
	template<typename T, int K>			inline T				maximum3 (const rgb_color<T,K>& c)							{ return std::max(std::max(c.red, c.green), c.blue); }
	template<typename T, int K>			inline T				maximum3 (const rgba_color<T,K>& c)							{ return std::max(std::max(c.red, c.green), c.blue); }
	template<typename T, int K>			inline rgb_color<T,K>			normalize  (const rgb_color<T,K>& c)							{ const T t = std::sqrt(c.red*c.red + c.green*c.green + c.blue*c.blue); return sx::rgb_color<T,K>(c.red/t, c.green/t, c.blue/t); }
	template<int K>						inline rgb_color<float,K>		clamp (const rgb_color<float,K>& c)							{ return sx::max(sx::min(c, rgb_color<float,K>{1,1,1}), rgb_color<float,K>{0,0,0}); }
	template<int K>						inline rgba_color<float,K>	clamp (const rgba_color<float,K>& c)							{ return sx::max(sx::min(c, rgba_color<float,K>{1,1,1,1}), rgba_color<float,K>{0,0,0,0}); }
	template<int K>						inline bool				isnan (const rgb_color<float,K>& c)							{ return (std::isnan(c.red) && std::isnan(c.green) && std::isnan(c.blue)); }
	template<int K>						inline bool				isnan (const rgba_color<float,K>& c)							{ return (std::isnan(c.red) && std::isnan(c.green) && std::isnan(c.blue) && std::isnan(c.alpha)); }
	template<int K>						inline bool				isnormal (const rgb_color<float,K>& c)						{ return (sx::isnormal(c.red) && sx::isnormal(c.green) && sx::isnormal(c.blue)); }
	template<int K>						inline bool				isnormal (const rgba_color<float,K>& c)						{ return (sx::isnormal(c.red) && sx::isnormal(c.green) && sx::isnormal(c.blue) && sx::isnormal(c.alpha)); }

	template<typename T>		class uv;
	template<typename T, int N>	class vec;
	template<typename T, int N> using mat = std::array<vec<T,N>,std::size_t(N)>;

	template <typename T> class vec<T,2> {
	public:
		using value_type		= T;
		using reference			= T&;
		using const_reference	= const T&;
		using iterator			= T*;

		T	x{}, y{};

		vec ()								= default;
		vec (const vec&)					= default;
		vec& operator= (const vec&) SXLREF	= default;

		vec (const T& x, const T& y) : x(x), y(y) { }
		template<typename U, int N> explicit vec (const vec<U,N>& v) : x(T(v.x)), y(T(v.y)) { }
		
		const T*	data () const					{ return &x; }
		T*			data ()							{ return &x; }
		const T&	operator[] (int i) const		{ SXASSERT((0<=i) && (i<2)); return (&x)[i]; }
		T&			operator[] (int i)				{ SXASSERT((0<=i) && (i<2)); return (&x)[i]; }
		int			size () const					{ return 2; }
		vec			operator- () const				{ return vec{-x, -y}; }
		// template<typename U>	vec			operator+ (const U& u) const	{ return {static_cast<T>(x+u), static_cast<T>(y+u)}; }
		// template<typename U>	vec			operator- (const U& u) const	{ return {static_cast<T>(x-u), static_cast<T>(y-u)}; }
		template<typename U>	vec			operator* (const U& u) const	{ return {static_cast<T>(x*u), static_cast<T>(y*u)}; }
		template<typename U>	vec			operator/ (const U& u) const	{ return {static_cast<T>(x/u), static_cast<T>(y/u)}; }
		// template<typename U>	vec&		operator+= (const U& u)			{ x+=u; y+=u; return *this; }
		// template<typename U>	vec&		operator-= (const U& u)			{ x-=u; y-=u; return *this; }
		template<typename U>	vec&		operator*= (const U& u)			{ x*=u; y*=u; return *this; }
		template<typename U>	vec&		operator/= (const U& u)			{ x/=u; y/=u; return *this; }

		template<typename U, int N>			vec		operator+ (const vec<U,N>& b) const		{ static_assert(2<=N, ""); return vec{static_cast<T>(x+b.x), static_cast<T>(y+b.y)}; }
		template<typename U, int N>			vec		operator- (const vec<U,N>& b) const		{ static_assert(2<=N, ""); return vec{static_cast<T>(x-b.x), static_cast<T>(y-b.y)}; }
		template<typename U, int N>			vec		operator* (const vec<U,N>& b) const		{ static_assert(2<=N, ""); return vec{static_cast<T>(x*b.x), static_cast<T>(y*b.y)}; }
		template<typename U, int N>			vec		operator/ (const vec<U,N>& b) const		{ static_assert(2<=N, ""); return vec{static_cast<T>(x/b.x), static_cast<T>(y/b.y)}; }
		template<typename U, int N>			vec&	operator+= (const vec<U,N>& b)			{ static_assert(2<=N, ""); x+=b.x; y+=b.y; return *this; }
		template<typename U, int N>			vec&	operator-= (const vec<U,N>& b)			{ static_assert(2<=N, ""); x-=b.x; y-=b.y; return *this; }
		template<typename U, int N>			vec&	operator*= (const vec<U,N>& b)			{ static_assert(2<=N, ""); x*=b.x; y*=b.y; return *this; }
		template<typename U, int N>			vec&	operator/= (const vec<U,N>& b)			{ static_assert(2<=N, ""); x/=b.x; y/=b.y; return *this; }
		template<typename U>	vec		operator* (const mat<U,4>& u) const;
		template<typename U>	vec&	operator*= (const mat<U,4>& u);

		#if SXAPI_COCOA
			operator NSSize () const {
				return { x, y };
			}
		#endif
		#if SXAPI_WIN32
			operator POINT () const {
				return { static_cast<LONG>(x), static_cast<LONG>(y) };
			}
		#endif

		void assert_invariant () const {
			sx::assert_invariant(x, y);
		}
	};

	template <typename T> class vec<T,3> {
	public:
		using value_type		= T;
		using reference			= T&;
		using const_reference	= const T&;
		using iterator			= T*;

		T	x{}, y{}, z{};

		vec ()								= default;
		vec (const vec&)					= default;
		vec& operator= (const vec&) SXLREF	= default;

		vec (const T& x, const T& y, const T& z) : x(x), y(y), z(z) { }
		explicit vec (const rgb_color<T>& c) : x(c.red), y(c.green), z(c.blue) { }
		explicit vec (const vec<T,2>& v, const T& z) : x(v.x), y(v.y), z(z) { }
		template<typename U, int N> explicit vec (const vec<U,N>& v) : x(T(v.x)), y(T(v.y)), z(T(v.z)) { }

		explicit operator vec<T,2> () const;

		const T*	data () const					{ return &x; }
		T*			data ()							{ return &x; }
		const T&	operator[] (int i) const		{ SXASSERT((0<=i) && (i<3)); return (&x)[i]; }
		T&			operator[] (int i)				{ SXASSERT((0<=i) && (i<3)); return (&x)[i]; }
		int			size () const					{ return 3; }
		vec			operator- () const				{ return vec{-x, -y, -z}; }
		// template<typename U>	vec			operator+ (const U& u) const	{ return vec(x+u, y+u, z+u); }
		// template<typename U>	vec			operator- (const U& u) const	{ return vec(x-u, y-u, z-u); }
		template<typename U>	vec			operator* (const U& u) const	{ return vec(x*u, y*u, z*u); }
		template<typename U>	vec			operator/ (const U& u) const	{ return vec(x/u, y/u, z/u); }
		// template<typename U>	vec&		operator+= (const U& u)			{ x+=u; y+=u; z+=u; return *this; }
		// template<typename U>	vec&		operator-= (const U& u)			{ x-=u; y-=u; z-=u; return *this; }
		template<typename U>	vec&		operator*= (const U& u)			{ x*=u; y*=u; z*=u; return *this; }
		template<typename U>	vec&		operator/= (const U& u)			{ x/=u; y/=u; z/=u; return *this; }

		template<typename U, int N>	vec		operator+ (const vec<U,N>& u) const		{ static_assert(3<=N, ""); return vec(x+u.x, y+u.y, z+u.z); }
		template<typename U, int N>	vec		operator- (const vec<U,N>& u) const		{ static_assert(3<=N, ""); return vec(x-u.x, y-u.y, z-u.z); }
		template<typename U, int N>	vec		operator* (const vec<U,N>& u) const		{ static_assert(3<=N, ""); return vec(x*u.x, y*u.y, z*u.z); }
		template<typename U, int N>	vec		operator/ (const vec<U,N>& u) const		{ static_assert(3<=N, ""); return vec(x/u.x, y/u.y, z/u.z); }
		template<typename U, int N>	vec&	operator+= (const vec<U,N>& u)			{ static_assert(3<=N, ""); x+=u.x; y+=u.y; z+=u.z; return *this; }
		template<typename U, int N>	vec&	operator-= (const vec<U,N>& u)			{ static_assert(3<=N, ""); x-=u.x; y-=u.y; z-=u.z; return *this; }
		template<typename U, int N>	vec&	operator*= (const vec<U,N>& u)			{ static_assert(3<=N, ""); x*=u.x; y*=u.y; z*=u.z; return *this; }
		template<typename U, int N>	vec&	operator/= (const vec<U,N>& u)			{ static_assert(3<=N, ""); x/=u.x; y/=u.y; z/=u.z; return *this; }
		template<typename U>		vec		operator* (const mat<U,4>& t) const;
		template<typename U>		vec&	operator*= (const mat<U,4>& t);

		void assert_invariant () const {
			sx::assert_invariant(x, y, z);
		}
	};

	template <typename T> class vec<T,4> {
	public:
		using value_type		= T;
		using reference			= T&;
		using const_reference	= const T&;
		using iterator			= T*;

		T	x{}, y{}, z{}, w{};

		vec ()								= default;
		vec (const vec&)					= default;
		vec& operator= (const vec&) SXLREF	= default;

		explicit operator vec<T,2> () const;
		explicit operator vec<T,3> () const;

		vec (const T& x, const T& y, const T& z, const T& w) : x(x), y(y), z(z), w(w) { }
		explicit vec (const rgba_color<T>& c) : x(c.red), y(c.green), z(c.blue), w(c.alpha) { }
		explicit vec (const vec<T,3>& v) : x(v.x), y(v.y), z(v.z), w(1.0f) { }
		explicit vec (const vec<T,3>& v, const T& w) : x(v.x), y(v.y), z(v.z), w(w) { }
		explicit vec (const vec<T,2>& v, const T& z, const T& w) : x(v.x), y(v.y), z(z), w(w) { }
		template<typename U, int N> explicit vec (const vec<U,N>& v) : x(T(v.x)), y(T(v.y)), z(T(v.z)), w(T(v.w)) { }

		const T*	data () const					{ return &x; }
		T*			data ()							{ return &x; }
		const T&	operator[] (int i) const		{ SXASSERT((0<=i) && (i<4)); return (&x)[i]; }
		T&			operator[] (int i)				{ SXASSERT((0<=i) && (i<4)); return (&x)[i]; }
		int			size () const					{ return 4; }
		vec			operator- () const				{ return vec{-x, -y, -z, -w}; }
		// template<typename U>	vec			operator+ (const U& u) const	{ return vec(x+u, y+u, z+u, w+u); }
		// template<typename U>	vec			operator- (const U& u) const	{ return vec(x-u, y-u, z-u, w-u); }
		template<typename U>	vec			operator* (const U& u) const	{ return vec(x*u, y*u, z*u, w*u); }
		template<typename U>	vec			operator/ (const U& u) const	{ return vec(x/u, y/u, z/u, w/u); }
		// template<typename U>	vec&		operator+= (const U& u)			{ x+=u; y+=u; z+=u; w+=u; return *this; }
		// template<typename U>	vec&		operator-= (const U& u)			{ x-=u; y-=u; z-=u; w-=u; return *this; }
		template<typename U>	vec&		operator*= (const U& u)			{ x*=u; y*=u; z*=u; w*=u; return *this; }
		template<typename U>	vec&		operator/= (const U& u)			{ x/=u; y/=u; z/=u; w/=u; return *this; }

		template<typename U, int N>	vec		operator+ (const vec<U,N>& u) const		{ static_assert(4<=N, ""); return vec(x+u.x, y+u.y, z+u.z, w+u.w); }
		template<typename U, int N>	vec		operator- (const vec<U,N>& u) const		{ static_assert(4<=N, ""); return vec(x-u.x, y-u.y, z-u.z, w-u.w); }
		template<typename U, int N>	vec		operator* (const vec<U,N>& u) const		{ static_assert(4<=N, ""); return vec(x*u.x, y*u.y, z*u.z, w*u.w); }
		template<typename U, int N>	vec		operator/ (const vec<U,N>& u) const		{ static_assert(4<=N, ""); return vec(x/u.x, y/u.y, z/u.z, w/u.w); }
		template<typename U, int N>	vec&	operator+= (const vec<U,N>& u)			{ static_assert(3<=N, ""); x+=u.x; y+=u.y; z+=u.z; w+=u.w; return *this; }
		template<typename U, int N>	vec&	operator-= (const vec<U,N>& u)			{ static_assert(3<=N, ""); x-=u.x; y-=u.y; z-=u.z; w-=u.w; return *this; }
		template<typename U, int N>	vec&	operator*= (const vec<U,N>& u)			{ static_assert(3<=N, ""); x*=u.x; y*=u.y; z*=u.z; w*=u.w; return *this; }
		template<typename U, int N>	vec&	operator/= (const vec<U,N>& u)			{ static_assert(3<=N, ""); x/=u.x; y/=u.y; z/=u.z; w/=u.w; return *this; }
		template<typename U>		vec		operator* (const mat<U,4>& t) const;
		template<typename U>		vec&	operator*= (const mat<U,4>& t);

		void assert_invariant () const {
			sx::assert_invariant(x, y, z, w);
		}
	};
	
	template<typename T> struct type<vec<T,2>> {
		template<typename U> inline static vec<T,2> cast (const vec<U,2>& v) { return {static_cast<T>(v.x), static_cast<T>(v.y)}; }
		template<typename U> inline static vec<T,2> cast (const U& u) { return { static_cast<T>(u), static_cast<T>(u) }; }
		inline  static vec<T,2> cast (const T v[]) { if (v) return { v[0], v[1] }; return {}; }
		#if SXAPI_COCOA || SXAPI_COCOA_TOUCH
			static inline vec<T,2>	cast (const ::CGPoint& p)	{ return {static_cast<T>(p.x), static_cast<T>(p.y)}; }
			static inline vec<T,2>	cast (const ::CGSize& p)	{ return {static_cast<T>(p.width), static_cast<T>(p.height)}; }
		#endif
		#if SXAPI_COCOA && !(__LP64__)
			static inline vec<T,2>	cast (const ::NSPoint& p)	{ return {static_cast<T>(p.x), static_cast<T>(p.y)}; }
			static inline vec<T,2>	cast (const ::NSSize& p)	{ return {static_cast<T>(p.width), static_cast<T>(p.height)}; }
		#endif
		#if SXAPI_WIN32
			static inline vec<T,2>	cast (const ::POINT& p)	{ return {static_cast<T>(p.x), static_cast<T>(p.y)}; }
			static inline vec<T,2>	cast (const ::SIZE& p)	{ return {static_cast<T>(p.cx), static_cast<T>(p.cy)}; }
		#endif
	};
	template<typename T> struct type<vec<T,3>> {
		template<typename U> inline static vec<T,3> cast (const vec<U,3>& v) { return {static_cast<T>(v.x), static_cast<T>(v.y), static_cast<T>(v.z)}; }
		inline  static vec<T,3> cast (const T v[]) { if (v) return { v[0], v[1], v[2] }; return {}; }
		template<typename U> inline static vec<T,3> cast (const U& u) { return { static_cast<T>(u), static_cast<T>(u), static_cast<T>(u) }; }
	};
	template<typename T> struct type<vec<T,4>> {
		template<typename U> inline static vec<T,4> cast (const vec<U,4>& v) { return {static_cast<T>(v.x), static_cast<T>(v.y), static_cast<T>(v.z), static_cast<T>(v.w)}; }
		inline  static vec<T,4> cast (const T v[]) { if (v) return { v[0], v[1], v[2], v[3] }; return {}; }
		template<typename U> inline static vec<T,4> cast (const U& u) { return { static_cast<T>(u), static_cast<T>(u), static_cast<T>(u), static_cast<T>(u) }; }
	};
	
	template<typename T> vec<T,3>::operator vec<T,2> () const {
		return vec<T,2>{x,y};
	}
	template<typename T> vec<T,4>::operator vec<T,2> () const {
		return vec<T,2>{x,y};
	}
	template<typename T> vec<T,4>::operator vec<T,3> () const {
		return vec<T,3>{x,y,z};
	}

	using ivec2	= vec<int,2>;
	using ivec3	= vec<int,3>;
	using ivec4	= vec<int,4>;
	using vec2	= vec<float,2>;
	using vec3	= vec<float,3>;
	using vec4	= vec<float,4>;

	template<int i0, int i1, int i2, int i3, typename T, int N> inline vec<T,4> shuffle (const vec<T,N>& v) {
		return vec<T,4>(v[i0], v[i1], v[i2], v[i3]);
	}
	template<int i0, int i1, typename T> inline vec<T,2> shuffle2 (const vec<T,2>& v) {
		return vec<T,2>(v[i0], v[i1]);
	}
	template<int i0, int i1, int i2, typename T> inline vec<T,3> shuffle3 (const vec<T,3>& v) {
		return vec<T,3>(v[i0], v[i1], v[i2]);
	}
	template<int i0, int i1, int i2, int i3, typename T> inline vec<T,4> shuffle4 (const vec<T,4>& v) {
		return vec<T,4>(v[i0], v[i1], v[i2], v[i3]);
	}

	template<typename T, int N> inline bool isnan (const sx::vec<T,N>& v) {
		for (int i = 0; i < N; ++i) if (std::isnan(v[i])) return true;
		return false;
	}

	inline bool zero (float t) { return (-epsilon <= t) && (t <= epsilon); }
	inline bool zero (double t) { return (-epsilon <= t) && (t <= epsilon); }
	inline bool zero (int t) { return (t == 0); }

	template<typename T, int N> inline bool zero (const vec<T,N>& v) {
		for (int i = 0; i < N; ++i) {
			if (!sx::zero(v[i])) return false;
		}
		return true;
	}

	inline float sqr (const float& t) { return t*t; }
	inline double sqr (const double& t) { return t*t; }
	inline int sqr (const int& t) { return t*t; }
	template<typename T, int N> inline sx::vec<T,N> sqr (const sx::vec<T,N>& a) {
		vec<T,N> v;
		for (int i = 0; i < N; ++i) v[i] = a[i] * a[i];
		return v;
	}

	template<typename T> T sqrt (const T&); // should be removed. also, cyclic dependency between vectors.hpp and sse.hpp should be broken.
	inline float sqrt (const float& t) { return std::sqrt(t); }
	inline double sqrt (const double& t) { return std::sqrt(t); }
	template<typename T, int N> inline sx::vec<T,N> sqrt (const sx::vec<T,N>& v) {
		vec<T,N> q;
		for (int i = 0; i < N; ++i) q[i] = sx::sqrt(v[i]);
		return q;
	}

	inline float rsqrt (const float& t) { return 1.0f / std::sqrt(t); }
	inline double rsqrt (const double& t) { return 1.0 / std::sqrt(t); }
	template<typename T, int N> inline sx::vec<T,N> rsqrt (const sx::vec<T,N>& v) {
		vec<T,N> q;
		for (int i = 0; i < N; ++i) q[i] = sx::rsqrt(v[i]);
		return q;
	}

	inline float reciprocal (const float& t) { return 1.0f / t; }
	inline double reciprocal (const double& t) { return 1.0 / t; }
	template<typename T, int N> inline sx::vec<T,N> reciprocal (const sx::vec<T,N>& v) {
		sx::vec<T,N> u;
		for (int i = 0; i < N; ++i) u[i] = sx::reciprocal(v[i]);
		return u;
	}

	inline float abs (const float& t) { return std::abs(t); }
	inline double abs (const double& t) { return std::abs(t); }
	inline int abs (const int& t) { return std::abs(t); }
	template<typename T, int N> inline sx::vec<T,N> abs (const sx::vec<T,N>& p) {
		vec<T,N> v;
		for (int i = 0; i < N; ++i) v[i] = sx::abs(p[i]);
		return v;
	}

	template<typename T> inline T sum (const sx::vec<T,2>& p) { return p.x + p.y; }
	template<typename T> inline T sum (const sx::vec<T,3>& p) { return p.x + p.y + p.z; }
	template<typename T> inline T sum (const sx::vec<T,4>& p) { return p.x + p.y + p.z + p.w; }

	template<typename T, int N> inline T sum2 (const sx::vec<T,N>& p) { return p.x + p.y; }
	template<typename T, int N> inline T sum3 (const sx::vec<T,N>& p) { return p.x + p.y + p.z; }
	template<typename T> inline T sum4 (const sx::vec<T,4>& p) { return p.x + p.y + p.z + p.w; }

	template<typename T> inline T inner_product (const sx::vec<T,4>& a, const sx::vec<T,4>& b) {
		return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
	}
	template<typename T> inline T inner_product (const sx::vec<T,3>& a, const sx::vec<T,3>& b) {
		return a.x*b.x + a.y*b.y + a.z*b.z;
	}
	template<typename T> inline T inner_product (const sx::vec<T,2>& a, const sx::vec<T,2>& b) {
		return a.x*b.x + a.y*b.y;
	}
	template<typename T, int N> inline static T inner_product4 (const sx::vec<T,N>& a, const sx::vec<T,N>& b) {
		return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
	}
	template<typename T, int N, int M> inline T inner_product3 (const sx::vec<T,N>& a, const sx::vec<T,M>& b) {
		return a.x*b.x + a.y*b.y + a.z*b.z;
	}
	template<typename T, int N, int M> inline T inner_product2 (const sx::vec<T,2>& a, const sx::vec<T,2>& b) {
		return a.x*b.x + a.y*b.y;
	}

	template<typename T, int M> inline vec<T,3> product (const vec<T,3>& a, const vec<T,M>& b) {
		return vec<T,3>(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
	}
	template<typename T, int M> inline vec<T,4> product (const vec<T,4>& a, const vec<T,M>& b) {
		return vec<T,4>(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x, 1.0f);
	}

	template<typename T, int N> inline T absolute2 (const sx::vec<T,N>& p) {
		return sx::sqrt(p.x*p.x + p.y*p.y);
	}
	template<typename T, int N> inline T sqrabsolute2 (const sx::vec<T,N>& p) {
		return (p.x*p.x + p.y*p.y);
	}
	template<typename T, int N> inline T absolute3 (const sx::vec<T,N>& p) {
		return sx::sqrt(p.x*p.x + p.y*p.y + p.z*p.z);
	}
	template<typename T, int N> inline T sqrabsolute3 (const sx::vec<T,N>& p) {
		return (p.x*p.x + p.y*p.y + p.z*p.z);
	}
	template<typename T, int N> inline T absolute4 (const sx::vec<T,N>& p) {
		return sx::sqrt(p.x*p.x + p.y*p.y + p.z*p.z + p.w*p.w);
	}

	template<typename T> inline T sqrabsolute4 (const sx::vec<T,4>& p) {
		return (p.x*p.x + p.y*p.y + p.z*p.z + p.w*p.w);
	}

	template<typename T> inline sx::vec<T,2> normalize (const sx::vec<T,2>& p) {
		static_assert(std::is_floating_point<T>::value, "");
//		T d = sqrabsolute2(p);
//		if (std::numeric_limits<T>::min() < d) {
//			return (p / sx::sqrt(d));
//		}
//		return p;
		const T q = std::sqrt(sqrabsolute2(p));
		if (0 < q) return (p / q);
		return p;
	}
	template<typename T> inline T normalize_self (vec<T,2>& p) {
		static_assert(std::is_floating_point<T>::value, "");
//		T d = sqrabsolute2(p);
//		if (std::numeric_limits<T>::min() < d) {
//			d = sx::sqrt(d);
//			const T r = 1.0f / d;
//			p.x *= r;
//			p.y *= r;
//		}
//		return d;
		const T q = std::sqrt(sqrabsolute2(p));
		if (0 < q) p /= q;
		return q;
	}

	template<typename T, int N> inline sx::vec<T,N> round (const sx::vec<T,N>& p) {
		vec<T,N> v;
		for (int i = 0; i < N; ++i) v[i] = std::floor(p[i]+0.5f);
		return v;
	}
	template<typename T, int N> inline sx::vec<T,N> trunc (const sx::vec<T,N>& p) {
		vec<T,N> v;
		for (int i = 0; i < N; ++i) v[i] = float(int(p[i]));
		return v;
	}
	template<typename T, int N> inline sx::vec<T,N> ceil (const sx::vec<T,N>& p) {
		vec<T,N> v;
		for (int i = 0; i < N; ++i) v[i] = std::ceil(p[i]);
		return v;
	}
	template<typename T, int N> inline sx::vec<T,N> floor (const sx::vec<T,N>& p) {
		vec<T,N> v;
		for (int i = 0; i < N; ++i) v[i] = std::floor(p[i]);
		return v;
	}
}
namespace sx {
	template<typename T, typename U, int N> inline sx::vec<T,N> min (const sx::vec<T,N>& a, const sx::vec<U,N>& b) {
		sx::vec<T,N> v;
		for (int i = 0; i < N; ++i) v[i] = sx::min(a[i], b[i]);
		return v;
	}
	template<typename T, typename U, int N> inline sx::vec<T,N> max (const sx::vec<T,N>& a, const sx::vec<U,N>& b) {
		sx::vec<T,N> v;
		for (int i = 0; i < N; ++i) v[i] = sx::max(a[i], b[i]);
		return v;
	}

	template<typename T, int N> inline T minimum2 (const sx::vec<T,N>& v) { return sx::min(v.x, v.y); }
	template<typename T, int N> inline T minimum3 (const sx::vec<T,N>& v) { return sx::min(sx::min(v.x, v.y), v.z); }
	template<typename T, int N> inline T minimum4 (const sx::vec<T,N>& v) { return sx::min(sx::min(sx::min(v.x, v.y), v.z), v.w); }

	template<typename T, int N> inline T maximum2 (const sx::vec<T,N>& v) { return sx::max(v.x, v.y); }
	template<typename T, int N> inline T maximum3 (const sx::vec<T,N>& v) { return sx::max(sx::max(v.x, v.y), v.z); }
	template<typename T, int N> inline T maximum4 (const sx::vec<T,N>& v) { return sx::max(sx::max(sx::max(v.x, v.y), v.z), v.w); }
}
namespace sx {
	using mat3	= mat<float,3>;
	using mat4	= mat<float,4>;

	template<typename T, int N> auto operator* (const sx::mat<T,N>&, const sx::mat<T,N>&) -> sx::mat<T,N>;
	template<typename T, int N> auto operator*= (sx::mat<T,N>&, const sx::mat<T,N>&) -> sx::mat<T,N>&;
	template<typename T, int N> auto operator* (const mat<T,N>& t, const vec<T,N>& p) -> vec<T,N> {
			sx::vec<T,N> q;
			for (auto i = 0; i < N; ++i) q[i] = sx::sum(t[i] * p);
			return q;
		}
}
namespace sx {
	template<typename T, int N> bool isnan (const mat<T,N>& a) {
		for (int i = 0; i < N; ++i) if (isnan(a[i])) return true;
		return false;
	}

	template<typename T> inline vec<T,3> transform_direction (const vec<T,3>& p, const sx::mat<T,4>& t) {
		return vec<T,3>(
			p[0] * t[0][0] + p[1] * t[1][0] + p[2] * t[2][0],
			p[0] * t[0][1] + p[1] * t[1][1] + p[2] * t[2][1],
			p[0] * t[0][2] + p[1] * t[1][2] + p[2] * t[2][2]
		);
	}

	template<typename T> inline vec<T,3> transform_normal (const mat<T,4>& it, const vec<T,3>& p) {
		return vec<T,3>(
			it[0][0] * p[0] + it[0][1] * p[1] + it[0][2] * p[2],
			it[1][0] * p[0] + it[1][1] * p[1] + it[1][2] * p[2],
			it[2][0] * p[0] + it[2][1] * p[1] + it[2][2] * p[2]
		);
	}

	template<typename T> inline T det (const sx::mat<T,4>& a, int i1, int i2, int i3, int j1, int j2, int j3) {
		sx::assert_invariant(a);
		return (	(a[i1][j1]*a[i2][j2]*a[i3][j3])
				+	(a[i1][j2]*a[i2][j3]*a[i3][j1])
				+	(a[i1][j3]*a[i2][j1]*a[i3][j2])
				-	(a[i1][j1]*a[i2][j3]*a[i3][j2])
				-	(a[i1][j2]*a[i2][j1]*a[i3][j3])
				-	(a[i1][j3]*a[i2][j2]*a[i3][j1]));
	}
	template<typename T> inline T det (const mat<T,4>& a) {
		sx::assert_invariant(a);
		enum { X, Y, Z, W };
		return (		(a[0][X]*a[1][Y]*a[2][Z])
					+	(a[0][Y]*a[1][Z]*a[2][X])
					+	(a[0][Z]*a[1][X]*a[2][Y])
					-	(a[0][X]*a[1][Z]*a[2][Y])
					-	(a[0][Y]*a[1][X]*a[2][Z])
					-	(a[0][Z]*a[1][Y]*a[2][X])
			);
	}

	mat<float,4> inv (const mat<float,4>& t);
	mat<float,4> inverse (const mat<float,4>& t);
	mat<float,4> transpose (const mat<float,4>& t);
	template<typename T> auto transpose (const mat<T,3>& t) -> mat<T,3> {
		return {{
			{ t[0][0],t[1][0],t[2][0], },
			{ t[0][1],t[1][1],t[2][1], },
			{ t[0][2],t[1][2],t[2][2], },
		}};
	}

	template<typename T, int N> sx::vec<T,N>& revert_endian (vec<T,N>& v) {
		for (int i = 0; i < N; ++i) revert_endian(v[i]);
		return v;
	}

	template<typename T, int N> sx::mat<T,N>& revert_endian (mat<T,N>& m) {
		for (int i = 0; i < N; ++i) revert_endian(m[i]);
		return m;
	}

	template<typename T> inline T clamp (const T& x, const T& min = T(0.0f), const T& max = T(1.0f)) {
		if (x < min) return min;
		if (max < x) return max;
		return x;
	}
	template<typename T, int N> inline sx::vec<T,N> clamp (const sx::vec<T,N>& v, const sx::vec<T,N>& min = (sx::type<sx::vec<T,N>>::cast(0)), const sx::vec<T,N>& max = (sx::type<sx::vec<T,N>>::cast(1))) {
		return sx::min(sx::max(v, min), max);
	}

	inline static sx::vec4 homogeneous (const sx::vec3& p) {
		return sx::vec4(p, 1.0f);
	}

	inline static vec<float,3> cartesian (const vec<float,4>& p) {
		return vec<float,3>(p.x/p.w, p.y/p.w, p.z/p.w);
	}

	namespace unittest {
		bool nearlyequal (const sx::vec2& a, const sx::vec2& b);
		bool nearlyequal (const sx::vec3& a, const sx::vec3& b);
		bool nearlyequal (const sx::vec4& a, const sx::vec4& b);
		bool nearlyequal (const mat<float,4>& a, const mat<float,4>& b);
		template<int K, int L> bool nearlyequal (const sx::rgb_color<float,K>& a, const sx::rgb_color<float,L>& b) { return nearlyequal(a.red,b.red) && nearlyequal(a.green,b.green) && nearlyequal(a.blue,b.blue); }
		template<int K, int L> bool nearlyequal (const sx::rgba_color<float,K>& a, const sx::rgba_color<float,L>& b) { return nearlyequal(a.red,b.red) && nearlyequal(a.green,b.green) && nearlyequal(a.blue,b.blue) && nearlyequal(a.alpha,b.alpha); }
	}
}

namespace sx {
template<typename S, typename T, int N> inline bool operator< (const sx::vec<S,N>& a, const sx::vec<T,N>& b) {
	for (int i = 0; i < N; ++i) if (!(a[i] < b[i])) return false;
	return true;
}

template<typename S, typename T, int N> inline bool operator<= (const sx::vec<S,N>& a, const sx::vec<T,N>& b) {
	for (int i = 0; i < N; ++i) if (!(a[i] <= b[i])) return false;
	return true;
}

template<typename S, typename T, int N> inline bool operator== (const sx::vec<S,N>& a, const sx::vec<T,N>& b) {
	for (int i = 0; i < N; ++i) if (!(a[i] == b[i])) return false;
	return true;
}

template<typename S, typename T, int N> inline bool operator!= (const sx::vec<S,N>& a, const sx::vec<T,N>& b) {
	return !(a == b);
}
}
namespace sx {
	template<typename T> template<typename U> inline sx::vec<T,4> sx::vec<T,4>::operator* (const sx::mat<U,4>& t) const {
		return sx::vec<T,4>(
			(*this)[0] * t[0][0] + (*this)[1] * t[1][0] + (*this)[2] * t[2][0] + (*this)[3] * t[3][0],
			(*this)[0] * t[0][1] + (*this)[1] * t[1][1] + (*this)[2] * t[2][1] + (*this)[3] * t[3][1],
			(*this)[0] * t[0][2] + (*this)[1] * t[1][2] + (*this)[2] * t[2][2] + (*this)[3] * t[3][2],
			(*this)[0] * t[0][3] + (*this)[1] * t[1][3] + (*this)[2] * t[2][3] + (*this)[3] * t[3][3]
		);
	}

	template<typename T> template<typename U> inline sx::vec<T,3> sx::vec<T,3>::operator* (const sx::mat<U,4>& t) const {
		return sx::vec<T,3>(
			(*this)[0] * t[0][0] + (*this)[1] * t[1][0] + (*this)[2] * t[2][0] + t[3][0],
			(*this)[0] * t[0][1] + (*this)[1] * t[1][1] + (*this)[2] * t[2][1] + t[3][1],
			(*this)[0] * t[0][2] + (*this)[1] * t[1][2] + (*this)[2] * t[2][2] + t[3][2]
		);
	}

	template<typename T> template<typename U> inline sx::vec<T,2> sx::vec<T,2>::operator* (const sx::mat<U,4>& t) const {
		return sx::vec<T,2>(
			(*this)[0] * t[0][0] + (*this)[1] * t[1][0] + t[3][0],
			(*this)[0] * t[0][1] + (*this)[1] * t[1][1] + t[3][1]
		);
	}

	template<typename T> template<typename U> inline sx::vec<T,2>& sx::vec<T,2>::operator*= (const sx::mat<U,4>& t) {
		return ((*this) = (*this) * t);
	}
	template<typename T> template<typename U> inline sx::vec<T,3>& sx::vec<T,3>::operator*= (const sx::mat<U,4>& t) {
		return ((*this) = (*this) * t);
	}
	template<typename T> template<typename U> inline sx::vec<T,4>& sx::vec<T,4>::operator*= (const sx::mat<U,4>& t) {
		return ((*this) = (*this) * t);
	}
}
namespace sx {
	template<typename T, int N> inline sx::vec<T,N> normalize (const sx::vec<T,N>& p) {
		static_assert(std::is_floating_point<T>::value, "");
//		T d = sqrabsolute3(p);
//		if (std::numeric_limits<T>::min() < d) {
//			return (p / sx::sqrt(d));
//		}
//		return p;
		const T q = std::sqrt(sqrabsolute3(p));
		if (0 < q) return (p / q);
		return p;
	}

	template<typename T> inline T normalize_self (vec<T,3>& p) {
		static_assert(std::is_floating_point<T>::value, "");
//		T d = sqrabsolute3(p);
//		if (std::numeric_limits<T>::min() < d) {
//			d = sx::sqrt(d);
//			p /= d;
//		}
//		return d;
		const T q = std::sqrt(sqrabsolute3(p));
		if (0 < q) p /= q;
		return q;
	}
}

template<typename T> inline sx::mat<T,4> operator* (const sx::mat<T,4>& a, const T& f) {
	return sx::mat<T,4>{ a[0] * f, a[1] * f, a[2] * f, a[3] * f };
}

template<typename T> inline sx::mat<T,4>& operator*= (sx::mat<T,4>& a, const T& f) {
	a[0] *= f;
	a[1] *= f;
	a[2] *= f;
	a[3] *= f;
	return a;
}

template<typename T, int N> inline sx::mat<T,N> operator+ (const sx::mat<T,N>& a, const sx::mat<T,N>& b) {
	sx::mat<T,N> t;
	for (int i = 0; i < N; ++i) t[i] = a[i] + b[i];
	return t;
}
template<typename T, int N> inline sx::mat<T,N> operator- (const sx::mat<T,N>& a, const sx::mat<T,N>& b) {
	sx::mat<T,N> t;
	for (int i = 0; i < N; ++i) t[i] = a[i] - b[i];
	return t;
}

namespace sx {
	template<typename T, int N> auto operator* (const sx::mat<T,N>& a, const sx::mat<T,N>& b) -> sx::mat<T,N> {
		sx::mat<T,N> t;
		for (int i = 0; i < N; ++i) t[i] = a[i] * b;
		return t;
	}
	template<typename T, int N> auto operator*= (sx::mat<T,N>& a, const sx::mat<T,N>& b) -> sx::mat<T,N>& {
		a = a * b;
		return a;
	}
}
template<typename T, int N> inline sx::mat<T,N>& operator>> (const sx::mat<T,N>& a, sx::mat<T,N>& b) {
	b = a * b;
	return b;
}

template<typename T, int N> bool operator== (const sx::mat<T,N>& a, const sx::mat<T,N>& b) {
	sx::assert_invariant(a, b);
	for (int i = 0; i < N; ++i) if (a[i] != b[i]) return false;
	return true;
}
template<typename T, int N> bool operator!= (const sx::mat<T,N>& a, const sx::mat<T,N>& b) {
	return !(a == b);
}

namespace sx {
	template<typename T> inline bool zero (const mat<T,4>& m) {
		sx::assert_invariant(m);
		const auto z = sx::vec<T,3>{0,0,0};
		return (reinterpret_cast<const sx::vec<T,3>&>(m[0]) == z) && (reinterpret_cast<const sx::vec<T,3>&>(m[1]) == z) && (reinterpret_cast<const sx::vec<T,3>&>(m[2]) == z);
	}
	template<typename T, int N> auto normalize_self (mat<T,N>& t) -> mat<T,N>& {
		const T k = reciprocal(t[N-1][N-1]);
		for (auto i = 0; i < N; ++i) t[i] *= k;
		return t;
	}

	template<typename T, int N> bool isfinite (const vec<T,N>& v) {
		sx::assert_invariant(v);
		if (std::numeric_limits<T>::has_infinity) {
			for (int i = 0; i < N; ++i) if (!std::isfinite(v[i])) return false;
		}
		return true;
	}

	template<typename T, int N> bool isnormal (const vec<T,N>& v) {
		sx::assert_invariant(v);
		if (std::numeric_limits<T>::has_denorm) {
			for (int i = 0; i < N; ++i) if (!sx::isnormal(v[i])) return false;
		}
		return true;
	}
	//template<typename T, int N> inline vec<T,N> undenormal (const vec<T,N>& v) {
	//	sx::assert_invariant(v);
	//	if (!std::numeric_limits<T>::has_denorm) return v;
	//	vec<T,N> w;
	//	for (int i = 0; i < N; ++i) w[i] = sx::isnormal(v[i]) ? v[i] : 0.0f;
	//	return w;
	//}
}

namespace sx {
	namespace matrix {
		extern const sx::mat4 identity;
		extern const sx::mat4 zero;
	}
}

namespace sx {
	template<typename T = float> class uv {
	public:
		T u, v;
		uv () = default;
		uv (const uv&) = default;
		uv& operator= (const uv&) SXLREF = default;
		explicit uv (T u, T v) : u(u), v(v) { }
		explicit uv (T f) : u(f), v(f) { }
		template<int N> explicit uv (const sx::vec<T,N>& v) : u(v.x), v(v.y) { }
		operator const sx::vec<T,2>& () const { return *reinterpret_cast<const sx::vec<T,2>*>(this); }
		operator sx::vec<T,2>& () { return *reinterpret_cast<sx::vec<T,2>*>(this); }

		const T& operator[] (int i) const { SXASSERT((0<=i) && (i<2)); return (&u)[i]; }
		T& operator[] (int i) { SXASSERT((0<=i) && (i<2)); return (&u)[i]; }
		int size () const { return 2; }
		void assert_invariant () const {
			sx::assert_invariant(u, v);
		}

		uv operator* (const sx::mat<T,4>& b) const {
			return uv(
					u * b[0][0] + v * b[1][0] + b[3][0],
					u * b[0][1] + v * b[1][1] + b[3][1]
					);
		}
		uv operator* (const T& b) const {
			return uv(u*b, v*b);
		}
		uv operator* (const uv& b) const {
			return uv(u*b.u, v*b.v);
		}
		uv& operator*= (const uv& b) {
			u *= b.u;
			v *= b.v;
			return *this;
		}
		uv& operator*= (const T& b) {
			u *= b;
			v *= b;
			return *this;
		}
		uv operator/ (const T& b) const {
			return uv(u/b, v/b);
		}
		uv& operator/= (const T& b) {
			u /= b;
			v /= b;
			return *this;
		}
		uv operator+ (const uv& b) const {
			return uv(u+b.u, v+b.v);
		}
		uv& operator+= (const uv& b) {
			u += b.u;
			v += b.v;
			return *this;
		}
		uv operator- () const {
			return uv{-u, -v};
		}
		uv operator- (const uv& b) const {
			return uv(u-b.u, v-b.v);
		}
		uv& operator-= (const uv& b) {
			u -= b.u;
			v -= b.v;
			return *this;
		}
		bool operator< (const uv& b) const {
			return (u < b.u) && (v < b.v);
		}
		bool operator<= (const uv& b) const {
			return (u <= b.u) && (v <= b.v);
		}
		bool operator== (const uv& b) const {
			return (u == b.u) && (v == b.v);
		}
		bool operator!= (const uv& b) const {
			return !(*this == b);
		}
	};
}
namespace sx {
	template<typename T> inline bool zero (const sx::uv<T>& a) {
		return (sx::zero(a.u) && sx::zero(a.v));
	}
	template<typename T>inline sx::uv<T> min (const sx::uv<T>& a, const sx::uv<T>& b) {
		return sx::uv<T>(std::min(a.u,b.u), std::min(a.v,b.v));
	}
	template<typename T>inline sx::uv<T> max (const sx::uv<T>& a, const sx::uv<T>& b) {
		return sx::uv<T>(std::max(a.u,b.u), std::max(a.v,b.v));
	}
	inline float mod1 (float x) {
		return x - std::floor(x);
	}
	template<int N> inline sx::vec<float,N> mod1 (const sx::vec<float,N>& x) {
		return x - sx::floor(x);
	}
}

// for debugging purposes only. formats may change.
template<typename T> std::ostream& operator<< (std::ostream& s, const sx::uv<T>& uv) {
	s << uv.u << " " << uv.v;
	return s;
}
template<int K> std::ostream& operator<< (std::ostream& s, const sx::rgb_color<std::uint8_t,K>& c) {
	s << int(c.red) << " " << int(c.green) << " " << int(c.blue);
	return s;
}
template<typename T, int K> std::ostream& operator<< (std::ostream& s, const sx::rgb_color<T,K>& c) {
	s << c.red << " " << c.green << " " << c.blue;
	return s;
}
template<int K> std::ostream& operator<< (std::ostream& s, const sx::rgba_color<std::uint8_t,K>& c) {
	s << int(c.red) << " " << int(c.green) << " " << int(c.blue) << " " << int(c.alpha);
	return s;
}
template<typename T, int K> std::ostream& operator<< (std::ostream& s, const sx::rgba_color<T,K>& c) {
	s << c.red << " " << c.green << " " << c.blue << " " << c.alpha;
	return s;
}
template<typename T, int N> std::ostream& operator<< (std::ostream& s, const sx::vec<T,N>& v) {
	for (int i = 0; i < N; ++i) s << v[i] << " ";
	return s;
}
template<typename T, int N> std::ostream& operator<< (std::ostream& s, const sx::mat<T,N>& m) {
	for (int i = 0; i < N; ++i) s << m[i] << " ";
	return s;
}

//namespace sx {
//	template<typename T> inline sx::vec<T,2> make (const T& t) {
//		return sx::vec<T,2>{ t, t };
//	}
//	template<typename T> inline sx::vec<T,3> make (const T& t) {
//		return sx::vec<T,3>{ t, t, t };
//	}
//	template<typename T> inline sx::vec<T,4> make (const T& t) {
//		return sx::vec<T,4>{ t, t, t, t };
//	}
//}

namespace std {
	template<typename T> class numeric_limits<sx::uv<T>> {
	public:
		static sx::uv<T> max ()		{ return sx::uv<T>(std::numeric_limits<T>::max()); }
		static sx::uv<T> lowest ()	{ return sx::uv<T>(std::numeric_limits<T>::lowest()); }
	};

//	template<typename T, int N> struct numeric_limits<sx::vec<T,N>> {
//		static sx::vec<T,N> max ()		{ return sx::make<sx::vec<T,2>>(std::numeric_limits<T>::max()); }
//		static sx::vec<T,N> lowest ()	{ return sx::make<sx::vec<T,2>>(std::numeric_limits<T>::lowest()); }
//	};
	template<typename T> struct numeric_limits<sx::vec<T,2>> {
		static sx::vec<T,2> max ()		{ return sx::vec<T,2>{ std::numeric_limits<T>::max(), std::numeric_limits<T>::max() }; }
		static sx::vec<T,2> lowest ()	{ return sx::vec<T,2>{ std::numeric_limits<T>::lowest(), std::numeric_limits<T>::lowest() }; }
	};
	template<typename T> struct numeric_limits<sx::vec<T,3>> {
		static sx::vec<T,3> max ()		{ return sx::vec<T,3>{ std::numeric_limits<T>::max(), std::numeric_limits<T>::max(), std::numeric_limits<T>::max() }; }
		static sx::vec<T,3> lowest ()	{ return sx::vec<T,3>{ std::numeric_limits<T>::lowest(), std::numeric_limits<T>::lowest(), std::numeric_limits<T>::lowest() }; }
	};
	template<typename T> struct numeric_limits<sx::vec<T,4>> {
		static sx::vec<T,4> max ()		{ return sx::vec<T,4>{ std::numeric_limits<T>::max(), std::numeric_limits<T>::max(), std::numeric_limits<T>::max(), std::numeric_limits<T>::max() }; }
		static sx::vec<T,4> lowest ()	{ return sx::vec<T,4>{ std::numeric_limits<T>::lowest(), std::numeric_limits<T>::lowest(), std::numeric_limits<T>::lowest(), std::numeric_limits<T>::lowest() }; }
	};

	template<typename T, int K> class numeric_limits<sx::rgb_color<T,K>> {
	public:
		static sx::rgb_color<T,K> max ()		{ return sx::rgb_color<T,K>(std::numeric_limits<T>::max()); }
		static sx::rgb_color<T,K> lowest ()	{ return sx::rgb_color<T,K>(std::numeric_limits<T>::lowest()); }
	};
	template<typename T, int K> class numeric_limits<sx::rgba_color<T,K>> {
	public:
		static sx::rgba_color<T,K> max ()		{ return sx::cast<sx::rgba_color<T,K>>(std::numeric_limits<T>::max()); }
		static sx::rgba_color<T,K> lowest ()	{ return sx::cast<sx::rgba_color<T,K>>(std::numeric_limits<T>::lowest()); }
	};
}

namespace sx {
	#if SXAPI_COCOA || SXAPI_COCOA_TOUCH
		template<typename T> inline CGPoint cgpoint (const sx::vec<T,2>& v) {
			return CGPointMake(float(v.x), float(v.y));
		}
		template<typename T> inline CGSize cgsize (const sx::vec<T,2>& v) {
			return CGSizeMake(float(v.x), float(v.y));
		}
	#endif
	#if SXAPI_COCOA
		template<typename T> inline NSPoint nspoint (const sx::vec<T,2>& v) {
			return NSMakePoint(float(v.x), float(v.y));
		}
		template<typename T> inline NSSize nssize (const sx::vec<T,2>& v) {
			return NSMakeSize(float(v.x), float(v.y));
		}
	#endif
}
namespace sx {
	namespace mm {
		using vec2		= sx::vec<vector, 2>;
		using vec3		= sx::vec<vector, 3>;
		using vec4		= sx::vec<vector, 4>;

		inline sx::mm::vector make_vector (const float f) {
			return _mm_set_ps1(f);
		}
		inline sx::mm::vector make_vector (const float x, const float y) {
			return _mm_setr_ps(x, y, 0.0f, 0.0f);
		}
		inline sx::mm::vector make_vector (const float x, const float y, const float z) {
			return _mm_setr_ps(x, y, z, 0.0f);
		}
		inline sx::mm::vector make_vector (const float x, const float y, const float z, const float w) {
			return _mm_setr_ps(x, y, z, w);
		}
		inline sx::mm::vector make_vector (const float *const p) {
			return _mm_loadu_ps(p);
		}

		inline sx::mm::vector make_vector (const vec<float,2>& v, const float z = 0.0f, const float w = 0.0f) {
			return _mm_setr_ps(v.x, v.y, z, w);
		}
		inline sx::mm::vector make_vector (const vec<float,2>& v0, const vec<float,2>& v1) {
			return _mm_setr_ps(v0.x, v0.y, v1.x, v1.y);
		}
		inline sx::mm::vector make_vector (const vec<float,3>& v, const float w = 0.0f) {
			return _mm_setr_ps(v.x, v.y, v.z, w);
		}
		inline sx::mm::vector make_vector (const vec<float,4>& v) {
			#if SXSIMD && __ARM_NEON__
				return vld1q_f32((const float32_t *)(&v));
			#else
				return _mm_setr_ps(v.x, v.y, v.z, v.w);
			#endif
		}
		inline sx::mm::vector make_vector (const rgb_color<float>& v, const float alpha = 0.0f) {
			return _mm_setr_ps(v.red, v.green, v.blue, alpha);
		}
		inline sx::mm::vector make_vector (const rgba_color<float>& v) {
			return _mm_setr_ps(v.red, v.green, v.blue, v.alpha);
		}

		inline sx::mm::vector make_vector (const vec<int,2>& v0) {
			#if SXSIMD
				#if __ARM_NEON__
					return _mm_setr_ps(float(v0.x), float(v0.y), 0.0f, 0.0f);
				#else
					#if (2 <= SXSIMD)
						return _mm_cvtepi32_ps(_mm_setr_epi32(v0.x, v0.y, 0, 0));
					#else
						const sx::mm::vector v = _mm_cvt_si2ss(_mm_setzero_ps(), v0.y);
						return _mm_cvt_si2ss(_mm_unpacklo_ps(v, v), v0.x);
					#endif
				#endif
			#else
				sx::mm::vector v; v[0] = float(v0.x); v[1] = float(v0.y); v[2] = 0.0f; v[3] = 0.0f; return v;
			#endif
		}
		inline sx::mm::vector make_vector (const vec<int,2>& v0, const vec<int,2>& v1) {
			#if SXSIMD
				#if __ARM_NEON__
					return _mm_setr_ps(float(v0.x), float(v0.y), float(v1.x), float(v1.y));
				#else
					#if (2 <= SXSIMD)
						return _mm_cvtepi32_ps(_mm_setr_epi32(v0.x, v0.y, v1.x, v1.y));
					#else
						sx::mm::vector h;
						h = _mm_cvt_si2ss(h, v1.y);
						h = _mm_cvt_si2ss(_mm_unpacklo_ps(h, h), v1.x);
						sx::mm::vector l;
						l = _mm_cvt_si2ss(l, v0.y);
						return _mm_cvt_si2ss(_mm_movelh_ps(_mm_unpacklo_ps(l, l), h), v0.x);
					#endif
				#endif
			#else
				sx::mm::vector v; v[0] = float(v0.x); v[1] = float(v0.y); v[2] = float(v1.x); v[3] = float(v1.y); return v;
			#endif
			//return _mm_setr_ps(v0.x, v0.y, v1.x, v1.y);
		}
		inline sx::mm::vector make_vector (const vec<int,3>& v0) {
			#if SXSIMD
				#if __ARM_NEON__
					return _mm_setr_ps(float(v0.x), float(v0.y), float(v0.z), 0.0f);
				#else
					#if (2 <= SXSIMD)
						return _mm_cvtepi32_ps(_mm_setr_epi32(v0.x, v0.y, v0.z, 0));
					#else
						sx::mm::vector l;
						l = _mm_cvt_si2ss(l, v0.y);
						return _mm_cvt_si2ss(_mm_movelh_ps(_mm_unpacklo_ps(l, l), _mm_cvt_si2ss(_mm_setzero_ps(), v0.z)), v0.x);
					#endif
				#endif
			#else
				sx::mm::vector v; v[0] = float(v0.x); v[1] = float(v0.y); v[2] = float(v0.z); v[3] = 0.0f; return v;
			#endif
		}
		inline sx::mm::vector make_vector (const vec<int,4>& v0) {
			#if SXSIMD
				#if __ARM_NEON__
					return _mm_setr_ps(float(v0.x), float(v0.y), float(v0.z), float(v0.w));
				#else
					#if (2 <= SXSIMD)
						return _mm_cvtepi32_ps(_mm_setr_epi32(v0.x, v0.y, v0.z, v0.w));
					#else
						sx::mm::vector h;
						h = _mm_cvt_si2ss(h, v0.w);
						h = _mm_cvt_si2ss(_mm_unpacklo_ps(h, h), v0.z);
						sx::mm::vector l;
						l = _mm_cvt_si2ss(l, v0.y);
						return _mm_cvt_si2ss(_mm_movelh_ps(_mm_unpacklo_ps(l, l), h), v0.x);
					#endif
				#endif
			#else
				sx::mm::vector v; v[0] = float(v0.x); v[1] = float(v0.y); v[2] = float(v0.z); v[3] = float(v0.w); return v;
			#endif
		}
		inline sx::mm::scalar make_scalar (const float f) {
			return _mm_set_ss(f);
		}

		namespace {
			const sx::mm::vector vone		= sx::mm::make_vector(1.0f);
			const sx::mm::vector vhalf		= sx::mm::make_vector(0.5f);
			const sx::mm::vector vnzero		= sx::mm::make_vector(-0.0f);
			const sx::mm::vector vinfinity	= sx::mm::make_vector(std::numeric_limits<float>::infinity());
			const sx::mm::vector vmax		= sx::mm::make_vector(std::numeric_limits<float>::max());
			const sx::mm::vector vmin		= sx::mm::make_vector(std::numeric_limits<float>::min());
			const sx::mm::vector vlowest	= sx::mm::make_vector(std::numeric_limits<float>::lowest());
			const sx::mm::vector vepsilon	= sx::mm::make_vector(1e-37f); // note that _mm_rsqrt_ps(sx::mm::scalar(1e-38)) returns #INF. should not use hard-coded magic number here. should be removed.
			const sx::mm::vector vmask1		= sx::mm::make_vector(1.0f,0.0f,0.0f,0.0f);
			const sx::mm::vector vmask2		= sx::mm::make_vector(1.0f,1.0f,0.0f,0.0f);
			const sx::mm::vector vmask3		= sx::mm::make_vector(1.0f,1.0f,1.0f,0.0f);

			const sx::mm::scalar sone		= sx::mm::make_scalar(1.0f);
			const sx::mm::scalar shalf		= sx::mm::make_scalar(0.5f);
			const sx::mm::scalar snzero		= sx::mm::make_scalar(-0.0f);
			const sx::mm::scalar sepsilon	= sx::mm::make_scalar(1e-37f); // note that _mm_rsqrt_ps(sx::mm::scalar(1e-38)) returns #INF. should not use hard-coded magic number here. should be removed.
			const sx::mm::scalar sinfinity	= sx::mm::make_scalar(std::numeric_limits<float>::infinity());
			const sx::mm::scalar smax		= sx::mm::make_scalar(std::numeric_limits<float>::max());
			const sx::mm::scalar smin		= sx::mm::make_scalar(std::numeric_limits<float>::min());
			const sx::mm::scalar slowest	= sx::mm::make_scalar(std::numeric_limits<float>::lowest());
		}
	}
	inline int movemask (const sx::mm::vector& a) {
		return _mm_movemask_ps(a);
	}
	template<int i0, int i1, int i2, int i3> inline sx::mm::vector shuffle (const sx::mm::vector& a) {
		#if SXSIMD
			#if __ARM_NEON__
				sx::mm::vector v; v[0] = a[i0]; v[1] = a[i1]; v[2] = a[i2]; v[3] = a[i3]; return v;
				//return _mm_setr_ps(vgetq_lane_f32(a,i0), vgetq_lane_f32(a,i1), vgetq_lane_f32(a,i2), vgetq_lane_f32(a,i3));
			#else
				return _mm_shuffle_ps(a, a, _MM_SHUFFLE(i3,i2,i1,i0));
			#endif
		#else
			sx::mm::vector v; v[0] = a[i0]; v[1] = a[i1]; v[2] = a[i2]; v[3] = a[i3]; return v;
		#endif
	}
	template<int i0, int i1, int i2, int i3> inline sx::mm::vector shuffle (const sx::mm::vector& a, const sx::mm::vector& b) {
		#if SXSIMD
			#if __ARM_NEON__
				sx::mm::vector v; v[0] = a[i0]; v[1] = a[i1]; v[2] = b[i2]; v[3] = b[i3]; return v;
				//return _mm_setr_ps(vgetq_lane_f32(a,i0), vgetq_lane_f32(a,i1), vgetq_lane_f32(b,i2), vgetq_lane_f32(b,i3));
			#else
				return _mm_shuffle_ps(a, b, _MM_SHUFFLE(i3,i2,i1,i0));
			#endif
		#else
			sx::mm::vector v; v[0] = a[i0]; v[1] = a[i1]; v[2] = b[i2]; v[3] = b[i3]; return v;
		#endif
	}
	template<int i> inline sx::mm::vector splat (const sx::mm::vector& a) {
		#if SXSIMD
			#if __ARM_NEON__
				sx::mm::vector v; v[0] = a[i]; v[1] = a[i]; v[2] = a[i]; v[3] = a[i]; return v;
				//return vdupq_n_f32(vgetq_lane_f32(a, i));
			#else
				return _mm_shuffle_ps(a, a, _MM_SHUFFLE(i,i,i,i));
			#endif
		#else
			sx::mm::vector v; v[0] = a[i]; v[1] = a[i]; v[2] = a[i]; v[3] = a[i]; return v;
		#endif
	}
	inline sx::mm::vector unpacklo (const sx::mm::vector& a, const sx::mm::vector& b) {
		#if SXSIMD
			#if __ARM_NEON__
				return _mm_setr_ps(vgetq_lane_f32(a,0), vgetq_lane_f32(b,0), vgetq_lane_f32(a,1), vgetq_lane_f32(b,1));
			#else
				return _mm_unpacklo_ps(a, b);
			#endif
		#else
			sx::mm::vector v; v[0] = a[0]; v[1] = b[0]; v[2] = a[1]; v[3] = b[1]; return v;
		#endif
	}
	inline sx::mm::vector unpackhi (const sx::mm::vector& a, const sx::mm::vector& b) {
		#if SXSIMD
			#if __ARM_NEON__
				return _mm_setr_ps(vgetq_lane_f32(a,2), vgetq_lane_f32(b,2), vgetq_lane_f32(a,3), vgetq_lane_f32(b,3));
			#else
				return _mm_unpackhi_ps(a, b);
			#endif
		#else
			sx::mm::vector v; v[0] = a[2]; v[1] = b[2]; v[2] = a[3]; v[3] = b[3]; return v;
		#endif
	}
	inline sx::mm::vector movehl (const sx::mm::vector& a, const sx::mm::vector& b) {
		#if SXSIMD
			#if __ARM_NEON__
				return _mm_setr_ps(vgetq_lane_f32(a,2), vgetq_lane_f32(a,3), vgetq_lane_f32(b,2), vgetq_lane_f32(b,3));
			#else
				return _mm_movehl_ps(b, a);
			#endif
		#else
			sx::mm::vector v; v[0] = a[2]; v[1] = a[3]; v[2] = b[2]; v[3] = b[3]; return v;
		#endif
	}
	inline sx::mm::vector movelh (const sx::mm::vector& a, const sx::mm::vector& b) {
		#if SXSIMD
			#if __ARM_NEON__
				return _mm_setr_ps(vgetq_lane_f32(a,0), vgetq_lane_f32(a,1), vgetq_lane_f32(b,0), vgetq_lane_f32(b,1));
			#else
				return _mm_movelh_ps(a, b);
			#endif
		#else
			sx::mm::vector v; v[0] = a[0]; v[1] = a[1]; v[2] = b[0]; v[3] = b[1]; return v;
		#endif
	}
	inline sx::mm::vector move_ss (const sx::mm::vector& a, const sx::mm::scalar& b) {
		#if SXSIMD
			#if __ARM_NEON__
				return vsetq_lane_f32(vgetq_lane_f32(b,0), a, 0);
			#else
				return _mm_move_ss(a, b);
			#endif
		#else
			sx::mm::vector v; v[0] = b[0]; v[1] = a[1]; v[2] = a[2]; v[3] = a[3]; return v;
		#endif
	}

	inline sx::mm::vector sqr (const sx::mm::vector& a) {
		return _mm_mul_ps(a, a);
	}
	inline sx::mm::scalar sqrs (const sx::mm::scalar& a) {
		return _mm_mul_ss(a, a);
	}

	inline int ltmask (const sx::mm::vector& a, const sx::mm::vector& b) {
		#if SXSIMD
			return _mm_movemask_ps(_mm_cmplt_ps(a, b));
		#else
			return int(a[0]<b[0]) | (int(a[1]<b[1])<<1) | (int(a[2]<b[2])<<2) | (int(a[3]<b[3])<<3);
		#endif
	}
	inline int lemask (const sx::mm::vector& a, const sx::mm::vector& b) {
		#if SXSIMD
			return _mm_movemask_ps(_mm_cmple_ps(a, b));
		#else
			return int(a[0]<=b[0]) | (int(a[1]<=b[1])<<1) | (int(a[2]<=b[2])<<2) | (int(a[3]<=b[3])<<3);
		#endif
	}
	inline int eqmask (const sx::mm::vector& a, const sx::mm::vector& b) {
		#if SXSIMD
			return _mm_movemask_ps(_mm_cmpeq_ps(a, b));
		#else
			return int(a[0]==b[0]) | (int(a[1]==b[1])<<1) | (int(a[2]==b[2])<<2) | (int(a[3]==b[3])<<3);
		#endif
	}
	inline int ltmasks (const sx::mm::scalar& a, const sx::mm::scalar& b) {
		#if SXSIMD
			#if __ARM_NEON__
				return (vgetq_lane_f32(a,0) < vgetq_lane_f32(b,0));
			#else
				return (_mm_movemask_ps(_mm_cmplt_ss(a, b)) & 0x01);
			#endif
		#else
			return (a[0]<b[0]);
		#endif
	}
	inline int lemasks (const sx::mm::scalar& a, const sx::mm::scalar& b) {
		#if SXSIMD
			#if __ARM_NEON__
				return (vgetq_lane_f32(a,0) <= vgetq_lane_f32(b,0));
			#else
				return (_mm_movemask_ps(_mm_cmple_ss(a, b)) & 0x01);
			#endif
		#else
			return (a[0]<=b[0]);
		#endif
	}
	inline int eqmasks (const sx::mm::scalar& a, const sx::mm::scalar& b) {
		#if SXSIMD
			#if __ARM_NEON__
				return (vgetq_lane_f32(a,0) == vgetq_lane_f32(b,0));
			#else
				return (_mm_movemask_ps(_mm_cmpeq_ss(a, b)) & 0x01);
			#endif
		#else
			return (a[0]==b[0]);
		#endif
	}
	inline int zeromasks (const sx::mm::scalar& a) {
		#if SXSIMD
			return (_mm_movemask_ps(_mm_and_ps(_mm_cmplt_ss(sx::mm::make_scalar(-epsilon), a), _mm_cmplt_ss(a, sx::mm::make_scalar(epsilon)))) & 0x01);
		#else
			return (-epsilon < a[0] && a[0] < epsilon);
		#endif
	}

	inline sx::mm::vector ltx (const sx::mm::vector& a, const sx::mm::vector& b) {
		return _mm_cmplt_ps(a, b);
	}
	inline sx::mm::vector lex (const sx::mm::vector& a, const sx::mm::vector& b) {
		return _mm_cmple_ps(a, b);
	}
	inline sx::mm::vector eqx (const sx::mm::vector& a, const sx::mm::vector& b) {
		return _mm_cmpeq_ps(a, b);
	}
	inline sx::mm::vector andx (const sx::mm::vector& a, const sx::mm::vector& b) {
		return _mm_and_ps(a, b);
	}
	inline sx::mm::vector orx (const sx::mm::vector& a, const sx::mm::vector& b) {
		return _mm_or_ps(a, b);
	}
	inline sx::mm::vector andnotx (const sx::mm::vector& a, const sx::mm::vector& b) {
		return _mm_andnot_ps(a, b);
	}
	inline sx::mm::vector xorx (const sx::mm::vector& a, const sx::mm::vector& b) {
		return _mm_xor_ps(a, b);
	}
	inline void swapx (const sx::mm::vector& c, sx::mm::vector& a, sx::mm::vector& b) {
		const sx::mm::vector v(_mm_and_ps(_mm_xor_ps(a, b), c));
		a = _mm_xor_ps(v, a);
		b = _mm_xor_ps(v, b);
	}
	inline sx::mm::vector conditional (const sx::mm::vector& c, const sx::mm::vector& a, const sx::mm::vector& b) {
		return _mm_or_ps(_mm_and_ps(c, a),_mm_andnot_ps(c, b));
	}
	inline sx::mm::scalar conditionals (const sx::mm::scalar& c, const sx::mm::scalar& a, const sx::mm::scalar& b) {
		return _mm_add_ss(_mm_and_ps(c, a), _mm_andnot_ps(c, b));
	}

	inline sx::mm::vector zerox (const sx::mm::vector& v) {
		return _mm_cmplt_ps(_mm_max_ps(v, _mm_sub_ps(sx::mm::vzero, v)), sx::mm::make_vector(epsilon));
	}
	inline int zeromask (const sx::mm::vector& v) {
		return sx::movemask(sx::zerox(v));
	}

	inline sx::mm::vector pack (const sx::mm::scalar& t0, const sx::mm::scalar& t1) {
		#if SXSIMD
			#if __ARM_NEON__
				return _mm_setr_ps(vgetq_lane_f32(t0,0), vgetq_lane_f32(t1,0), 0.0f, 0.0f);
			#else
				return _mm_unpacklo_ps(t0, t1);
			#endif
		#else
			sx::mm::vector v;
			v[0] = t0[0]; v[1] = t1[0]; v[2] = 0.0f; v[3] = 0.0f;
			return v;
		#endif
	}
	inline sx::mm::vector pack (const sx::mm::scalar& t0, const sx::mm::scalar& t1, const sx::mm::scalar& t2) {
		#if SXSIMD
			#if __ARM_NEON__
				return _mm_setr_ps(vgetq_lane_f32(t0,0), vgetq_lane_f32(t1,0), vgetq_lane_f32(t2,0), 0.0f);
			#else
				return _mm_movelh_ps(_mm_unpacklo_ps(t0, t1), t2);
			#endif
		#else
			sx::mm::vector v;
			v[0] = t0[0]; v[1] = t1[0]; v[2] = t2[0]; v[3] = 0.0f;
			return v;
		#endif
	}
	inline sx::mm::vector pack (const sx::mm::scalar& t0, const sx::mm::scalar& t1, const sx::mm::scalar& t2, const sx::mm::scalar& t3) {
		#if SXSIMD
			#if __ARM_NEON__
				return _mm_setr_ps(vgetq_lane_f32(t0,0), vgetq_lane_f32(t1,0), vgetq_lane_f32(t2,0), vgetq_lane_f32(t3,0));
			#else
				return _mm_movelh_ps(_mm_unpacklo_ps(t0, t1), _mm_unpacklo_ps(t2, t3));
			#endif
		#else
			sx::mm::vector v;
			v[0] = t0[0]; v[1] = t1[0]; v[2] = t2[0]; v[3] = t3[0];
			return v;
		#endif
	}
	inline sx::mm::vector packx (const sx::mm::vector& t0, const sx::mm::vector& t1, const sx::mm::vector& t2, const sx::mm::vector& t3) {
		#if SXSIMD
			#if __ARM_NEON__
				return _mm_setr_ps(vgetq_lane_f32(t0,0), vgetq_lane_f32(t1,0), vgetq_lane_f32(t2,0), vgetq_lane_f32(t3,0));
			#else
				return _mm_movelh_ps(_mm_unpacklo_ps(t0, t1), _mm_unpacklo_ps(t2, t3));
			#endif
		#else
			sx::mm::vector v;
			v[0] = t0[0]; v[1] = t1[0]; v[2] = t2[0]; v[3] = t3[0];
			return v;
		#endif
	}
	inline sx::mm::vector packy (const sx::mm::vector& t0, const sx::mm::vector& t1, const sx::mm::vector& t2, const sx::mm::vector& t3) {
		#if SXSIMD
			#if __ARM_NEON__
				return _mm_setr_ps(vgetq_lane_f32(t0,1), vgetq_lane_f32(t1,1), vgetq_lane_f32(t2,1), vgetq_lane_f32(t3,1));
			#else
				return _mm_movehl_ps(_mm_unpacklo_ps(t2, t3), _mm_unpacklo_ps(t0, t1));
			#endif
		#else
			sx::mm::vector v;
			v[0] = t0[1]; v[1] = t1[1]; v[2] = t2[1]; v[3] = t3[1];
			return v;
		#endif
	}
	inline sx::mm::vector packz (const sx::mm::vector& t0, const sx::mm::vector& t1, const sx::mm::vector& t2, const sx::mm::vector& t3) {
		#if SXSIMD
			#if __ARM_NEON__
				return _mm_setr_ps(vgetq_lane_f32(t0,2), vgetq_lane_f32(t1,2), vgetq_lane_f32(t2,2), vgetq_lane_f32(t3,2));
			#else
				return _mm_movelh_ps(_mm_unpackhi_ps(t0, t1), _mm_unpackhi_ps(t2, t3));
			#endif
		#else
			sx::mm::vector v;
			v[0] = t0[2]; v[1] = t1[2]; v[2] = t2[2]; v[3] = t3[2];
			return v;
		#endif
	}
	inline sx::mm::vector packw (const sx::mm::vector& t0, const sx::mm::vector& t1, const sx::mm::vector& t2, const sx::mm::vector& t3) {
		#if SXSIMD
			#if __ARM_NEON__
				return _mm_setr_ps(vgetq_lane_f32(t0,3), vgetq_lane_f32(t1,3), vgetq_lane_f32(t2,3), vgetq_lane_f32(t3,3));
			#else
				return _mm_movehl_ps(_mm_unpackhi_ps(t2, t3), _mm_unpackhi_ps(t0, t1));
			#endif
		#else
			sx::mm::vector v;
			v[0] = t0[3]; v[1] = t1[3]; v[2] = t2[3]; v[3] = t3[3];
			return v;
		#endif
	}

	template<> inline sx::mm::vector sqrt (const sx::mm::vector& t) {
		return sx::conditional(_mm_cmplt_ps(sx::mm::vmax,t), sx::mm::vinfinity, _mm_sqrt_ps(t));
	}
	inline sx::mm::scalar sqrts (const sx::mm::scalar& t) {
		return sx::conditional(_mm_cmplt_ss(sx::mm::smax,t), sx::mm::sinfinity, _mm_sqrt_ps(t));
	}

	inline sx::mm::vector rsqrt (const sx::mm::vector& v) {
		#if SXSIMD
			#if __ARM_NEON__
				// should be reviewed. see SSE code below.
				const sx::mm::vector y0 = vrsqrteq_f32(v);
				const sx::mm::vector t0 = _mm_mul_ps(y0, y0);
				const sx::mm::vector t1 = _mm_mul_ps(y0, sx::mm::vhalf);
				return _mm_add_ps(_mm_mul_ps(_mm_sub_ps(sx::mm::vone, _mm_mul_ps(v, t0)), t1), y0);
			#else
				const __m128 y0	= _mm_rsqrt_ps(v);																// return y0;
				const __m128 t0	= _mm_mul_ps(y0, y0);
				const __m128 t1	= _mm_mul_ps(y0, sx::mm::vhalf);
				const __m128 q	= _mm_add_ps(_mm_mul_ps(_mm_sub_ps(sx::mm::vone, _mm_mul_ps(v, t0)), t1), y0);	// return q;
//				const __m128 c	= _mm_cmpeq_ps(v,_mm_xor_ps(q,q)); return _mm_or_ps(_mm_and_ps(c, y0),_mm_andnot_ps(c, q));	// return inf. when zero is given.
				return sx::conditional(_mm_cmplt_ps(sx::mm::vmax,v), sx::mm::vzero, sx::conditional(_mm_cmpeq_ps(v,sx::mm::vzero), y0, q));	// return inf. when zero is given.
			#endif
		#else
			sx::mm::vector u;
			for (int i = 0; i < 4; ++i) u[i] = 1.0f / std::sqrt(v[i]);
			return u;
		#endif
	}
	inline sx::mm::scalar rsqrts (const sx::mm::scalar& v) {
		#if SXSIMD
			#if __ARM_NEON__
				sx::mm::vector_union v_; v_.m = v;
				sx::mm::vector_union u;
				u.f[0] = 1.0f / std::sqrt(v_.f[0]);
				for (int i = 1; i < 4; ++i) u.f[i] = v_.f[i];
				return u.m;
			#else
				const __m128 y0	= _mm_rsqrt_ss(v);																// return y0;
				const __m128 t0	= _mm_mul_ss(y0, y0);
				const __m128 t1	= _mm_mul_ss(y0, sx::mm::shalf);
				const __m128 q	= _mm_add_ss(_mm_mul_ss(_mm_sub_ss(sx::mm::sone, _mm_mul_ss(v, t0)), t1), y0);	// return q;
//				const __m128 c	= _mm_cmpeq_ss(v,_mm_xor_ps(q,q)); return _mm_or_ps(_mm_and_ps(c, y0),_mm_andnot_ps(c, q));	// return inf. when zero is given.
				return sx::conditional(_mm_cmplt_ss(sx::mm::smax,v), sx::mm::szero, sx::conditional(_mm_cmpeq_ss(v,sx::mm::szero), y0, q));	// return inf. when zero is given.
			#endif
		#else
			sx::mm::vector u;
			u[0] = 1.0f / std::sqrt(v[0]);
			for (int i = 1; i < 4; ++i) u[i] = v[i];
			return u;
		#endif
	}

	inline sx::mm::vector rcp (const sx::mm::vector& a) {
		#if SXSIMD
			#if __ARM_NEON__
				const sx::mm::vector y0 = vrecpeq_f32(a);
				return _mm_add_ps(_mm_mul_ps(y0, _mm_sub_ps(sx::mm::vone, _mm_mul_ps(y0, a))), y0);
			#else
				const __m128 y0 = _mm_rcp_ps(a);																													// return y0;
				const __m128 y1 = sx::conditional(_mm_cmpeq_ps(sx::mm::vzero,y0), y0, _mm_add_ps(_mm_mul_ps(y0, _mm_sub_ps(sx::mm::vone, _mm_mul_ps(y0, a))), y0));	// return y1;
				return sx::conditional(_mm_cmplt_ps(y0,sx::mm::vlowest), y0, sx::conditional(_mm_cmplt_ps(sx::mm::vmax,y0), y0, y1));
			#endif
		#else
			sx::mm::vector u;
			for (int i = 0; i < 4; ++i) u[i] = 1.0f / a[i];
			return u;
		#endif
	}

	inline sx::mm::scalar rcps (const sx::mm::scalar& a) {
		#if SXSIMD
			#if __ARM_NEON__
//				sx::mm::vector_union u; u.m = a;
//				sx::mm::vector_union v;
//				v.f[0] = 1.0f / u.f[0];
//				for (int i = 1; i < 4; ++i) v.f[i] = u.f[i];
//				return v.m;
				const sx::mm::vector y0 = vrecpeq_f32(a);
				return _mm_add_ps(_mm_mul_ps(y0, _mm_sub_ps(sx::mm::vone, _mm_mul_ps(y0, a))), y0);
			#else
				const __m128 y0 = _mm_rcp_ss(a);																													// return y0;
				const __m128 y1 = sx::conditional(_mm_cmpeq_ss(sx::mm::szero,y0), y0, _mm_add_ss(_mm_mul_ss(y0, _mm_sub_ss(sx::mm::sone, _mm_mul_ss(y0, a))), y0));	// return y1;
				return sx::conditional(_mm_cmplt_ss(y0,sx::mm::slowest), y0, sx::conditional(_mm_cmplt_ss(sx::mm::smax,y0), y0, y1));
			#endif
		#else
			sx::mm::vector u;
			u[0] = 1.0f / a[0];
			for (int i = 1; i < 4; ++i) u[i] = a[i];
			return u;
		#endif
	}

	inline sx::mm::vector reciprocal (const sx::mm::vector& a) { return rcp(a); }

	inline sx::mm::vector min (const sx::mm::vector& a, const sx::mm::vector& b) { // element-wise
		return _mm_min_ps(a, b);
	}
	inline sx::mm::scalar mins (const sx::mm::scalar& a, const sx::mm::scalar& b) {
		return _mm_min_ss(a, b);
	}

	inline sx::mm::vector max (const sx::mm::vector& a, const sx::mm::vector& b) { // element-wise
		return _mm_max_ps(a, b);
	}
	inline sx::mm::scalar maxs (const sx::mm::scalar& a, const sx::mm::scalar& b) {
		return _mm_max_ss(a, b);
	}

	inline sx::mm::scalar min3 (const sx::mm::vector& a) {
		const sx::mm::vector v(sx::min(a, sx::unpackhi(a, a)));	// sx::unpackhi(a): sx::shuffle<2,2,3,3>(a)
		return sx::mins(v, sx::splat<1>(v));
	}
	inline sx::mm::scalar min4 (const sx::mm::vector& a) {
		const sx::mm::vector v(sx::min(a, sx::movehl(a, a)));	// sx::movehl(a): sx::shuffle<2,3,2,3>(a)
		return sx::mins(v, sx::splat<1>(v));
	}
	inline sx::mm::vector min4 (const sx::mm::vector& a, const sx::mm::vector& b) {	// returns sx::mm::make_vector(min4(a), min4(b))
		const sx::mm::vector v(sx::min(sx::unpacklo(a, b), sx::unpackhi(a, b)));
		return sx::min(v, sx::movehl(v, v));	// sx::movehl(a): sx::shuffle<2,3,2,3>(a)
	}
	inline sx::mm::scalar max3 (const sx::mm::vector& a) {
		const sx::mm::vector v(sx::max(a, sx::unpackhi(a, a)));	// sx::unpackhi(a): sx::shuffle<2,2,3,3>(a)
		return sx::maxs(v, sx::splat<1>(v));
	}
	inline sx::mm::scalar max4 (const sx::mm::vector& a) {
		const sx::mm::vector v(sx::max(a, sx::movehl(a, a)));	// sx::movehl(a): sx::shuffle<2,3,2,3>(a)
		return sx::maxs(v, sx::splat<1>(v));
	}
	inline sx::mm::vector max4 (const sx::mm::vector& a, const sx::mm::vector& b) {	// returns sx::mm::make_vector(max4(a), max4(b))
		const sx::mm::vector v(sx::max(sx::unpacklo(a, b), sx::unpackhi(a, b)));
		return sx::max(v, sx::movehl(v, v));	// sx::movehl(a): sx::shuffle<2,3,2,3>(a)
	}

	inline sx::mm::vector abs (const sx::mm::vector& p) {
		return _mm_max_ps(p, _mm_sub_ps(sx::mm::vzero, p));
	}
	inline sx::mm::scalar abss (const sx::mm::scalar& p) {
		return _mm_max_ss(p, _mm_sub_ss(sx::mm::szero, p));
	}

	inline sx::mm::scalar sum4 (const sx::mm::vector& p) {
		sx::assert_invariant(p);
		#if SXSIMD
			#if __ARM_NEON__
				return vdupq_n_f32(vgetq_lane_f32(p,0) + vgetq_lane_f32(p,1) + vgetq_lane_f32(p,2) + vgetq_lane_f32(p,3));
			#else
				#if (3 <= SXSIMD)
					const sx::mm::vector v = _mm_hadd_ps(p,p);
					return _mm_hadd_ps(v,v);
				#else
					const sx::mm::vector q = _mm_add_ps(p, _mm_movehl_ps(p, p));
					return _mm_add_ss(q, sx::splat<1>(q));
				#endif
			#endif
		#else
			sx::mm::vector u;
			u[0] = p[0] + p[1] + p[2] + p[3];
			for (int i = 1; i < 4; ++i) u[i] = p[i];
			return u;
		#endif
	}
	inline sx::mm::vector sum4 (const sx::mm::vector& p0, const sx::mm::vector& p1, const sx::mm::vector& p2, const sx::mm::vector& p3) {
		sx::assert_invariant(p0, p1, p2, p3);
		#if SXSIMD
			#if __ARM_NEON__
				return _mm_setr_ps(
					vgetq_lane_f32(p0,0) + vgetq_lane_f32(p0,1) + vgetq_lane_f32(p0,2) + vgetq_lane_f32(p0,3),
					vgetq_lane_f32(p1,0) + vgetq_lane_f32(p1,1) + vgetq_lane_f32(p1,2) + vgetq_lane_f32(p1,3),
					vgetq_lane_f32(p2,0) + vgetq_lane_f32(p2,1) + vgetq_lane_f32(p2,2) + vgetq_lane_f32(p2,3),
					vgetq_lane_f32(p3,0) + vgetq_lane_f32(p3,1) + vgetq_lane_f32(p3,2) + vgetq_lane_f32(p3,3)
				);
			#else
				#if (3 <= SXSIMD)
					return _mm_hadd_ps(_mm_hadd_ps(p0, p1), _mm_hadd_ps(p2, p3));
				#else
					const sx::mm::vector q01 = _mm_add_ps(_mm_unpacklo_ps(p0, p1), _mm_unpackhi_ps(p0, p1));
					const sx::mm::vector q23 = _mm_add_ps(_mm_unpacklo_ps(p2, p3), _mm_unpackhi_ps(p2, p3));
					return _mm_add_ps(_mm_movelh_ps(q01, q23), _mm_movehl_ps(q23, q01));
				#endif
			#endif
		#else
			sx::mm::vector u;
			u[0] = p0[0] + p0[1] + p0[2] + p0[3];
			u[1] = p1[0] + p1[1] + p1[2] + p1[3];
			u[2] = p2[0] + p2[1] + p2[2] + p2[3];
			u[3] = p3[0] + p3[1] + p3[2] + p3[3];
			return u;
		#endif
	}
	inline sx::mm::scalar sum3 (const sx::mm::vector& p) {
		sx::assert_invariant(p);
		#if SXSIMD
			#if __ARM_NEON__
				return vdupq_n_f32(vgetq_lane_f32(p,0) + vgetq_lane_f32(p,1) + vgetq_lane_f32(p,2));
			#else
				#if (3 <= SXSIMD)
					return _mm_add_ss(_mm_hadd_ps(p,p), _mm_movehl_ps(p, p));
				#else
					return _mm_add_ss(_mm_add_ss(p, sx::splat<1>(p)), _mm_movehl_ps(p, p));
				#endif
			#endif
		#else
			sx::mm::vector u;
			u[0] = p[0] + p[1] + p[2];
			for (int i = 1; i < 4; ++i) u[i] = p[i];
			return u;
		#endif
	}
	inline sx::mm::vector sum3 (const sx::mm::vector& p0, const sx::mm::vector& p1, const sx::mm::vector& p2, const sx::mm::vector& p3) {
		sx::assert_invariant(p0, p1, p2, p3);
		#if SXSIMD
			#if __ARM_NEON__
				return _mm_setr_ps(
					vgetq_lane_f32(p0,0) + vgetq_lane_f32(p0,1) + vgetq_lane_f32(p0,2),
					vgetq_lane_f32(p1,0) + vgetq_lane_f32(p1,1) + vgetq_lane_f32(p1,2),
					vgetq_lane_f32(p2,0) + vgetq_lane_f32(p2,1) + vgetq_lane_f32(p2,2),
					vgetq_lane_f32(p3,0) + vgetq_lane_f32(p3,1) + vgetq_lane_f32(p3,2)
				);
			#else
				#if (3 <= SXSIMD)
					return _mm_add_ps(_mm_hadd_ps(_mm_movelh_ps(p0, p1), _mm_movelh_ps(p2, p3)), packz(p0, p1, p2, p3));
				#else
					const sx::mm::vector q01 = _mm_unpacklo_ps(p0, p1);
					const sx::mm::vector q23 = _mm_unpacklo_ps(p2, p3);
					return _mm_add_ps(_mm_add_ps(_mm_movelh_ps(q01, q23), _mm_movehl_ps(q23, q01)), packz(p0, p1, p2, p3));
				#endif
			#endif
		#else
			sx::mm::vector u;
			u[0] = p0[0] + p0[1] + p0[2];
			u[1] = p1[0] + p1[1] + p1[2];
			u[2] = p2[0] + p2[1] + p2[2];
			u[3] = p3[0] + p3[1] + p3[2];
			return u;
		#endif
	}
	inline sx::mm::vector sum3 (const sx::mm::vector& p0, const sx::mm::vector& p1, const sx::mm::vector& p2) {
		sx::assert_invariant(p0, p1, p2);
		#if SXSIMD
			#if __ARM_NEON__
				return _mm_setr_ps(
					vgetq_lane_f32(p0,0) + vgetq_lane_f32(p0,1) + vgetq_lane_f32(p0,2),
					vgetq_lane_f32(p1,0) + vgetq_lane_f32(p1,1) + vgetq_lane_f32(p1,2),
					vgetq_lane_f32(p2,0) + vgetq_lane_f32(p2,1) + vgetq_lane_f32(p2,2),
					0.0f
				);
			#else
				#if (3 <= SXSIMD)
					return _mm_add_ps(_mm_hadd_ps(_mm_movelh_ps(p0, p1), p2), sx::shuffle<0, 1, 2, 3>(_mm_unpackhi_ps(p0, p1), p2));
				#else
					const sx::mm::vector q01 = _mm_unpacklo_ps(p0, p1);
					return _mm_add_ps(_mm_add_ps(_mm_movelh_ps(q01, p2), sx::shuffle<2, 3, 1, 0>(q01, p2)), sx::shuffle<0, 1, 2, 3>(_mm_unpackhi_ps(p0, p1), p2));
				#endif
			#endif
		#else
			sx::mm::vector u;
			u[0] = p0[0] + p0[1] + p0[2];
			u[1] = p1[0] + p1[1] + p1[2];
			u[2] = p2[0] + p2[1] + p2[2];
			u[3] = 0.0f;
			return u;
		#endif
	}
	inline sx::mm::scalar sum2 (const sx::mm::vector& p) {
		#if SXSIMD
			#if __ARM_NEON__
				return vdupq_n_f32(vgetq_lane_f32(p,0) + vgetq_lane_f32(p,1));
			#else
				#if (3 <= SXSIMD)
					return _mm_hadd_ps(p,p);
				#else
					return _mm_add_ss(p, sx::splat<1>(p));
				#endif
			#endif
		#else
			sx::mm::vector u;
			u[0] = p[0] + p[1];
			for (int i = 1; i < 4; ++i) u[i] = p[i];
			return u;
		#endif
	}
	inline sx::mm::scalar inner_product4 (const sx::mm::vector& a, const sx::mm::vector& b) {
		return sum4(_mm_mul_ps(a, b));
	}
	inline sx::mm::vector inner_product4 (const sx::mm::vector& a0, const sx::mm::vector& b0, const sx::mm::vector& a1, const sx::mm::vector& b1, const sx::mm::vector& a2, const sx::mm::vector& b2, const sx::mm::vector& a3, const sx::mm::vector& b3) {
		return sum4(_mm_mul_ps(a0, b0), _mm_mul_ps(a1, b1), _mm_mul_ps(a2, b2), _mm_mul_ps(a3, b3));
	}
	inline sx::mm::vector inner_product4 (const sx::mm::vector& a0, const sx::mm::vector& b0, const sx::mm::vector& a1, const sx::mm::vector& b1, const sx::mm::vector& a2, const sx::mm::vector& b2) {
		return sum4(_mm_mul_ps(a0, b0), _mm_mul_ps(a1, b1), _mm_mul_ps(a2, b2), sx::mm::vzero);
	}
	inline sx::mm::scalar inner_product3 (const sx::mm::vector& a, const sx::mm::vector& b) {
		sx::assert_invariant(a, b);
		return sum3(_mm_mul_ps(a, b));
	}
	inline sx::mm::vector inner_product3 (const sx::mm::vector& a0, const sx::mm::vector& b0, const sx::mm::vector& a1, const sx::mm::vector& b1, const sx::mm::vector& a2, const sx::mm::vector& b2, const sx::mm::vector& a3, const sx::mm::vector& b3) {
		return sum3(_mm_mul_ps(a0, b0), _mm_mul_ps(a1, b1), _mm_mul_ps(a2, b2), _mm_mul_ps(a3, b3));
	}
	inline sx::mm::vector inner_product3 (const sx::mm::vector& a0, const sx::mm::vector& b0, const sx::mm::vector& a1, const sx::mm::vector& b1, const sx::mm::vector& a2, const sx::mm::vector& b2) {
		return sum3(_mm_mul_ps(a0, b0), _mm_mul_ps(a1, b1), _mm_mul_ps(a2, b2));
	}

	inline sx::mm::vector product (const sx::mm::vector& a, const sx::mm::vector& b) {
		const sx::mm::vector ayzx = sx::shuffle<1,2,0, 3>(a);
		const sx::mm::vector bzxy = sx::shuffle<2,0,1, 3>(b);
		const sx::mm::vector azxy = sx::shuffle<2,0,1, 3>(a);
		const sx::mm::vector byzx = sx::shuffle<1,2,0, 3>(b);
		return _mm_sub_ps(_mm_mul_ps(ayzx, bzxy), _mm_mul_ps(azxy, byzx));
	}

	inline sx::mm::vector normalize (const sx::mm::vector& p) {
//		const sx::mm::scalar a = inner_product3(p, p);
//		#if SXSIMD
//			return _mm_mul_ps(p, sx::splat<0>(rsqrts(sx::conditionals(_mm_cmplt_ss(sx::mm::sepsilon, a), a, sx::mm::sone))));
//		#else
//			return lts(sx::mm::sepsilon, a) ? _mm_mul_ps(p, sx::splat<0>(rsqrts(a))) : p;
//		#endif
		const sx::mm::scalar q = rsqrts(inner_product3(p, p));
		return _mm_mul_ps(p, sx::splat<0>(sx::conditionals(_mm_cmple_ss(q,sx::mm::smax), q, sx::mm::sone)));
	}
	inline sx::mm::scalar normalize_self (sx::mm::vector& p) {
//		const sx::mm::scalar a = inner_product3(p, p);
//		if (lts(a, sx::mm::sepsilon)) return a;
//		const sx::mm::scalar b = rsqrts(a);
//		p = _mm_mul_ps(p, sx::splat<0>(b));
//		return rcps(b);
		const sx::mm::scalar q = rsqrts(inner_product3(p, p));
		p = _mm_mul_ps(p, sx::splat<0>(sx::conditionals(_mm_cmple_ss(q,sx::mm::smax), q, sx::mm::sone)));
		return rcps(q);
	}

	inline sx::mm::scalar sqrabsolute3 (const sx::mm::vector& p) {
		return inner_product3(p, p);
	}
	inline sx::mm::scalar sqrabsolute4 (const sx::mm::vector& p) {
		return inner_product4(p, p);
	}

	inline sx::mm::scalar absolute3 (const sx::mm::vector& p) {
		const sx::mm::scalar a = inner_product3(p, p);
		#if SXSIMD
			return sx::conditionals(_mm_cmplt_ss(sx::mm::sepsilon, a), sqrts(a), a);
		#else
			return lts(sx::mm::sepsilon, a) ? sqrts(a) : a;
		#endif
	}
	inline sx::mm::scalar absolute4 (const sx::mm::vector& p) {
		const sx::mm::scalar a = inner_product4(p, p);
		#if SXSIMD
			return sx::conditionals(_mm_cmplt_ss(sx::mm::sepsilon, a), sqrts(a), a);
		#else
			return lts(sx::mm::sepsilon, a) ? sqrts(a) : a;
		#endif
	}

	inline sx::mm::vector transform44 (const sx::mm::vector& p, const sx::mm::vector& t0, const sx::mm::vector& t1, const sx::mm::vector& t2, const sx::mm::vector& t3) {
		return _mm_add_ps(_mm_mul_ps(sx::splat<0>(p), t0), _mm_add_ps(_mm_mul_ps(sx::splat<1>(p), t1), _mm_add_ps(_mm_mul_ps(sx::splat<2>(p), t2), _mm_mul_ps(sx::splat<3>(p), t3))));
	}
	inline sx::mm::vector transform44 (const sx::mm::vector& p, const sx::mm::matrix& t) {
		return transform44(p, t[0], t[1], t[2], t[3]);
	}
	inline sx::mm::vector transform44 (const sx::mm::matrix& it, const sx::mm::vector& p) {
		return sx::sum4(_mm_mul_ps(it[0], p), _mm_mul_ps(it[1], p), _mm_mul_ps(it[2], p), _mm_mul_ps(it[3], p));
	}
	inline sx::mm::vector transform33 (const sx::mm::vector& p, const sx::mm::vector& t0, const sx::mm::vector& t1, const sx::mm::vector& t2, const sx::mm::vector& t3) {
		SXTEST((check_invariant(p), check_invariant(t0), check_invariant(t1), check_invariant(t2), check_invariant(t3)));
		const sx::mm::vector q = _mm_mul_ps(_mm_add_ps(_mm_mul_ps(sx::splat<0>(p), t0), _mm_add_ps(_mm_mul_ps(sx::splat<1>(p), t1), _mm_add_ps(_mm_mul_ps(sx::splat<2>(p), t2), t3))), sx::mm::vmask3);
		sxassert(sx::check3(q));
		return q;
	}
	inline sx::mm::vector transform33 (const sx::mm::vector& p, const sx::mm::matrix& t) {
		return transform33(p, t[0], t[1], t[2], t[3]);
	}
	inline sx::mm::vector transform22 (const sx::mm::vector& p, const sx::mm::vector& t0, const sx::mm::vector& t1, const sx::mm::vector& t2, const sx::mm::vector& t3) {
		sxassert(sx::check2(p));
		const sx::mm::vector q = _mm_mul_ps(_mm_add_ps(_mm_mul_ps(sx::splat<0>(p), t0), _mm_add_ps(_mm_mul_ps(sx::splat<1>(p), t1), t3)), sx::mm::vmask2);
		sxassert(sx::check2(q));
		return q;
	}
	inline sx::mm::vector transform22 (const sx::mm::vector& p, const sx::mm::matrix& t) {
		return transform33(p, t[0], t[1], t[2], t[3]);
	}

	inline sx::mm::vector transform_direction (const sx::mm::vector& p, const sx::mm::vector& t0, const sx::mm::vector& t1, const sx::mm::vector& t2) {
		SXTEST((check_invariant(p), check_invariant(t0), check_invariant(t1), check_invariant(t2)));
		const sx::mm::vector q = _mm_add_ps(_mm_mul_ps(sx::splat<0>(p), t0), _mm_add_ps(_mm_mul_ps(sx::splat<1>(p), t1), _mm_mul_ps(sx::splat<2>(p), t2)));
		sxassert(sx::check3(q));
		return q;
	}
	inline sx::mm::vector transform_direction (const sx::mm::vector& p, const sx::mm::matrix& t) {
		return transform_direction(p, t[0], t[1], t[2]);
	}

	inline sx::mm::vector transform_normal (const sx::mm::matrix& it, const sx::mm::vector& p) {
		sxassert(sx::check3(p));
		return inner_product3(it[0], p, it[1], p, it[2], p);
	}

	inline sx::mm::matrix transpose (const sx::mm::matrix& t) {
		#if SXSIMD
			#if __ARM_NEON__
				sx::mm::matrix m;
				m[0] = sx::mm::make_vector(vgetq_lane_f32(t[0], 0), vgetq_lane_f32(t[1], 0), vgetq_lane_f32(t[2], 0), vgetq_lane_f32(t[3], 0));
				m[1] = sx::mm::make_vector(vgetq_lane_f32(t[0], 1), vgetq_lane_f32(t[1], 1), vgetq_lane_f32(t[2], 1), vgetq_lane_f32(t[3], 1));
				m[2] = sx::mm::make_vector(vgetq_lane_f32(t[0], 2), vgetq_lane_f32(t[1], 2), vgetq_lane_f32(t[2], 2), vgetq_lane_f32(t[3], 2));
				m[3] = sx::mm::make_vector(vgetq_lane_f32(t[0], 3), vgetq_lane_f32(t[1], 3), vgetq_lane_f32(t[2], 3), vgetq_lane_f32(t[3], 3));
				return m;
			#else
				sx::mm::matrix m(t);
				_MM_TRANSPOSE4_PS(m[0], m[1], m[2], m[3]);
				return m;
			#endif
		#else
			sx::mm::matrix m;
			for (int i = 0; i < t.size(); ++i) m[i] = sx::mm::make_vector(t[0][i], t[1][i], t[2][i], t[3][i]);
			return m;
		#endif
	}
	inline const sx::mm::matrix& transpose_self (sx::mm::matrix& t) {
		#if SXSIMD
			#if __ARM_NEON__
				const sx::mm::vector v0 = t[0];
				const sx::mm::vector v1 = t[1];
				const sx::mm::vector v2 = t[2];
				const sx::mm::vector v3 = t[3];
				t[0] = sx::mm::make_vector(vgetq_lane_f32(v0, 0), vgetq_lane_f32(v1, 0), vgetq_lane_f32(v2, 0), vgetq_lane_f32(v3, 0));
				t[1] = sx::mm::make_vector(vgetq_lane_f32(v0, 1), vgetq_lane_f32(v1, 1), vgetq_lane_f32(v2, 1), vgetq_lane_f32(v3, 1));
				t[2] = sx::mm::make_vector(vgetq_lane_f32(v0, 2), vgetq_lane_f32(v1, 2), vgetq_lane_f32(v2, 2), vgetq_lane_f32(v3, 2));
				t[3] = sx::mm::make_vector(vgetq_lane_f32(v0, 3), vgetq_lane_f32(v1, 3), vgetq_lane_f32(v2, 3), vgetq_lane_f32(v3, 3));
			#else
				_MM_TRANSPOSE4_PS(t[0], t[1], t[2], t[3]);
			#endif
		#else
			sx::mm::matrix m;
			for (int i = 0; i < t.size(); ++i) m[i] = sx::mm::make_vector(t[0][i], t[1][i], t[2][i], t[3][i]);
			t = m;
		#endif
		return t;
	}
	inline sx::mm::scalar det (const sx::mm::vector& t0, const sx::mm::vector& t1, const sx::mm::vector& t2, const sx::mm::vector& t3) {
		return sum3(_mm_mul_ps(t0, sx::shuffle<1,2,0, 3>(_mm_sub_ps(_mm_mul_ps(t1, sx::shuffle<1,2,0, 3>(t2)), _mm_mul_ps(sx::shuffle<1,2,0, 3>(t1), t2)))));
		//return sum3(_mm_sub_ps(	_mm_mul_ps(_mm_mul_ps(t0, sx::shuffle<1,2,0, 3>(t1)), sx::shuffle<2,0,1, 3>(t2)),
		//							_mm_mul_ps(_mm_mul_ps(t0, sx::shuffle<2,0,1, 3>(t1)), sx::shuffle<1,2,0, 3>(t2))
		//			));
	}
	inline sx::mm::scalar det (const sx::mm::matrix& t) {
		return det(t[0], t[1], t[2], t[3]);
	}
	const sx::mm::matrix inv (const sx::mm::matrix& t);
	const sx::mm::matrix inverse (const sx::mm::matrix& t);

	inline sx::mm::scalar divs (const sx::mm::scalar& a, const sx::mm::scalar& b) {
		return _mm_mul_ss(a, rcps(b));
	}

	inline void cat (sx::mm::matrix& t, const sx::mm::matrix& a, const sx::mm::matrix& b) { // concatinate
		t[0] = transform44(a[0], b);
		t[1] = transform44(a[1], b);
		t[2] = transform44(a[2], b);
		t[3] = transform44(a[3], b);
	}
	
	inline sx::mm::vector& revert_endian (sx::mm::vector& v) {
		float *const p = sx::floatp(v);
		sx::revert_endian(p[0]);
		sx::revert_endian(p[1]);
		sx::revert_endian(p[2]);
		sx::revert_endian(p[3]);
		return v;
	}

	inline sx::mm::vector lerp (const sx::mm::vector& a, const sx::mm::vector& b, const sx::mm::vector& t) {
		return _mm_add_ps(_mm_mul_ps(_mm_sub_ps(b, a), t), a);
	}
	inline sx::mm::matrix lerp (const sx::mm::matrix& a, const sx::mm::matrix& b, const sx::mm::vector& t) {
		sx::mm::matrix c;
		c[0] = _mm_add_ps(_mm_mul_ps(_mm_sub_ps(b[0], a[0]), t), a[0]);
		c[1] = _mm_add_ps(_mm_mul_ps(_mm_sub_ps(b[1], a[1]), t), a[1]);
		c[2] = _mm_add_ps(_mm_mul_ps(_mm_sub_ps(b[2], a[2]), t), a[2]);
		c[3] = _mm_add_ps(_mm_mul_ps(_mm_sub_ps(b[3], a[3]), t), a[3]);
		return c;
	}

	inline sx::mm::vector trunc (const sx::mm::vector& v) {
		#if SXSIMD
			#if __ARM_NEON__
				sx::mm::vector_union u; u.m = v;
				for (int i = 0; i < 4; ++i) u.f[i] = int(u.f[i]);
				return u.m;
			#else
				#if (2 <= SXSIMD)
					return _mm_cvtepi32_ps(_mm_cvttps_epi32(v));
				#else
					sx::mm::vector t = sx::shuffle<1, 2, 3, 0>(_mm_cvt_si2ss(v, _mm_cvtt_ss2si(v)));
					for (int i = 1; i < 4; ++i) {
						t = sx::shuffle<1, 2, 3, 0>(_mm_cvt_si2ss(t, _mm_cvtt_ss2si(t)));
					}
					return t;
				#endif
			#endif
		#else
			sx::mm::vector t;
			for (int i = 0; i < 4; ++i) t[i] = int(v[i]);
			return t;
		#endif
	}
	inline sx::mm::vector floor (const sx::mm::vector& v) {
		#if SXSIMD
			const sx::mm::vector vt(trunc(v));
			return _mm_sub_ps(vt, sx::andx(sx::ltx(v, vt), sx::mm::make_vector(1.0f)));
		#else
			sx::mm::vector t;
			for (int i = 0; i < 4; ++i) t[i] = std::floor(v[i]);
			return t;
		#endif
	}
	inline sx::mm::vector ceil (const sx::mm::vector& v) {
		#if SXSIMD
			const sx::mm::vector vt(trunc(v));
			return _mm_add_ps(vt, sx::andx(sx::ltx(vt, v), sx::mm::make_vector(1.0f)));
		#else
			sx::mm::vector t;
			for (int i = 0; i < 4; ++i) t[i] = std::ceil(v[i]);
			return t;
		#endif
	}
	template<int N> inline const vec<float,N>& vectort (const sx::mm::vector& v) {
		return reinterpret_cast<const vec<float,N>&>(v);
	}
	template<int N> inline const mat<float,N>& matrixt (const sx::mm::matrix& v) {
		static_assert(N == 4, "");
		return reinterpret_cast<const mat<float,N>&>(v);
	}
	template<int N> inline vec<int,N> trunct (const sx::mm::vector& v) {
		vec<int,N> vi;
		#if SXSIMD
			#if __ARM_NEON__
				sx::mm::vector_union u; u.m = v;
				for (int i = 0; i < N; ++i) vi[i] = int(u.f[i]);
			#else
				vi[0] = _mm_cvtt_ss2si(v);
				sx::mm::vector t = sx::shuffle<1, 2, 3, 0>(v);
				for (int i = 1; i < (N-1); ++i) {
					vi[i] = _mm_cvtt_ss2si(t);
					t = sx::shuffle<1, 2, 3, 0>(t);
				}
				vi[(N-1)] = _mm_cvtt_ss2si(t);
			#endif
		#else
			for (int i = 0; i < N; ++i) vi[i] = int(v[i]);
		#endif
		return vi;
	}
	inline rgb_color<float> rgbt (const sx::mm::vector& v) {
		return sx::cast<rgb_color<float>>(sx::floatp(v));
	}
	inline rgba_color<float> rgbat (const sx::mm::vector& v) {
		return sx::cast<rgba_color<float>>(sx::floatp(v));
	}
	inline float floatt (const sx::mm::vector& v) {
		#if SXSIMD && __ARM_NEON__
			return vgetq_lane_f32(v, 0);
		#else
			union u {
				sx::mm::vector	v;
				float			f;
			};
			return reinterpret_cast<const u*>(&v)->f;
		#endif
	}
	inline sx::mm::matrix matrixxt (float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k, float l, float m, float n, float o, float p) {
		sx::mm::matrix u;
		u[0] = sx::mm::make_vector(a, b, c, d);
		u[1] = sx::mm::make_vector(e, f, g, h);
		u[2] = sx::mm::make_vector(i, j, k, l);
		u[3] = sx::mm::make_vector(m, n, o, p);
		return u;
	}
	inline sx::mm::matrix matrixxt (const sx::mat4& t) {
		sx::mm::matrix u; u[0] = sx::mm::make_vector(t[0]); u[1] = sx::mm::make_vector(t[1]); u[2] = sx::mm::make_vector(t[2]); u[3] = sx::mm::make_vector(t[3]);
		return u;
	}
	inline bool eq (const sx::mm::matrix& a, const sx::mm::matrix& b) {
		return ((sx::eqmask(a[0], b[0]) == 0x0f) && (sx::eqmask(a[1], b[1]) == 0x0f) && (sx::eqmask(a[2], b[2]) == 0x0f) && (sx::eqmask(a[3], b[3]) == 0x0f));
	}

	inline sx::mm::vector clamp (const sx::mm::vector& v, const sx::mm::vector& min = sx::mm::make_vector(0.0f), const sx::mm::vector& max = sx::mm::make_vector(1.0f)) {
		return sx::max(sx::min(v, max), min);
	}
	inline sx::mm::scalar clamps (const sx::mm::scalar& v, const sx::mm::scalar& min = sx::mm::make_scalar(0.0f), const sx::mm::scalar& max = sx::mm::make_scalar(1.0f)) {
		return sx::maxs(sx::mins(v, max), min);
	}
}
namespace sx {
	inline sx::mm::vector isfinitex (const sx::mm::vector& v) {
		return sx::andx(sx::lex(sx::mm::vlowest, v), sx::lex(v, sx::mm::vmax));
	}
	inline bool isfinite (const sx::mm::vector& v) {
		return (_mm_movemask_ps(isfinitex(v)) == 0xf);
	}
	inline bool isnan (const sx::mm::vector& v) {
		return (_mm_movemask_ps(sx::eqx(v, v)) == 0x0);
	}
	inline bool isinf (const sx::mm::vector& v) {
		return (!isnan(v) && !isfinite(v));
	}
}
namespace sx {
	template<> struct type<sx::mm::vec2> {
		inline static sx::mm::vec2 cast (const float f) {
			return {sx::mm::make_vector(f), sx::mm::make_vector(f)};
		}
		inline static sx::mm::vec2 cast (const sx::vec2& v) {
			return {sx::mm::make_vector(v.x), sx::mm::make_vector(v.y)};
		}
		inline static sx::mm::vec2 cast (const sx::vec2& a, const sx::vec2& b, const sx::vec2& c, const sx::vec2& d) {
			return {sx::mm::make_vector(a.x, b.x, c.x, d.x), sx::mm::make_vector(a.y, b.y, c.y, d.y)};
		}
		inline static sx::mm::vec2 splat (const sx::mm::vector& v) {
			return { sx::splat<0>(v), sx::splat<1>(v) };
		}
	};
	template<> struct type<sx::mm::vec3> {
		inline static sx::mm::vec3 cast (const float f) {
			return {sx::mm::make_vector(f), sx::mm::make_vector(f), sx::mm::make_vector(f)};
		}
		inline static sx::mm::vec3 cast (const float x, const float y, const float z) {
			return { sx::mm::make_vector(x), sx::mm::make_vector(y), sx::mm::make_vector(z) };
		}
		inline static sx::mm::vec3 cast (const vec3& v) {
			return { sx::mm::make_vector(v.x), sx::mm::make_vector(v.y), sx::mm::make_vector(v.z) };
		}
		inline static sx::mm::vec3 cast (const sx::vec3& a, const sx::vec3& b, const sx::vec3& c, const sx::vec3& d) {
			return { sx::mm::make_vector(a.x, b.x, c.x, d.x), sx::mm::make_vector(a.y, b.y, c.y, d.y), sx::mm::make_vector(a.z, b.z, c.z, d.z) };
		}
		inline static sx::mm::vec3 cast (const sx::rgb& c) {
			return { sx::mm::make_vector(c.red), sx::mm::make_vector(c.green), sx::mm::make_vector(c.blue) };
		}
		inline static sx::mm::vec3 cast (const sx::rgb& a, const sx::rgb& b, const sx::rgb& c, const sx::rgb& d) {
			return { sx::mm::make_vector(a.red, b.red, c.red, d.red), sx::mm::make_vector(a.green, b.green, c.green, d.green), sx::mm::make_vector(a.blue, b.blue, c.blue, d.blue) };
		}
		inline static sx::mm::vec3 splat (const sx::mm::vector& v) {
			return { sx::splat<0>(v), sx::splat<1>(v), sx::splat<2>(v) };
		}
	};
	template<> struct type<sx::mm::vec4> {
		inline static sx::mm::vec4 cast (const float& f) {
			return { sx::mm::make_vector(f), sx::mm::make_vector(f), sx::mm::make_vector(f), sx::mm::make_vector(f) };
		}
		inline static sx::mm::vec4 cast (const float& x, const float& y, const float& z, const float& w) {
			return { sx::mm::make_vector(x), sx::mm::make_vector(y), sx::mm::make_vector(z), sx::mm::make_vector(w) };
		}
		inline static sx::mm::vec4 cast (const vec4& v) {
			return { sx::mm::make_vector(v.x), sx::mm::make_vector(v.y), sx::mm::make_vector(v.z), sx::mm::make_vector(v.w) };
		}
		inline static sx::mm::vec4 cast (const sx::vec4& a, const sx::vec4& b, const sx::vec4& c, const sx::vec4& d) {
			return { sx::mm::make_vector(a.x, b.x, c.x, d.x), sx::mm::make_vector(a.y, b.y, c.y, d.y), sx::mm::make_vector(a.z, b.z, c.z, d.z), sx::mm::make_vector(a.w, b.w, c.w, d.w) };
		}
		inline static sx::mm::vec4 cast (const sx::rgba& c) {
			return { sx::mm::make_vector(c.red), sx::mm::make_vector(c.green), sx::mm::make_vector(c.blue), sx::mm::make_vector(c.alpha) };
		}
		inline static sx::mm::vec4 cast (const sx::rgba& a, const sx::rgba& b, const sx::rgba& c, const sx::rgba& d) {
			return { sx::mm::make_vector(a.red, b.red, c.red, d.red), sx::mm::make_vector(a.green, b.green, c.green, d.green), sx::mm::make_vector(a.blue, b.blue, c.blue, d.blue), sx::mm::make_vector(a.alpha, b.alpha, c.alpha, d.alpha) };
		}
		inline static sx::mm::vec4 splat (const sx::mm::vector& v) {
			return { sx::splat<0>(v), sx::splat<1>(v), sx::splat<2>(v), sx::splat<3>(v) };
		}
	};
	
//	template<> inline sx::mm::vec2 make (const float& f) {
//		return sx::mm::vec2{sx::mm::make_vector(f), sx::mm::make_vector(f)};
//	}
//	template<> inline sx::mm::vec2 make (const sx::vec2& v) {
//		return sx::mm::vec2{sx::mm::make_vector(v.x), sx::mm::make_vector(v.y)};
//	}
//	template<> inline sx::mm::vec2 make (const sx::vec2& a, const sx::vec2& b, const sx::vec2& c, const sx::vec2& d) {
//		return sx::mm::vec2{sx::mm::make_vector(a.x, b.x, c.x, d.x), sx::mm::make_vector(a.y, b.y, c.y, d.y)};
//	}
	inline void set (sx::mm::vec2& m, int i, const sx::mm::vector& v) {
		sx::floatp(m.x)[i] = sx::floatp(v)[0]; sx::floatp(m.y)[i] = sx::floatp(v)[1];
	}
	inline void set (sx::mm::vec2& m, int i, const sx::vec2& v) {
		sx::floatp(m.x)[i] = v.x; sx::floatp(m.y)[i] = v.y;
	}

//	template<> inline sx::mm::vec3 make (const float& f) {
//		return sx::mm::vec3{ sx::mm::make_vector(f), sx::mm::make_vector(f), sx::mm::make_vector(f) };
//	}
//	template<> inline sx::mm::vec3 make (const float& x, const float& y, const float& z) {
//		return sx::mm::vec3{ sx::mm::make_vector(x), sx::mm::make_vector(y), sx::mm::make_vector(z) };
//	}
//	template<> inline sx::mm::vec3 make (const vec3& v) {
//		return sx::mm::vec3{ sx::mm::make_vector(v.x), sx::mm::make_vector(v.y), sx::mm::make_vector(v.z) };
//	}
//	template<> inline sx::mm::vec3 make (const sx::vec3& a, const sx::vec3& b, const sx::vec3& c, const sx::vec3& d) {
//		return sx::mm::vec3{ sx::mm::make_vector(a.x, b.x, c.x, d.x), sx::mm::make_vector(a.y, b.y, c.y, d.y), sx::mm::make_vector(a.z, b.z, c.z, d.z) };
//	}
//	template<> inline sx::mm::vec3 make (const sx::rgb& c) {
//		return sx::mm::vec3{ sx::mm::make_vector(c.red), sx::mm::make_vector(c.green), sx::mm::make_vector(c.blue) };
//	}
//	template<> inline sx::mm::vec3 make (const sx::rgb& a, const sx::rgb& b, const sx::rgb& c, const sx::rgb& d) {
//		return sx::mm::vec3{ sx::mm::make_vector(a.red, b.red, c.red, d.red), sx::mm::make_vector(a.green, b.green, c.green, d.green), sx::mm::make_vector(a.blue, b.blue, c.blue, d.blue) };
//	}
	inline void set (sx::mm::vec3& m, int i, const sx::mm::vector& v) {
		sx::floatp(m.x)[i] = sx::floatp(v)[0]; sx::floatp(m.y)[i] = sx::floatp(v)[1]; sx::floatp(m.z)[i] = sx::floatp(v)[2];
	}
	inline void set (sx::mm::vec3& m, int i, const sx::vec3& v) {
		sx::floatp(m.x)[i] = v.x; sx::floatp(m.y)[i] = v.y; sx::floatp(m.z)[i] = v.z;
	}
	inline void set (sx::mm::vec3& m, int i, const rgb_color<>& c) {
		sx::floatp(m.x)[i] = c.red; sx::floatp(m.y)[i] = c.green; sx::floatp(m.z)[i] = c.blue;
	}
	inline void transpose (sx::mm::vec3& m) {
		#if SXSIMD
			#if __ARM_NEON__
				sx::mm::vector_union u0; u0.m = m.x;
				sx::mm::vector_union u1; u1.m = m.y;
				sx::mm::vector_union u2; u2.m = m.z;
				m.x = sx::mm::make_vector(u0.f[0], u1.f[0], u2.f[0], 0.0f);
				m.y = sx::mm::make_vector(u0.f[1], u1.f[1], u2.f[1], 0.0f);
				m.z = sx::mm::make_vector(u0.f[2], u1.f[2], u2.f[2], 0.0f);
			#else
				const sx::mm::vector tmp_xy(_mm_unpacklo_ps(m.x, m.y));
				const sx::mm::vector tmp_z(_mm_unpackhi_ps(m.x, m.y));
				m.x = _mm_movelh_ps(tmp_xy, m.z);
				m.y = _mm_movehl_ps(_mm_unpacklo_ps(m.z, m.z), tmp_xy);
				m.z = _mm_movelh_ps(tmp_z, _mm_unpackhi_ps(m.z, m.z));
			#endif
		#else
			const sx::mm::vec3 t(m);
			m.x = sx::mm::make_vector(t.x[0], t.y[0], t.z[0], 0.0f);
			m.y = sx::mm::make_vector(t.x[1], t.y[1], t.z[1], 0.0f);
			m.z = sx::mm::make_vector(t.x[2], t.y[2], t.z[2], 0.0f);
		#endif
	}

//	template<> inline sx::mm::vec4 make (const float& f) {
//		return sx::mm::vec4{ sx::mm::make_vector(f), sx::mm::make_vector(f), sx::mm::make_vector(f), sx::mm::make_vector(f) };
//	}
//	template<> inline sx::mm::vec4 make (const float& x, const float& y, const float& z, const float& w) {
//		return sx::mm::vec4{ sx::mm::make_vector(x), sx::mm::make_vector(y), sx::mm::make_vector(z), sx::mm::make_vector(w) };
//	}
//	template<> inline sx::mm::vec4 make (const vec4& v) {
//		return sx::mm::vec4{ sx::mm::make_vector(v.x), sx::mm::make_vector(v.y), sx::mm::make_vector(v.z), sx::mm::make_vector(v.w) };
//	}
//	template<> inline sx::mm::vec4 make (const sx::vec4& a, const sx::vec4& b, const sx::vec4& c, const sx::vec4& d) {
//		return sx::mm::vec4{ sx::mm::make_vector(a.x, b.x, c.x, d.x), sx::mm::make_vector(a.y, b.y, c.y, d.y), sx::mm::make_vector(a.z, b.z, c.z, d.z), sx::mm::make_vector(a.w, b.w, c.w, d.w) };
//	}
//	template<> inline sx::mm::vec4 make (const sx::rgba& c) {
//		return sx::mm::vec4{ sx::mm::make_vector(c.red), sx::mm::make_vector(c.green), sx::mm::make_vector(c.blue), sx::mm::make_vector(c.alpha) };
//	}
//	template<> inline sx::mm::vec4 make (const sx::rgba& a, const sx::rgba& b, const sx::rgba& c, const sx::rgba& d) {
//		return sx::mm::vec4{ sx::mm::make_vector(a.red, b.red, c.red, d.red), sx::mm::make_vector(a.green, b.green, c.green, d.green), sx::mm::make_vector(a.blue, b.blue, c.blue, d.blue), sx::mm::make_vector(a.alpha, b.alpha, c.alpha, d.alpha) };
//	}
	inline void transpose (sx::mm::vec4& m) {
		#if SXSIMD
			#if __ARM_NEON__
				sx::mm::vector_union u0; u0.m = m.x;
				sx::mm::vector_union u1; u1.m = m.y;
				sx::mm::vector_union u2; u2.m = m.z;
				sx::mm::vector_union u3; u3.m = m.w;
				m.x = sx::mm::make_vector(u0.f[0], u1.f[0], u2.f[0], u3.f[0]);
				m.y = sx::mm::make_vector(u0.f[1], u1.f[1], u2.f[1], u3.f[1]);
				m.z = sx::mm::make_vector(u0.f[2], u1.f[2], u2.f[2], u3.f[2]);
				m.w = sx::mm::make_vector(u0.f[3], u1.f[3], u2.f[3], u3.f[3]);
			#else
				_MM_TRANSPOSE4_PS(m.x, m.y, m.z, m.w);
			#endif
		#else
			const sx::mm::vec4 t(m);
			m.x = sx::mm::make_vector(t.x[0], t.y[0], t.z[0], t.w[0]);
			m.y = sx::mm::make_vector(t.x[1], t.y[1], t.z[1], t.w[1]);
			m.z = sx::mm::make_vector(t.x[2], t.y[2], t.z[2], t.w[2]);
			m.w = sx::mm::make_vector(t.x[3], t.y[3], t.z[3], t.w[3]);
		#endif
	}

	inline void set (sx::mm::vec4& m, int i, const sx::mm::vector& v) {
		sx::floatp(m.x)[i] = sx::floatp(v)[0]; sx::floatp(m.y)[i] = sx::floatp(v)[1]; sx::floatp(m.z)[i] = sx::floatp(v)[2]; sx::floatp(m.w)[i] = sx::floatp(v)[3];
	}
	inline void set (sx::mm::vec4& m, int i, const sx::vec4& v) {
		sx::floatp(m.x)[i] = v.x; sx::floatp(m.y)[i] = v.y; sx::floatp(m.z)[i] = v.z; sx::floatp(m.w)[i] = v.w;
	}
	inline void set (sx::mm::vec4& m, int i, const rgba_color<float>& c) {
		sx::floatp(m.x)[i] = c.red; sx::floatp(m.y)[i] = c.green; sx::floatp(m.z)[i] = c.blue; sx::floatp(m.w)[i] = c.alpha;
	}

	inline sx::mm::vector zerox (const sx::mm::vec2& v) {
		return sx::andx(sx::zerox(v.x), sx::zerox(v.y));
	}
	inline sx::mm::vector zerox (const sx::mm::vec3& v) {
		return sx::andx(sx::andx(sx::zerox(v.x), sx::zerox(v.y)), sx::zerox(v.z));
	}
	inline sx::mm::vector zerox (const sx::mm::vec4& v) {
		return sx::andx(sx::andx(sx::andx(sx::zerox(v.x), sx::zerox(v.y)), sx::zerox(v.z)), sx::zerox(v.w));
	}
	inline sx::mm::vector ltx (const sx::mm::vec2& v, const sx::mm::vector& b) {
		return sx::andx(sx::ltx(v.x, b), sx::ltx(v.y, b));
	}
	inline sx::mm::vector ltx (const sx::mm::vec3& v, const sx::mm::vector& b) {
		return sx::andx(sx::andx(sx::ltx(v.x, b), sx::ltx(v.y, b)), sx::ltx(v.z, b));
	}
	inline sx::mm::vector ltx (const sx::mm::vec4& v, const sx::mm::vector& b) {
		return sx::andx(sx::andx(sx::andx(sx::ltx(v.x, b), sx::ltx(v.y, b)), sx::ltx(v.z, b)), sx::ltx(v.w, b));
	}
	inline sx::mm::vector lex (const sx::mm::vec2& v, const sx::mm::vector& b) {
		return sx::andx(sx::lex(v.x, b), sx::lex(v.y, b));
	}
	inline sx::mm::vector lex (const sx::mm::vec3& v, const sx::mm::vector& b) {
		return sx::andx(sx::andx(sx::lex(v.x, b), sx::lex(v.y, b)), sx::lex(v.z, b));
	}
	inline sx::mm::vector lex (const sx::mm::vec4& v, const sx::mm::vector& b) {
		return sx::andx(sx::andx(sx::andx(sx::lex(v.x, b), sx::lex(v.y, b)), sx::lex(v.z, b)), sx::lex(v.w, b));
	}
	inline sx::mm::vector ltx (const sx::mm::vector& a, const sx::mm::vec2& v) {
		return sx::andx(sx::ltx(a, v.x), sx::ltx(a, v.y));
	}
	inline sx::mm::vector ltx (const sx::mm::vector& a, const sx::mm::vec3& v) {
		return sx::andx(sx::andx(sx::ltx(a, v.x), sx::ltx(a, v.y)), sx::ltx(a, v.z));
	}
	inline sx::mm::vector ltx (const sx::mm::vector& a, const sx::mm::vec4& v) {
		return sx::andx(sx::andx(sx::andx(sx::ltx(a, v.x), sx::ltx(a, v.y)), sx::ltx(a, v.z)), sx::ltx(a, v.w));
	}
	inline sx::mm::vector lex (const sx::mm::vector& a, const sx::mm::vec2& v) {
		return sx::andx(sx::lex(a, v.x), sx::lex(a, v.y));
	}
	inline sx::mm::vector lex (const sx::mm::vector& a, const sx::mm::vec3& v) {
		return sx::andx(sx::andx(sx::lex(a, v.x), sx::lex(a, v.y)), sx::lex(a, v.z));
	}
	inline sx::mm::vector lex (const sx::mm::vector& a, const sx::mm::vec4& v) {
		return sx::andx(sx::andx(sx::andx(sx::lex(a, v.x), sx::lex(a, v.y)), sx::lex(a, v.z)), sx::lex(a, v.w));
	}
	inline sx::mm::vector ltx (const sx::mm::vec2& a, const sx::mm::vec2& b) {
		return sx::andx(sx::ltx(a.x, b.x), sx::ltx(a.y, b.y));
	}
	inline sx::mm::vector ltx (const sx::mm::vec3& a, const sx::mm::vec3& b) {
		return sx::andx(sx::andx(sx::ltx(a.x, b.x), sx::ltx(a.y, b.y)), sx::ltx(a.z, b.z));
	}
	inline sx::mm::vector ltx (const sx::mm::vec4& a, const sx::mm::vec4& b) {
		return sx::andx(sx::andx(sx::andx(sx::ltx(a.x, b.x), sx::ltx(a.y, b.y)), sx::ltx(a.z, b.z)), sx::ltx(a.w, b.w));
	}
	inline sx::mm::vector lex (const sx::mm::vec2& a, const sx::mm::vec2& b) {
		return sx::andx(sx::lex(a.x, b.x), sx::lex(a.y, b.y));
	}
	inline sx::mm::vector lex (const sx::mm::vec3& a, const sx::mm::vec3& b) {
		return sx::andx(sx::andx(sx::lex(a.x, b.x), sx::lex(a.y, b.y)), sx::lex(a.z, b.z));
	}
	inline sx::mm::vector lex (const sx::mm::vec4& a, const sx::mm::vec4& b) {
		return sx::andx(sx::andx(sx::andx(sx::lex(a.x, b.x), sx::lex(a.y, b.y)), sx::lex(a.z, b.z)), sx::lex(a.w, b.w));
	}

	inline sx::mm::vector absolute3 (const sx::mm::vec3& p) {
		return sx::sqrt(p.x*p.x + p.y*p.y + p.z*p.z);
	}

	inline vec<float,2> vectort (const sx::mm::vec2& v, int i) {
		return vec<float,2>{sx::floatp(v.x)[i], sx::floatp(v.y)[i]};
	}
	inline vec<float,3> vectort (const sx::mm::vec3& v, int i) {
		return vec<float,3>(sx::floatp(v.x)[i], sx::floatp(v.y)[i], sx::floatp(v.z)[i]);
	}
	inline vec<float,4> vectort (const sx::mm::vec4& v, int i) {
		return vec<float,4>(sx::floatp(v.x)[i], sx::floatp(v.y)[i], sx::floatp(v.z)[i], sx::floatp(v.w)[i]);
	}
	inline rgb_color<float> rgbt (const sx::mm::vec3& v, int i) {
		return rgb_color<float>(sx::floatp(v.x)[i], sx::floatp(v.y)[i], sx::floatp(v.z)[i]);
	}
	inline rgba_color<float> rgbat (const sx::mm::vec4& v, int i) {
		return rgba_color<float>(sx::floatp(v.x)[i], sx::floatp(v.y)[i], sx::floatp(v.z)[i], sx::floatp(v.w)[i]);
	}

	inline sx::mm::vec2 normalize (const sx::mm::vec2& p) {
//		const sx::mm::vector d(sqrabsolute2(p));
//		return p * rsqrt(sx::conditional(sx::ltx(sx::mm::vepsilon, d), d, sx::mm::vone));
		const sx::mm::vector q(rsqrt(sqrabsolute2(p)));
		return p * sx::conditional(sx::lex(q,sx::mm::vmax), q, sx::mm::vone);
	}
	inline sx::mm::vec3 normalize (const sx::mm::vec3& p) {
//		const sx::mm::vector d(sqrabsolute3(p));
//		return p * rsqrt(sx::conditional(sx::ltx(sx::mm::vepsilon, d), d, sx::mm::vone));
		const sx::mm::vector q(rsqrt(sqrabsolute3(p)));
		return p * sx::conditional(sx::lex(q,sx::mm::vmax), q, sx::mm::vone);
	}
	inline sx::mm::vec4 normalize (const sx::mm::vec4& p) {
//		const sx::mm::vector d(sqrabsolute3(p));
//		return p * rsqrt(sx::conditional(sx::ltx(sx::mm::vepsilon, d), d, sx::mm::vone));
		const sx::mm::vector q(rsqrt(sqrabsolute3(p)));
		return p * sx::conditional(sx::lex(q,sx::mm::vmax), q, sx::mm::vone);
	}
	inline sx::mm::vector normalize_self (sx::mm::vec2& p) {
//		const sx::mm::vector e(sqrabsolute2(p));
//		const sx::mm::vector b(sx::ltx(sx::mm::vepsilon, e));
//		const sx::mm::vector d(rsqrt(sx::conditional(b, e, sx::mm::vone)));
//		p *= d;
//		return sx::conditional(b, rcp(d), e);
		const sx::mm::vector q(rsqrt(sqrabsolute2(p)));
		const sx::mm::vector b(sx::lex(q,sx::mm::vmax));
		p *= sx::conditional(b, q, sx::mm::vone);
		return sx::conditional(b, rcp(q), sx::mm::vzero);
	}
	inline sx::mm::vector normalize_self (sx::mm::vec3& p) {
//		const sx::mm::vector e(sqrabsolute3(p));
//		const sx::mm::vector b(sx::ltx(sx::mm::vepsilon, e));
//		const sx::mm::vector d(rsqrt(sx::conditional(b, e, sx::mm::vone)));
//		p *= d;
//		return sx::conditional(b, rcp(d), e);
		const sx::mm::vector q(rsqrt(sqrabsolute3(p)));
		const sx::mm::vector b(sx::lex(q,sx::mm::vmax));
		p *= sx::conditional(b, q, sx::mm::vone);
		return sx::conditional(b, rcp(q), sx::mm::vzero);
	}
	inline sx::mm::vector normalize_self (sx::mm::vec4& p) {
//		const sx::mm::vector e(sqrabsolute3(p));
//		const sx::mm::vector b(sx::ltx(sx::mm::vepsilon, e));
//		const sx::mm::vector d(rsqrt(sx::conditional(b, e, sx::mm::vone)));
//		p *= d;
//		return sx::conditional(b, rcp(d), e);
		const sx::mm::vector q(rsqrt(sqrabsolute3(p)));
		const sx::mm::vector b(sx::lex(q,sx::mm::vmax));
		p *= sx::conditional(b, q, sx::mm::vone);
		return sx::conditional(b, rcp(q), sx::mm::vzero);
	}


	template<int i> inline sx::mm::vector transformi (const sx::mm::vec3& p, const sx::mm::matrix& t) {
		return p.x * sx::splat<i>(t[0]) + p.y * sx::splat<i>(t[1]) + p.z * sx::splat<i>(t[2]) + sx::splat<i>(t[3]);
	}
	inline sx::mm::vec3 transform33 (const sx::mm::vec3& p, const sx::mm::matrix& t) {
		return sx::mm::vec3(transformi<0>(p, t), transformi<1>(p, t), transformi<2>(p, t));
	}
	template<int i> inline sx::mm::vector transform4i (const sx::mm::vec3& p, const sx::mm::matrix& t) {
		return p.x * sx::splat<i>(t[0]) + p.y * sx::splat<i>(t[1]) + p.z * sx::splat<i>(t[2]) + sx::splat<i>(t[3]);
	}
	inline sx::mm::vec3 transform44 (const sx::mm::vec3& p, const sx::mm::matrix& t) {
		return sx::mm::vec3(transform4i<0>(p, t), transform4i<1>(p, t), transform4i<2>(p, t));
	}
	template<int i> inline sx::mm::vector transformi (const sx::mm::vec4& p, const sx::mm::matrix& t) {
		return p.x * sx::splat<i>(t[0]) + p.y * sx::splat<i>(t[1]) + p.z * sx::splat<i>(t[2]) + p.w * sx::splat<i>(t[3]);
	}
	inline sx::mm::vec4 transform44 (const sx::mm::vec4& p, const sx::mm::matrix& t) {
		return sx::mm::vec4(transformi<0>(p, t), transformi<1>(p, t), transformi<2>(p, t), transformi<3>(p, t));
	}

	template<int N> inline bool operator< (const sx::vec<sx::mm::vector,N>& a, const sx::vec<sx::mm::vector,N>& b) {
		for (int i = 0; i < N; ++i) if (!sx::ltmask(a[i], b[i])) return false;
		return true;
	}
	template<int N> inline bool operator<= (const sx::vec<sx::mm::vector,N>& a, const sx::vec<sx::mm::vector,N>& b) {
		for (int i = 0; i < N; ++i) if (!sx::lemask(a[i], b[i])) return false;
		return true;
	}
	template<int N> inline bool operator== (const sx::vec<sx::mm::vector,N>& a, const sx::vec<sx::mm::vector,N>& b) {
		for (int i = 0; i < N; ++i) if (!sx::eqmask(a[i], b[i])) return false;
		return true;
	}
	template<int N> inline bool operator!= (const sx::vec<sx::mm::vector,N>& a, const sx::vec<sx::mm::vector,N>& b) {
		return !(a == b);
	}

	template<typename T> struct type<mat<T,3>> {
		inline static auto cast (const std::array<T,3*3>& t) -> mat<T,4> {
			return {{ {t[0],t[1],t[2]},{t[3],t[4],t[5]},{t[6],t[7],t[8]} }};
		}
		inline static auto cast (const T(&t)[3*3]) -> mat<T,4> {
			return {{ {t[0],t[1],t[2]},{t[3],t[4],t[5]},{t[6],t[7],t[8]} }};
		}
		inline static auto cast (const mat<T,4>& t) -> mat<T,3> {
			return {{ {t[0][0],t[0][1],t[0][2]},{t[1][0],t[1][1],t[1][2]},{t[2][0],t[2][1],t[2][2]} }};
		}
	};
	template<typename T> struct type<mat<T,4>> {
		inline static auto cast (const std::array<T,4*4>& t) -> mat<T,4> {
			return {{ {t[0],t[1],t[2],t[3]},{t[4],t[5],t[6],t[7]},{t[8],t[9],t[10],t[11]},{t[12],t[13],t[14],t[15]} }};
		}
		inline static auto cast (const T(&t)[4*4]) -> mat<T,4> {
			return {{ {t[0],t[1],t[2],t[3]},{t[4],t[5],t[6],t[7]},{t[8],t[9],t[10],t[11]},{t[12],t[13],t[14],t[15]} }};
		}
		inline static auto cast (const sx::mm::matrix& t) -> mat<T,4> {
			const auto p = floatp(t);
			return {{ {p[0],p[1],p[2],p[3]},{p[4],p[5],p[6],p[7]},{p[8],p[9],p[10],p[11]},{p[12],p[13],p[14],p[15]} }};
		}
	};
}

namespace std {
	template<> class numeric_limits<sx::mm::vector> {
	public:
		static sx::mm::vector max ()	{ return sx::mm::make_vector(std::numeric_limits<float>::max(),std::numeric_limits<float>::max(),std::numeric_limits<float>::max(),std::numeric_limits<float>::max()); }
		static sx::mm::vector lowest ()	{ return sx::mm::make_vector(std::numeric_limits<float>::lowest(),std::numeric_limits<float>::lowest(),std::numeric_limits<float>::lowest(),std::numeric_limits<float>::lowest()); }
	};
}

std::ostream& operator<< (std::ostream& stream, const sx::mm::vector& v);

#endif