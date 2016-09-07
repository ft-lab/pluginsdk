#pragma once

namespace sxsdk {
	class shape_class;
	class ik_data_class;

	class ik_class {
	public:
		ik_class () = default;
		ik_class (const ik_class&) = default;
		ik_class& operator= (const ik_class&) = default;
		virtual ~ik_class () = default;
		VTABLE_PADDING
#pragma clang diagnostics push
#pragma clang diagnostic ignored "-Wold-style-cast"
	virtual sxsdk::ik_data_class& find_ik_data (sxsdk::shape_class& shape, void* aux = 0) = 0; // 0
	virtual bool set_ik (sxsdk::shape_class* root_shape, sxsdk::shape_class* end_shape, sxsdk::shape_class* goal_shape, void* aux = 0) = 0; // 1
	virtual int has_ik (sxsdk::shape_class& shape, bool depth, void* aux = 0) = 0; // 2
	virtual sxsdk::ik_data_class& get_ik_data (sxsdk::shape_class& shape, void* aux = 0) = 0; // 3
	virtual bool set_ik_data (sxsdk::shape_class& shape, sxsdk::ik_data_class& ik_data, void* aux = 0) = 0; // 4
	virtual bool delete_ik_data (sxsdk::shape_class& shape, void* aux = 0) = 0; // 5
	virtual int get_number_of_ik (void* aux = 0) = 0; // 6
	virtual sxsdk::ik_data_class& get_ik_data_index (int index, void* aux = 0) = 0; // 7
	virtual sxsdk::vec3 get_shape_world_position (sxsdk::shape_class& shape, void* aux = 0) = 0; // 8
	virtual sxsdk::vec3 get_pole_vector (sxsdk::shape_class& end_shape, void* aux = 0) = 0; // 9
	virtual float get_twist (sxsdk::shape_class& end_shape, void* aux = 0) = 0; // 10
	virtual sxsdk::quaternion_class get_joint_rotation (sxsdk::shape_class& shape, void* aux = 0) = 0; // 11
	virtual sxsdk::vec3 get_joint_offset (sxsdk::shape_class& shape, void* aux = 0) = 0; // 12
#pragma clang diagnostics pop
	};
}
