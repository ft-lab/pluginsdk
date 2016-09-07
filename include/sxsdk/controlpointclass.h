#pragma once

namespace sxsdk {
	class control_point_class {
	public:
		control_point_class () = default;
		control_point_class (const control_point_class&) = default;
		control_point_class& operator= (const control_point_class&) = default;
		virtual ~control_point_class ();
		VTABLE_PADDING
#pragma clang diagnostics push
#pragma clang diagnostic ignored "-Wold-style-cast"
	virtual control_point_class &set_position (const sxsdk::vec3 &position_param, void* = nullptr) = 0; // 0
	virtual sxsdk::vec3 get_position (void* = nullptr) const = 0; // 1
	virtual control_point_class &set_in_handle (const sxsdk::vec3 &in_handle_param, void* = nullptr) = 0; // 2
	virtual sxsdk::vec3 get_in_handle (void* = nullptr) const = 0; // 3
	virtual control_point_class &set_out_handle (const sxsdk::vec3 &out_handle_param, void* = nullptr) = 0; // 4
	virtual sxsdk::vec3 get_out_handle (void* = nullptr) const = 0; // 5
	virtual control_point_class &set_linked (bool linked_param, void* = nullptr) = 0; // 6
	virtual bool get_linked (void* = nullptr) const = 0; // 7
	virtual control_point_class &set_lateral_in_handle (const sxsdk::vec3 &lateral_in_handle_param, void* = nullptr) = 0; // 8
	virtual sxsdk::vec3 get_lateral_in_handle (void* = nullptr) const = 0; // 9
	virtual control_point_class &set_lateral_out_handle (const sxsdk::vec3 &lateral_out_handle_param, void* = nullptr) = 0; // 10
	virtual sxsdk::vec3 get_lateral_out_handle (void* = nullptr) const = 0; // 11
	virtual control_point_class &set_lateral_linked (bool lateral_linked_param, void* = nullptr) = 0; // 12
	virtual bool get_lateral_linked (void* = nullptr) const = 0; // 13
	virtual bool has_in_handle (void* = nullptr) const = 0; // 14
	virtual bool has_out_handle (void* = nullptr) const = 0; // 15
	virtual bool has_lateral_in_handle (void* = nullptr) const = 0; // 16
	virtual bool has_lateral_out_handle (void* = nullptr) const = 0; // 17
	virtual void remove_in_handle (void* aux = 0) = 0; // 18
	virtual void remove_out_handle (void* aux = 0) = 0; // 19
	virtual void remove_lateral_in_handle (void* aux = 0) = 0; // 20
	virtual void remove_lateral_out_handle (void* aux = 0) = 0; // 21
	virtual void stream_to_clipboard (void* aux = 0) = 0; // 22
	virtual skin_class& get_skin (void* = nullptr) const = 0; // 23
#pragma clang diagnostics pop
	};
}
