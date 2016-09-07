#pragma once
SXMODULE_SXCORE

namespace sx {
	template<typename T> class setting {
	public:
		explicit setting () { }
		template<typename U> explicit setting (const U& u) : _persistent_value(u), _default_value(u) { }
		template<typename U, typename V> explicit setting (const U& u, const V& v) : _persistent_value(u, v), _default_value(u, v) { }
		template<typename U, typename V, typename W> explicit setting (const U& u, const V& v, const W& w) : _persistent_value(u, v, w), _default_value(u, v, w) { }
		const T& operator() () { // getter
			return _overriding_value ? *_overriding_value : _persistent_value;
		}
		void operator() (const T& t) { // setter
			_persistent_value = t;
		}
		const T& persistent_value () const {
			return _persistent_value;
		}
		const T& default_value () const {
			return _default_value;
		}
		void override (const T& t) {
			_overriding_value = t;
		}
		const std::optional<T>& overriding_value () const {
			return _overriding_value;
		}
	private:
		std::optional<T>	_overriding_value;
		T					_persistent_value;
		const T				_default_value;
	};
}
