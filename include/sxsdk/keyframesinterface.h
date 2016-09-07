#pragma once

namespace sxsdk {
	class keyframes_interface : public shade_interface {
	public:
#pragma clang diagnostics push
#pragma clang diagnostic ignored "-Wold-style-cast"
	virtual void delete_active_keyframes (void * aux = 0) = 0; // 0
	virtual void stream_to_clipboard (void* aux = 0) = 0; // 1
#pragma clang diagnostics pop
	};
}
