//================================================================================================================
//	Adding noise plugin
//
//	Copyright 2015 Shade3D Co.,Ltd.
//================================================================================================================

#include "sxsdk.cxx"
#include "SampleEffectorGlobals.h"
#include "SampleEffector.h"
#include "add_noise.h"
#include "image_utility.h"

#include "boost/scoped_array.hpp"

///ja @breif 「ノイズを追加する」の設定パラメータの構造体 
struct add_noise_stream
{
	enum {Full_screen, Background_only, Foreground_only};

	add_noise_stream() : opacity(0.2f), color_noise(false), color(1.0f, 1.0f, 1.0f, 0.0f), apply_to(Full_screen), animated_noise(false), z_depth_effect(0.0f), revert_z_depth(false) {}

	bool read_stream (sxsdk::stream_interface* stream);
	bool write_stream (sxsdk::stream_interface* stream);

	bool color_noise;
	sxsdk::vec4 color;
	int apply_to;
	bool animated_noise;
	float z_depth_effect;
	float revert_z_depth;
	float opacity;
};

bool add_noise_stream::read_stream (sxsdk::stream_interface* stream)
{
	int tmp = 0;
	bool result = false;

	stream->enter_critical_section();
	try {
		stream->set_pointer(0);
		stream->read_int(tmp);	color_noise = tmp ? true : false;
		stream->read_float(color.x);
		stream->read_float(color.y);
		stream->read_float(color.z);
		stream->read_float(color.w);
		stream->read_int(apply_to);
		stream->read_int(tmp);	animated_noise = tmp ? true : false;
		stream->read_float(z_depth_effect);
		stream->read_int(tmp);	revert_z_depth = tmp ? true : false;
		stream->read_float(opacity);
		result = true;
	} catch(...) {}
	stream->leave_critical_section();

	return result;
}

bool add_noise_stream::write_stream (sxsdk::stream_interface* stream)
{
	int tmp = 0;
	bool result = false;

	stream->enter_critical_section();
	try {
		stream->set_pointer(0);
		stream->write_int(color_noise ? 1 : 0);
		stream->write_float(color.x);
		stream->write_float(color.y);
		stream->write_float(color.z);
		stream->write_float(color.w < 1.0f ? color.w : 1.0f);
		stream->write_int(apply_to);
		stream->write_int(animated_noise ? 1 : 0);
		stream->write_float(z_depth_effect);
		stream->write_int(revert_z_depth ? 1 : 0);
		stream->write_float(opacity);
		result = true;
	} catch(...) {}
	stream->leave_critical_section();

	return result;
}


