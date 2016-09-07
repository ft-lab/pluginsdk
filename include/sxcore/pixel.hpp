#pragma once
SXMODULE_SXCORE
#include "sxcore/vectors.hpp"
#include "sxcore/float16.hpp"

namespace sx {

	#if SXAPI_WIN32
		inline const sx::rgb vector3t (COLORREF colorref) {
			return sx::rgb(GetRValue(colorref) * (1.0f/255.0f), GetGValue(colorref) * (1.0f/255.0f), GetBValue(colorref) * (1.0f/255.0f));
		}
		inline const sx::rgba vector4t (COLORREF colorref) {
			return sx::rgba(GetRValue(colorref) * (1.0f/255.0f), GetGValue(colorref) * (1.0f/255.0f), GetBValue(colorref) * (1.0f/255.0f), 1.0f);
		}
		inline const COLORREF RGB_ (const sx::rgb &v) {
			const sx::rgb c = sx::clamp(v);
			return RGB(BYTE(c.red*255.0f+0.5f), BYTE(c.green*255.0f+0.5f), BYTE(c.blue*255.0f+0.5f));
		}
	#endif

	using pixel24		= sx::rgb_color<std::uint8_t>;
	using pixel32		= sx::rgba_color<std::uint8_t>;
	using rgba8_class	= sx::rgba_color<std::uint8_t,RGBA>;

	namespace pixel {
		// pixel operations
		inline sx::rgba multiply_alpha (const sx::rgba &c) {
			return sx::rgba(c.red * c.alpha, c.green * c.alpha, c.blue * c.alpha, c.alpha);
		}
		inline sx::rgba alpha_blend (const sx::rgba &d, const sx::rgba &s) {
			return sx::rgba(d + (s * (1.0f-d.alpha)), d.alpha);
		}
		inline sx::rgba add (const sx::rgba &d, const sx::rgba &s) {
			return sx::rgba(d + s, d.alpha);
		}
		inline sx::rgba sub (const sx::rgba &d, const sx::rgba &s) {
			return sx::rgba(d - s, d.alpha);
		}
		inline sx::rgba mul (const sx::rgba &d, const sx::rgba &s) {
			return sx::rgba(d * s, d.alpha);
		}
		inline sx::rgba min (const sx::rgba &d, const sx::rgba &s) {
			return sx::rgba(sx::min(d, s), d.alpha);
		}
		inline sx::rgba max (const sx::rgba &d, const sx::rgba &s) {
			return sx::rgba(sx::max(d, s), d.alpha);
		}
		inline sx::rgba blend (const sx::rgba &d, const sx::rgba &s, float weight) {
			return sx::rgba((d * (std::max((1.0f-weight), 0.0f))) + (s * weight), d.alpha);
		}
		inline sx::rgb gamma (const sx::rgb &c, float v) {
			const float rv = 1.0f / std::max(sx::epsilon, v);
			return sx::rgb(std::pow(c.red, rv), std::pow(c.green, rv), std::pow(c.blue, rv));
		}
		inline sx::rgba gamma (const sx::rgba &c, float v) {
			return sx::rgba(gamma(sx::rgb(c), v), c.alpha);
		}

		namespace imp {
			template<typename T, typename U> inline T convert (U u);
			template<typename T, typename U> inline T convert (U u, U v, U w);

			template<> inline std::uint8_t convert (std::uint8_t u)		{ return u; }
			template<> inline std::uint8_t convert (std::uint16_t u)	{ return std::uint8_t(u >> 8); }
			template<> inline std::uint8_t convert (std::uint32_t u)	{ return std::uint8_t(u >> 24); }
			template<> inline std::uint8_t convert (sx::float16_t u)	{ return std::uint8_t(sx::clamp(float(u)) * float(std::numeric_limits<std::uint8_t>::max()) + 0.5f); }
			template<> inline std::uint8_t convert (float u)			{ return std::uint8_t(sx::clamp(u) * float(std::numeric_limits<std::uint8_t>::max()) + 0.5f); }

			template<> inline std::uint16_t convert (std::uint8_t u)	{ return (std::uint16_t(u) << 8) + std::uint16_t(u); }
			template<> inline std::uint16_t convert (std::uint16_t u)	{ return u; }
			template<> inline std::uint16_t convert (std::uint32_t u)	{ return std::uint16_t(u >> 16); }
			template<> inline std::uint16_t convert (sx::float16_t u)	{ return std::uint16_t(sx::clamp(float(u)) * float(std::numeric_limits<std::uint16_t>::max()) + 0.5f); }
			template<> inline std::uint16_t convert (float u)			{ return std::uint16_t(sx::clamp(u) * float(std::numeric_limits<std::uint16_t>::max()) + 0.5f); }

			template<> inline std::uint32_t convert (std::uint8_t u)	{ return (std::uint32_t(u) << 24) + (std::uint32_t(u) << 16) + (std::uint32_t(u) << 8) + std::uint32_t(u); }
			template<> inline std::uint32_t convert (std::uint16_t u)	{ return (std::uint32_t(u) << 16) + std::uint32_t(u); }
			template<> inline std::uint32_t convert (std::uint32_t u)	{ return u; }
			template<> inline std::uint32_t convert (sx::float16_t u)	{ return std::uint32_t(sx::clamp(double(u)) * double(std::numeric_limits<std::uint32_t>::max()) + 0.5); }
			template<> inline std::uint32_t convert (float u)			{ return std::uint32_t(sx::clamp(double(u)) * double(std::numeric_limits<std::uint32_t>::max()) + 0.5); }

			template<> inline sx::float16_t convert (std::uint8_t u)	{ return sx::float16_t(float(u) / float(std::numeric_limits<std::uint8_t>::max())); }
			template<> inline sx::float16_t convert (std::uint16_t u)	{ return sx::float16_t(float(u) / float(std::numeric_limits<std::uint16_t>::max())); }
			template<> inline sx::float16_t convert (std::uint32_t u)	{ return sx::float16_t(double(u) / double(std::numeric_limits<std::uint32_t>::max())); }
			template<> inline sx::float16_t convert (sx::float16_t u)	{ return u; }
			template<> inline sx::float16_t convert (float u)			{ return sx::float16_t(u); }

			template<> inline float convert (std::uint8_t u)	{ return (float(u) / float(std::numeric_limits<std::uint8_t>::max())); }
			template<> inline float convert (std::uint16_t u)	{ return (float(u) / float(std::numeric_limits<std::uint16_t>::max())); }
			template<> inline float convert (std::uint32_t u)	{ return (float(u) / double(std::numeric_limits<std::uint32_t>::max())); }
			template<> inline float convert (sx::float16_t u)	{ return float(u); }
			template<> inline float convert (float u)			{ return u; }

