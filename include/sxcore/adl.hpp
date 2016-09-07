#pragma once
SXMODULE_SXCORE
#include "sxcore/type_traits.hpp"
//// workaround for ADL (Koenig lookup) problem. see http://accu.org/index.php/journals/466

namespace sx {
	namespace adl {
		template<typename T> void swap (T& left, T& right) {
			using std::swap;
			swap(left,right);
		}
		template<typename T, std::size_t N> void swap(T (&left)[N], T (&right)[N]) {
			for (std::size_t i = 0; i < N; ++i) sx::adl::swap(left[i], right[i]);
		}
	}
}
