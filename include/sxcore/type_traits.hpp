#pragma once
SXMODULE_SXCORE

namespace sx {
//	#if defined(__apple_build_version__) && (5030000 <= __apple_build_version__)
//		template<typename T>			struct is_movable							: std::integral_constant<bool, (std::is_nothrow_move_constructible<T>::value && std::is_nothrow_move_assignable<T>::value)> { };
//	#else	// noexcept not supported.
//		template<typename T>			struct is_movable							: std::integral_constant<bool, (std::is_move_constructible<T>::value && std::is_move_assignable<T>::value)> { };
//	#endif

	namespace core {
		namespace unittest {
			void test_type_traits ();
		}
	}
}