			template<> inline std::uint8_t convert (std::uint8_t u, std::uint8_t v, std::uint8_t w)		{ return std::uint8_t((unsigned(u) + unsigned(v) + unsigned(w) + 2) / 3); }
			template<> inline std::uint8_t convert (std::uint16_t u, std::uint16_t v, std::uint16_t w)	{ return imp::convert<std::uint8_t>(std::uint16_t((unsigned(u) + unsigned(v) + unsigned(w) + 2) / 3)); }
			template<> inline std::uint8_t convert (std::uint32_t u, std::uint32_t v, std::uint32_t w)	{ return imp::convert<std::uint8_t>(std::uint32_t((std::uint64_t(u) + std::uint64_t(v) + std::uint64_t(w) + 2) / 3)); }
			template<> inline std::uint8_t convert (sx::float16_t u, sx::float16_t v, sx::float16_t w)	{ return imp::convert<std::uint8_t>((float(u) + float(v) + float(w)) * (1.0f/3.0f)); }
			template<> inline std::uint8_t convert (float u, float v, float w)							{ return imp::convert<std::uint8_t>((u + v + w) * (1.0f/3.0f)); }

			template<> inline std::uint16_t convert (std::uint8_t u, std::uint8_t v, std::uint8_t w)	{ return imp::convert<std::uint16_t>(std::uint8_t((unsigned(u) + unsigned(v) + unsigned(w) + 2) / 3)); }
			template<> inline std::uint16_t convert (std::uint16_t u, std::uint16_t v, std::uint16_t w)	{ return std::uint16_t((unsigned(u) + unsigned(v) + unsigned(w) + 2) / 3); }
			template<> inline std::uint16_t convert (std::uint32_t u, std::uint32_t v, std::uint32_t w)	{ return imp::convert<std::uint16_t>(std::uint32_t((std::uint64_t(u) + std::uint64_t(v) + std::uint64_t(w) + 2) / 3)); }
			template<> inline std::uint16_t convert (sx::float16_t u, sx::float16_t v, sx::float16_t w)	{ return imp::convert<std::uint16_t>((float(u) + float(v) + float(w)) * (1.0f/3.0f)); }
			template<> inline std::uint16_t convert (float u, float v, float w)							{ return imp::convert<std::uint16_t>((u + v + w) * (1.0f/3.0f)); }

			template<> inline std::uint32_t convert (std::uint8_t u, std::uint8_t v, std::uint8_t w)	{ return imp::convert<std::uint32_t>(std::uint8_t((unsigned(u) + unsigned(v) + unsigned(w) + 2) / 3)); }
			template<> inline std::uint32_t convert (std::uint16_t u, std::uint16_t v, std::uint16_t w)	{ return imp::convert<std::uint32_t>(std::uint16_t((unsigned(u) + unsigned(v) + unsigned(w) + 2) / 3)); }
			template<> inline std::uint32_t convert (std::uint32_t u, std::uint32_t v, std::uint32_t w)	{ return std::uint32_t((std::uint64_t(u) + std::uint64_t(v) + std::uint64_t(w) + 2) / 3); }
			template<> inline std::uint32_t convert (sx::float16_t u, sx::float16_t v, sx::float16_t w)	{ return imp::convert<std::uint32_t>((float(u) + float(v) + float(w)) * (1.0f/3.0f)); }
			template<> inline std::uint32_t convert (float u, float v, float w)							{ return imp::convert<std::uint32_t>((u + v + w) * (1.0f/3.0f)); }

			template<> inline sx::float16_t convert (std::uint8_t u, std::uint8_t v, std::uint8_t w)	{ return sx::float16_t((float(u) + float(v) + float(w)) / (float(std::numeric_limits<std::uint8_t>::max()) * 3.0f)); }
			template<> inline sx::float16_t convert (std::uint16_t u, std::uint16_t v, std::uint16_t w)	{ return sx::float16_t((float(u) + float(v) + float(w)) / (float(std::numeric_limits<std::uint16_t>::max()) * 3.0f)); }
			template<> inline sx::float16_t convert (std::uint32_t u, std::uint32_t v, std::uint32_t w)	{ return sx::float16_t((double(u) + double(v) + double(w)) / (double(std::numeric_limits<std::uint32_t>::max()) * 3.0)); }
			template<> inline sx::float16_t convert (sx::float16_t u, sx::float16_t v, sx::float16_t w)	{ return sx::float16_t((float(u) + float(v) + float(w)) / 3.0f); }
			template<> inline sx::float16_t convert (float u, float v, float w)							{ return sx::float16_t((u + v + w) / 3.0f); }

			template<> inline float convert (std::uint8_t u, std::uint8_t v, std::uint8_t w)	{ return ((float(u) + float(v) + float(w)) / (float(std::numeric_limits<std::uint8_t>::max()) * 3.0f)); }
			template<> inline float convert (std::uint16_t u, std::uint16_t v, std::uint16_t w)	{ return ((float(u) + float(v) + float(w)) / (float(std::numeric_limits<std::uint16_t>::max()) * 3.0f)); }
			template<> inline float convert (std::uint32_t u, std::uint32_t v, std::uint32_t w)	{ return ((double(u) + double(v) + double(w)) / (double(std::numeric_limits<std::uint32_t>::max()) * 3.0)); }
			template<> inline float convert (sx::float16_t u, sx::float16_t v, sx::float16_t w)	{ return ((float(u) + float(v) + float(w)) / 3.0f); }
			template<> inline float convert (float u, float v, float w)							{ return ((u + v + w) / 3.0f); }
			
			#if SXAPI_COCOA
				inline sx::rgba convert (NSColor *nscolor) {
					NSColor *const color = [nscolor colorUsingColorSpaceName:@"NSDeviceRGBColorSpace"];
					#if CGFLOAT_DEFINED
						CGFloat r, g, b, a;
					#else
						float r, g, b, a;
					#endif
					[color getRed:&r green:&g blue:&b alpha:&a];
					return sx::rgba(r, g, b, a);
				}
			#endif
		}

		//template<typename T> inline void convert (T &t, const T &u) { t = u; }
		template<typename T, typename U> inline T convert (U u);
		
