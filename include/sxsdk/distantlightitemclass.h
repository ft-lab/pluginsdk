#pragma once

namespace sxsdk {
	class distant_light_item_class {
	public:
		virtual ~distant_light_item_class () = default;
		VTABLE_PADDING
#pragma clang diagnostics push
#pragma clang diagnostic ignored "-Wold-style-cast"
	virtual distant_light_item_class &set_direction (const sxsdk::vec3 &direction_param, void* = nullptr) = 0; // 0
	virtual sxsdk::vec3 get_direction (void* = nullptr) const = 0; // 1
	virtual distant_light_item_class &set_intensity (float intensity_param, void* = nullptr) = 0; // 2
	virtual float get_intensity (void* = nullptr) const = 0; // 3
	virtual distant_light_item_class &set_softness (float softness_param, void* = nullptr) = 0; // 4
	virtual float get_softness (void* = nullptr) const = 0; // 5
	virtual distant_light_item_class &set_shadow (float shadow_param, void* = nullptr) = 0; // 6
	virtual float get_shadow (void* = nullptr) const = 0; // 7
	virtual distant_light_item_class &set_ambient (float ambient_param, void* = nullptr) = 0; // 8
	virtual float get_ambient (void* = nullptr) const = 0; // 9
	virtual distant_light_item_class &set_light_color (const sxsdk::rgb_class &light_color_param, void* = nullptr) = 0; // 10
	virtual sxsdk::rgb_class get_light_color (void* = nullptr) const = 0; // 11
	virtual distant_light_item_class &set_glare (float glare_param, void* = nullptr) = 0; // 12
	virtual float get_glare (void* = nullptr) const = 0; // 13
	virtual distant_light_item_class &set_specular (float specular_param, void* = nullptr) = 0; // 14
	virtual float get_specular (void* = nullptr) const = 0; // 15
	virtual distant_light_item_class &set_diffuse (float diffuse_param, void* = nullptr) = 0; // 16
	virtual float get_diffuse (void* = nullptr) const = 0; // 17
	virtual distant_light_item_class &set_shadow_type (int shadow_type_param, void* = nullptr) = 0; // 18
	virtual int get_shadow_type (void* = nullptr) const = 0; // 19
	virtual distant_light_item_class &set_has_shadow_map_size (bool has_shadow_map_size_param, void* = nullptr) = 0; // 20
	virtual bool get_has_shadow_map_size (void* = nullptr) const = 0; // 21
	virtual distant_light_item_class &set_shadow_map_size (int shadow_map_size_param, void* = nullptr) = 0; // 22
	virtual int get_shadow_map_size (void* = nullptr) const = 0; // 23
	virtual distant_light_item_class &set_has_shadow_map_bias (bool has_shadow_map_bias_param, void* = nullptr) = 0; // 24
	virtual bool get_has_shadow_map_bias (void* = nullptr) const = 0; // 25
	virtual distant_light_item_class &set_shadow_map_bias (float shadow_map_bias_param, void* = nullptr) = 0; // 26
	virtual float get_shadow_map_bias (void* = nullptr) const = 0; // 27
	virtual distant_light_item_class &set_has_shadow_map_blur (bool has_shadow_map_blur_param, void* = nullptr) = 0; // 28
	virtual bool get_has_shadow_map_blur (void* = nullptr) const = 0; // 29
	virtual distant_light_item_class &set_shadow_map_blur (float shadow_map_blur_param, void* = nullptr) = 0; // 30
	virtual float get_shadow_map_blur (void* = nullptr) const = 0; // 31
	virtual sxsdk::stream_interface* create_attribute_stream_interface (int attribute_id, void* aux = 0) = 0; // 32
	virtual sxsdk::stream_interface* get_attribute_stream_interface (int attribute_id, void* aux = 0) = 0; // 33
	virtual bool delete_attribute (int attribute_id, void* aux = 0) = 0; // 34
	virtual void stream_to_clipboard (void* aux = 0) = 0; // 35
	virtual sxsdk::stream_interface* create_attribute_stream_interface_with_uuid (const sx::uuid_class& attribute_id, void* aux = 0) = 0; // 36
	virtual sxsdk::stream_interface* get_attribute_stream_interface_with_uuid (const sx::uuid_class& attribute_id, void* aux = 0) = 0; // 37
	virtual bool delete_attribute_with_uuid (const sx::uuid_class& attribute_id, void* aux = 0) = 0; // 38
	virtual distant_light_item_class &set_size (float size_param, void* = nullptr) = 0; // 39
	virtual float get_size (void* = nullptr) const = 0; // 40
#pragma clang diagnostics pop
	};
}