///ja @breif 「ノイズを追加する」イメージへの描画処理 
///ja \relates add_noise_stream
void draw_noise(sxsdk::image_interface* image, const add_noise_stream& settings, const sx::rectangle_class& bounds, unsigned int seed, sxsdk::rendering_context_interface *rendering_context = 0)
{
	boost::scoped_array<sxsdk::vec4> lineBuffer(new sxsdk::vec4[bounds.max.x-bounds.min.x]);

	//initializing random seed
	srand(seed);
	const float rand_max1 = 1.0f/(RAND_MAX + 1.0f);	// RAND_MAX = 32767 (Macintosh) , 2147483647 (Linux)

	int weight_add = 1;
	int weight_mul = 0;
	if(settings.apply_to == add_noise_stream::Background_only) {
		weight_add = 1;
		weight_mul = -1;
	}
	else if(settings.apply_to == add_noise_stream::Foreground_only)	{
		weight_add = 0;
		weight_mul = 1;
	}

	if(settings.z_depth_effect <= 0.0f || !image->has_z()) {
		for (int line = bounds.min.y ; line < bounds.max.y ; ++line) {
			image->get_pixels_rgba_float(bounds.min.x, line, (bounds.max.x-bounds.min.x), 1, (sxsdk::rgba_class*)lineBuffer.get()); 
			for (int i = 0; i < (bounds.max.x-bounds.min.x) ; ++i) {
				const float weight = settings.opacity * (weight_add + weight_mul * lineBuffer[i].w);

				if(weight <= 0.0f) continue;
				const float value = rand() * rand_max1;
				const sxsdk::vec4 noise_rand = settings.color_noise ? sxsdk::vec4(rand(), rand(), rand(), rand()) * rand_max1 : sxsdk::vec4(value, value, value, value);
				lineBuffer[i] = lineBuffer[i] * (1.0f - weight) + sxsdk::mul(noise_rand, settings.color) * weight;
			}
			image->set_pixels_rgba_float(bounds.min.x, line, (bounds.max.x-bounds.min.x), 1, (sxsdk::rgba_class*)lineBuffer.get()); 
			if (rendering_context) rendering_context->yield();
		}
	}
	else {
		boost::scoped_array<float> zlineBuffer(new float[(bounds.max.x-bounds.min.x)]);
		const std::pair<float, float> z_range = get_zdepth_range(image, bounds, zlineBuffer.get());
		const float extract_ratio = 1.0f / (z_range.second - z_range.first);

		const int revert_z_mul = ((settings.revert_z_depth) ? -1 : 1);
		const int revert_z_add = ((settings.revert_z_depth) ? 1 : 0);

		for (int line = bounds.min.y ; line < bounds.max.y ; ++line) {
			image->get_pixels_rgba_float(bounds.min.x, line, (bounds.max.x-bounds.min.x), 1, (sxsdk::rgba_class*)lineBuffer.get()); 
			image->get_zs(bounds.min.x, line, (bounds.max.x-bounds.min.x), 1, zlineBuffer.get());

			for (int i = 0; i < (bounds.max.x-bounds.min.x) ; ++i) {
				float weight = settings.opacity * (weight_add + weight_mul * lineBuffer[i].w);
				const float z = (zlineBuffer[i] - z_range.first) * extract_ratio;
				weight *= 1.0f - (revert_z_add + revert_z_mul * z) * settings.z_depth_effect;

				if(weight <= 0.0f) continue;

				const float value = rand() * rand_max1;
				const sxsdk::vec4 noise_rand = settings.color_noise ? sxsdk::vec4(rand(), rand(), rand(), rand()) * rand_max1 : sxsdk::vec4(value, value, value, value);
				lineBuffer[i] = lineBuffer[i] * (1.0f - weight) + sxsdk::mul(noise_rand, settings.color) * weight;
			}
			image->set_pixels_rgba_float(bounds.min.x, line, (bounds.max.x-bounds.min.x), 1, (sxsdk::rgba_class*)lineBuffer.get()); 
			if (rendering_context) rendering_context->yield();
		}
	}
}

///ja @breif 「ノイズを追加する」のダイアログボックスを扱うクラス 
///ja
///ja ダイアログボックスのインターフェイスを add_noise_dialog_id.sxul で定義しています。 
///ja 各言語のSXULファイルは、Shadeの表示言語に合わせ自動で選択されます。 
///ja
///ja sxsdk::dialog_interface::set_responder() には、通常、プラグイン自身のinterfaceを渡しますが、 
///ja sxsdk::plugin_interface から派生したクラスであればコールバックを受けることが可能です。 
///ja これはダイアログボックスに関する操作を一つのクラスに纏めたい場合に有効です。
///ja 
///ja この場合、sxsdk::dialog_interfaceが破棄されるときに参照カウンタが減るので、 
///ja set_responder() と合わせて、必ず AddRef() を呼び、オブジェクトが破棄されないようにします。 
///ja
struct add_noise_dialog : sxsdk::plugin_interface
{
	enum {
		sImage_size_id				= 101,
		wPreview_image_id			= 111,
		pPreview_mode_id			= 121, bRedraw_preview_id, 
		bAuto_preview_id			= 131,

