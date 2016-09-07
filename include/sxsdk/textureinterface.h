#pragma once

namespace sxsdk {
	class texture_interface : public unknown_interface, public sx::object {
	public:
#pragma clang diagnostics push
#pragma clang diagnostic ignored "-Wold-style-cast"
	virtual sxsdk::rgb_class calculate_diffuse_color_deprecated (void* aux = 0) = 0; // 0
	virtual const char* get_exception_string () const = 0; // 1
	virtual sxsdk::vec3 calculate_normal_deprecated (void* aux = 0) = 0; // 2
	virtual bool calculate_trim_deprecated (void* aux = 0) = 0; // 3
	virtual sxsdk::vec3 get_position (void* = nullptr) const = 0; // 4
	virtual texture_interface &set_position (const sxsdk::vec3 &position_param, void* = nullptr) = 0; // 5
	virtual sxsdk::vec3 get_shading_normal (void* = nullptr) const = 0; // 6
	virtual texture_interface &set_shading_normal (const sxsdk::vec3 &shading_normal_param, void* = nullptr) = 0; // 7
	virtual sxsdk::vec3 get_geometric_normal (void* = nullptr) const = 0; // 8
	virtual texture_interface &set_geometric_normal (const sxsdk::vec3 &geometric_normal_param, void* = nullptr) = 0; // 9
	virtual sxsdk::vec3 get_incident (void* = nullptr) const = 0; // 10
	virtual texture_interface &set_incident (const sxsdk::vec3 &incident_param, void* = nullptr) = 0; // 11
	virtual sxsdk::vec2 get_uv (void* = nullptr) const = 0; // 12
	virtual texture_interface &set_uv (const sxsdk::vec2 &uv_param, void* = nullptr) = 0; // 13
	virtual sxsdk::vec3 get_du (void* = nullptr) const = 0; // 14
	virtual texture_interface &set_du (const sxsdk::vec3 &du_param, void* = nullptr) = 0; // 15
	virtual sxsdk::vec3 get_dv (void* = nullptr) const = 0; // 16
	virtual texture_interface &set_dv (const sxsdk::vec3 &dv_param, void* = nullptr) = 0; // 17
	virtual sxsdk::rgb_class get_diffuse_color_deprecated (void* = nullptr) const = 0; // 18
	virtual sxsdk::vec3 get_normal_deprecated (void* = nullptr) const = 0; // 19
	virtual bool get_trim (void* = nullptr) const = 0; // 20
	virtual sxsdk::texture_interface& update (void* aux = 0) = 0; // 21
	virtual sxsdk::rgb_class calculate_specular (const sxsdk::vec3& L, void* aux = 0) const = 0; // 22
	virtual sxsdk::rgb_class get_diffuse (void* = nullptr) const = 0; // 23
	virtual sxsdk::rgb_class get_specular1 (void* = nullptr) const = 0; // 24
	virtual sxsdk::rgb_class get_specular2 (void* = nullptr) const = 0; // 25
	virtual sxsdk::rgb_class get_reflection (void* = nullptr) const = 0; // 26
	virtual sxsdk::rgb_class get_transparency (void* = nullptr) const = 0; // 27
	virtual sxsdk::rgb_class get_glow (void* = nullptr) const = 0; // 28
	virtual sxsdk::vec3 get_perturbed_normal (void* = nullptr) const = 0; // 29
	virtual sxsdk::vec4 get_uvs (void* = nullptr) const = 0; // 30
	virtual texture_interface &set_uvs (const sxsdk::vec4 &uvs_param, void* = nullptr) = 0; // 31
	virtual sxsdk::vec2 get_uvs2 (int index, void* aux = 0) const = 0; // 32
	virtual sxsdk::texture_interface& set_uvs2 (int index, const sxsdk::vec2& uv, void* aux = 0) = 0; // 33
#pragma clang diagnostics pop
	};
}
