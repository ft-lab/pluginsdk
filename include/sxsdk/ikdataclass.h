#pragma once

namespace sxsdk {
	class ik_data_class {
	public:
		virtual ~ik_data_class () { }
		VTABLE_PADDING
#pragma clang diagnostics push
#pragma clang diagnostic ignored "-Wold-style-cast"
	virtual ik_data_class &set_root_shape (sxsdk::shape_class* root_shape_param, void* = nullptr) = 0; // 0
	virtual sxsdk::shape_class* get_root_shape (void* = nullptr) const = 0; // 1
	virtual ik_data_class &set_end_shape (sxsdk::shape_class* end_shape_param, void* = nullptr) = 0; // 2
	virtual sxsdk::shape_class* get_end_shape (void* = nullptr) const = 0; // 3
	virtual ik_data_class &set_goal_shape (sxsdk::shape_class* goal_shape_param, void* = nullptr) = 0; // 4
	virtual sxsdk::shape_class* get_goal_shape (void* = nullptr) const = 0; // 5
	virtual ik_data_class &set_enable (bool enable_param, void* = nullptr) = 0; // 6
	virtual bool get_enable (void* = nullptr) const = 0; // 7
	virtual ik_data_class &set_angle_lock (bool angle_lock_param, void* = nullptr) = 0; // 8
	virtual bool get_angle_lock (void* = nullptr) const = 0; // 9
#pragma clang diagnostics pop
	};
}