		noise_Opacity_group_id		= 200, fOpacity_id, bColor_noise_id,
		noise_Color_group_id		= 210, fOpacity_R_id, fOpacity_G_id, fOpacity_B_id, fOpacity_A_id,
		noise_Apply_group_id		= 220, pApply_to_id,
		noise_Z_depth_group_id		= 230, fZ_Effect_id, bRevert_Z_id,
		noise_animation_group_id	= 240, bAnimated_noise_id
	};

	enum preview_mode_types {
		Before_effect, After_effect, Preview_spacer, Alpha_channel, Z_buffer
	};

	// sxsdk::plugin_interface から派生したクラスは必ず get_shade_version() を定義する。 
	virtual int get_shade_version () const { return SHADE_BUILD_NUMBER; }

	///ja @breif プレビューを取り扱うクラス 
	///ja 
	///ja SXULにcustomで定義したアイテムには、sxsdk::window_interfaceを貼り付けることが可能です。 
	///ja プレビューやグラフなど、イメージをダイアログボックスで使用することができます。 
	struct preview_window : sxsdk::window_interface
	{
		preview_window (sxsdk::shade_interface &shade, sxsdk::dialog_interface &dialog, sxsdk::dialog_item_class &item, sxsdk::image_interface* image, const add_noise_stream& settings) : stored_image(image), preview_image(NULL), dirty(false), settings(settings), sxsdk::window_interface(shade, dialog, item, 0) {}

		virtual int get_shade_version () const { return SHADE_BUILD_NUMBER; }

		virtual void resize (int x, int y, bool remake, void *aux);
		virtual void paint (sxsdk::graphic_context_interface &gc, const sx::rectangle_class *visible_rectangle, void *);

		bool update_preview_image (int preview_mode);
	private:
		bool dirty;

		compointer<sxsdk::image_interface> preview_image;
		sxsdk::image_interface* stored_image;
		const add_noise_stream& settings;
	};

	add_noise_dialog (sxsdk::shade_interface& shade) : stored_image(NULL), preview_mode(After_effect), auto_preview(true), shade(shade)	{}

	bool ask (sxsdk::stream_interface* stream, sxsdk::image_interface* src_image, bool full_mode);

	virtual void initialize_dialog (sxsdk::dialog_interface &dialog, void *aux=0);
	virtual void load_dialog_data (sxsdk::dialog_interface &d, void * = 0);
	virtual void save_dialog_data (sxsdk::dialog_interface &d, void * = 0);
	virtual bool respond (sxsdk::dialog_interface &d, sxsdk::dialog_item_class &item, int action, void * = 0);

	add_noise_stream settings;

private:
	sxsdk::shade_interface& shade;

	compointer<preview_window> noise_preview;
	sxsdk::image_interface* stored_image;

	bool full_settings;

	int preview_mode;
	bool auto_preview;
};

void add_noise_dialog::preview_window::resize (int x, int y, bool remake, void *aux)
{
	sxsdk::window_interface::resize(x, y, remake, aux);
	sxsdk::window_interface::paint();
}

void add_noise_dialog::preview_window::paint (sxsdk::graphic_context_interface &gc, const sx::rectangle_class *visible_rectangle, void *) {
	if(!preview_image)	return;

	if(dirty) {
		const sx::vec<int,2> size(preview_image->get_size());
		boost::scoped_array<sxsdk::rgb_class> buffer_rgb(new sxsdk::rgb_class[size.x * size.y]);		// cut alpha channel.
		preview_image->get_real_colors(0, 0, size.x, size.y, buffer_rgb.get());
		preview_image->set_real_colors(0, 0, size.x, size.y, buffer_rgb.get());
		dirty = false;
	}

	// 画像の縦横比に合わせて、余白部分を塗りつぶし 
	const sx::vec<int,2> preview_size = stored_image->uniscale_rectangle(get_client_rectangle(false), sx::rectangle_class(sx::vec<int,2>(0,0), stored_image->get_size())).size();
	const sx::vec<int,2> background_size = get_client_rectangle(false).size();
	sx::rectangle_class draw_rectangle(sx::vec<int, 2>(0, 0), preview_size);
	const sx::vec<int,2> margin = (background_size - draw_rectangle.size()) / 2;
//	draw_rectangle.offset(margin);
	draw_rectangle += margin;

	gc.set_color(sxsdk::rgb_class(0.2f, 0.2f, 0.2f));
	if(margin.x > 0) {
		sx::rectangle_class mask({ 0, 0 }, { margin.x, background_size.y });
		gc.paint_rectangle(mask);
		gc.paint_rectangle(mask + sx::vec<int,2>(draw_rectangle.max.x, 0));
	}
	if(margin.y > 0) {
		sx::rectangle_class mask({ 0, 0 }, { background_size.x, margin.y });
		gc.paint_rectangle(mask);
		gc.paint_rectangle(mask + sx::vec<int,2>(0, draw_rectangle.max.y));
	}
	gc.restore_color(); // set_color() に対応して必ず呼び出す 

	// プレビューイメージを描画 
	gc.draw_image(preview_image, draw_rectangle);
}

