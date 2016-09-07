#pragma once
SXMODULE_SXCORE

namespace sx {
	class float16_t {
	public:
		float16_t () { }
		float16_t (float f);
		operator float() const;
	private:
		std::uint16_t _h;
		union uif {
			std::uint32_t	i;
			float			f;
		};
		static const uif	        _toFloat[1 << 16];
		static const unsigned short _eLut[1 << 9];
		static std::int16_t convert (std::int32_t i);
		static float overflow ();
	};
	inline sx::float16_t::float16_t (float f) {
		uif x;

		x.f = f;

		if (f == 0)
		{
		//
		// Common special case - zero.
		// Preserve the zero's sign bit.
		//

		_h = (x.i >> 16);
		}
		else
		{
		//
		// We extract the combined sign and exponent, e, from our
		// floating-point number, f.  Then we convert e to the sign
		// and exponent of the half number via a table lookup.
		//
		// For the most common case, where a normalized half is produced,
		// the table lookup returns a non-zero value; in this case, all
		// we have to do is round f's significand to 10 bits and combine
		// the result with e.
		//
		// For all other cases (overflow, zeroes, denormalized numbers
		// resulting from underflow, infinities and NANs), the table
		// lookup returns zero, and we call a longer, non-inline function
		// to do the float-to-half conversion.
		//

		int e = (x.i >> 23) & 0x000001ff;

		e = _eLut[e];

		if (e)
		{
			//
			// Simple case - round the significand, m, to 10
			// bits and combine it with the sign and exponent.
			//

			int m = x.i & 0x007fffff;
			_h = e + ((m + 0x00000fff + ((m >> 13) & 1)) >> 13);
		}
		else
		{
			//
			// Difficult case - call a function.
			//

			_h = convert (x.i);
		}
		}
	}
	inline float16_t::operator float() const {
		return _toFloat[_h].f;
	}
}
namespace std {
	inline sx::float16_t abs (const sx::float16_t t) { return sx::float16_t(std::abs(float(t))); }
}
