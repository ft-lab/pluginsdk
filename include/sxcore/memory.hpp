#pragma once
SXMODULE_SXCORE
#include "sxcore/settings.hpp"
#include "sxcore/debug.hpp"

namespace sx {
	class recursive_mutex;

	namespace memory {
		#if SXAPI_MACH
			extern sx::recursive_mutex& mutex (); // vm_(de)allocate() routines are not thread safe.
		#endif

		void initialize ();
		void finalize ();

		void*	virtual_allocate (std::size_t n); // accepts multiiple of granularity although allocation may be slow. fast and efficient when n is multiple of chunk_size.
		void	virtual_deallocate (void* p, std::size_t n);
		void	virtual_query ();

		extern std::size_t	page_size;
		extern std::size_t	granularity;

		inline void* malloc (const std::size_t n) {
			if (n == 0) return nullptr;
			#if SXOS_WINDOWS
			void* const p = ::_aligned_malloc(n, 16);
			#else
			void* const p = ::malloc(n);
			#endif
			if (!p) {
				throw std::bad_alloc();
			}
			return p;
		}
		inline void* realloc (void* const q, const std::size_t n) {
			#if SXOS_WINDOWS
			SXCHECKLASTERROR;
			void* const p = ::_aligned_realloc(q, n, 16);
			if (::GetLastError() == ERROR_NOT_ENOUGH_MEMORY) ::SetLastError(0);
			SXCHECKLASTERROR;
			#else
			void* const p = ::realloc(q, n);
			#endif
			if ((0 < n) && !p) {
				throw std::bad_alloc();
			}
			return p;
		}
		inline void free (void* const p) {
			if (p) {
				#if SXOS_WINDOWS
				::_aligned_free(p);
				#else
				::free(p);
				#endif
			}
		}
		inline std::size_t malloc_size (const void* const p) {
			if (p) {
				#if SXOS_WINDOWS
					return p ? ::_aligned_msize((void*)p, 16, 0) : 0;
				#elif SXAPI_MACH
					return ::malloc_size(p);
                #else
                    return malloc_usable_size(p);
				#endif
			}
			return 0;
		}

		template<bool SIZE = true> class pointer {
		public:
			pointer () = default;
			explicit pointer (void* const p) noexcept : _p(p), _capacity(sx::memory::malloc_size(_p)) { }

			template<typename T>		operator T* () const		{ return static_cast<T*>(_p); }
			sx::size_t					capacity () const			{ return _capacity; }
			pointer&					operator= (void* const p)	{ _p = p; _capacity = sx::memory::malloc_size(_p); return *this; }
			void						malloc (sx::size_t n)		{ SXASSERT(!_p); _p = sx::memory::malloc(n); _capacity = sx::memory::malloc_size(_p); }
			void						realloc (sx::size_t n)		{ _p = sx::memory::realloc(_p, n); _capacity = sx::memory::malloc_size(_p); }
			void						free ()						{ sx::memory::free(_p); _p = nullptr; _capacity = 0; }
			template<typename T> void*	operator+ (T t) const		{ return (static_cast<char*>(_p) + t); }
			explicit					operator bool () const		{ return bool(_p); }
		private:
			void*		_p{};
			sx::size_t	_capacity{ 0 };
		};
		template<> class pointer<false> {
		public:
			pointer () = default;
			explicit pointer (void* const p) noexcept : _p(p) { }

			template<typename T>	operator T* () const		{ return static_cast<T*>(_p); }
			sx::size_t				size () const				{ if (_p) return sx::memory::malloc_size(_p); return 0; }
			pointer&				operator= (void* const p)	{ _p = p; return *this; }
			void					free ()						{ sx::memory::free(_p); _p = nullptr; }
			explicit				operator bool () const		{ return bool(_p); }
		private:
			void*		_p{};
		};

		void		check_heap ();
		std::size_t	used_bytes ();
		std::size_t	max_used_bytes ();
		void		reset_bytes ();

		class block_info_class {
		public:
			std::size_t size;
			bool extra;
			explicit block_info_class (std::size_t size, bool extra) : size(size), extra(extra) { }
		};

		std::size_t get_free_size ();
		void eat (std::size_t free);

//		template<typename T> class allocator {
//		public : 
//			using value_type								= T;
//			using pointer									= value_type*;
//			using const_pointer								= const value_type*;
//			using reference									= value_type&;
//			using const_reference							= const value_type&;
//			using size_type									= std::size_t;
//			using difference_type							= std::ptrdiff_t;
//			using propagate_on_container_move_assignment	= std::true_type;
//		public : 
//			template<typename U> struct rebind {
//				using other	= allocator<U>;
//			};
//		public : 
//			allocator () = default;
//			template<typename U> allocator (allocator<U> const&) noexcept { }
//
//			pointer			address (reference r) noexcept			{ return &r; }
//			const_pointer	address (const_reference r) noexcept	{ return &r; }
//
//			pointer allocate (size_type n, typename std::allocator<void>::const_pointer = nullptr) { 
//				return static_cast<pointer>(sx::memory::malloc(n * sizeof(T))); 
//			}
//			void deallocate (pointer p, size_type) noexcept { 
//				sx::memory::free(p); 
//			}
//
//			size_type max_size () const noexcept { 
//				return std::numeric_limits<size_type>::max() / sizeof(T);
//			 }
//
//			void construct (pointer p, const T& t)	{ new(p) T(t); }
//			void construct (pointer p, T&& t)	{ new(p) T(std::move(t)); }
//			void destroy (pointer p)				{ p->~T(); }
//
//			bool operator== (allocator const&) noexcept		{ return true; }
//			bool operator!=( allocator const& a) noexcept	{ return !operator==(a); }
//		};
	}
}

