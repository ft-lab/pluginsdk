//================================================================================================================
//	Image flipping function
//
//	Copyright 2015 Shade3D Co.,Ltd.
//================================================================================================================

#include "sxsdk.cxx"
#include "sample_effector_base.h"
#include "previewimagedialog.h"
#include "boost/scoped_array.hpp"

void sample_effector_base_interface::begin (sxsdk::rendering_context_interface *rendering_context, sxsdk::stream_interface *stream, void *) {
	this->rendering_context = rendering_context;
	this->stream = stream;
}

void sample_effector_base_interface::do_post_effect (void *) {
	sxsdk::rendering_context_interface &rc = *rendering_context;
	compointer<sxsdk::image_interface> img(rc.get_image_interface());
	
	sx::rectangle_class bounds(sx::vec<int, 2>(0, 0), img->get_size());
	if(rc.is_partial_rendering()) {
//		bounds.offset(img->get_offset());
		bounds += img->get_offset();
	}
	try {
		do_post_effect_imp(img, bounds);
	} catch (...) {
		boost::scoped_array<char> title( strdup(plugin_name_imp(img)) );
		img->message( boost::str(boost::format(img->gettext("message_out_of_memory")) % title.get()).c_str() );
		return;
	}
}

bool sample_rip_base_interface::do_rendering_image (sxsdk::scene_interface *scene, void *) {
	compointer<sxsdk::rendering_interface> render(scene->get_rendering_interface());
	compointer<sxsdk::image_interface> img(render->get_image_interface());
	if (!img || !(img->has_image())) {
		scene->show_message_box(scene->gettext("error_no_rendered_image"), false);
		return false;
	}

	if (!check_image_imp(img, scene)) return false;

	const sx::rectangle_class bounds(sx::vec<int, 2>(0, 0), img->get_size());

	compointer<sxsdk::image_interface> prevew_img(img->create_duplicate_image_interface());
	try {
		do_rendering_image_imp(prevew_img, bounds);

		preview_image_dialog dialog(*scene);
		if(!dialog.ask(prevew_img)) return false;

		do_rendering_image_imp(img, bounds);
	} catch (...) {
		scene->show_message_box(scene->gettext("error_out_of_memory"), false);
		return false;
	}

	render->update_image();
	return true;
}

bool sample_ibp_base_interface::do_image_box (sxsdk::image_interface* image, void* aux)
{
	if (!image || !(image->has_image())) {
		image->show_message_box(image->gettext("error_no_rendered_image"), false);
		return false;
	}

	if (!check_image_imp(image)) return false;

	const sx::rectangle_class bounds(sx::vec<int, 2>(0, 0), image->get_size());

	compointer<sxsdk::image_interface> prevew_img(image->create_duplicate_image_interface());
	try {
		do_image_box_imp(prevew_img, bounds);

		preview_image_dialog dialog(*image);
		if(!dialog.ask(prevew_img)) return false;

		do_image_box_imp(image, bounds);
	} catch (...) {
		image->show_message_box(image->gettext("error_out_of_memory"), false);
		return false;
	}

	image->pump();
	return true;
}