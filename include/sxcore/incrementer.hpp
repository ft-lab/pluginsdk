#pragma once
SXMODULE_SXCORE

namespace sx {
	template<typename T> class incrementer {
	public:
		explicit incrementer (T& t) noexcept : _p(&t) {
			++t;
		}
		explicit incrementer (T* const t) noexcept : _p(t) {
			if (_p) ++(*_p);
		}
		incrementer (incrementer&& i) noexcept : _p(i._p) {
			i._p = nullptr;
		}
		incrementer& operator= (incrementer&& i) noexcept {
			if (this != &i) {
				if (_p) --(*_p);
				_p = i._p;
				i._p = nullptr;
			}
			return *this;
		}
		~incrementer () {
			if (_p) --(*_p);
		}
	private:
		T*	_p;
		incrementer (const incrementer&) = delete;
		incrementer& operator= (const incrementer&) = delete;
	};
	template<typename T> class decrementer {
	public:
		explicit decrementer (T& t) noexcept : _p(&t) {
			--t;
		}
		explicit decrementer (T* const t) noexcept : _p(t) {
			if (_p) --(*_p);
		}
		decrementer (decrementer&& i) noexcept : _p(i._p) {
			i._p = nullptr;
		}
		decrementer& operator= (decrementer&& i) noexcept {
			if (this != &i) {
				if (_p) ++(*_p);
				_p = i._p;
				i._p = nullptr;
			}
			return *this;
		}
		~decrementer () {
			if (_p) ++(*_p);
		}
	private:
		T*	_p;
		decrementer (const decrementer&) = delete;
		decrementer& operator= (const decrementer&) = delete;
	};
	
	template<typename T> incrementer<T> make_incrementer (T& t) {
		return incrementer<T>(t);
	}
	template<typename T> incrementer<T> make_incrementer (T* const t) {
		return incrementer<T>(t);
	}
	template<typename T> decrementer<T> make_decrementer (T& t) {
		return decrementer<T>(t);
	}
	template<typename T> decrementer<T> make_decrementer (T* const t) {
		return decrementer<T>(t);
	}
}
