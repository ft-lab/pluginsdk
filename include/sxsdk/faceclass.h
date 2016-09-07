#pragma once

namespace sxsdk {
	class vertex_class;

	class face_class  {
	public:
		face_class () = default;
		face_class (const face_class&) = default;
		face_class& operator= (const face_class&) SXLREF = default;
		virtual ~face_class () { }
		VTABLE_PADDING
#pragma clang diagnostics push
#pragma clang diagnostic ignored "-Wold-style-cast"
	virtual int get_number_of_vertices (void* = nullptr) const = 0; // 0
	virtual vertex_class& vertex_deprecated (int i, void* aux = 0) = 0; // 1
	virtual int get_vertex_indices (int list[], void* = nullptr) const = 0; // 2
	virtual face_class &set_vertex_indices (int n, int list[], void* = nullptr) = 0; // 3
	virtual bool has_uv (void* = nullptr) const = 0; // 4
	virtual int get_parameter_uv (sxsdk::vec2 list[], void* = nullptr) const = 0; // 5
	virtual face_class &set_parameter_uv (int n, sxsdk::vec2 list[], void* = nullptr) = 0; // 6
	virtual int get_distance_uv (sxsdk::vec2 list[], void* = nullptr) const = 0; // 7
	virtual face_class &set_distance_uv (int n, sxsdk::vec2 list[], void* = nullptr) = 0; // 8
	virtual void remove (void* aux = 0) = 0; // 9
	virtual void stream_to_clipboard (void* aux = 0) = 0; // 10
	virtual int get_normals (sxsdk::vec3 list[], void* = nullptr) const = 0; // 11
	virtual face_class &set_normals (int n, sxsdk::vec3 list[], void* = nullptr) = 0; // 12
	virtual int get_sub_surface_index (void* = nullptr) const = 0; // 13
	virtual face_class &set_sub_surface_index (int sub_surface_index_param, void* = nullptr) = 0; // 14
	virtual face_class &set_active (bool active_param, void* = nullptr) = 0; // 15
	virtual bool get_active (void* = nullptr) const = 0; // 16
	virtual face_class &set_active_order (int active_order_param, void* = nullptr) = 0; // 17
	virtual int get_active_order (void* = nullptr) const = 0; // 18
	virtual sxsdk::vec2 get_face_uv (int uv_layer_index, int v_index, void* aux = 0) = 0; // 19
	virtual void set_face_uv (int uv_layer_index, int v_index, sxsdk::vec2 uv, void* aux = 0) = 0; // 20
	virtual void flip (void* aux = 0) = 0; // 21
	virtual int get_active_vertex_indices (int list[], void* = nullptr) const = 0; // 22
	virtual face_class &set_active_vertex_indices (int n, int list[], void* = nullptr) = 0; // 23
	virtual int get_active_edge_indices (int list[], void* = nullptr) const = 0; // 24
	virtual face_class &set_active_edge_indices (int n, int list[], void* = nullptr) = 0; // 25
	virtual sxsdk::rgba_class get_vertex_color (int layer_index, int fv_index, void* aux = 0) = 0; // 26
	virtual void set_vertex_color (int layer_index, int fv_index, sxsdk::rgba_class color, void* aux = 0) = 0; // 27
	virtual int get_shading_normals (sxsdk::vec3 list[], void* = nullptr) const = 0; // 28
#pragma clang diagnostics pop
	};
}
