#pragma once

namespace sxsdk {
	class critical_section_interface : public unknown_interface {
	public:
#pragma clang diagnostics push
#pragma clang diagnostic ignored "-Wold-style-cast"
	virtual void enter (void * aux = 0) = 0; // 0
	virtual void leave (void * aux = 0) = 0; // 1
#pragma clang diagnostics pop
	};
}
