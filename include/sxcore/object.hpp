#pragma once
SXMODULE_SXCORE
#include "sxcore/debug.hpp"

namespace sx {
	class object {
	public:
		object& operator= (const object&) SXLREF = default;
	public:
		#if SXSIGNATURE
			explicit object () noexcept : _signature(0xB13919F7) {
				sx::assert_invariant(*this);
			}
			object (const object&) = default;
			~object () noexcept {
				sx::assert_invariant(*this);
				_signature = 0xbaadf00d;
			}
			void check_signature () const {
				SXASSERT(_signature == 0xB13919F7);
			}
			unsigned _signature;
		#else
			object () = default;
			object (const object&) = default;
			~object () = default;
			void check_signature () const {
			}
		#endif
		void assert_invariant () const {
			SXASSERT(bool(this));
			check_signature();
		}
	public:
		#if SXOS_WINDOWS && SX32
			void *operator new (std::size_t size) {
				void* const p = ::_aligned_malloc(size, 16);
				if (!p) throw std::bad_alloc();
				return p;
			}
			void *operator new (std::size_t size, void *p) {
				return p;
			}
			void operator delete (void *p) {
				::_aligned_free(p);
			}
			void operator delete (void *p, void *q) {
			}
			void *operator new[] (std::size_t size) {
				void* const p = ::_aligned_malloc(size, 16);
				if (!p) throw std::bad_alloc();
				return p;
			}
			void operator delete[] (void *p) {
				::_aligned_free(p);
			}
		#endif
	};
}
