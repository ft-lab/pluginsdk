#pragma once
SXMODULE_SXCORE

namespace sx {
	template<typename T, typename U> inline typename std::enable_if<std::is_arithmetic<T>::value,T>::type min (T t, U u) { return std::min<T>(t,u); }
	template<typename T, typename U> inline typename std::enable_if<std::is_arithmetic<T>::value,T>::type max (T t, U u) { return std::max<T>(t,u); }
	
	namespace core {
		namespace unittest {
			inline void test_minmax () {
			}
		}
	}
}
