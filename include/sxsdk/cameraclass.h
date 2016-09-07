#pragma once

namespace sxsdk {
	class camera_class : public sx::object {
	public:
		virtual ~camera_class () { }
		VTABLE_PADDING
#pragma clang diagnostics push
#pragma clang diagnostic ignored "-Wold-style-cast"
	virtual void save (const char* path, void* aux = 0) = 0; // 0
	virtual void load (const char* path, void* aux = 0) = 0; // 1
	virtual sxsdk::vec3 get_eye (void* = nullptr) const = 0; // 2
	virtual camera_class &set_eye (const sxsdk::vec3 &eye_param, void* = nullptr) = 0; // 3
	virtual sxsdk::vec3 get_target (void* = nullptr) const = 0; // 4
	virtual camera_class &set_target (const sxsdk::vec3 &target_param, void* = nullptr) = 0; // 5
	virtual float get_zoom (void* = nullptr) const = 0; // 6
	virtual camera_class &set_zoom (float zoom_param, void* = nullptr) = 0; // 7
	virtual bool get_distant_camera (void* = nullptr) const = 0; // 8
	virtual camera_class &set_distant_camera (bool distant_camera_param, void* = nullptr) = 0; // 9
	virtual float get_bank (void* = nullptr) const = 0; // 10
	virtual camera_class &set_bank (float bank_param, void* = nullptr) = 0; // 11
	virtual sxsdk::vec3 get_focus_point (void* = nullptr) const = 0; // 12
	virtual camera_class &set_focus_point (const sxsdk::vec3 &focus_point_param, void* = nullptr) = 0; // 13
	virtual float get_focus (void* = nullptr) const = 0; // 14
	virtual camera_class &set_focus (float focus_param, void* = nullptr) = 0; // 15
	virtual float get_shift (void* = nullptr) const = 0; // 16
	virtual camera_class &set_shift (float shift_param, void* = nullptr) = 0; // 17
	virtual float get_rise (void* = nullptr) const = 0; // 18
	virtual camera_class &set_rise (float rise_param, void* = nullptr) = 0; // 19
	virtual void memorize_as_line_object (void* aux = 0) = 0; // 20
	virtual void memorize_as_spotlight (void* aux = 0) = 0; // 21
	virtual void memorize_as_camera (void* aux = 0) = 0; // 22
	virtual sxsdk::stream_interface* create_attribute_stream_interface (int attribute_id, void* aux = 0) = 0; // 23
	virtual sxsdk::stream_interface* get_attribute_stream_interface (int attribute_id, void* aux = 0) = 0; // 24
	virtual sxsdk::mat4 get_world_to_view_matrix (void* = nullptr) const = 0; // 25
	virtual sxsdk::mat4 get_world_to_perspective_matrix (void* = nullptr) const = 0; // 26
	virtual bool delete_attribute (int attribute_id, void* aux = 0) = 0; // 27
	virtual void restore_from_object (sxsdk::shape_class& shape, void* aux = 0) = 0; // 28
	virtual void restore_from_default (void* aux = 0) = 0; // 29
	virtual camera_class &set_correction (float correction_param, void* = nullptr) = 0; // 30
	virtual float get_correction (void* = nullptr) const = 0; // 31
	virtual camera_class &set_film_shift (float film_shift_param, void* = nullptr) = 0; // 32
	virtual float get_film_shift (void* = nullptr) const = 0; // 33
	virtual camera_class &set_film_rise (float film_rise_param, void* = nullptr) = 0; // 34
	virtual float get_film_rise (void* = nullptr) const = 0; // 35
	virtual camera_class &set_film_swing (float film_swing_param, void* = nullptr) = 0; // 36
	virtual float get_film_swing (void* = nullptr) const = 0; // 37
	virtual camera_class &set_film_tilt (float film_tilt_param, void* = nullptr) = 0; // 38
	virtual float get_film_tilt (void* = nullptr) const = 0; // 39
	virtual camera_class &set_lens_swing (float lens_swing_param, void* = nullptr) = 0; // 40
	virtual float get_lens_swing (void* = nullptr) const = 0; // 41
	virtual camera_class &set_lens_tilt (float lens_tilt_param, void* = nullptr) = 0; // 42
	virtual float get_lens_tilt (void* = nullptr) const = 0; // 43
	virtual camera_class &set_scale (float scale_param, void* = nullptr) = 0; // 44
	virtual float get_scale (void* = nullptr) const = 0; // 45
	virtual sxsdk::enums::camera_mode get_camera_mode (void* = nullptr) const = 0; // 46
	virtual camera_class &set_camera_mode (sxsdk::enums::camera_mode camera_mode_param, void* = nullptr) = 0; // 47
	virtual part_class* get_camera_object (void* = nullptr) const = 0; // 48
	virtual void stream_to_clipboard (void* aux = 0) = 0; // 49
	virtual void memorize_as_metacamera (void* aux = 0) = 0; // 50
	virtual sxsdk::stream_interface* create_attribute_stream_interface_with_uuid (const sx::uuid_class& attribute_id, void* aux = 0) = 0; // 51
	virtual sxsdk::stream_interface* get_attribute_stream_interface_with_uuid (const sx::uuid_class& attribute_id, void* aux = 0) const = 0; // 52
	virtual bool delete_attribute_with_uuid (const sx::uuid_class& attribute_id, void* aux = 0) = 0; // 53
	virtual void rotate_eye (float h, void* aux = 0) = 0; // 54
#pragma clang diagnostics pop
	};
}
