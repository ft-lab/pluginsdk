#pragma once

namespace sxsdk {
	class points_interface : public shade_interface {
	public:
#pragma clang diagnostics push
#pragma clang diagnostic ignored "-Wold-style-cast"
	virtual int get_n (void* = nullptr) const = 0; // 0
	virtual sxsdk::vec3& operator[] (int i) = 0; // 1
#pragma clang diagnostics pop
	};
}
