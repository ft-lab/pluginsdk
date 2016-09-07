#pragma once

namespace sxsdk {
	class vertex_class {
	public:
		vertex_class () = default;
		vertex_class (const vertex_class&) = default;
		vertex_class& operator= (const vertex_class&) = default;
		virtual ~vertex_class () = default;
		VTABLE_PADDING
#pragma clang diagnostics push
#pragma clang diagnostic ignored "-Wold-style-cast"
	virtual vertex_class &set_position (const sxsdk::vec3 &position_param, void* = nullptr) = 0; // 0
	virtual sxsdk::vec3 get_position (void* = nullptr) const = 0; // 1
	virtual vertex_class &set_normal (const sxsdk::vec3 &normal_param, void* = nullptr) = 0; // 2
	virtual sxsdk::vec3 get_normal (void* = nullptr) const = 0; // 3
	virtual void remove (void* aux = 0) = 0; // 4
	virtual void stream_to_clipboard (void* aux = 0) = 0; // 5
	virtual skin_class& get_skin (void* = nullptr) const = 0; // 6
	virtual vertex_class &set_active (bool active_param, void* = nullptr) = 0; // 7
	virtual bool get_active (void* = nullptr) const = 0; // 8
	virtual vertex_class &set_active_order (int active_order_param, void* = nullptr) = 0; // 9
	virtual int get_active_order (void* = nullptr) const = 0; // 10
#pragma clang diagnostics pop
	};
}
