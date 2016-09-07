#pragma once

namespace sxsdk {
	class rendering_light_class {
	public:
		virtual ~rendering_light_class ();
		VTABLE_PADDING
#pragma clang diagnostics push
#pragma clang diagnostic ignored "-Wold-style-cast"
	virtual bool is_distant_light (void* = nullptr) const = 0; // 0
	virtual bool is_point_light (void* = nullptr) const = 0; // 1
	virtual bool is_spotlight (void* = nullptr) const = 0; // 2
	virtual sxsdk::vec4 get_position (void* = nullptr) const = 0; // 3
	virtual sxsdk::vec3 get_direction (void* = nullptr) const = 0; // 4
	virtual sxsdk::rgb_class get_color (void* = nullptr) const = 0; // 5
	virtual float get_ambient (void* = nullptr) const = 0; // 6
	virtual float get_intensity (void* = nullptr) const = 0; // 7
	virtual float get_brightness (void* = nullptr) const = 0; // 8
	virtual sxsdk::rgb_class get_visibility (void* = nullptr) const = 0; // 9
	virtual float get_softness (void* = nullptr) const = 0; // 10
	virtual float get_shadow (void* = nullptr) const = 0; // 11
	virtual float get_shadow_softness (void* = nullptr) const = 0; // 12
	virtual bool get_square (void* = nullptr) const = 0; // 13
	virtual float get_angle (void* = nullptr) const = 0; // 14
	virtual float get_specular (void* = nullptr) const = 0; // 15
	virtual float get_diffuse (void* = nullptr) const = 0; // 16
	virtual sxsdk::stream_interface* get_attribute_stream_interface (int attribute_id, void* aux = 0) const = 0; // 17
		sxsdk::stream_interface* get_attribute_stream (int attribute_id, void* aux = 0) { return get_attribute_stream_interface(attribute_id); }
	virtual int rendering_light_class_dummy18(void *) { assert(false); throw "invalid interface rendering_light_class"; return 0; } // 18
	virtual bool is_area_light (void* = nullptr) const = 0; // 19
	virtual const char* get_exception_string () const = 0; // 20
	virtual bool is_distribution_light (void* = nullptr) const = 0; // 21
	virtual float evaluate_distribution ( const sxsdk::vec3& p, void* aux = 0) const = 0; // 22
	virtual sxsdk::stream_interface* get_attribute_stream_interface_with_uuid (const sx::uuid_class& id, void* aux = 0) const = 0; // 23
	virtual sxsdk::rgb_class get_shadow_color (void* = nullptr) const = 0; // 24
#pragma clang diagnostics pop
	};

	class rendering_light_compatibility_class {
	public:
		virtual ~rendering_light_compatibility_class ();
		virtual bool is_distant_light (void* aux =0) const;
		virtual bool is_point_light (void* aux =0) const;
		virtual bool is_spotlight (void* aux =0) const;
		virtual sx::vec4 get_position (void* aux =0) const;
		virtual sx::vec3 get_direction (void* aux =0) const;
		virtual sx::rgb get_color (void* aux =0) const;
		virtual float get_ambient (void* aux =0) const;
		virtual float get_intensity (void* aux =0) const;
		virtual float get_brightness (void* aux =0) const;
		virtual sx::rgb get_visibility (void* aux =0) const;
		virtual float get_softness (void* aux =0) const;
		virtual float get_shadow (void* aux =0) const;
		virtual float get_shadow_softness (void* aux =0) const;
		virtual bool get_square (void* aux =0) const;
		virtual float get_angle (void* aux =0) const;
		virtual float get_specular (void* aux =0) const;
		virtual float get_diffuse (void* aux =0) const;
		virtual sxsdk::stream_interface *get_attribute_stream_interface (int attribute_id, void* aux =0) const; sxsdk::stream_interface *get_attribute_stream (int attribute_id, void *aux = 0) const { return get_attribute_stream_interface(attribute_id, aux); } //33042 changed to get_attribute_stream_interface
		virtual int rendering_light_interface_test_1 (void* aux =0) { return 1; }
		virtual bool is_area_light (void* aux =0) const;
	};

}
