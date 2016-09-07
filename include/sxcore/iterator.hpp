#pragma once
SXMODULE_SXCORE
#include "sxcore/object.hpp"

namespace sx {
	template<typename T> class const_iterator_template;

	template<typename T> class iterator_template : public sx::object { using base = sx::object; friend class const_iterator_template<T>;
	public:
		using iterator_category	= std::random_access_iterator_tag;
		using value_type		= typename T::value_type;
		using difference_type	= std::ptrdiff_t;
		using pointer			= value_type*;
		using reference			= value_type&;
		
		iterator_template (const iterator_template<T>& t) : _selfp(t._selfp), _index(t._index) {
		}
		explicit iterator_template () : _index(0), _selfp(nullptr) {
			sx::assert_invariant(*this);
		}
		explicit iterator_template (const T& self, std::size_t index) : _selfp(&self), _index(index) {
			sx::assert_invariant(*this);
		}
		iterator_template& operator= (const iterator_template& i) {
			auto _ = sx::make_invariant_scope(*this);
			sx::assert_invariant(i);
			_index = i._index;
			_selfp = i._selfp;
			return *this;
		}
		iterator_template& operator++ () { ++_index; return *this; }
		iterator_template operator++ (int) { const iterator_template i(*this); ++_index; return i; }
		iterator_template& operator-- () { --_index; return *this; }
		iterator_template operator-- (int) { const iterator_template i(*this); --_index; return i; }
		iterator_template operator+ (difference_type i) const {
			return iterator_template(*_selfp, _index + i);
		}
		iterator_template operator- (difference_type i) const {
			return iterator_template(*_selfp, _index - i);
		}
		difference_type operator- (const iterator_template& i) const {
			return _index - i._index;
		}
		iterator_template& operator+= (difference_type i) {
			_index += i;
			return *this;
		}
		iterator_template& operator-= (difference_type i) {
			_index -= i;
			return *this;
		}
		value_type& operator* () const {
			return const_cast<value_type&>((*_selfp)[_index]);
		}
		value_type* operator-> () const {
			return const_cast<value_type*>(&(*_selfp)[_index]);
		}
		value_type& operator[] (const std::size_t i) const {
			return const_cast<value_type&>((*_selfp)[_index + i]);
		}
		bool operator== (const iterator_template& i) const { sxassert(_selfp == i._selfp); return (_index == i._index); }
		bool operator!= (const iterator_template& i) const { return !((*this) == i); }
		bool operator< (const iterator_template& i) const {
			SXASSERT(_selfp == i._selfp);
			return (_index < i._index);
		}
		bool operator<= (const iterator_template& i) const {
			SXASSERT(_selfp == i._selfp);
			return (_index <= i._index);
		}
		bool operator> (const iterator_template& i) const {
			SXASSERT(_selfp == i._selfp);
			return (_index > i._index);
		}
		bool operator>= (const iterator_template& i) const {
			SXASSERT(_selfp == i._selfp);
			return (_index >= i._index);
		}
		difference_type offset () const {
			return _index;
		}
		void assert_invariant () const {
			sx::assert_invariant<sx::object>(*this);
			if (_selfp)	SXASSERT((-1 <= _index) && (_index <= _selfp->size()));
			else		SXASSERT(_index == 0);
		}
	private:
		std::ptrdiff_t	_index;
		const T*		_selfp;
	};

	template<typename T> class const_iterator_template : public sx::object { using base = sx::object;
	public:
		using iterator_category	= std::random_access_iterator_tag;
		using value_type		= const typename T::value_type;
		using difference_type	= std::ptrdiff_t;
		using pointer			= value_type*;
		using reference			= value_type&;
		
		const_iterator_template (const iterator_template<T>& t) : _selfp(t._selfp), _index(t._index) {
		}
		const_iterator_template (const const_iterator_template<T>& t) : _selfp(t._selfp), _index(t._index) {
		}
		explicit const_iterator_template () : _index(0), _selfp(nullptr) {
			sx::assert_invariant(*this);
		}
		explicit const_iterator_template (const T& self, std::size_t index) : _selfp(&self), _index(index) {
			sx::assert_invariant(*this);
		}
		const_iterator_template& operator= (const const_iterator_template& i) {
			auto _ = sx::make_invariant_scope(*this);
			sx::assert_invariant(i);
			_index = i._index;
			_selfp = i._selfp;
			return *this;
		}
		const_iterator_template& operator++ () { ++_index; return *this; }
		const_iterator_template operator++ (int) { const const_iterator_template i(*this); ++_index; return i; }
		const_iterator_template& operator-- () { --_index; return *this; }
		const_iterator_template operator-- (int) { const const_iterator_template i(*this); --_index; return i; }
		const_iterator_template operator+ (difference_type i) const {
			return const_iterator_template(*_selfp, _index + i);
		}
		const_iterator_template operator- (difference_type i) const {
			return const_iterator_template(*_selfp, _index - i);
		}
		difference_type operator- (const const_iterator_template& i) const {
			return _index - i._index;
		}
		const_iterator_template& operator+= (difference_type i) {
			_index += i;
			return *this;
		}
		const_iterator_template& operator-= (difference_type i) {
			_index -= i;
			return *this;
		}
		value_type& operator* () const {
			return const_cast<value_type&>((*_selfp)[_index]);
		}
		value_type* operator-> () const {
			return const_cast<value_type*>(&(*_selfp)[_index]);
		}
		value_type& operator[] (const std::size_t i) const {
			return const_cast<value_type&>((*_selfp)[_index + i]);
		}
		bool operator== (const const_iterator_template& i) const { sxassert(_selfp == i._selfp); return (_index == i._index); }
		bool operator!= (const const_iterator_template& i) const { return !((*this) == i); }
		bool operator< (const const_iterator_template& i) const {
			SXASSERT(_selfp == i._selfp);
			return (_index < i._index);
		}
		bool operator<= (const const_iterator_template& i) const {
			SXASSERT(_selfp == i._selfp);
			return (_index <= i._index);
		}
		bool operator> (const const_iterator_template& i) const {
			SXASSERT(_selfp == i._selfp);
			return (_index > i._index);
		}
		bool operator>= (const const_iterator_template& i) const {
			SXASSERT(_selfp == i._selfp);
			return (_index >= i._index);
		}
		difference_type offset () const {
			return _index;
		}
		void assert_invariant () const {
			sx::assert_invariant<sx::object>(*this);
			if (_selfp)	SXASSERT((-1 <= _index) && (_index <= _selfp->size()));
			else		SXASSERT(_index == 0);
		}
	private:
		std::ptrdiff_t	_index;
		const T*		_selfp;
	};
}
