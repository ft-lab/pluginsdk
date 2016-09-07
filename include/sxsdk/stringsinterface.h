#pragma once

namespace sxsdk {
	class strings_interface : public shade_interface {
	public:
#pragma clang diagnostics push
#pragma clang diagnostic ignored "-Wold-style-cast"
	virtual const char* get_string (int at, void* aux = 0) = 0; // 0
#pragma clang diagnostics pop
	};
}