		// target: std::uint8_t
		inline void convert (std::uint8_t &t, std::uint8_t u) {
			t = imp::convert<std::uint8_t>(u);
		}
		inline void convert (std::uint8_t &t, std::uint16_t u) {
			t = imp::convert<std::uint8_t>(u);
		}
		inline void convert (std::uint8_t &t, std::uint32_t u) {
			t = imp::convert<std::uint8_t>(u);
		}
		inline void convert (std::uint8_t &t, sx::float16_t u) {
			t = imp::convert<std::uint8_t>(u);
		}
		inline void convert (std::uint8_t &t, float u) {
			t = imp::convert<std::uint8_t>(u);
		}
		template<int K> inline void convert (std::uint8_t &t, const sx::rgb_color<std::uint8_t,K> &u) {
			t = imp::convert<std::uint8_t>(u.red, u.green, u.blue);
		}
		template<int K> inline void convert (std::uint8_t &t, const sx::rgba_color<std::uint8_t,K> &u) {
			t = imp::convert<std::uint8_t>(u.red, u.green, u.blue);
		}
		template<int K> inline void convert (std::uint8_t &t, const sx::rgb_color<std::uint32_t,K> &u) {
			t = imp::convert<std::uint8_t>(u.red, u.green, u.blue);
		}
		template<int K> inline void convert (std::uint8_t &t, const sx::rgba_color<std::uint32_t,K> &u) {
			t = imp::convert<std::uint8_t>(u.red, u.green, u.blue);
		}
		template<int K> inline void convert (std::uint8_t &t, const sx::rgb_color<sx::float16_t,K> &u) {
			t = imp::convert<std::uint8_t>(u.red, u.green, u.blue);
		}
		template<int K> inline void convert (std::uint8_t &t, const sx::rgba_color<sx::float16_t,K> &u) {
			t = imp::convert<std::uint8_t>(u.red, u.green, u.blue);
		}
		template<int K> inline void convert (std::uint8_t &t, const sx::rgb_color<float,K> &u) {
			t = imp::convert<std::uint8_t>(u.red, u.green, u.blue);
		}
		template<int K> inline void convert (std::uint8_t &t, const sx::rgba_color<float,K> &u) {
			t = imp::convert<std::uint8_t>(u.red, u.green, u.blue);
		}

		// target: std::uint16_t
		inline void convert (std::uint16_t &t, std::uint8_t u) {
			t = imp::convert<std::uint16_t>(u);
		}
		inline void convert (std::uint16_t &t, std::uint16_t u) {
			t = imp::convert<std::uint16_t>(u);
		}
		inline void convert (std::uint16_t &t, std::uint32_t u) {
			t = imp::convert<std::uint16_t>(u);
		}
		inline void convert (std::uint16_t &t, sx::float16_t u) {
			t = imp::convert<std::uint16_t>(u);
		}
		inline void convert (std::uint16_t &t, float u) {
			t = imp::convert<std::uint16_t>(u);
		}
		template<int K> inline void convert (std::uint16_t &t, const sx::rgb_color<std::uint8_t,K> &u) {
			t = imp::convert<std::uint16_t>(u.red, u.green, u.blue);
		}
		template<int K> inline void convert (std::uint16_t &t, const sx::rgba_color<std::uint8_t,K> &u) {
			t = imp::convert<std::uint16_t>(u.red, u.green, u.blue);
		}
		template<int K> inline void convert (std::uint16_t &t, const sx::rgb_color<sx::float16_t,K> &u) {
			t = imp::convert<std::uint16_t>(u.red, u.green, u.blue);
		}
		template<int K> inline void convert (std::uint16_t &t, const sx::rgba_color<sx::float16_t,K> &u) {
			t = imp::convert<std::uint16_t>(u.red, u.green, u.blue);
		}
		template<int K> inline void convert (std::uint16_t &t, const sx::rgb_color<float,K> &u) {
			t = imp::convert<std::uint16_t>(u.red, u.green, u.blue);
		}
		template<int K> inline void convert (std::uint16_t &t, const sx::rgba_color<float,K> &u) {
			t = imp::convert<std::uint16_t>(u.red, u.green, u.blue);
		}

		// target: std::uint32_t
		inline void convert (std::uint32_t &t, std::uint8_t u) {
			t = imp::convert<std::uint32_t>(u);
		}
		inline void convert (std::uint32_t &t, std::uint16_t u) {
			t = imp::convert<std::uint32_t>(u);
		}
		inline void convert (std::uint32_t &t, std::uint32_t u) {
			t = imp::convert<std::uint32_t>(u);
		}
		inline void convert (std::uint32_t &t, sx::float16_t u) {
			t = imp::convert<std::uint32_t>(u);
		}
		inline void convert (std::uint32_t &t, float u) {
			t = imp::convert<std::uint32_t>(u);
		}
		template<int K> inline void convert (std::uint32_t &t, const sx::rgb_color<std::uint8_t,K> &u) {
			t = imp::convert<std::uint32_t>(u.red, u.green, u.blue);
		}
		template<int K> inline void convert (std::uint32_t &t, const sx::rgba_color<std::uint8_t,K> &u) {
			t = imp::convert<std::uint32_t>(u.red, u.green, u.blue);
		}
		template<int K> inline void convert (std::uint32_t &t, const sx::rgb_color<sx::float16_t,K> &u) {
			t = imp::convert<std::uint32_t>(u.red, u.green, u.blue);
		}
		template<int K> inline void convert (std::uint32_t &t, const sx::rgba_color<sx::float16_t,K> &u) {
			t = imp::convert<std::uint32_t>(u.red, u.green, u.blue);
		}
		template<int K> inline void convert (std::uint32_t &t, const sx::rgb_color<float,K> &u) {
			t = imp::convert<std::uint32_t>(u.red, u.green, u.blue);
		}
		template<int K> inline void convert (std::uint32_t &t, const sx::rgba_color<float,K> &u) {
			t = imp::convert<std::uint32_t>(u.red, u.green, u.blue);
		}

