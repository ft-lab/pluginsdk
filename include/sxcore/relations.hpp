#pragma once
SXMODULE_SXCORE

namespace sx {
	template<typename T> class reference_ptr {
	public:
		explicit reference_ptr (T* p) : _p(p) { }

		reference_ptr (const reference_ptr&) = default;
		reference_ptr& operator= (const reference_ptr&) { return *this; }

		T& operator* () const {
			SXASSERT(_p);
			return *_p;
		}
		T* operator-> () const {
			SXASSERT(_p);
			return _p;
		}
		T* get () const {
			return _p;
		}
		operator T *() const {
			return _p;
		}
	private:
		T* const	_p;
	};
	
	template<typename T> class composition {
	public:
		template<typename... U> explicit composition (U&&... u) : _p(std::make_unique<T>(std::forward<U>(u)...)) { }
	
		composition (const composition& c) : _p(std::make_unique<T>(*c._p)) { }
		composition (composition&& c) noexcept : _p(std::move(c._p)) { }	// default
		composition& operator= (const composition& c) { if (this != &c) *_p = *c._p; return *this; }
		composition& operator= (composition&& c) noexcept { _p = std::move(c._p); return *this; }	// default

		const T& operator-- (int) const {
			SXASSERT(_p);
			return *_p;
		}
		T& operator++ (int) {
			SXASSERT(_p);
			return *_p;
		}
	private:
		std::unique_ptr<T>	_p;
	};
}

