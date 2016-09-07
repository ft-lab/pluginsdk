#pragma once

namespace sxsdk {
	class polygon_mesh_saver_class : public sx::object {
	public:
		virtual ~polygon_mesh_saver_class ();
		virtual void release (void* aux = nullptr); //30013
		virtual sx::vec3 get_point (int i, void* aux = nullptr)	const; //30013
		virtual void set_undo (void* aux = nullptr);
	};

	class sphere_saver_class : public sx::object {
	public:
		virtual ~sphere_saver_class ();
		virtual void release (void* aux = nullptr); //30013
		virtual sx::vec3 get_center (void* aux = nullptr)	const; //30013
		virtual sx::vec3 get_radius (void* aux = nullptr)	const; //30013
		virtual mat4 get_matrix (void* aux = nullptr)	const; //30013
	};

	class disk_saver_class : public sx::object {
	public:
		virtual ~disk_saver_class ();
		virtual void release (void* aux = nullptr); //30013
		virtual sx::vec3 get_center (void* aux = nullptr)	const; //30013
		virtual sx::vec2 get_radius (void* aux = nullptr)	const; //30013
		virtual mat4 get_matrix (void* aux = nullptr)	const; //30013
	};

	class line_saver_class : public sx::object {
	public:
		virtual ~line_saver_class ();
		virtual void release (void* aux = nullptr); //30013
		virtual int get_number_of_control_points (void* aux = nullptr)	const; //30013
		virtual sx::vec3 get_anchor_point (int i, void* aux = nullptr)	const; //30013
		virtual sx::vec3 get_inhandle (int i, void* aux = nullptr)	const; //30013
		virtual sx::vec3 get_outhandle (int i, void* aux = nullptr)	const; //30013
		virtual bool get_handle_linked (int i, void* aux = nullptr)	const; //30013
		virtual sx::vec3 get_lateral_inhandle (int i, void* aux = nullptr)	const; //30013
		virtual sx::vec3 get_lateral_outhandle (int i, void* aux = nullptr)	const; //30013
		virtual bool get_lateral_handle_linked (int i, void* aux = nullptr)	const; //30013
	};

	class surface_saver_class : public sx::object {
	public:
		virtual ~surface_saver_class ();
		virtual void release (void* aux = nullptr); //30013
		virtual int get_m (void* aux = nullptr)	const; //30013
		virtual int get_n (void* aux = nullptr)	const; //30013
		virtual sx::vec3 get_anchor_point (int i, int j, void* aux = nullptr)	const; //30013
		virtual sx::vec3 get_inhandle (int i, int j, void* aux = nullptr)	const; //30013
		virtual sx::vec3 get_outhandle (int i, int j, void* aux = nullptr)	const; //30013
		virtual bool get_handle_linked (int i, int j, void* aux = nullptr)	const; //30013
		virtual sx::vec3 get_lateral_inhandle (int i, int j, void* aux = nullptr)	const; //30013
		virtual sx::vec3 get_lateral_outhandle (int i, int j, void* aux = nullptr)	const; //30013
		virtual bool get_lateral_handle_linked (int i, int j, void* aux = nullptr)	const; //30013
	};
}