		// target: sx::float16_t
		inline void convert (sx::float16_t &t, std::uint8_t u) {
			t = imp::convert<sx::float16_t>(u);
		}
		inline void convert (sx::float16_t &t, std::uint16_t u) {
			t = imp::convert<sx::float16_t>(u);
		}
		inline void convert (sx::float16_t &t, std::uint32_t u) {
			t = imp::convert<sx::float16_t>(u);
		}
		inline void convert (sx::float16_t &t, sx::float16_t u) {
			t = imp::convert<sx::float16_t>(u);
		}
		inline void convert (sx::float16_t &t, float u) {
			t = imp::convert<sx::float16_t>(u);
		}
		template<int K> inline void convert (sx::float16_t &t, const sx::rgb_color<std::uint8_t,K> &u) {
			t = imp::convert<sx::float16_t>(u.red, u.green, u.blue);
		}
		template<int K> inline void convert (sx::float16_t &t, const sx::rgba_color<std::uint8_t,K> &u) {
			t = imp::convert<sx::float16_t>(u.red, u.green, u.blue);
		}
		template<int K> inline void convert (sx::float16_t &t, const sx::rgb_color<sx::float16_t,K> &u) {
			t = imp::convert<sx::float16_t>(u.red, u.green, u.blue);
		}
		template<int K> inline void convert (sx::float16_t &t, const sx::rgba_color<sx::float16_t,K> &u) {
			t = imp::convert<sx::float16_t>(u.red, u.green, u.blue);
		}
		template<int K> inline void convert (sx::float16_t &t, const sx::rgb_color<float,K> &u) {
			t = imp::convert<sx::float16_t>(u.red, u.green, u.blue);
		}
		template<int K> inline void convert (sx::float16_t &t, const sx::rgba_color<float,K> &u) {
			t = imp::convert<sx::float16_t>(u.red, u.green, u.blue);
		}

		// target: float
		inline void convert (float &t, std::uint8_t u) {
			t = imp::convert<float>(u);
		}
		inline void convert (float &t, std::uint16_t u) {
			t = imp::convert<float>(u);
		}
		inline void convert (float &t, std::uint32_t u) {
			t = imp::convert<float>(u);
		}
		inline void convert (float &t, sx::float16_t u) {
			t = imp::convert<float>(u);
		}
		inline void convert (float &t, float u) {
			t = imp::convert<float>(u);
		}
		template<int K> inline void convert (float &t, const sx::rgb_color<std::uint8_t,K> &u) {
			t = imp::convert<float>(u.red, u.green, u.blue);
		}
		template<int K> inline void convert (float &t, const sx::rgba_color<std::uint8_t,K> &u) {
			t = imp::convert<float>(u.red, u.green, u.blue);
		}
		template<int K> inline void convert (float &t, const sx::rgb_color<sx::float16_t,K> &u) {
			t = imp::convert<float>(u.red, u.green, u.blue);
		}
		template<int K> inline void convert (float &t, const sx::rgba_color<sx::float16_t,K> &u) {
			t = imp::convert<float>(u.red, u.green, u.blue);
		}
		template<int K> inline void convert (float &t, const sx::rgb_color<float,K> &u) {
			t = imp::convert<float>(u.red, u.green, u.blue);
		}
		template<int K> inline void convert (float &t, const sx::rgba_color<float,K> &u) {
			t = imp::convert<float>(u.red, u.green, u.blue);
		}

		// target: sx::rgb_color<std::uint8_t,K>
		template<int K> inline void convert (sx::rgb_color<std::uint8_t,K> &t, std::uint8_t u) {
			const std::uint8_t k = imp::convert<std::uint8_t>(u);
			t.red = k; t.green = k; t.blue = k;
		}
		template<int K> inline void convert (sx::rgb_color<std::uint8_t,K> &t, std::uint16_t u) {
			const std::uint8_t k = imp::convert<std::uint8_t>(u);
			t.red = k; t.green = k; t.blue = k;
		}
		template<int K> inline void convert (sx::rgb_color<std::uint8_t,K> &t, std::uint32_t u) {
			const std::uint8_t k = imp::convert<std::uint8_t>(u);
			t.red = k; t.green = k; t.blue = k;
		}
		template<int K> inline void convert (sx::rgb_color<std::uint8_t,K> &t, sx::float16_t u) {
			const std::uint8_t k = imp::convert<std::uint8_t>(u);
			t.red = k; t.green = k; t.blue = k;
		}
		template<int K> inline void convert (sx::rgb_color<std::uint8_t,K> &t, float u) {
			const std::uint8_t k = imp::convert<std::uint8_t>(u);
			t.red = k; t.green = k; t.blue = k;
		}
		template<int K, int L> inline void convert (sx::rgb_color<std::uint8_t,K> &t, const sx::rgb_color<std::uint8_t,L> &u) {
			t.red = imp::convert<std::uint8_t>(u.red); t.green = imp::convert<std::uint8_t>(u.green); t.blue = imp::convert<std::uint8_t>(u.blue);
		}
		template<int K, int L> inline void convert (sx::rgb_color<std::uint8_t,K> &t, const sx::rgba_color<std::uint8_t,L> &u) {
			t.red = imp::convert<std::uint8_t>(u.red); t.green = imp::convert<std::uint8_t>(u.green); t.blue = imp::convert<std::uint8_t>(u.blue);
		}
		template<int K, int L> inline void convert (sx::rgb_color<std::uint8_t,K> &t, const sx::rgb_color<sx::float16_t,L> &u) {
			t.red = imp::convert<std::uint8_t>(u.red); t.green = imp::convert<std::uint8_t>(u.green); t.blue = imp::convert<std::uint8_t>(u.blue);
		}
		template<int K, int L> inline void convert (sx::rgb_color<std::uint8_t,K> &t, const sx::rgba_color<sx::float16_t,L> &u) {
			t.red = imp::convert<std::uint8_t>(u.red); t.green = imp::convert<std::uint8_t>(u.green); t.blue = imp::convert<std::uint8_t>(u.blue);
		}
		template<int K, int L> inline void convert (sx::rgb_color<std::uint8_t,K> &t, const sx::rgb_color<float,L> &u) {
			t.red = imp::convert<std::uint8_t>(u.red); t.green = imp::convert<std::uint8_t>(u.green); t.blue = imp::convert<std::uint8_t>(u.blue);
		}
		template<int K, int L> inline void convert (sx::rgb_color<std::uint8_t,K> &t, const sx::rgba_color<float,L> &u) {
			t.red = imp::convert<std::uint8_t>(u.red); t.green = imp::convert<std::uint8_t>(u.green); t.blue = imp::convert<std::uint8_t>(u.blue);
		}
		
