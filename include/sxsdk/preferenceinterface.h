#pragma once


namespace sxsdk {
	class preference_interface : public shade_interface {
	public:
#pragma clang diagnostics push
#pragma clang diagnostic ignored "-Wold-style-cast"
	virtual bool get_backward_compatible_file_obsolete (void* = nullptr) const = 0; // 0
	virtual preference_interface &set_backward_compatible_file_obsolete (bool backward_compatible_file_obsolete_param, void* = nullptr) = 0; // 1
	virtual bool get_add_preview_image (void* = nullptr) const = 0; // 2
	virtual preference_interface &set_add_preview_image (bool add_preview_image_param, void* = nullptr) = 0; // 3
	virtual bool get_language (void* = nullptr) const = 0; // 4
	virtual preference_interface &set_language (bool language_param, void* = nullptr) = 0; // 5
	virtual bool get_crossup_menu_obsolete (void* = nullptr) const = 0; // 6
	virtual preference_interface &set_crossup_menu_obsolete (bool crossup_menu_obsolete_param, void* = nullptr) = 0; // 7
	virtual bool get_sticky_mouse (void* = nullptr) const = 0; // 8
	virtual preference_interface &set_sticky_mouse (bool sticky_mouse_param, void* = nullptr) = 0; // 9
	virtual const char* get_image_creator_obsolete (void* = nullptr) const = 0; // 10
	virtual preference_interface &set_image_creator_obsolete (const char* image_creator_obsolete_param, void* = nullptr) = 0; // 11
	virtual bool get_use_image_cache_obsolete (void* = nullptr) const = 0; // 12
	virtual preference_interface &set_use_image_cache_obsolete (bool use_image_cache_obsolete_param, void* = nullptr) = 0; // 13
	virtual const char *get_temporary_drive_obsolete (void* aux = 0) = 0; // 14
	virtual void set_temporary_drive_obsolete (int temporary_drive, void* aux = 0) = 0; // 15
	virtual int get_number_of_temporary_dirves_obsolete (void* aux = 0) = 0; // 16
	virtual const char *get_temporary_drive_name_obsolete (int i, void* aux = 0) = 0; // 17
	virtual sxsdk::rgb_class get_background_color (void* = nullptr) const = 0; // 18
	virtual preference_interface &set_background_color (const sxsdk::rgb_class &background_color_param, void* = nullptr) = 0; // 19
	virtual sxsdk::rgb_class get_active_wireframe_color (void* = nullptr) const = 0; // 20
	virtual preference_interface &set_active_wireframe_color (const sxsdk::rgb_class &active_wireframe_color_param, void* = nullptr) = 0; // 21
	virtual sxsdk::rgb_class get_modify_wireframe_color (void* = nullptr) const = 0; // 22
	virtual preference_interface &set_modify_wireframe_color (const sxsdk::rgb_class &modify_wireframe_color_param, void* = nullptr) = 0; // 23
	virtual sxsdk::rgb_class get_inactive_wireframe_color (void* = nullptr) const = 0; // 24
	virtual preference_interface &set_inactive_wireframe_color (const sxsdk::rgb_class &inactive_wireframe_color_param, void* = nullptr) = 0; // 25
	virtual sxsdk::rgb_class get_ruler_color (void* = nullptr) const = 0; // 26
	virtual preference_interface &set_ruler_color (const sxsdk::rgb_class &ruler_color_param, void* = nullptr) = 0; // 27
	virtual sxsdk::rgb_class get_major_grid_color (void* = nullptr) const = 0; // 28
	virtual preference_interface &set_major_grid_color (const sxsdk::rgb_class &major_grid_color_param, void* = nullptr) = 0; // 29
	virtual sxsdk::rgb_class get_minor_grid_color (void* = nullptr) const = 0; // 30
	virtual preference_interface &set_minor_grid_color (const sxsdk::rgb_class &minor_grid_color_param, void* = nullptr) = 0; // 31
	virtual sxsdk::rgb_class get_active_point_color (void* = nullptr) const = 0; // 32
	virtual preference_interface &set_active_point_color (const sxsdk::rgb_class &active_point_color_param, void* = nullptr) = 0; // 33
	virtual sxsdk::rgb_class get_inactive_point_color (void* = nullptr) const = 0; // 34
	virtual preference_interface &set_inactive_point_color (const sxsdk::rgb_class &inactive_point_color_param, void* = nullptr) = 0; // 35
	virtual sxsdk::rgb_class get_x_axis_color (void* = nullptr) const = 0; // 36
	virtual preference_interface &set_x_axis_color (const sxsdk::rgb_class &x_axis_color_param, void* = nullptr) = 0; // 37
	virtual sxsdk::rgb_class get_y_axis_color (void* = nullptr) const = 0; // 38
	virtual preference_interface &set_y_axis_color (const sxsdk::rgb_class &y_axis_color_param, void* = nullptr) = 0; // 39
	virtual sxsdk::rgb_class get_z_axis_color (void* = nullptr) const = 0; // 40
	virtual preference_interface &set_z_axis_color (const sxsdk::rgb_class &z_axis_color_param, void* = nullptr) = 0; // 41
	virtual int get_colored_wireframe (void* = nullptr) const = 0; // 42
	virtual preference_interface &set_colored_wireframe (int colored_wireframe_param, void* = nullptr) = 0; // 43
	virtual bool get_white_cursor (void* = nullptr) const = 0; // 44
	virtual preference_interface &set_white_cursor (bool white_cursor_param, void* = nullptr) = 0; // 45
	virtual bool get_opengl_accelerator (void* = nullptr) const = 0; // 46
	virtual preference_interface &set_opengl_accelerator (bool opengl_accelerator_param, void* = nullptr) = 0; // 47
	virtual int get_opengl_scroll_mode (void* = nullptr) const = 0; // 48
	virtual preference_interface &set_opengl_scroll_mode (int opengl_scroll_mode_param, void* = nullptr) = 0; // 49
	virtual int get_facet_h (void* aux = 0) = 0; // 50
	virtual void set_facet_h (int facet_h, void* aux = 0) = 0; // 51
	virtual int preference_interface_dummy52(void *) { assert(false); throw "invalid interface preference_interface"; return 0; } // 52
	virtual int preference_interface_dummy53(void *) { assert(false); throw "invalid interface preference_interface"; return 0; } // 53
	virtual int preference_interface_dummy54(void *) { assert(false); throw "invalid interface preference_interface"; return 0; } // 54
	virtual int preference_interface_dummy55(void *) { assert(false); throw "invalid interface preference_interface"; return 0; } // 55
	virtual bool get_break_menu (void* = nullptr) const = 0; // 56
	virtual preference_interface &set_break_menu (bool break_menu_param, void* = nullptr) = 0; // 57
	virtual int get_targa_color_depth (void* = nullptr) const = 0; // 58
	virtual preference_interface &set_targa_color_depth (int targa_color_depth_param, void* = nullptr) = 0; // 59
	virtual bool get_targa_compression (void* = nullptr) const = 0; // 60
	virtual preference_interface &set_targa_compression (bool targa_compression_param, void* = nullptr) = 0; // 61
	virtual int get_encoding (void* = nullptr) const = 0; // 62
	virtual preference_interface &set_encoding (int encoding_param, void* = nullptr) = 0; // 63
	virtual int get_opengl_cursor_mode (void* = nullptr) const = 0; // 64
	virtual preference_interface &set_opengl_cursor_mode (int opengl_cursor_mode_param, void* = nullptr) = 0; // 65
	virtual bool get_opengl_vertexarray_wireframe (void* = nullptr) const = 0; // 66
	virtual preference_interface &set_opengl_vertexarray_wireframe (bool opengl_vertexarray_wireframe_param, void* = nullptr) = 0; // 67
	virtual int get_opengl_texture_mode (void* = nullptr) const = 0; // 68
	virtual preference_interface &set_opengl_texture_mode (int opengl_texture_mode_param, void* = nullptr) = 0; // 69
	virtual bool get_opengl_backing_store (void* = nullptr) const = 0; // 70
	virtual preference_interface &set_opengl_backing_store (bool opengl_backing_store_param, void* = nullptr) = 0; // 71
	virtual int preference_interface_dummy72(void *) { assert(false); throw "invalid interface preference_interface"; return 0; } // 72
	virtual int preference_interface_dummy73(void *) { assert(false); throw "invalid interface preference_interface"; return 0; } // 73
	virtual stream_interface *create_attribute_stream_interface (int id, void* aux = 0) = 0; // 74
	virtual sxsdk::stream_interface* get_attribute_stream_interface (int id, void* aux = 0) = 0; // 75
	virtual bool delete_attribute (int id, void* aux = 0) = 0; // 76
	virtual int get_font_size (void* = nullptr) const = 0; // 77
	virtual preference_interface &set_font_size (int font_size_param, void* = nullptr) = 0; // 78
	virtual const char* get_plugin_blacklist (void* = nullptr) const = 0; // 79
	virtual preference_interface &set_plugin_blacklist (const char* plugin_blacklist_param, void* = nullptr) = 0; // 80
	virtual int preference_interface_test_1 ( void* aux =0 ) { return 1; } // 81
	virtual bool get_opengl_vertexarray_quickrendering (void* = nullptr) const = 0; // 82
	virtual preference_interface &set_opengl_vertexarray_quickrendering (bool opengl_vertexarray_quickrendering_param, void* = nullptr) = 0; // 83
	virtual property_list_interface* get_property_list_interface (void* = nullptr) const = 0; // 84
	virtual bool get_opengl_texture_object (void* = nullptr) const = 0; // 85
	virtual preference_interface &set_opengl_texture_object (bool opengl_texture_object_param, void* = nullptr) = 0; // 86
	virtual bool get_sort_on_copy (void* = nullptr) const = 0; // 87
	virtual preference_interface &set_sort_on_copy (bool sort_on_copy_param, void* = nullptr) = 0; // 88
	virtual int get_verbose_level (void* = nullptr) const = 0; // 89
	virtual preference_interface &set_verbose_level (int verbose_level_param, void* = nullptr) = 0; // 90
	virtual sxsdk::rgb_class get_active_shape_background_color (void* = nullptr) const = 0; // 91
	virtual preference_interface &set_active_shape_background_color (const sxsdk::rgb_class &active_shape_background_color_param, void* = nullptr) = 0; // 92
	virtual int get_text_file_encoding (void* = nullptr) const = 0; // 93
	virtual preference_interface &set_text_file_encoding (int text_file_encoding_param, void* = nullptr) = 0; // 94
	virtual bool get_opengl_swap_rect (void* = nullptr) const = 0; // 95
	virtual preference_interface &set_opengl_swap_rect (bool opengl_swap_rect_param, void* = nullptr) = 0; // 96
	virtual bool get_opengl_diamond_handle_obsolete (void* = nullptr) const = 0; // 97
	virtual preference_interface &set_opengl_diamond_handle_obsolete (bool opengl_diamond_handle_obsolete_param, void* = nullptr) = 0; // 98
	virtual bool get_auto_encoding (void* = nullptr) const = 0; // 99
	virtual preference_interface &set_auto_encoding (bool auto_encoding_param, void* = nullptr) = 0; // 100
	virtual bool get_opengl_invalidate_context (void* = nullptr) const = 0; // 101
	virtual preference_interface &set_opengl_invalidate_context (bool opengl_invalidate_context_param, void* = nullptr) = 0; // 102
	virtual bool get_window_shadows_transparency (void* = nullptr) const = 0; // 103
	virtual preference_interface &set_window_shadows_transparency (bool window_shadows_transparency_param, void* = nullptr) = 0; // 104
	virtual unsigned get_langid (void* = nullptr) const = 0; // 105
	virtual preference_interface &set_langid (unsigned langid_param, void* = nullptr) = 0; // 106
	virtual bool get_r4_compatible_file_obsolete (void* = nullptr) const = 0; // 107
	virtual preference_interface &set_r4_compatible_file_obsolete (bool r4_compatible_file_obsolete_param, void* = nullptr) = 0; // 108
	virtual bool get_opengl_vertexbufferobject (void* = nullptr) const = 0; // 109
	virtual preference_interface &set_opengl_vertexbufferobject (bool opengl_vertexbufferobject_param, void* = nullptr) = 0; // 110
	virtual bool get_finish_beep (void* = nullptr) const = 0; // 111
	virtual preference_interface &set_finish_beep (bool finish_beep_param, void* = nullptr) = 0; // 112
	virtual bool get_opengl_overlay (void* = nullptr) const = 0; // 113
	virtual preference_interface &set_opengl_overlay (bool opengl_overlay_param, void* = nullptr) = 0; // 114
	virtual bool get_lazy_message (void* = nullptr) const = 0; // 115
	virtual preference_interface &set_lazy_message (bool lazy_message_param, void* = nullptr) = 0; // 116
	virtual bool get_menutips (void* = nullptr) const = 0; // 117
	virtual preference_interface &set_menutips (bool menutips_param, void* = nullptr) = 0; // 118
	virtual int get_motion_graph_height (void* = nullptr) const = 0; // 119
	virtual preference_interface &set_motion_graph_height (int motion_graph_height_param, void* = nullptr) = 0; // 120
	virtual int get_motion_panel_width (void* = nullptr) const = 0; // 121
	virtual preference_interface &set_motion_panel_width (int motion_panel_width_param, void* = nullptr) = 0; // 122
	virtual const char* get_ip_addresses_obsolete (void* = nullptr) const = 0; // 123
	virtual preference_interface &set_ip_addresses_obsolete (const char* ip_addresses_obsolete_param, void* = nullptr) = 0; // 124
	virtual sxsdk::rgb_class get_normals_color (void* = nullptr) const = 0; // 125
	virtual preference_interface &set_normals_color (const sxsdk::rgb_class &normals_color_param, void* = nullptr) = 0; // 126
	virtual bool get_opengl_always_redraw (void* = nullptr) const = 0; // 127
	virtual preference_interface &set_opengl_always_redraw (bool opengl_always_redraw_param, void* = nullptr) = 0; // 128
	virtual bool get_must_be_saved (void* = nullptr) const = 0; // 129
	virtual preference_interface &set_must_be_saved (bool must_be_saved_param, void* = nullptr) = 0; // 130
	virtual void stream_to_clipboard (void* aux = 0) = 0; // 131
	virtual bool get_smooth_text (void* = nullptr) const = 0; // 132
	virtual preference_interface &set_smooth_text (bool smooth_text_param, void* = nullptr) = 0; // 133
	virtual bool get_supress_image_window (void* = nullptr) const = 0; // 134
	virtual preference_interface &set_supress_image_window (bool supress_image_window_param, void* = nullptr) = 0; // 135
	virtual bool get_opengl_pbuffer (void* = nullptr) const = 0; // 136
	virtual preference_interface &set_opengl_pbuffer (bool opengl_pbuffer_param, void* = nullptr) = 0; // 137
	virtual stream_interface *create_attribute_stream_interface_with_uuid (const sx::uuid_class& id, void* aux = 0) = 0; // 138
	virtual sxsdk::stream_interface* get_attribute_stream_interface_with_uuid (const sx::uuid_class& id, void* aux = 0) = 0; // 139
	virtual bool delete_attribute_with_uuid (const sx::uuid_class& id, void* aux = 0) = 0; // 140
	virtual bool get_skip_mainframe_rectangle (void* = nullptr) const = 0; // 141
	virtual preference_interface &set_skip_mainframe_rectangle (bool skip_mainframe_rectangle_param, void* = nullptr) = 0; // 142
	virtual bool get_skip_redraw (void* = nullptr) const = 0; // 143
	virtual preference_interface &set_skip_redraw (bool skip_redraw_param, void* = nullptr) = 0; // 144
	virtual bool get_continuous_redraw (void* = nullptr) const = 0; // 145
	virtual preference_interface &set_continuous_redraw (bool continuous_redraw_param, void* = nullptr) = 0; // 146
	virtual bool get_continuous_pick (void* = nullptr) const = 0; // 147
	virtual preference_interface &set_continuous_pick (bool continuous_pick_param, void* = nullptr) = 0; // 148
	virtual sxsdk::rgb_class get_active_face_color (void* = nullptr) const = 0; // 149
	virtual preference_interface &set_active_face_color (const sxsdk::rgb_class &active_face_color_param, void* = nullptr) = 0; // 150
	virtual sxsdk::rgb_class get_active_edge_color (void* = nullptr) const = 0; // 151
	virtual preference_interface &set_active_edge_color (const sxsdk::rgb_class &active_edge_color_param, void* = nullptr) = 0; // 152
	virtual bool get_radiosity (void* = nullptr) const = 0; // 153
	virtual preference_interface &set_radiosity (bool radiosity_param, void* = nullptr) = 0; // 154
	virtual sxsdk::rgb_class get_handle_color (void* = nullptr) const = 0; // 155
	virtual preference_interface &set_handle_color (const sxsdk::rgb_class &handle_color_param, void* = nullptr) = 0; // 156
	virtual const char* get_finish_sound_filename (void* = nullptr) const = 0; // 157
	virtual preference_interface &set_finish_sound_filename (const char* finish_sound_filename_param, void* = nullptr) = 0; // 158
	virtual const char* get_poser_app_location (void* = nullptr) const = 0; // 159
	virtual preference_interface &set_poser_app_location (const char* poser_app_location_param, void* = nullptr) = 0; // 160
	virtual bool get_poser_integer_frame (void* = nullptr) const = 0; // 161
	virtual preference_interface &set_poser_integer_frame (bool poser_integer_frame_param, void* = nullptr) = 0; // 162
	virtual bool get_poser_normals (void* = nullptr) const = 0; // 163
	virtual preference_interface &set_poser_normals (bool poser_normals_param, void* = nullptr) = 0; // 164
	virtual int get_window_color (void* = nullptr) const = 0; // 165
	virtual preference_interface &set_window_color (int window_color_param, void* = nullptr) = 0; // 166
	virtual float get_base_brightness (void* = nullptr) const = 0; // 167
	virtual preference_interface &set_base_brightness (float base_brightness_param, void* = nullptr) = 0; // 168
	virtual float get_scroll_wheel_speed (void* = nullptr) const = 0; // 169
	virtual preference_interface &set_scroll_wheel_speed (float scroll_wheel_speed_param, void* = nullptr) = 0; // 170
	virtual float get_active_font_brightness (void* = nullptr) const = 0; // 171
	virtual preference_interface &set_active_font_brightness (float active_font_brightness_param, void* = nullptr) = 0; // 172
	virtual float get_inactive_font_brightness (void* = nullptr) const = 0; // 173
	virtual preference_interface &set_inactive_font_brightness (float inactive_font_brightness_param, void* = nullptr) = 0; // 174
	virtual bool get_opengl_drawelements (void* = nullptr) const = 0; // 175
	virtual preference_interface &set_opengl_drawelements (bool opengl_drawelements_param, void* = nullptr) = 0; // 176
	virtual bool get_skip_volume_rendering (void* = nullptr) const = 0; // 177
	virtual preference_interface &set_skip_volume_rendering (bool skip_volume_rendering_param, void* = nullptr) = 0; // 178
	virtual bool get_update_check (void* = nullptr) const = 0; // 179
	virtual preference_interface &set_update_check (bool update_check_param, void* = nullptr) = 0; // 180
	virtual bool get_skip_logging (void* = nullptr) const = 0; // 181
	virtual preference_interface &set_skip_logging (bool skip_logging_param, void* = nullptr) = 0; // 182
	virtual float get_manip_global_scale (void* = nullptr) const = 0; // 183
	virtual preference_interface &set_manip_global_scale (float manip_global_scale_param, void* = nullptr) = 0; // 184
	virtual float get_manip_handle_scale (void* = nullptr) const = 0; // 185
	virtual preference_interface &set_manip_handle_scale (float manip_handle_scale_param, void* = nullptr) = 0; // 186
	virtual int get_opengl_texture_size (void* = nullptr) const = 0; // 187
	virtual preference_interface &set_opengl_texture_size (int opengl_texture_size_param, void* = nullptr) = 0; // 188
	virtual sxsdk::rgb_class get_manip_color_x (void* = nullptr) const = 0; // 189
	virtual preference_interface &set_manip_color_x (const sxsdk::rgb_class &manip_color_x_param, void* = nullptr) = 0; // 190
	virtual sxsdk::rgb_class get_manip_color_y (void* = nullptr) const = 0; // 191
	virtual preference_interface &set_manip_color_y (const sxsdk::rgb_class &manip_color_y_param, void* = nullptr) = 0; // 192
	virtual sxsdk::rgb_class get_manip_color_z (void* = nullptr) const = 0; // 193
	virtual preference_interface &set_manip_color_z (const sxsdk::rgb_class &manip_color_z_param, void* = nullptr) = 0; // 194
	virtual sxsdk::rgb_class get_manip_color_center (void* = nullptr) const = 0; // 195
	virtual preference_interface &set_manip_color_center (const sxsdk::rgb_class &manip_color_center_param, void* = nullptr) = 0; // 196
	virtual sxsdk::rgb_class get_manip_color_active (void* = nullptr) const = 0; // 197
	virtual preference_interface &set_manip_color_active (const sxsdk::rgb_class &manip_color_active_param, void* = nullptr) = 0; // 198
	virtual int get_statusbar_location (void* = nullptr) const = 0; // 199
	virtual preference_interface &set_statusbar_location (int statusbar_location_param, void* = nullptr) = 0; // 200
	virtual bool get_opengl_disablestipple (void* = nullptr) const = 0; // 201
	virtual preference_interface &set_opengl_disablestipple (bool opengl_disablestipple_param, void* = nullptr) = 0; // 202
	virtual bool get_show_axis_info (void* = nullptr) const = 0; // 203
	virtual preference_interface &set_show_axis_info (bool show_axis_info_param, void* = nullptr) = 0; // 204
	virtual sxsdk::rgb_class get_work_plane_grid_color (void* = nullptr) const = 0; // 205
	virtual preference_interface &set_work_plane_grid_color (const sxsdk::rgb_class &work_plane_grid_color_param, void* = nullptr) = 0; // 206
	virtual sxsdk::rgb_class get_work_plane_minor_grid_color (void* = nullptr) const = 0; // 207
	virtual preference_interface &set_work_plane_minor_grid_color (const sxsdk::rgb_class &work_plane_minor_grid_color_param, void* = nullptr) = 0; // 208
	virtual sxsdk::rgb_class get_work_plane_axis_color (void* = nullptr) const = 0; // 209
	virtual preference_interface &set_work_plane_axis_color (const sxsdk::rgb_class &work_plane_axis_color_param, void* = nullptr) = 0; // 210
	virtual sxsdk::rgb_class get_work_plane_lock_color (void* = nullptr) const = 0; // 211
	virtual preference_interface &set_work_plane_lock_color (const sxsdk::rgb_class &work_plane_lock_color_param, void* = nullptr) = 0; // 212
	virtual sxsdk::rgb_class get_work_plane_unlock_color (void* = nullptr) const = 0; // 213
	virtual preference_interface &set_work_plane_unlock_color (const sxsdk::rgb_class &work_plane_unlock_color_param, void* = nullptr) = 0; // 214
	virtual sxsdk::rgb_class get_marker_color (int index, void* aux = 0) const = 0; // 215
	virtual sxsdk::rgb_class get_boundingbox_color (void* = nullptr) const = 0; // 216
	virtual preference_interface &set_boundingbox_color (const sxsdk::rgb_class &boundingbox_color_param, void* = nullptr) = 0; // 217
	virtual sxsdk::rgb_class get_work_plane_color (void* = nullptr) const = 0; // 218
	virtual preference_interface &set_work_plane_color (const sxsdk::rgb_class &work_plane_color_param, void* = nullptr) = 0; // 219
	virtual float get_work_plane_alpha (void* = nullptr) const = 0; // 220
	virtual preference_interface &set_work_plane_alpha (float work_plane_alpha_param, void* = nullptr) = 0; // 221
	virtual int get_work_plane_minor_grid_shape (void* = nullptr) const = 0; // 222
	virtual preference_interface &set_work_plane_minor_grid_shape (int work_plane_minor_grid_shape_param, void* = nullptr) = 0; // 223
	virtual float get_work_plane_grid_alpha (void* = nullptr) const = 0; // 224
	virtual preference_interface &set_work_plane_grid_alpha (float work_plane_grid_alpha_param, void* = nullptr) = 0; // 225
	virtual void save_colors (const char*const& in, void* aux = 0) = 0; // 226
	virtual void load_colors (const char*const& from, void* aux = 0) = 0; // 227
	virtual int get_windows_color (void* aux = 0) = 0; // 228
	virtual void set_windows_color (int window_color, void* aux = 0) = 0; // 229
	virtual bool get_break_on_outofmemory (void* = nullptr) const = 0; // 230
	virtual preference_interface &set_break_on_outofmemory (bool break_on_outofmemory_param, void* = nullptr) = 0; // 231
	virtual bool get_skip_raytracing (void* = nullptr) const = 0; // 232
	virtual preference_interface &set_skip_raytracing (bool skip_raytracing_param, void* = nullptr) = 0; // 233
	virtual bool get_autosave (void* = nullptr) const = 0; // 234
	virtual preference_interface &set_autosave (bool autosave_param, void* = nullptr) = 0; // 235
	virtual int get_autosave_interval (void* = nullptr) const = 0; // 236
	virtual preference_interface &set_autosave_interval (int autosave_interval_param, void* = nullptr) = 0; // 237
	virtual const char* get_autosave_outputpath (void* = nullptr) const = 0; // 238
	virtual preference_interface &set_autosave_outputpath (const char* autosave_outputpath_param, void* = nullptr) = 0; // 239
	virtual int get_autosave_history (void* = nullptr) const = 0; // 240
	virtual preference_interface &set_autosave_history (int autosave_history_param, void* = nullptr) = 0; // 241
	virtual bool get_opengl_stereo (void* = nullptr) const = 0; // 242
	virtual preference_interface &set_opengl_stereo (bool opengl_stereo_param, void* = nullptr) = 0; // 243
	virtual sxsdk::rgb_class get_perspective_sky_color (void* = nullptr) const = 0; // 244
	virtual preference_interface &set_perspective_sky_color (const sxsdk::rgb_class &perspective_sky_color_param, void* = nullptr) = 0; // 245
	virtual sxsdk::rgb_class get_perspective_sky_center_color (void* = nullptr) const = 0; // 246
	virtual preference_interface &set_perspective_sky_center_color (const sxsdk::rgb_class &perspective_sky_center_color_param, void* = nullptr) = 0; // 247
	virtual sxsdk::rgb_class get_perspective_ground_center_color (void* = nullptr) const = 0; // 248
	virtual preference_interface &set_perspective_ground_center_color (const sxsdk::rgb_class &perspective_ground_center_color_param, void* = nullptr) = 0; // 249
	virtual sxsdk::rgb_class get_perspective_ground_color (void* = nullptr) const = 0; // 250
	virtual preference_interface &set_perspective_ground_color (const sxsdk::rgb_class &perspective_ground_color_param, void* = nullptr) = 0; // 251
	virtual sxsdk::rgb_class get_selected_control_color (void* = nullptr) const = 0; // 252
	virtual preference_interface &set_selected_control_color (const sxsdk::rgb_class &selected_control_color_param, void* = nullptr) = 0; // 253
	virtual bool get_opengl_fbo (void* = nullptr) const = 0; // 254
	virtual preference_interface &set_opengl_fbo (bool opengl_fbo_param, void* = nullptr) = 0; // 255
	virtual sxsdk::rgb_class get_button_frame_color (void* = nullptr) const = 0; // 256
	virtual preference_interface &set_button_frame_color (const sxsdk::rgb_class &button_frame_color_param, void* = nullptr) = 0; // 257
	virtual sxsdk::rgb_class get_group_header_color (void* = nullptr) const = 0; // 258
	virtual preference_interface &set_group_header_color (const sxsdk::rgb_class &group_header_color_param, void* = nullptr) = 0; // 259
	virtual sxsdk::rgb_class get_tool_header_color (void* = nullptr) const = 0; // 260
	virtual preference_interface &set_tool_header_color (const sxsdk::rgb_class &tool_header_color_param, void* = nullptr) = 0; // 261
	virtual sxsdk::rgb_class get_mainframe_background_color (void* = nullptr) const = 0; // 262
	virtual preference_interface &set_mainframe_background_color (const sxsdk::rgb_class &mainframe_background_color_param, void* = nullptr) = 0; // 263
	virtual bool get_opengl_update_background (void* = nullptr) const = 0; // 264
	virtual preference_interface &set_opengl_update_background (bool opengl_update_background_param, void* = nullptr) = 0; // 265
	virtual const char* get_locale (void* = nullptr) const = 0; // 266
	virtual bool get_opengl_multisample (void* = nullptr) const = 0; // 267
	virtual preference_interface &set_opengl_multisample (bool opengl_multisample_param, void* = nullptr) = 0; // 268
	virtual sxsdk::rgb_class get_backfaces_shading_color (void* = nullptr) const = 0; // 269
	virtual preference_interface &set_backfaces_shading_color (const sxsdk::rgb_class &backfaces_shading_color_param, void* = nullptr) = 0; // 270
	virtual sxsdk::rgb_class get_nonmanifold_wireframe_color (void* = nullptr) const = 0; // 271
	virtual preference_interface &set_nonmanifold_wireframe_color (const sxsdk::rgb_class &nonmanifold_wireframe_color_param, void* = nullptr) = 0; // 272
	virtual bool get_show_ruler (void* = nullptr) const = 0; // 273
	virtual preference_interface &set_show_ruler (bool show_ruler_param, void* = nullptr) = 0; // 274
	virtual bool get_show_divider (void* = nullptr) const = 0; // 275
	virtual preference_interface &set_show_divider (bool show_divider_param, void* = nullptr) = 0; // 276
	virtual bool get_show_menubar_overwrap (void* = nullptr) const = 0; // 277
	virtual preference_interface &set_show_menubar_overwrap (bool show_menubar_overwrap_param, void* = nullptr) = 0; // 278
	virtual bool get_show_menubar_padding (void* = nullptr) const = 0; // 279
	virtual preference_interface &set_show_menubar_padding (bool show_menubar_padding_param, void* = nullptr) = 0; // 280
#pragma clang diagnostics pop
	};
}
