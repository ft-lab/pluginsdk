#pragma once
SXMODULE_SXCORE
#include "sxcore/cocoa.hpp"
#include "sxcore/property.hpp"
#include "sxcore/relations.hpp"
#include "sxcore/vectors.hpp"
#include "sxcore/bounds.hpp"

namespace sx {
	class image_interface;
	class bitmap_class;
	class file_types_class;
	class image_interface;
	class dll_class;
	class image_utility_interface;
	class progress_interface;
	class pixelop_interface;

	namespace image {
		/// \en blah \enden \ja イメージの合成モード \endja
		enum mix_mode {
			blend_mode,			// \en blah \enden \ja 通常\endja
			alpha_blend_mode,	// \en blah \enden \ja αブレンド\endja
			add_mode,			// \en blah \enden \ja 加算\endja
			sub_mode,			// \en blah \enden \ja 減算\endja
			mul_mode,			// \en blah \enden \ja 乗算\endja
			max_mode,			// \en blah \enden \ja 比較(明)\endja
			min_mode,			// \en blah \enden \ja 比較(暗)\endja
			last_mix_mode		// \en blah \enden \ja 未使用\endja
		};
		extern const sx::ivec2 preview_size;
		#if SXAPI_COCOA
			bool load (image_interface &image, NSBitmapImageRep *bitmap);
			bool load (image_interface &image, NSImage *nsimage);
		#endif
		#if SXAPI_WIN32
			bool load (image_interface &image, Gdiplus::Bitmap &bitmap, bool premultiplied = false);
		#endif
		#if SXAPI_WIN32 || SXAPI_WINRT
			bool load (image_interface &image, BITMAPINFO &bitmapinfo, void *buff);
			bool load (image_interface &image, HBITMAP hbitmap);
		#endif
		bool load (image_interface &image, const sx::bitmap_class &bitmap);

		bool similar (const image_interface &a, const image_interface &b);
		void diff (image_interface &image, const image_interface &a, const image_interface &b);
		void draw (image_interface &target, const image_interface &image, const sx::ibounds2 &clipping, const sx::ivec2 &target_position);
		void draw (image_interface &target, const image_interface &image, const sx::ibounds2 &clipping);
		void draw (image_interface &target, const image_interface &image);

		void register_utility (const image_utility_interface &utility);
		void unregister_utility (const image_utility_interface &utility);
		void append_exporter_types (file_types_class &types);
		void append_importer_types (file_types_class &types);
		bool load (image_interface &image, const std::string& path, sx::progress_interface *progressp);
		bool save (const image_interface &image, const std::string& path, sx::progress_interface *progressp);
		bool can_save (const std::string& extension);
		bool can_load (const std::string& extension);
		int can_save_alpha (const std::string& extension);
		bool is_movie (const std::string& path);
		int get_frame_length (const std::string& path);

		class attribute_class : public sx::object {
		public:
			property<std::string> name;				// UTF-8 string: localized name
			property<std::string> id_name;			// ASCII string: identify name
			std::vector<std::string> channel_names;	// ASCII string: channel names
			property<image::mix_mode> mix;			// blend mode
			property<bounds<rgba_color<float>>> range;	// dynamic range

			enum channel_type { none_type, color_type, geometry_type, uvw_type, mask_type };

			explicit attribute_class ();
			explicit attribute_class (const std::string& name, const std::string& id_name, channel_type type = color_type, sx::image::mix_mode mix = sx::image::blend_mode);
			explicit attribute_class (const std::string& name, const std::string& id_name, const std::vector<std::string> &channel_names, image::mix_mode mix = sx::image::blend_mode);

			void setup_channel_names (channel_type type, int format = -1);
			std::string create_filepath (const std::string& path) const;

			bool operator== (const attribute_class &b);
			bool operator!= (const attribute_class &b) { return !(*this == b); }
		};
		class layer_class {
		public: // relations
			std::shared_ptr<image_interface>	image;
			attribute_class						attribute;

			explicit layer_class ();
			explicit layer_class (const std::shared_ptr<image_interface> &image, const attribute_class &attribute);
			explicit layer_class (const std::shared_ptr<image_interface> &image, const std::string& name, const std::string& id_name, attribute_class::channel_type type = attribute_class::color_type, sx::image::mix_mode mix = sx::image::blend_mode);

			void setup_channel_names (attribute_class::channel_type type, int format = -1);

			bool operator== (const layer_class &b);
			bool operator!= (const layer_class &b) { return !(*this == b); }
		};
		class multilayer_class : public std::vector<layer_class> {
		public:
			property<bool> mapped;
		public:
			explicit multilayer_class () : mapped(false) { }
			explicit multilayer_class (const std::string& path, bool mapped, sx::progress_interface *progressp) : mapped(mapped) { load(path, progressp); }
			virtual ~multilayer_class () { }

