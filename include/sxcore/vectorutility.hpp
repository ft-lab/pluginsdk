#pragma once
SXMODULE_SXCORE
#include "sxcore/adl.hpp"
#include "sxcore/vectors.hpp"

namespace sx {
	template<typename T> const std::vector<const T*> &vector_const_cast (const std::vector<T*> &v) {
		return reinterpret_cast<const std::vector<const T*>&>(v);
	}
	template<typename T, std::size_t N> const sx::small_vector<const T *, N> &vector_const_cast (const sx::small_vector<T *, N> &v) {
		return reinterpret_cast<const sx::small_vector<const T *, N>&>(v);
	}
	template<typename T> typename T::size_type find_index (const T &v, const typename T::value_type &u) {
		sxassert(std::find(v.begin(), v.end(), u) != v.end());
		return std::find(v.begin(), v.end(), u)-v.begin();
	}
	template<typename T> inline void verify_index (const T &v, typename T::size_type i) {
		if (v.size() <= i) throw "Index out of range.";
	}
	template<typename T> class is_delete_flag_set {
	public:
		bool operator() (const T &t) const { return t.delete_flag(); }
	};
	template<typename T> class is_delete_flag_set<std::unique_ptr<T>> {
	public:
		bool operator() (const std::unique_ptr<T> &t) const { return t->delete_flag(); }
	};
	template<typename T> void remove (T &v) {
		v.resize(std::remove_if(begin(v), end(v), is_delete_flag_set<typename T::value_type>()) - begin(v));
		sxassert(std::find_if(begin(v), end(v), is_delete_flag_set<typename T::value_type>()) == end(v));
	}

	template<typename T> class vector_accessor_interface {
	public:
		using value_type	= T;

		vector_accessor_interface () = default;
		vector_accessor_interface (const vector_accessor_interface&) = default;
		vector_accessor_interface& operator= (const vector_accessor_interface&) = default;
		virtual ~vector_accessor_interface () = default;
		std::size_t size () const { return size_imp(); }
		bool empty () const { return empty_imp(); }
		T &operator[] (std::size_t i) {
			auto _ = sx::make_invariant_scope(*this);
			return const_cast<T&>(const_cast<const vector_accessor_interface<T>&>(*this)[i]);
		}
		const T &operator[] (std::size_t i) const {
			auto _ = sx::make_invariant_scope(*this);
			return item_at_imp(i);
		}
	private:
		virtual std::size_t size_imp () const = 0;
		virtual bool empty_imp () const = 0;
		virtual const T &item_at_imp (std::size_t i) const = 0;
	};

	template<typename T> class vector_accessor : public vector_accessor_interface<typename T::value_type> {
	public:
		explicit vector_accessor (const T &t) : t(t) { }
	private:
		const T &t;

		virtual std::size_t size_imp () const { return t.size(); }
		virtual bool empty_imp () const { return t.empty(); }
		virtual const typename T::value_type &item_at_imp (std::size_t i) const { return t[i]; }
	};
	template<typename T> class vector_accessor<T*> : public vector_accessor_interface<T> {
	public:
		explicit vector_accessor (const T *t, std::size_t n) : t(t), n(n) { }
	private:
		const T *const t;
		const std::size_t n;

		virtual std::size_t size_imp () const { return n; }
		virtual bool empty_imp () const { return (n == 0); }
		virtual const T &item_at_imp (std::size_t i) const { return t[i]; }
	};
	template<typename T, typename S> class vector_index_accessor : public vector_accessor_interface<typename T::value_type> {
	public:
		explicit vector_index_accessor (T &t, const S &indices) : t(t), indices(indices) { }
	private:
		T &t;
		const S &indices;

		virtual std::size_t size_imp () const { return indices.size(); }
		virtual bool empty_imp () const { return indices.empty(); }
		virtual const typename T::value_type &item_at_imp (std::size_t i) const { return t[indices[i]]; }
	};
	template<typename T, typename S> class vector_index_accessor<T *, S*> : public vector_accessor_interface<T> {
	public:
		explicit vector_index_accessor (const T *t, const S *indices, std::size_t n) : t(t), indices(indices), n(n) { }
	private:
		const T *const t;
		const S *const indices;
		const std::size_t n;

		virtual std::size_t size_imp () const { return n; }
		virtual bool empty_imp () const { return (n == 0); }
		virtual const T &item_at_imp (std::size_t i) const { return t[indices[i]]; }
	};
	template<typename T> class vector_fill_accessor : public vector_accessor_interface<T> {
	public:
		explicit vector_fill_accessor (const T &t, std::size_t n) : t(t), n(n) { }
	private:
		const T &t;
		const std::size_t n;

		virtual std::size_t size_imp () const { return n; }
		virtual bool empty_imp () const { return (n == 0); }
		virtual const T &item_at_imp (std::size_t i) const { return t; }
	};
	template<typename T> class empty_vector_accessor : public vector_accessor_interface<T> {
	public:
		explicit empty_vector_accessor () { }
	private:
		virtual std::size_t size_imp () const { return 0; }
		virtual bool empty_imp () const { return true; }
		virtual const T &item_at_imp (std::size_t i) const { SXASSERT(false); return *static_cast<const T*>(nullptr); }
	};

	template<typename T> auto resize (T& t, const std::size_t n) -> decltype(t.T::capacity(), t.T::reserve(0), t.T::resize(0), void()) {
		if (t.capacity() < n) t.reserve(std::max(t.size()*2,n));
		t.resize(n);
	}
	template<typename T, typename U> auto resize (T& t, const std::size_t n, const U& u) -> decltype(t.T::capacity(), t.T::reserve(0), t.T::resize(0,u), void()) {
		if (t.capacity() < n) t.reserve(std::max(t.size()*2,n));
		t.resize(n, u);
	}
	template<typename T, typename U> auto append (T& t, const U& u) -> decltype(t.T::capacity(), t.T::reserve(0), void()) {
		if (t.capacity() < (t.size()+u.size())) t.reserve(std::max(t.size()*2,t.size()+u.size()));
		t.insert(end(t), begin(u), end(u));
	}
}


template<typename T, typename U> auto operator<< (T& t, U&& u) -> decltype(t.T::push_back(std::forward<U>(u)), t) { static_assert(std::is_reference<decltype(void(),t)>::value, "");
	t.push_back(std::forward<U>(u));
	return t;
};
