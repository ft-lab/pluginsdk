#pragma once

namespace sxsdk {
	class graphics_interface : public plugin_interface {
	public:
#pragma clang diagnostics push
#pragma clang diagnostic ignored "-Wold-style-cast"
	virtual void line (const sx::vec<float,3>& p0, const sx::vec<float,3>& p1, void* aux = 0) { } // -1
#pragma clang diagnostics pop
	};
}
