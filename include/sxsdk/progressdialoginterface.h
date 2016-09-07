#pragma once

namespace sxsdk {
	class progress_dialog_interface : public unknown_interface {
	public:
#pragma clang diagnostics push
#pragma clang diagnostic ignored "-Wold-style-cast"
	virtual void set_range (int start, int end, void* aux = 0) = 0; // 0
	virtual progress_dialog_interface &set_position (int position_param, void* = nullptr) = 0; // 1
	virtual void step (int step = 1, void* aux = 0) = 0; // 2
	virtual bool set_position_nothrow (int at) = 0; // 3
	virtual bool step_nothrow (int by = 1) = 0; // 4
#pragma clang diagnostics pop
	};
}
