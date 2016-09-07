#pragma once
SXMODULE_SXCORE

#include "sxcore/adl.hpp"
#include "sxcore/memory.hpp"
#include "sxcore/type_traits.hpp"
#include "sxcore/debug.hpp"
#include "sxcore/settings.hpp"
#include "sxcore/iterator.hpp"

//	23.2.1 General container requirements 
//
//	Unless otherwise specified (see 23.2.4.1, 23.2.5.1, 23.3.3.4, and 23.3.6.5) all container types defined in this Clause meet the following additional requirements:
//
//	- if an exception is thrown by an insert() or emplace() function while inserting a single element, that function has no effects.
//	- if an exception is thrown by a push_back() or push_front() function, that function has no effects.
//	- no erase(), clear(), pop_back() or pop_front() function throws an exception.
//	- no copy constructor or assignment operator of a returned iterator throws an exception.
//	- no swap() function throws an exception.
//	- no swap() function invalidates any references, pointers, or iterators referring to the elements of the containers being swapped.

namespace sx {
	using ::boost::container::static_vector;

	template<typename T> using is_relocatable = std::is_trivially_copyable<T>;

	template<typename T> class _small_vector_pointer {
	public:
		T*			get () const						{ return static_cast<T*>(_p); }
		T&			operator[] (std::size_t i) const	{ return get()[i]; }
		std::size_t	capacity () const					{ return _p.capacity() / sizeof(T); }
		void		malloc (std::size_t n)				{ _p.malloc(sizeof(T) * n); }
		void		realloc (std::size_t n)				{ _p.realloc(sizeof(T) * n); }
		void		free ()								{ _p.free(); }
		void		operator= (void* q)					{ _p = q; }
		explicit operator bool () const	{ return bool(_p); }
	private:
		sx::memory::pointer<true>	_p;
	};

	template<typename T, typename U, std::size_t N> class _small_vector_base {
	public:
		using iterator			= iterator_template<T>;
		using const_iterator	= const_iterator_template<T>;
	protected:
		U*				storagep ()										{ return reinterpret_cast<U*>(_storage); }
		const U*		storagep () const								{ return reinterpret_cast<const U*>(_storage); }
		iterator		make_iterator (T& t, std::size_t i)				{ return iterator(t, i); }
		const_iterator	make_iterator (const T& t, std::size_t i) const	{ return iterator(t, i); }
	private:
		typename std::aligned_storage<sizeof(U),std::alignment_of<U>::value>::type	_storage[N];
	};
	template<typename T, typename U> class _small_vector_base<T,U,0> {
	public:
		using iterator			= U*;
		using const_iterator	= const U*;
	protected:
		U*				storagep ()										{ SXASSERT(false); return nullptr; }
		const U*		storagep () const								{ SXASSERT(false); return nullptr; }
		iterator		make_iterator (T& t, std::size_t i)				{ SXASSERT(t._p || (i == 0)); return (t._p.get() + i); }
		const_iterator	make_iterator (const T& t, std::size_t i) const	{ SXASSERT(t._p || (i == 0)); return (t._p.get() + i); }
	};

