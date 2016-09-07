//================================================================================================================
//	単純なエフェクタの基底クラス 
//	
//    Copyright(C) 2015 Shade3D Co.,Ltd.
//================================================================================================================

#pragma once
#ifndef sample_effector_base_H
#define sample_effector_base_H

#include "sxsdk/sxsdk.h"

struct sample_effector_base_interface : sxsdk::effector_interface {
	virtual int get_shade_version () const		{ return SHADE_BUILD_NUMBER; }

	virtual void end () { }
	virtual void do_pre_effect (void * = 0) { }
	virtual void ask (sxsdk::stream_interface *stream, void * = 0) { }

	virtual bool needs_z (void * = 0) { return false; }

	virtual void begin (sxsdk::rendering_context_interface *rendering_context, sxsdk::stream_interface *stream, void * = 0);
	virtual void do_post_effect (void * = 0);
private:
	virtual const char* plugin_name_imp (sxsdk::shade_interface *shade) {return "";}
	virtual void do_post_effect_imp (sxsdk::image_interface* image, const sx::rectangle_class& bounds) {}

	sxsdk::rendering_context_interface *rendering_context;
	sxsdk::stream_interface *stream;
};

struct sample_rip_base_interface : sxsdk::rendering_image_interface {
	virtual int get_shade_version () const		{ return SHADE_BUILD_NUMBER; }

	virtual bool do_rendering_image (sxsdk::scene_interface *scene, void * = 0);
private:
	virtual const char* plugin_name_imp (sxsdk::shade_interface *shade) {return "";}
	virtual bool check_image_imp (sxsdk::image_interface* image, sxsdk::scene_interface* scene)	{return true;}
	virtual void do_rendering_image_imp (sxsdk::image_interface* image, const sx::rectangle_class& bounds) {}
};

struct sample_ibp_base_interface : sxsdk::image_box_interface { 
	virtual int get_shade_version () const		{ return SHADE_BUILD_NUMBER; }

	virtual bool do_image_box (sxsdk::image_interface* image, void* aux = 0);
private:
	virtual const char* plugin_name_imp (sxsdk::shade_interface *shade) {return "";}
	virtual bool check_image_imp (sxsdk::image_interface* image)	{return true;}
	virtual void do_image_box_imp (sxsdk::image_interface* image, const sx::rectangle_class& bounds) {}
};

#endif