#pragma once
SXMODULE_SXCORE
#include "sxcore/adl.hpp"
#include "sxcore/debug.hpp"
#include "sxcore/vector.hpp"

namespace sx {
	template<typename T> class property {
	public:
		property () = default;
		property (const T& t) : _data(t) { }
		void operator () (const T& t) { // setter
			_data = t;
		}
		const T& operator() () const { // getter
			return _data;
		}
		template<typename U> const T& operator<< (const U& value) {
			_data << value;
			return _data;
		}
		template<typename U> const T& operator^= (const U& value) {
			_data ^= value;
			return _data;
		}
		template<typename U> const T& operator|= (const U& value) {
			_data |= value;
			return _data;
		}
		template<typename U> const T& operator&= (const U& value) {
			_data &= value;
			return _data;
		}
		template<typename U> const T& operator+= (const U& value) {
			_data += value;
			return _data;
		}
		template<typename U> const T& operator-= (const U& value) {
			_data -= value;
			return _data;
		}
		template<typename U> const T& operator*= (const U& value) {
			_data *= value;
			return _data;
		}
		template<typename U> const T& operator/= (const U& value) {
			_data /= value;
			return _data;
		}
		T& operator++ () {
			return ++_data;
		}
		T& operator-- () {
			return --_data;
		}
		T operator++ (int) {
			const T t = _data;
			++_data;
			return t;
		}
		T operator-- (int) {
			const T t = _data;
			--_data;
			return t;
		}
		operator const T&  () const {
			return _data;
		}
		void swap (property<T>& t) { adl::swap(_data, t._data); }
	private:
		T _data{};
	};
	template<typename T> inline void swap (property<T>& a, property<T>& b) {
		a.swap(b);
	}

	// pointers and (simple) containers should not be treated as properties
	template<typename T> class property<std::unique_ptr<T>> {
		property () = delete;
	};
	template<typename T> class property<std::shared_ptr<T>> {
		property () = delete;
	};
	template<typename T> class property<T*> {
		property () = delete;
	};
	template<typename T> class property<std::vector<T>> {
		property () = delete;
	};
	template<typename T> class property<sx::vector<T>> {
		property () = delete;
	};
}