	template<typename T, std::size_t N> class small_vector : public _small_vector_base<small_vector<T,N>,T,N> { using base = _small_vector_base<small_vector<T,N>,T,N>; friend class _small_vector_base<small_vector<T,N>,T,N>;
		inline static T&& _move (T& t) {
			#if defined(__apple_build_version__) && (5030000 <= __apple_build_version__)
				static_assert((std::is_nothrow_move_constructible<T>::value && std::is_nothrow_move_assignable<T>::value), "");
			#endif
			return static_cast<T&&>(t);
		}
		template<typename U> void _resize (std::size_t first, std::size_t last, U u) {
			reserve(last);
			{	std::size_t i = first;
				try {
					for (; i < last; ++i) u(i);
				}
				catch (...) {
					for (std::size_t j = first; j < i; ++j) _at(j)->~T();
					throw;
				}
			}
			_size = last;
		}
	public:
		using size_type			= std::size_t;
		using value_type		= T;
		using difference_type	= std::ptrdiff_t;
		using iterator			= typename _small_vector_base<small_vector<T,N>,T,N>::iterator;
		using const_iterator	= typename _small_vector_base<small_vector<T,N>,T,N>::const_iterator;

		explicit small_vector () noexcept {
			sx::assert_invariant(*this);
		}
		explicit small_vector (size_type n) {
			auto _ = sx::make_invariant_scope(*this);
			resize(n);
		}
        explicit small_vector (size_type n, const T& t) {
			auto _ = sx::make_invariant_scope(*this);
			resize(n, t);
		}
		
        template<typename INPUTIT> small_vector (typename std::enable_if<!std::is_convertible<size_type,INPUTIT>::value,INPUTIT>::type first, INPUTIT last) {
			auto _ = sx::make_invariant_scope(*this);
			insert(end(), first, last);
		}
		small_vector (const small_vector& t) : small_vector(t.begin(), t.end()) {
			sx::assert_invariant(*this);
		}
		template<std::size_t M> small_vector (const small_vector<T,M>& t) : small_vector(t.begin(), t.end()) {
		}
        small_vector (small_vector&& t) noexcept {
			auto _ = sx::make_invariant_scope(*this);
			(*this) = std::move(t);
		}
		small_vector (std::initializer_list<T> t) : small_vector(t.begin(), t.end()) {
		}
		
		small_vector& operator= (const small_vector& t) {
			auto _ = sx::make_invariant_scope(*this);
			if (this != &t) {
				resize(0);
				insert(end(), t.begin(), t.end());
			}
			return *this;
		}
		small_vector& operator= (small_vector&& t) noexcept {
			auto _ = sx::make_invariant_scope(*this);
			if (0 < N) {
				const auto n = std::min(N, t._size);
				const auto m = std::min(N, _size);
				const auto c = std::min(m, n);
				for (size_type i = 0; i < c; ++i) _storagep()[i] = _move(t._storagep()[i]);
				for (size_type i = c; i < n; ++i) ::new (_storagep()+i) T(_move(t._storagep()[i]));
				for (size_type i = n; i < m; ++i) _storagep()[i].~T();
				for (size_type i = 0; i < n; ++i) t._storagep()[i].~T();
			}
			_p.free();
			_p		= t._p;
			t._p 	= nullptr;
			_size	= t._size;
			t._size	= 0;
			return *this;
		}
		~small_vector () {
			sx::assert_invariant(*this);
			clear();
		}
		
		bool empty () const {
			return (_size == 0);
		}
		size_type size () const {
			return _size;
		}
		size_type capacity () const {
			return (N + _p.capacity());
		}
		void reserve (size_type n) {
			auto _ = sx::make_invariant_scope(*this);
			if (N < n) {
				n -= N;
				if (_p.capacity() < n) {
					if (sx::is_relocatable<T>::value) {
						_p.realloc(n);
					}
					else {
						_small_vector_pointer<T>	q; q.malloc(n);
						if (N < _size) {
							for (size_type i = 0; i < _size-N; ++i) ::new (&q[i]) T(_move(_p[i]));
							for (size_type i = 0; i < _size-N; ++i) _p[i].~T();
						}
						_p.free();
						_p 	= q;
						q	= nullptr;
					}
				}
			}
		}
		void resize (size_type n) {
			auto _ = sx::make_invariant_scope(*this);
			if (_size < n) {
				_resize(_size, n, [&](size_type i) { ::new (_at(i)) T(); });
			}
			else if (n < _size) {
				for (size_type i = n; i < _size; ++i) _at(i)->~T();
				_size = n;
			}
		}
		void resize (size_type n, const T& t) {
			auto _ = sx::make_invariant_scope(*this);
			if (_size < n) {
				_resize(_size, n, [&](size_type i) { ::new (_at(i)) T(t); });
			}
			else if (n < _size) {
				for (size_type i = n; i < _size; ++i) _at(i)->~T();
				_size = n;
			}
		}
		void clear () {
			for (size_type i = 0; i < _size; ++i) _at(i)->~T();
			_p.free();
			_size = 0;
		}
		
		iterator insert (const_iterator pos, const T& t) {
			auto _ = sx::make_invariant_scope(*this);
			const auto n = pos - cbegin(); SXASSERT(0 <= n && n <= _size);
			resize(_size+1);
			for (auto i = _size-1; n < i; --i) *_at(i) = _move(*_at(i-1));
			*_at(n) = t;
			return begin() + (pos - cbegin());
		}
		iterator insert (const_iterator pos, T&& t) {
			auto _ = sx::make_invariant_scope(*this);
			const auto n = pos - cbegin(); SXASSERT(0 <= n && n <= _size);
			resize(_size+1);
			for (auto i = _size-1; n < i; --i) *_at(i) = _move(*_at(i-1));
			*_at(n) = _move(t);
			return begin() + (pos - cbegin());
		}
		template<typename INPUTIT> iterator insert (const_iterator pos, INPUTIT first, INPUTIT last) {
			const auto n = pos - cbegin();	SXASSERT(0 <= n && n <= _size);
			const auto k = last - first;	SXASSERT(0 <= k);
			if (0 < k) {
				if (pos == end()) {
					_resize(_size, (_size + k), [&](size_type i) { ::new (_at(i)) T(*(first+(i-_size))); });
				}
				else {
					resize(_size + k);
					for (auto i = _size-1; n+k <= i; --i) *_at(i) = _move(*_at(i-k));
					for (size_type i = 0; i < k; ++i) *_at(n+i) = *(first+i);
				}
			}
			return (begin() + n);
		}
		template<typename INPUTIT> void append (INPUTIT first, INPUTIT last) {	// bandaid: faster than insert(end(), first, last). Mantis #12780.
			const auto k = last - first;	SXASSERT(0 <= k);
			if (0 < k) _resize(_size, (_size + k), [&](size_type i) { ::new (_at(i)) T(*(first+(i-_size))); });
		}

		T& front () {
			auto _ = sx::make_invariant_scope(*this);
			SXASSERT(0 < _size);
			return *_at(0);
		}
		const T& front () const {
			auto _ = sx::make_invariant_scope(*this);
			SXASSERT(0 < _size);
			return *_at(0);
		}
		T& back () {
			auto _ = sx::make_invariant_scope(*this);
			SXASSERT(0 < _size);
			return *_at(_size-1);
		}
		const T& back () const {
			auto _ = sx::make_invariant_scope(*this);
			SXASSERT(0 < _size);
			return *_at(_size-1);
		}
		T& operator[] (size_type i) {
			auto _ = sx::make_invariant_scope(*this);
			SXASSERT(0 <= i && i < _size);
			return *_at(i);
		}
		const T& operator[] (size_type i) const {
			auto _ = sx::make_invariant_scope(*this);
			SXASSERT(0 <= i && i < _size);
			return *_at(i);
		}
		void push_back (const T& t) {
			auto _ = sx::make_invariant_scope(*this);
			if (_size < capacity()) { ::new (_at(_size)) T(t); ++_size; }
			else { // should move outline
				T u(t);
				reserve(std::max(_size+1,_size*2));
				::new (_at(_size)) T(_move(u));
				++_size;
			}
		}
		void push_back (T&& t) {
			auto _ = sx::make_invariant_scope(*this);
			if (_size < capacity()) { ::new (_at(_size)) T(_move(t)); ++_size; }
			else { // should move outline
				reserve(std::max(_size+1,_size*2));
				::new (_at(_size)) T(_move(t));
				++_size;
			}
		}
		void pop_back () {
			auto _ = sx::make_invariant_scope(*this);
			SXASSERT(0 < _size);
			_at(_size-1)->~T();
			--_size;
		}
		iterator erase (const_iterator pos) {
			auto _ = sx::make_invariant_scope(*this);
			return erase(pos, pos+1);
		}
		iterator erase (const_iterator first, const_iterator last) {
			auto _ = sx::make_invariant_scope(*this);
			const auto n = first - cbegin();	SXASSERT(0 <= n && n <= _size);
			const auto k = last - first;		SXASSERT(0 <= k && (n+k) <= _size);
			if (0 < k) {
				SXASSERT(0 <= n && n+k <= _size);
				for (auto i = n+k; i < _size; ++i) *_at(i-k) = _move(*_at(i));
				resize(_size-k);
			}
			return (begin() + n);			
		}
		void shrink_to_fit () {
			auto _ = sx::make_invariant_scope(*this);
			if (_p && _size < capacity()) {
				if (_size <= N)	_p.free();
				else {
					if (sx::is_relocatable<T>::value) {
						_p.realloc(_size-N);
					}
					else {
						_small_vector_pointer<T>	q; q.malloc(_size-N);
						if (N < _size) {
							for (size_type i = 0; i < _size-N; ++i) ::new (&q[i]) T(_move(_p[i]));
							for (size_type i = 0; i < _size-N; ++i) _p[i].~T();
						}
						_p.free();
						_p 	= q;
						q	= nullptr;
					}
				}
			}
		}
		
		iterator begin () {
			auto _ = sx::make_invariant_scope(*this);
			return base::make_iterator(*this, 0);
		}
		const_iterator begin () const {
			auto _ = sx::make_invariant_scope(*this);
			return base::make_iterator(*this, 0);
		}
		const_iterator cbegin () const {
			auto _ = sx::make_invariant_scope(*this);
			return base::make_iterator(*this, 0);
		}
		iterator end () {
			auto _ = sx::make_invariant_scope(*this);
			return base::make_iterator(*this, _size);
		}
		const_iterator end () const {
			auto _ = sx::make_invariant_scope(*this);
			return base::make_iterator(*this, _size);
		}
		const_iterator cend () const {
			auto _ = sx::make_invariant_scope(*this);
			return base::make_iterator(*this, _size);
		}
		
		T* data () {
			if (N == 0) return _p.get();
		}
		const T* data () const {
			if (N == 0) return _p.get();
		}

		void assert_invariant () const {
			SXASSERT(_size <= (N + _p.capacity()));
		}
	private:
        std::size_t					_size{ 0 };
		_small_vector_pointer<T>	_p;

		T*			_storagep ()					{ return _small_vector_base<small_vector<T,N>,T,N>::storagep(); }
		const T*	_storagep () const				{ return _small_vector_base<small_vector<T,N>,T,N>::storagep(); }
		T*			_at (const size_type i)			{ SXASSERT(0 <= i && i < (N + _p.capacity())); if (i < N) return (_storagep() + i); SXASSERT(_p); return (_p.get() + (i-N)); }
		const T*	_at (const size_type i) const	{ SXASSERT(0 <= i && i < (N + _p.capacity())); if (i < N) return (_storagep() + i); SXASSERT(_p); return (_p.get() + (i-N)); }
	};
	
