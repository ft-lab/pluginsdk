#pragma once


namespace sxsdk {
	class mesh_interface : public shade_interface {
	public:
#pragma clang diagnostics push
#pragma clang diagnostic ignored "-Wold-style-cast"
	virtual int get_m_number (void* = nullptr) const = 0; // 0
	virtual int get_n_number (void* = nullptr) const = 0; // 1
	virtual sxsdk::vec3& operator[] (int i) = 0; // 2
	virtual sxsdk::vec3& operator() (int i, int j, void* aux = 0) = 0; // 3
	virtual bool get_m_closed (void* = nullptr) const = 0; // 4
	virtual bool get_n_closed (void* = nullptr) const = 0; // 5
#pragma clang diagnostics pop
	};
}
