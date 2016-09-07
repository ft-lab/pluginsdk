//================================================================================================================
//	Preview dialog class
//
//	Copyright 2015 Shade3D Co.,Ltd.
//================================================================================================================

#include "sxsdk.cxx"
#include "previewimagedialog.h"
#include "boost/scoped_array.hpp"

void preview_image_dialog::preview_window::resize (int x, int y, bool remake, void *aux)
{
	sxsdk::window_interface::resize(x, y, remake, aux);
	sxsdk::window_interface::paint();
}

void preview_image_dialog::preview_window::initialize (void *aux) {
	(new sxsdk::window_interface::image_box_class(*this, 100000, 0, 0, 256, 256))->set_image(*stored_image);
}


void preview_image_dialog::preview_window::paint (sxsdk::graphic_context_interface &gc, const sx::rectangle_class *visible_rectangle, void *)
{
	if(!stored_image)	return;

	// 画像の縦横比に合わせて、余白部分を塗りつぶし 
	const sx::vec<int, 2> preview_size = stored_image->uniscale_rectangle(get_client_rectangle(false), sx::rectangle_class(sx::vec<int, 2>(0, 0), stored_image->get_size())).size();

	sx::rectangle_class draw_rectangle(sx::vec<int, 2>(0, 0), preview_size);
	const sx::vec<int,2> background_size = get_client_rectangle(false).size();
	const sx::vec<int,2> margin = (background_size - draw_rectangle.size()) / 2;
	draw_rectangle += margin;

	gc.set_color(sxsdk::rgb_class(0.2f, 0.2f, 0.2f));
	if(margin.x > 0) {
		sx::rectangle_class belt({ 0, 0 }, { margin.x, background_size.y });
		gc.paint_rectangle(belt);
		gc.paint_rectangle(belt + sx::vec<int,2>(draw_rectangle.max.x, 0));
	}
	if(margin.y > 0) {
		sx::rectangle_class belt({ 0, 0 }, { background_size.x, margin.y });
		gc.paint_rectangle(belt);
		gc.paint_rectangle(belt + sx::vec<int,2>(0, draw_rectangle.max.y));
	}
	gc.restore_color(); // set_color() に対応して必ず呼び出す 

	// プレビューイメージを描画 
	gc.draw_image(stored_image, draw_rectangle);
}

void preview_image_dialog::preview_window::setup_preview_image ()
{
	if(!stored_image)	return;

	// cut alpha channel.
	const sx::vec<int,2> size(stored_image->get_size());
	boost::scoped_array<sxsdk::rgb_class> buffer_rgb(new sxsdk::rgb_class[size.x * size.y]);
	stored_image->get_real_colors(0, 0, size.x, size.y, buffer_rgb.get());
	stored_image->set_real_colors(0, 0, size.x, size.y, buffer_rgb.get());
}


bool preview_image_dialog::ask (sxsdk::image_interface* src_image)
{
	stored_image = src_image;

	compointer<sxsdk::dialog_interface> dialog(shade.create_dialog_interface_with_uuid());
	dialog->set_resource_name("preview_image_dialog_id");
	dialog->set_responder(this);
	this->AddRef();	// set_responder()に合わせて、参照カウンタを増やす。 

	if(!dialog->ask())	return false;

	return true;
}

void preview_image_dialog::initialize_dialog (sxsdk::dialog_interface &dialog, void *aux)
{
	sxsdk::dialog_item_class& item = dialog.get_dialog_item(100);
	preview = new preview_window(shade, dialog, item, stored_image);
	item.set_window(preview);

	preview->setup_preview_image();
	((sxsdk::window_interface*)preview)->paint();
}