	template<typename T> using vector = small_vector<T,0>;
	
	template<typename T, std::size_t N> bool check_each (const sx::small_vector<T, N>& v) {
		v.check_each();
		return true;
	}

	template<typename T, std::size_t N> std::size_t using_size (const small_vector<T, N>& v) {	// without storage. for debugging.
		return ((v.size() <= N) ? 0 : (v.size() * sizeof(T)));
	}
	
	template<typename T, std::size_t N> auto begin	(small_vector<T,N>& t)			-> decltype(t.begin())	{ return t.begin(); }
	template<typename T, std::size_t N> auto begin	(const small_vector<T,N>& t)	-> decltype(t.begin())	{ return t.begin(); }
	template<typename T, std::size_t N> auto cbegin	(const small_vector<T,N>& t)	-> decltype(t.cbegin())	{ return t.cbegin(); }
	template<typename T, std::size_t N> auto end	(small_vector<T,N>& t)			-> decltype(t.end())	{ return t.end(); }
	template<typename T, std::size_t N> auto end	(const small_vector<T,N>& t)	-> decltype(t.end())	{ return t.end(); }
	template<typename T, std::size_t N> auto cend	(const small_vector<T,N>& t)	-> decltype(t.cend())	{ return t.cend(); }

	template<typename T, std::size_t M, std::size_t N> bool operator== (const sx::small_vector<T,M>& a, const sx::small_vector<T,N>& b) {
		if (a.size() != b.size()) return false;
		for (std::size_t i = 0, n = a.size(); i < n; ++i) if (a[i] != b[i]) return false;
		return true;
	}
	template<typename T, std::size_t M, std::size_t N> bool operator!= (const sx::small_vector<T,M>& a, const sx::small_vector<T,N>& b) {
		return !(a == b);
	}

	namespace core {
		namespace unittest {
			void test_xvector ();
		}
	}
}
template<typename T, std::size_t N> std::ostream& operator<< (std::ostream& stream, const sx::small_vector<T,N>& v) {
	for (const T& t : v) stream << t << std::endl;
	return stream;
}

namespace sx {
	template<typename T> sx::size_t heap_size (const std::vector<T>& t) {
		return (sizeof(T) * t.capacity());
	}
	template<typename T> sx::size_t heap_size (const sx::vector<T>& t) {
		return (sizeof(T) * t.capacity());
	}
	template<typename T,std::size_t N> sx::size_t heap_size (const sx::small_vector<T,N>& t) {
		return (sizeof(T) * std::max(sx::size_t(t.capacity())-sx::size_t(N),sx::size_t(0)));
	}
}
