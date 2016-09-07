#pragma once
SXMODULE_SXCORE
#include "sxcore/types.hpp"
#include "sxcore/unittest.hpp"

namespace sx {
	inline bool little_endian () {
		#if __LITTLE_ENDIAN__
			return true;
		#elif __BIG_ENDIAN__
			return false;
		#else
			unsigned u = 0x01234567;
			return *(unsigned char *)&u == 0x67;
		#endif
	}
	inline bool big_endian () {
		#if __BIG_ENDIAN__
			return true;
		#elif __LITTLE_ENDIAN__
			return false;
		#else
			unsigned u = 0x01234567;
			return *(unsigned char *)&u == 0x01;
		#endif
	}
	namespace imp {
		template<int N> inline void revert_endian (void *p) {
			char b[N];
			for (int i = 0; i < N; ++i) b[N-1-i] = static_cast<char*>(p)[i];
			memcpy(p, b, N);
		}
	}
	template<typename T> inline void revert_endian (T& t) { static_assert(std::is_trivially_copyable<T>::value, "");
		if (1 < sizeof(t)) sx::imp::revert_endian<sizeof(t)>(&t);
	}

	template<typename T> inline void to_little_endian (T &t) {
		if (big_endian()) revert_endian(t);
	}
	template<typename T> inline void to_big_endian (T &t) {
		if (little_endian()) revert_endian(t);
	}
	template<typename T> inline void from_little_endian (T &t) {
		if (big_endian()) revert_endian(t);
	}
	template<typename T> inline void from_big_endian (T &t) {
		if (little_endian()) revert_endian(t);
	}
}