		// target: sx::rgba_color<std::uint8_t,K>
		template<int K> inline void convert (sx::rgba_color<std::uint8_t,K> &t, std::uint8_t u) {
			const std::uint8_t k = imp::convert<std::uint8_t>(u);
			t.red = k; t.green = k; t.blue = k; t.alpha = std::numeric_limits<std::uint8_t>::max();
		}
		template<int K> inline void convert (sx::rgba_color<std::uint8_t,K> &t, std::uint16_t u) {
			const std::uint8_t k = imp::convert<std::uint8_t>(u);
			t.red = k; t.green = k; t.blue = k; t.alpha = std::numeric_limits<std::uint8_t>::max();
		}
		template<int K> inline void convert (sx::rgba_color<std::uint8_t,K> &t, std::uint32_t u) {
			const std::uint8_t k = imp::convert<std::uint8_t>(u);
			t.red = k; t.green = k; t.blue = k; t.alpha = std::numeric_limits<std::uint8_t>::max();
		}
		template<int K> inline void convert (sx::rgba_color<std::uint8_t,K> &t, sx::float16_t u) {
			const std::uint8_t k = imp::convert<std::uint8_t>(u);
			t.red = k; t.green = k; t.blue = k; t.alpha = std::numeric_limits<std::uint8_t>::max();
		}
		template<int K> inline void convert (sx::rgba_color<std::uint8_t,K> &t, float u) {
			const std::uint8_t k = imp::convert<std::uint8_t>(u);
			t.red = k; t.green = k; t.blue = k; t.alpha = std::numeric_limits<std::uint8_t>::max();
		}
		template<int K, int L> inline void convert (sx::rgba_color<std::uint8_t,K> &t, const sx::rgba_color<std::uint16_t,L> &u) {
			t.red = imp::convert<std::uint8_t>(u.red); t.green = imp::convert<std::uint8_t>(u.green); t.blue = imp::convert<std::uint8_t>(u.blue); t.alpha = imp::convert<std::uint8_t>(u.alpha);
		}
		template<int K, int L> inline void convert (sx::rgba_color<std::uint8_t,K> &t, const sx::rgb_color<std::uint8_t,L> &u) {
			t.red = imp::convert<std::uint8_t>(u.red); t.green = imp::convert<std::uint8_t>(u.green); t.blue = imp::convert<std::uint8_t>(u.blue); t.alpha = std::numeric_limits<std::uint8_t>::max();
		}
		template<int K, int L> inline void convert (sx::rgba_color<std::uint8_t,K> &t, const sx::rgba_color<std::uint8_t,L> &u) {
			t.red = imp::convert<std::uint8_t>(u.red); t.green = imp::convert<std::uint8_t>(u.green); t.blue = imp::convert<std::uint8_t>(u.blue); t.alpha = imp::convert<std::uint8_t>(u.alpha);
		}
		template<int K, int L> inline void convert (sx::rgba_color<std::uint8_t,K> &t, const sx::rgb_color<std::uint32_t,L> &u) {
			t.red = imp::convert<std::uint8_t>(u.red); t.green = imp::convert<std::uint8_t>(u.green); t.blue = imp::convert<std::uint8_t>(u.blue); t.alpha = std::numeric_limits<std::uint8_t>::max();
		}
		template<int K, int L> inline void convert (sx::rgba_color<std::uint8_t,K> &t, const sx::rgba_color<std::uint32_t,L> &u) {
			t.red = imp::convert<std::uint8_t>(u.red); t.green = imp::convert<std::uint8_t>(u.green); t.blue = imp::convert<std::uint8_t>(u.blue); t.alpha = imp::convert<std::uint8_t>(u.alpha);
		}
		template<int K, int L> inline void convert (sx::rgba_color<std::uint8_t,K> &t, const sx::rgb_color<sx::float16_t,L> &u) {
			t.red = imp::convert<std::uint8_t>(u.red); t.green = imp::convert<std::uint8_t>(u.green); t.blue = imp::convert<std::uint8_t>(u.blue); t.alpha = std::numeric_limits<std::uint8_t>::max();
		}
		template<int K, int L> inline void convert (sx::rgba_color<std::uint8_t,K> &t, const sx::rgba_color<sx::float16_t,L> &u) {
			t.red = imp::convert<std::uint8_t>(u.red); t.green = imp::convert<std::uint8_t>(u.green); t.blue = imp::convert<std::uint8_t>(u.blue); t.alpha = imp::convert<std::uint8_t>(u.alpha);
		}
		template<int K, int L> inline void convert (sx::rgba_color<std::uint8_t,K> &t, const sx::rgb_color<float,L> &u) {
			t.red = imp::convert<std::uint8_t>(u.red); t.green = imp::convert<std::uint8_t>(u.green); t.blue = imp::convert<std::uint8_t>(u.blue); t.alpha = std::numeric_limits<std::uint8_t>::max();
		}
		template<int K, int L> inline void convert (sx::rgba_color<std::uint8_t,K> &t, const sx::rgba_color<float,L> &u) {
			t.red = imp::convert<std::uint8_t>(u.red); t.green = imp::convert<std::uint8_t>(u.green); t.blue = imp::convert<std::uint8_t>(u.blue); t.alpha = imp::convert<std::uint8_t>(u.alpha);
		}

