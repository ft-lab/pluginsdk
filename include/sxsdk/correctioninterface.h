#pragma once

namespace sxsdk {
	class correction_interface : public shade_interface {
	public:
#pragma clang diagnostics push
#pragma clang diagnostic ignored "-Wold-style-cast"
	virtual void save (const char*const& in, void * aux = 0) = 0; // 0
	virtual void load (const char*const& from, void * aux = 0) = 0; // 1
	virtual correction_interface &set_gain (float gain_param, void* = nullptr) = 0; // 2
	virtual float get_gain (void* = nullptr) const = 0; // 3
	virtual correction_interface &set_gamma (float gamma_param, void* = nullptr) = 0; // 4
	virtual float get_gamma (void* = nullptr) const = 0; // 5
	virtual correction_interface &set_contrast (float contrast_param, void* = nullptr) = 0; // 6
	virtual float get_contrast (void* = nullptr) const = 0; // 7
	virtual correction_interface &set_highlight_hue (float highlight_hue_param, void* = nullptr) = 0; // 8
	virtual float get_highlight_hue (void* = nullptr) const = 0; // 9
	virtual correction_interface &set_highlight_saturation (float highlight_saturation_param, void* = nullptr) = 0; // 10
	virtual float get_highlight_saturation (void* = nullptr) const = 0; // 11
	virtual correction_interface &set_mid_hue (float mid_hue_param, void* = nullptr) = 0; // 12
	virtual float get_mid_hue (void* = nullptr) const = 0; // 13
	virtual correction_interface &set_mid_saturation (float mid_saturation_param, void* = nullptr) = 0; // 14
	virtual float get_mid_saturation (void* = nullptr) const = 0; // 15
	virtual correction_interface &set_shadow_hue (float shadow_hue_param, void* = nullptr) = 0; // 16
	virtual float get_shadow_hue (void* = nullptr) const = 0; // 17
	virtual correction_interface &set_shadow_saturation (float shadow_saturation_param, void* = nullptr) = 0; // 18
	virtual float get_shadow_saturation (void* = nullptr) const = 0; // 19
	virtual correction_interface &set_color_shift (float color_shift_param, void* = nullptr) = 0; // 20
	virtual float get_color_shift (void* = nullptr) const = 0; // 21
	virtual correction_interface &set_bias (float bias_param, void* = nullptr) = 0; // 22
	virtual float get_bias (void* = nullptr) const = 0; // 23
	virtual sxsdk::rgb_class correct (const sxsdk::rgb_class& color, void * aux = 0) = 0; // 24
	virtual void stream_to_clipboard (void* aux = 0) = 0; // 25
	virtual correction_interface &set_keep_saturation (bool keep_saturation_param, void* = nullptr) = 0; // 26
	virtual bool get_keep_saturation (void* = nullptr) const = 0; // 27
#pragma clang diagnostics pop
	};
}
