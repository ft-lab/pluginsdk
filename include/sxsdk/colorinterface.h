#pragma once

namespace sxsdk {
	class color_interface : public shade_interface {
	public:
#pragma clang diagnostics push
#pragma clang diagnostic ignored "-Wold-style-cast"
	virtual sxsdk::rgb_class get_color (void* = nullptr) const = 0; // 0
	virtual color_interface &set_color (const sxsdk::rgb_class &color_param, void* = nullptr) = 0; // 1
	virtual int color_interface_test_1 ( void* aux =0 ) { return 1; } // 2
#pragma clang diagnostics pop
	// implementation
	public:
		explicit color_interface ()	= default;
	private:
		virtual bool is_same_as (shade_interface* i, void * aux = 0);
	// end
	};
}
