#pragma once

namespace sxsdk {
	class image_layer_interface : public shade_interface {
	public:
#pragma clang diagnostics push
#pragma clang diagnostic ignored "-Wold-style-cast"
	virtual sxsdk::image_interface* get_image_interface (void* = nullptr) const = 0; // 0
	virtual image_layer_interface &set_name (const char* name_param, void* = nullptr) = 0; // 1
	virtual const char* get_name (void* = nullptr) const = 0; // 2
	virtual image_layer_interface &set_id_name (const char* id_name_param, void* = nullptr) = 0; // 3
	virtual const char* get_id_name (void* = nullptr) const = 0; // 4
	virtual void set_channel_name (int index, const char* name, void* aux = 0) = 0; // 5
	virtual const char* get_channel_name (int index, void* aux = 0) const = 0; // 6
	virtual int get_number_of_channels (void* = nullptr) const = 0; // 7
	virtual image_layer_interface &set_pixel_depth (int pixel_depth_param, void* = nullptr) = 0; // 8
	virtual int get_pixel_depth (void* = nullptr) const = 0; // 9
	virtual image_layer_interface &set_mix_mode (sx::image::mix_mode mix_mode_param, void* = nullptr) = 0; // 10
	virtual sx::image::mix_mode get_mix_mode (void* = nullptr) const = 0; // 11
	virtual image_layer_interface &set_maximum_value (sxsdk::rgba_class maximum_value_param, void* = nullptr) = 0; // 12
	virtual sxsdk::rgba_class get_maximum_value (void* = nullptr) const = 0; // 13
	virtual image_layer_interface &set_minimum_value (sxsdk::rgba_class minimum_value_param, void* = nullptr) = 0; // 14
	virtual sxsdk::rgba_class get_minimum_value (void* = nullptr) const = 0; // 15
#pragma clang diagnostics pop
	};
}