			bool load (const std::string& path, sx::progress_interface *progressp);
			bool save (const std::string& path, sx::progress_interface *progressp) const;
			bool save_division (const std::string& path, sx::progress_interface *progressp) const;

			std::string create_division_filepath (const std::string& path, int index, sx::progress_interface *progressp) const;

			static void append_exporter_types (file_types_class &types);
			static void append_importer_types (file_types_class &types);
		};
		bool has_multilayer (const std::string& path);
	}

	#if SXAPI_COCOA
		NSImage *nsimage (const image_interface &image, float opacity, bool bitmapformat, bool alpha);
		NSBitmapImageRep *nsbitmapimagerep (const image_interface &image, float opacity, bool bitmapformat, bool alpha);
		void save (const image_interface &image, const std::string& path, NSBitmapImageFileType file_type);
		void copy_to_pasteboard (const image_interface &image, NSPasteboard *pasteboard);
	#endif
	void duplicate_image (image_interface& new_image, const image_interface& image, const sx::ibounds2& r, int depth, bool alpha_brend, bool fit);
	void duplicate_image_to_thumbnail (image_interface &new_image, const image_interface &image, const int pixel_x, const int pixel_y, bool prop = false); // kishi
	void create_preview (const image_interface &image); // Creates a preview if there is none.
	void update_preview (const image_interface &image); // Always (re)creates a preview.

	std::shared_ptr<image_interface> create_subimage (const std::shared_ptr<image_interface> image, const sx::ibounds2 &bounds);
	std::shared_ptr<image_interface> create_subimage (const std::shared_ptr<const image_interface> image, const sx::ibounds2 &bounds);
	std::shared_ptr<image_interface> create_disable_alpha_image (const std::shared_ptr<const image_interface> image);
	std::shared_ptr<image_interface> create_alpha_image (const std::shared_ptr<const image_interface> image);
	std::shared_ptr<image_interface> create_corrected_image (const std::shared_ptr<const image_interface> image, const image::attribute_class &attribute);
	std::shared_ptr<image_interface> create_anaglyph_dubois_image (const std::shared_ptr<const image_interface> left_image, const std::shared_ptr<const image_interface> right_image);
	std::shared_ptr<image_interface> create_anaglyph_halfcolor_image (const std::shared_ptr<const image_interface> left_image, const std::shared_ptr<const image_interface> right_image);
	std::shared_ptr<image_interface> create_anaglyph_monochrome_image (const std::shared_ptr<const image_interface> left_image, const std::shared_ptr<const image_interface> right_image);
	std::shared_ptr<image_interface> create_striping_image (const std::shared_ptr<const image_interface> image, int number_of_views, bool reverse);
	std::shared_ptr<image_interface> create_linebyline_image (const std::shared_ptr<const image_interface> left_image, const std::shared_ptr<const image_interface> right_image);
	std::shared_ptr<image_interface> create_connected_image (const std::shared_ptr<const image_interface> left_image, const std::shared_ptr<const image_interface> right_image, bool vertical);
	std::shared_ptr<image_interface> create_connected_image (const std::vector<std::shared_ptr<const image_interface>> images, bool vertical);
	std::shared_ptr<image_interface> create_comparing_image (const std::shared_ptr<const image_interface> left_image, const std::shared_ptr<const image_interface> right_image, float percent, bool vertical);
	std::shared_ptr<image_interface> create_comparing_image (const std::vector<std::shared_ptr<const image_interface>> images, float percent, bool vertical);
	std::shared_ptr<image_interface> create_absdiff_image (const std::shared_ptr<const image_interface> left_image, const std::shared_ptr<const image_interface> right_image);
	std::vector<std::shared_ptr<image_interface>> create_divided_images (const std::shared_ptr<const image_interface> image, const sx::ivec2 &division);
	std::shared_ptr<image_interface> create_alphablend_image (const std::shared_ptr<const image_interface> image, const std::shared_ptr<const image_interface> backdrop, const sx::ivec2& whole_size);
	std::shared_ptr<image_interface> create_alphablend_image (const std::shared_ptr<const image_interface> image, const std::shared_ptr<const image_interface> backdrop);
	std::shared_ptr<image_interface> create_pixelop_image (const std::shared_ptr<const image_interface> image, std::unique_ptr<pixelop_interface> pixelop);
	std::shared_ptr<image_interface> create_lapped_image (const std::shared_ptr<const image_interface> top_image, const std::shared_ptr<const image_interface> bottom_image, const sx::ivec2& offset);

	std::shared_ptr<image_interface> create_image_tiled_logo (const std::shared_ptr<const image_interface> image, const std::shared_ptr<const image_interface> logo);
	std::shared_ptr<image_interface> create_image_logo (const std::shared_ptr<const image_interface> image, const std::shared_ptr<const image_interface> logo);
}
std::ostream& operator<< (std::ostream &s, const sx::image::layer_class &a);
std::ostream& operator<< (std::ostream &s, const sx::image::attribute_class &a);