bool add_noise_dialog::preview_window::update_preview_image (int preview_mode)
{
	if(!stored_image)	return false;

	dirty = true;
	preview_image = stored_image->create_duplicate_image_interface();	// reflesh preview_image.

	bool result = true;
	switch(preview_mode)
	{
	case After_effect:
		draw_noise(preview_image, settings, preview_image->get_bounds(), sxsdk::absolute(settings.color));
	// TODO Check fallthrough or break.
	fallthrough; case Before_effect:
		break;
	case Alpha_channel:
		extract_alpha(preview_image, preview_image->get_bounds());
		break;
	case Z_buffer:
		if ( !(preview_image->has_z()) ) {
			shade.show_message_box(shade.gettext("error_no_Zdepth_data"), false);
			result = false;
			break;
		}
		extract_zdepth(preview_image, preview_image->get_bounds());
		break;
	default:
		result = false;
	};

	sxsdk::window_interface::paint();
	return result;
}


bool add_noise_dialog::ask (sxsdk::stream_interface* stream, sxsdk::image_interface* src_image, bool full_mode)
{
	settings.read_stream(stream);
	full_settings = full_mode;
	stored_image = src_image;

	compointer<sxsdk::dialog_interface> dialog(stream->create_dialog_interface_with_uuid(add_noize_dialog_id));
	dialog->set_resource_name("add_noise_dialog_id");
	dialog->set_responder(this);
	this->AddRef();	// set_responder()に合わせて、参照カウンタを増やす。 

	if(!dialog->ask())	return false;

	return settings.write_stream(stream);
}

void add_noise_dialog::initialize_dialog (sxsdk::dialog_interface &dialog, void *aux)
{
	// custom アイテムにプレビュー画像用のsxsdk::window_interfaceを設定 
	sxsdk::dialog_item_class& item = dialog.get_dialog_item(wPreview_image_id);
	noise_preview = new preview_window(shade, dialog, item, stored_image, settings);
	noise_preview->AddRef();
	item.set_window(noise_preview);

	noise_preview->update_preview_image(preview_mode);
}