		// target: sx::rgba_color<std::uint32_t,K>
		template<int K> inline void convert (sx::rgba_color<std::uint32_t,K> &t, std::uint8_t u) {
			const std::uint32_t k = imp::convert<std::uint32_t>(u);
			t.red = k; t.green = k; t.blue = k; t.alpha = std::numeric_limits<std::uint32_t>::max();
		}
		template<int K> inline void convert (sx::rgba_color<std::uint32_t,K> &t, std::uint16_t u) {
			const std::uint32_t k = imp::convert<std::uint32_t>(u);
			t.red = k; t.green = k; t.blue = k; t.alpha = std::numeric_limits<std::uint32_t>::max();
		}
		template<int K> inline void convert (sx::rgba_color<std::uint32_t,K> &t, std::uint32_t u) {
			const std::uint32_t k = imp::convert<std::uint32_t>(u);
			t.red = k; t.green = k; t.blue = k; t.alpha = std::numeric_limits<std::uint32_t>::max();
		}
		template<int K> inline void convert (sx::rgba_color<std::uint32_t,K> &t, sx::float16_t u) {
			const std::uint32_t k = imp::convert<std::uint32_t>(u);
			t.red = k; t.green = k; t.blue = k; t.alpha = std::numeric_limits<std::uint32_t>::max();
		}
		template<int K> inline void convert (sx::rgba_color<std::uint32_t,K> &t, float u) {
			const std::uint32_t k = imp::convert<std::uint32_t>(u);
			t.red = k; t.green = k; t.blue = k; t.alpha = std::numeric_limits<std::uint32_t>::max();
		}
		template<int K, int L> inline void convert (sx::rgba_color<std::uint32_t,K> &t, const sx::rgb_color<std::uint8_t,L> &u) {
			t.red = imp::convert<std::uint32_t>(u.red); t.green = imp::convert<std::uint32_t>(u.green); t.blue = imp::convert<std::uint32_t>(u.blue); t.alpha = std::numeric_limits<std::uint32_t>::max();
		}
		template<int K, int L> inline void convert (sx::rgba_color<std::uint32_t,K> &t, const sx::rgba_color<std::uint8_t,L> &u) {
			t.red = imp::convert<std::uint32_t>(u.red); t.green = imp::convert<std::uint32_t>(u.green); t.blue = imp::convert<std::uint32_t>(u.blue); t.alpha = imp::convert<std::uint32_t>(u.alpha);
		}
		template<int K, int L> inline void convert (sx::rgba_color<std::uint32_t,K> &t, const sx::rgb_color<std::uint32_t,L> &u) {
			t.red = imp::convert<std::uint32_t>(u.red); t.green = imp::convert<std::uint32_t>(u.green); t.blue = imp::convert<std::uint32_t>(u.blue); t.alpha = std::numeric_limits<std::uint32_t>::max();
		}
		template<int K, int L> inline void convert (sx::rgba_color<std::uint32_t,K> &t, const sx::rgba_color<std::uint32_t,L> &u) {
			t.red = imp::convert<std::uint32_t>(u.red); t.green = imp::convert<std::uint32_t>(u.green); t.blue = imp::convert<std::uint32_t>(u.blue); t.alpha = imp::convert<std::uint32_t>(u.alpha);
		}
		template<int K, int L> inline void convert (sx::rgba_color<std::uint32_t,K> &t, const sx::rgb_color<sx::float16_t,L> &u) {
			t.red = imp::convert<std::uint32_t>(u.red); t.green = imp::convert<std::uint32_t>(u.green); t.blue = imp::convert<std::uint32_t>(u.blue); t.alpha = std::numeric_limits<std::uint32_t>::max();
		}
		template<int K, int L> inline void convert (sx::rgba_color<std::uint32_t,K> &t, const sx::rgba_color<sx::float16_t,L> &u) {
			t.red = imp::convert<std::uint32_t>(u.red); t.green = imp::convert<std::uint32_t>(u.green); t.blue = imp::convert<std::uint32_t>(u.blue); t.alpha = imp::convert<std::uint32_t>(u.alpha);
		}
		template<int K, int L> inline void convert (sx::rgba_color<std::uint32_t,K> &t, const sx::rgb_color<float,L> &u) {
			t.red = imp::convert<std::uint32_t>(u.red); t.green = imp::convert<std::uint32_t>(u.green); t.blue = imp::convert<std::uint32_t>(u.blue); t.alpha = std::numeric_limits<std::uint32_t>::max();
		}
		template<int K, int L> inline void convert (sx::rgba_color<std::uint32_t,K> &t, const sx::rgba_color<float,L> &u) {
			t.red = imp::convert<std::uint32_t>(u.red); t.green = imp::convert<std::uint32_t>(u.green); t.blue = imp::convert<std::uint32_t>(u.blue); t.alpha = imp::convert<std::uint32_t>(u.alpha);
		}

		// target: sx::rgb_color<sx::float16_t,K>
		template<int K> inline void convert (sx::rgb_color<sx::float16_t,K> &t, std::uint8_t u) {
			const sx::float16_t k = imp::convert<sx::float16_t>(u);
			t.red = k; t.green = k; t.blue = k;
		}
		template<int K> inline void convert (sx::rgb_color<sx::float16_t,K> &t, std::uint16_t u) {
			const sx::float16_t k = imp::convert<sx::float16_t>(u);
			t.red = k; t.green = k; t.blue = k;
		}
		template<int K> inline void convert (sx::rgb_color<sx::float16_t,K> &t, std::uint32_t u) {
			const sx::float16_t k = imp::convert<sx::float16_t>(u);
			t.red = k; t.green = k; t.blue = k;
		}
		template<int K, int L> inline void convert (sx::rgb_color<sx::float16_t,K> &t, const sx::rgb_color<std::uint8_t,L> &u) {
			t.red = imp::convert<sx::float16_t>(u.red); t.green = imp::convert<sx::float16_t>(u.green); t.blue = imp::convert<sx::float16_t>(u.blue);
		}
		template<int K, int L> inline void convert (sx::rgb_color<sx::float16_t,K> &t, const sx::rgba_color<std::uint8_t,L> &u) {
			t.red = imp::convert<sx::float16_t>(u.red); t.green = imp::convert<sx::float16_t>(u.green); t.blue = imp::convert<sx::float16_t>(u.blue);
		}
		template<int K, int L> inline void convert (sx::rgb_color<sx::float16_t,K> &t, const sx::rgb_color<sx::float16_t,L> &u) {
			t.red = imp::convert<sx::float16_t>(u.red); t.green = imp::convert<sx::float16_t>(u.green); t.blue = imp::convert<sx::float16_t>(u.blue);
		}
		template<int K, int L> inline void convert (sx::rgb_color<sx::float16_t,K> &t, const sx::rgba_color<sx::float16_t,L> &u) {
			t.red = imp::convert<sx::float16_t>(u.red); t.green = imp::convert<sx::float16_t>(u.green); t.blue = imp::convert<sx::float16_t>(u.blue);
		}
		template<int K, int L> inline void convert (sx::rgb_color<sx::float16_t,K> &t, const sx::rgb_color<float,L> &u) {
			t.red = imp::convert<sx::float16_t>(u.red); t.green = imp::convert<sx::float16_t>(u.green); t.blue = imp::convert<sx::float16_t>(u.blue);
		}
		template<int K, int L> inline void convert (sx::rgb_color<sx::float16_t,K> &t, const sx::rgba_color<float,L> &u) {
			t.red = imp::convert<sx::float16_t>(u.red); t.green = imp::convert<sx::float16_t>(u.green); t.blue = imp::convert<sx::float16_t>(u.blue);
		}
		
