#pragma once

namespace sxsdk {
	class edge_class {
	public:
		edge_class () = default;
		edge_class (const edge_class&) = default;
		edge_class& operator= (const edge_class&) SXLREF = default;
		virtual ~edge_class () { }
		VTABLE_PADDING
#pragma clang diagnostics push
#pragma clang diagnostic ignored "-Wold-style-cast"
	virtual int get_v0 (void* = nullptr) const = 0; // 0
	virtual edge_class &set_v0 (int v0_param, void* = nullptr) = 0; // 1
	virtual int get_v1 (void* = nullptr) const = 0; // 2
	virtual edge_class &set_v1 (int v1_param, void* = nullptr) = 0; // 3
	virtual void remove (void* aux = 0) = 0; // 4
	virtual void stream_to_clipboard (void* aux = 0) = 0; // 5
	virtual edge_class &set_active (bool active_param, void* = nullptr) = 0; // 6
	virtual bool get_active (void* = nullptr) const = 0; // 7
	virtual edge_class &set_active_order (int active_order_param, void* = nullptr) = 0; // 8
	virtual int get_active_order (void* = nullptr) const = 0; // 9
	virtual float get_sharpness (void* = nullptr) const = 0; // 10
	virtual edge_class &set_sharpness (float sharpness_param, void* = nullptr) = 0; // 11
#pragma clang diagnostics pop
	};
}
