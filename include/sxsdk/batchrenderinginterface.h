#pragma once

namespace sxsdk {
	class batch_rendering_interface : public shade_interface {
	public:
		virtual ~batch_rendering_interface () { }
#pragma clang diagnostics push
#pragma clang diagnostic ignored "-Wold-style-cast"
	virtual void start (void* aux = 0) = 0; // 0
	virtual bool is_still_rendering (void* = nullptr) const = 0; // 1
	virtual void stop (void* aux = 0) = 0; // 2
#pragma clang diagnostics pop
	};
}