void add_noise_dialog::load_dialog_data (sxsdk::dialog_interface &d, void *)
{
	bool has_z = stored_image->has_z() || full_settings;

	d.get_dialog_item(sImage_size_id).set_string(boost::str(boost::format("%1% * %2%") % stored_image->get_size().x % stored_image->get_size().y).c_str());

	d.get_dialog_item(fOpacity_id).set_range(sxsdk::vec2(0.0f, 1.0f)).set_float(settings.opacity);
	d.get_dialog_item(bColor_noise_id).set_bool(!settings.color_noise);

	d.get_dialog_item(fOpacity_R_id).set_range(sxsdk::vec2(0.0f, 1.0f)).set_float(settings.color.x);
	d.get_dialog_item(fOpacity_G_id).set_range(sxsdk::vec2(0.0f, 1.0f)).set_float(settings.color.y);
	d.get_dialog_item(fOpacity_B_id).set_range(sxsdk::vec2(0.0f, 1.0f)).set_float(settings.color.z);
	d.get_dialog_item(fOpacity_A_id).set_range(sxsdk::vec2(0.0f, 1.0f)).set_float(settings.color.w);

	d.get_dialog_item(pApply_to_id).set_selection(settings.apply_to);
	d.get_dialog_item(fZ_Effect_id).set_enabled(has_z).set_range(sxsdk::vec2(0.0f, 1.0f)).set_float(settings.z_depth_effect);
	d.get_dialog_item(bRevert_Z_id).set_enabled(has_z).set_bool(settings.revert_z_depth);
	d.get_dialog_item(bAnimated_noise_id).set_enabled(full_settings).set_bool(settings.animated_noise);

	d.get_dialog_item(pPreview_mode_id).set_selection(preview_mode);
	d.get_dialog_item(bAuto_preview_id).set_bool(auto_preview);
}

void add_noise_dialog::save_dialog_data (sxsdk::dialog_interface &d, void *)
{
	settings.opacity	= d.get_dialog_item(fOpacity_id).get_float();
	settings.color_noise	= !(d.get_dialog_item(bColor_noise_id).get_bool());

	settings.color.x		= d.get_dialog_item(fOpacity_R_id).get_float();
	settings.color.y		= d.get_dialog_item(fOpacity_G_id).get_float();
	settings.color.z		= d.get_dialog_item(fOpacity_B_id).get_float();
	settings.color.w		= d.get_dialog_item(fOpacity_A_id).get_float();

	settings.apply_to		= d.get_dialog_item(pApply_to_id).get_selection();
	settings.z_depth_effect	= d.get_dialog_item(fZ_Effect_id).get_float();
	settings.revert_z_depth	= d.get_dialog_item(bRevert_Z_id).get_bool();
	settings.animated_noise	= d.get_dialog_item(bAnimated_noise_id).get_bool();

	preview_mode	= d.get_dialog_item(pPreview_mode_id).get_selection();
	auto_preview	= d.get_dialog_item(bAuto_preview_id).get_bool();
}

bool add_noise_dialog::respond (sxsdk::dialog_interface &d, sxsdk::dialog_item_class &item, int action, void *)
{
	if(action == sx::dialog_item::changing_action) return false;

	bool update = auto_preview;

	switch(item.get_id())
	{
	case bAuto_preview_id:
		if(!item.get_bool()) {
			update = false;
			break;
		}
	// TODO Check fallthrough or break.
	fallthrough; case bRedraw_preview_id:
		d.get_dialog_item(pPreview_mode_id).set_selection(After_effect);
	// TODO Check fallthrough or break.
	fallthrough; case pPreview_mode_id:
		update = true;
		break;
	case fOpacity_id:
	case fOpacity_A_id:
		if(item.get_float() > 1.0f)	item.set_float(1.0f);
	// TODO Check fallthrough or break.
	fallthrough; case fZ_Effect_id:
	case fOpacity_R_id:
	case fOpacity_G_id:
	case fOpacity_B_id:
		if(item.get_float() < 0.0f)	item.set_float(0.0f);
	// TODO Check fallthrough or break.
	fallthrough; default:
		if(auto_preview)	d.get_dialog_item(pPreview_mode_id).set_selection(After_effect);
	}

	save_dialog_data(d);
	if((update || auto_preview) && noise_preview) {
		try {
			if (!noise_preview->update_preview_image(preview_mode)) {
				preview_mode = Before_effect;
			}
		} catch (...) {
			shade.show_message_box(shade.gettext("error_out_of_memory"), false);
			preview_mode = Before_effect;
		}
		d.get_dialog_item(pPreview_mode_id).set_selection(preview_mode);
	}

	return false;
}