		// target: sx::rgba_color<sx::float16_t,K>
		template<int K> inline void convert (sx::rgba_color<sx::float16_t,K> &t, std::uint8_t u) {
			const sx::float16_t k = imp::convert<sx::float16_t>(u);
			t.red = k; t.green = k; t.blue = k; t.alpha = sx::float16_t(1.0f);
		}
		template<int K> inline void convert (sx::rgba_color<sx::float16_t,K> &t, std::uint16_t u) {
			const sx::float16_t k = imp::convert<sx::float16_t>(u);
			t.red = k; t.green = k; t.blue = k; t.alpha = sx::float16_t(1.0f);
		}
		template<int K> inline void convert (sx::rgba_color<sx::float16_t,K> &t, std::uint32_t u) {
			const sx::float16_t k = imp::convert<sx::float16_t>(u);
			t.red = k; t.green = k; t.blue = k; t.alpha = sx::float16_t(1.0f);
		}
		template<int K> inline void convert (sx::rgba_color<sx::float16_t,K> &t, sx::float16_t u) {
			const sx::float16_t k = imp::convert<sx::float16_t>(u);
			t.red = k; t.green = k; t.blue = k; t.alpha = sx::float16_t(1.0f);
		}
		template<int K> inline void convert (sx::rgba_color<sx::float16_t,K> &t, float u) {
			const sx::float16_t k = imp::convert<sx::float16_t>(u);
			t.red = k; t.green = k; t.blue = k; t.alpha = sx::float16_t(1.0f);
		}
		template<int K, int L> inline void convert (sx::rgba_color<sx::float16_t,K> &t, const sx::rgb_color<std::uint8_t,L> &u) {
			t.red = imp::convert<sx::float16_t>(u.red); t.green = imp::convert<sx::float16_t>(u.green); t.blue = imp::convert<sx::float16_t>(u.blue); t.alpha = sx::float16_t(1.0f);
		}
		template<int K, int L> inline void convert (sx::rgba_color<sx::float16_t,K> &t, const sx::rgba_color<std::uint8_t,L> &u) {
			t.red = imp::convert<sx::float16_t>(u.red); t.green = imp::convert<sx::float16_t>(u.green); t.blue = imp::convert<sx::float16_t>(u.blue); t.alpha = imp::convert<sx::float16_t>(u.alpha);
		}
		template<int K, int L> inline void convert (sx::rgba_color<sx::float16_t,K> &t, const sx::rgba_color<std::uint16_t,L> &u) {
			t.red = imp::convert<sx::float16_t>(u.red); t.green = imp::convert<sx::float16_t>(u.green); t.blue = imp::convert<sx::float16_t>(u.blue); t.alpha = imp::convert<sx::float16_t>(u.alpha);
		}
		template<int K, int L> inline void convert (sx::rgba_color<sx::float16_t,K> &t, const sx::rgb_color<sx::float16_t,L> &u) {
			t.red = imp::convert<sx::float16_t>(u.red); t.green = imp::convert<sx::float16_t>(u.green); t.blue = imp::convert<sx::float16_t>(u.blue); t.alpha = sx::float16_t(1.0f);
		}
		template<int K, int L> inline void convert (sx::rgba_color<sx::float16_t,K> &t, const sx::rgba_color<sx::float16_t,L> &u) {
			t.red = imp::convert<sx::float16_t>(u.red); t.green = imp::convert<sx::float16_t>(u.green); t.blue = imp::convert<sx::float16_t>(u.blue); t.alpha = imp::convert<sx::float16_t>(u.alpha);
		}
		template<int K, int L> inline void convert (sx::rgba_color<sx::float16_t,K> &t, const sx::rgb_color<float,L> &u) {
			t.red = imp::convert<sx::float16_t>(u.red); t.green = imp::convert<sx::float16_t>(u.green); t.blue = imp::convert<sx::float16_t>(u.blue); t.alpha = sx::float16_t(1.0f);
		}
		template<int K, int L> inline void convert (sx::rgba_color<sx::float16_t,K> &t, const sx::rgba_color<float,L> &u) {
			t.red = imp::convert<sx::float16_t>(u.red); t.green = imp::convert<sx::float16_t>(u.green); t.blue = imp::convert<sx::float16_t>(u.blue); t.alpha = imp::convert<sx::float16_t>(u.alpha);
		}

		// target: sx::rgb_color<float,K>
		template<int K> inline void convert (sx::rgb_color<float,K> &t, std::uint8_t u) {
			const float k = imp::convert<float>(u);
			t.red = k; t.green = k; t.blue = k;
		}
		template<int K> inline void convert (sx::rgb_color<float,K> &t, std::uint16_t u) {
			const float k = imp::convert<float>(u);
			t.red = k; t.green = k; t.blue = k;
		}
		template<int K> inline void convert (sx::rgb_color<float,K> &t, std::uint32_t u) {
			const float k = imp::convert<float>(u);
			t.red = k; t.green = k; t.blue = k;
		}
		template<int K> inline void convert (sx::rgb_color<float,K> &t, sx::float16_t u) {
			const float k = imp::convert<float>(u);
			t.red = k; t.green = k; t.blue = k;
		}
		template<int K> inline void convert (sx::rgb_color<float,K> &t, float u) {
			t.red = u; t.green = u; t.blue = u;
		}
		template<int K, int L> inline void convert (sx::rgb_color<float,K> &t, const sx::rgb_color<std::uint8_t,L> &u) {
			t.red = imp::convert<float>(u.red); t.green = imp::convert<float>(u.green); t.blue = imp::convert<float>(u.blue);
		}
		template<int K, int L> inline void convert (sx::rgb_color<float,K> &t, const sx::rgba_color<std::uint8_t,L> &u) {
			t.red = imp::convert<float>(u.red); t.green = imp::convert<float>(u.green); t.blue = imp::convert<float>(u.blue);
		}
		template<int K, int L> inline void convert (sx::rgb_color<float,K> &t, const sx::rgb_color<sx::float16_t,L> &u) {
			t.red = imp::convert<float>(u.red); t.green = imp::convert<float>(u.green); t.blue = imp::convert<float>(u.blue);
		}
		template<int K, int L> inline void convert (sx::rgb_color<float,K> &t, const sx::rgba_color<sx::float16_t,L> &u) {
			t.red = imp::convert<float>(u.red); t.green = imp::convert<float>(u.green); t.blue = imp::convert<float>(u.blue);
		}
		template<int K, int L> inline void convert (sx::rgb_color<float,K> &t, const sx::rgb_color<float,L> &u) {
			t.red = imp::convert<float>(u.red); t.green = imp::convert<float>(u.green); t.blue = imp::convert<float>(u.blue);
		}
		template<int K, int L> inline void convert (sx::rgb_color<float,K> &t, const sx::rgba_color<float,L> &u) {
			t.red = imp::convert<float>(u.red); t.green = imp::convert<float>(u.green); t.blue = imp::convert<float>(u.blue);
		}

		// target: sx::rgba_color<float,K>
		template<int K> inline void convert (sx::rgba_color<float,K> &t, std::uint8_t u) {
			const float k = imp::convert<float>(u);
			t.red = k; t.green = k; t.blue = k; t.alpha = 1.0f;
		}
		template<int K> inline void convert (sx::rgba_color<float,K> &t, std::uint16_t u) {
			const float k = imp::convert<float>(u);
			t.red = k; t.green = k; t.blue = k; t.alpha = 1.0f;
		}
		template<int K> inline void convert (sx::rgba_color<float,K> &t, std::uint32_t u) {
			const float k = imp::convert<float>(u);
			t.red = k; t.green = k; t.blue = k; t.alpha = 1.0f;
		}
		template<int K> inline void convert (sx::rgba_color<float,K> &t, sx::float16_t u) {
			const float k = imp::convert<float>(u);
			t.red = k; t.green = k; t.blue = k; t.alpha = 1.0f;
		}
		template<int K> inline void convert (sx::rgba_color<float,K> &t, float u) {
			t.red = u; t.green = u; t.blue = u; t.alpha = 1.0f;
		}
		template<int K, int L> inline void convert (sx::rgba_color<float,K> &t, const sx::rgb_color<std::uint8_t,L> &u) {
			t.red = imp::convert<float>(u.red); t.green = imp::convert<float>(u.green); t.blue = imp::convert<float>(u.blue); t.alpha = 1.0f;
		}
		template<int K, int L> inline void convert (sx::rgba_color<float,K> &t, const sx::rgba_color<std::uint8_t,L> &u) {
			t.red = imp::convert<float>(u.red); t.green = imp::convert<float>(u.green); t.blue = imp::convert<float>(u.blue); t.alpha = imp::convert<float>(u.alpha);
		}
		template<int K, int L> inline void convert (sx::rgba_color<float,K> &t, const sx::rgb_color<std::uint16_t,L> &u) {
			t.red = imp::convert<float>(u.red); t.green = imp::convert<float>(u.green); t.blue = imp::convert<float>(u.blue); t.alpha = 1.0f;
		}
		template<int K, int L> inline void convert (sx::rgba_color<float,K> &t, const sx::rgba_color<std::uint16_t,L> &u) {
			t.red = imp::convert<float>(u.red); t.green = imp::convert<float>(u.green); t.blue = imp::convert<float>(u.blue); t.alpha = imp::convert<float>(u.alpha);
		}
		template<int K, int L> inline void convert (sx::rgba_color<float,K> &t, const sx::rgb_color<sx::float16_t,L> &u) {
			t.red = imp::convert<float>(u.red); t.green = imp::convert<float>(u.green); t.blue = imp::convert<float>(u.blue); t.alpha = 1.0f;
		}
		template<int K, int L> inline void convert (sx::rgba_color<float,K> &t, const sx::rgba_color<sx::float16_t,L> &u) {
			t.red = imp::convert<float>(u.red); t.green = imp::convert<float>(u.green); t.blue = imp::convert<float>(u.blue); t.alpha = imp::convert<float>(u.alpha);
		}
		template<int K, int L> inline void convert (sx::rgba_color<float,K> &t, const sx::rgb_color<float,L> &u) {
			t.red = imp::convert<float>(u.red); t.green = imp::convert<float>(u.green); t.blue = imp::convert<float>(u.blue); t.alpha = 1.0f;
		}
		template<int K, int L> inline void convert (sx::rgba_color<float,K> &t, const sx::rgba_color<float,L> &u) {
			t.red = imp::convert<float>(u.red); t.green = imp::convert<float>(u.green); t.blue = imp::convert<float>(u.blue); t.alpha = imp::convert<float>(u.alpha);
		}
		
		#if SXAPI_COCOA
			template<typename T> inline void convert (NSColor* __strong& t, const T &u) {
				const sx::rgba c = convert<sx::rgba>(u);
				t = [NSColor colorWithDeviceRed:c.red green:c.green blue:c.blue alpha:c.alpha];
			}
			template<typename T> inline void convert (T &t, NSColor *u) {
				convert(t, imp::convert(u));
			}
		#endif

		template<typename T> inline void convert (T &t, std::int32_t u) {
			SXASSERT(false);
		}
		template<> inline void convert (unsigned int &t, std::int32_t u) {
			t = u;
		}
		template<int K> inline void convert (sx::rgb_color<std::uint8_t,K> &t, std::int32_t u) {
			t = sx::rgba_color<std::uint8_t,K>((u & 0x00ff0000) >> 16, (u & 0x0000ff00) >> 8, (u & 0x000000ff));
		}
		template<int K> inline void convert (sx::rgba_color<std::uint8_t,K> &t, std::int32_t u) {
			t = sx::rgba_color<std::uint8_t,K>((u & 0x00ff0000) >> 16, (u & 0x0000ff00) >> 8, (u & 0x000000ff), 0);
		}
		template<typename U> inline void convert (std::int32_t &t, const U &u) {
			SXASSERT(false);
		}
		template<> inline void convert (std::int32_t &t, const unsigned int &u) {
			t = u;
		}
		template<int K> inline void convert (std::int32_t &t, const sx::rgb_color<std::uint8_t,K> &u) {
			t = (std::uint32_t(u.red) << 16) | (std::uint32_t(u.green) << 8) | (std::uint32_t(u.blue));
		}
		template<int K> inline void convert (std::int32_t &t, const sx::rgba_color<std::uint8_t,K> &u) {
			t = (std::uint32_t(u.red) << 16) | (std::uint32_t(u.green) << 8) | (std::uint32_t(u.blue));
		}

		template<typename T, typename U> inline T convert (U u) { T t; convert(t, u); return t; }
	}

	class pixelop_interface {
	public:
		void operator() (sx::rgba_color<std::uint8_t> *dst, const sx::rgba *src, int n) const { pixelop_imp(dst, src, n); } // should be removed and cleaned up
		void operator() (sx::rgba *dst, const sx::rgba *src, int n) const { pixelopfloat_imp(dst, src, n); }
		virtual ~pixelop_interface () { }
	private:
		virtual void pixelop_imp (sx::rgba_color<std::uint8_t> *dst, const sx::rgba *src, int n) const { SXASSERT(false); } // should be removed and cleaned up
		virtual void pixelopfloat_imp (sx::rgba *dst, const sx::rgba *src, int n) const { SXASSERT(false); }
	};

	class opacity_pixelop_class : public pixelop_interface {
	public:
		explicit opacity_pixelop_class (float opacity) : opacity(opacity) { }
	private:
		const float opacity;
		virtual void pixelop_imp (sx::rgba_color<std::uint8_t> *dst, const sx::rgba *src, int n) const {
			for (int i = 0; i < n; ++i)  pixel::convert(dst[i], src[i] * opacity);
		}
	};
	extern opacity_pixelop_class opacity_pixelop;

}

inline const sx::rgba_color<std::uint8_t> operator* (const sx::rgba_color<std::uint8_t> &c, unsigned k) {
	return sx::rgba_color<std::uint8_t>((c.red*k), (c.green*k), (c.blue*k), (c.alpha*k));
}