///ja @breif 「ノイズを追加する」のエフェクタプラグインインターフェイス
///ja
void add_noise_effector::ask (sxsdk::stream_interface *stream, void *) {
	compointer<sxsdk::scene_interface> scene(stream->get_scene_interface());
	compointer<sxsdk::rendering_interface> render(scene->get_rendering_interface());
	compointer<sxsdk::image_interface> image(render->get_image_interface());
	if(!image || image->is_fake())	image = scene->create_image_interface("add_noise_sample");

	add_noise_dialog dialog(shade);
	dialog.ask(stream, image, true);
}

void add_noise_effector::begin (sxsdk::rendering_context_interface *rendering_context, sxsdk::stream_interface *stream, void *) {
	this->rendering_context = rendering_context;
	this->stream = stream;
}

void add_noise_effector::do_post_effect (void *)
{
	add_noise_stream datas;
	datas.read_stream(stream);

	sxsdk::rendering_context_interface &rc = *rendering_context;
	compointer<sxsdk::image_interface> image(rc.get_image_interface());
	sx::rectangle_class bounds(sx::vec<int, 2>(0, 0), image->get_size());
	if(rc.is_partial_rendering()) {
//		bounds.offset(image->get_offset());
		bounds += image->get_offset();
	}
	unsigned int random_seed = (unsigned int)(sxsdk::absolute(datas.color) + (datas.animated_noise ? rc.get_current_frame() : 0));

	try {
		draw_noise(image, datas, bounds, random_seed, rendering_context);
	} catch (...) {
		boost::scoped_array<char> title( strdup(name(image)) );
		image->message( boost::str(boost::format(image->gettext("message_out_of_memory")) % title.get()).c_str() );
		return;
	}
}

///ja @breif 「ノイズを追加する」のレンダリングイメージプラグインインターフェイス
///ja
bool add_noise_rip::do_rendering_image (sxsdk::scene_interface *scene, void *)
{
	compointer<sxsdk::rendering_interface> render(scene->get_rendering_interface());
	compointer<sxsdk::image_interface> image(render->get_image_interface());
	if (!image || !(image->has_image())) {
		scene->show_message_box(scene->gettext("error_no_rendered_image"), false);
		return false;
	}

	add_noise_dialog dialog(shade);
	compointer<sxsdk::preference_interface> preference(scene->get_preference_interface());
	compointer<sxsdk::stream_interface> stream(preference->get_attribute_stream_interface_with_uuid(uuid()));
	if(!stream)	stream = preference->create_attribute_stream_interface_with_uuid(uuid());
	if(!dialog.ask(stream, image, false))	return false;

	const sx::rectangle_class bounds(sx::vec<int, 2>(0, 0), image->get_size());
	unsigned int random_seed = (unsigned int)sxsdk::absolute(dialog.settings.color);

	try {
		draw_noise(image, dialog.settings, bounds, random_seed);
	} catch (...) {
		image->show_message_box(image->gettext("error_out_of_memory"), false);
		return false;
	}

	render->update_image();		
	return true;
}

///ja @breif 「ノイズを追加する」のイメージボックスプラグインインターフェイス
///ja
bool add_noise_ibp::do_image_box (sxsdk::image_interface* image, void* aux)
{
	if (!image || !(image->has_image())) {
		image->show_message_box(image->gettext("error_no_rendered_image"), false);
		return false;
	}

	add_noise_dialog dialog(shade);
	compointer<sxsdk::preference_interface> preference(image->get_preference_interface());
	compointer<sxsdk::stream_interface> stream(preference->get_attribute_stream_interface_with_uuid(uuid()));
	if(!stream)	stream = preference->create_attribute_stream_interface_with_uuid(uuid());
	if(!dialog.ask(stream, image, false))	return false;

	const sx::rectangle_class bounds(sx::vec<int, 2>(0, 0), image->get_size());
	unsigned int random_seed = (unsigned int)sxsdk::absolute(dialog.settings.color);

	try {
		draw_noise(image, dialog.settings, bounds, random_seed);
	} catch (...) {
		image->show_message_box(image->gettext("error_out_of_memory"), false);
		return false;
	}

	image->pump();
	return true;
}


