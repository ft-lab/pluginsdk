//================================================================================================================
//	階調の反転 エフェクタ 
//	
//    Copyright(C) 2015 Shade3D Co.,Ltd.
//================================================================================================================

#pragma once
#ifndef invert_image_H
#define invert_image_H

#include "sample_effector_base.h"
#include "SampleEffector.h"
#include "image_utility.h"

struct invert_image_effector : sample_effector_base_interface {
	static const sx::uuid_class uuid()					{return invert_image_effector_id;}
	static const char* name( sxsdk::shade_interface *shade )	{return shade->gettext("invert_image_title");}
	virtual sx::uuid_class get_uuid (void *)			{ return uuid(); }

	virtual bool needs_z (void * = 0) { return false; }
private:
	virtual const char* plugin_name_imp (sxsdk::shade_interface *shade) {return name(shade);}
	virtual void do_post_effect_imp (sxsdk::image_interface* image, const sx::rectangle_class& bounds) {invert_image(image, bounds);}
};

struct invert_image_rip : sample_rip_base_interface {
	static const sx::uuid_class uuid()					{return invert_image_rip_id;}
	static const char* name( sxsdk::shade_interface *shade )	{return shade->gettext("invert_image_title");}
	virtual sx::uuid_class get_uuid (void *)			{ return uuid(); }
private:
	virtual const char* plugin_name_imp (sxsdk::shade_interface *shade) {return name(shade);}
	virtual void do_rendering_image_imp (sxsdk::image_interface* image, const sx::rectangle_class& bounds) {invert_image(image, bounds);}
};

struct invert_image_ibp : sample_ibp_base_interface { 
	static const sx::uuid_class uuid()					{return invert_image_ibp_id;}
	static const char* name( sxsdk::shade_interface *shade )	{return shade->gettext("invert_image_title");}
	virtual sx::uuid_class get_uuid (void *)			{ return uuid(); }
private:
	virtual const char* plugin_name_imp (sxsdk::shade_interface *shade) {return name(shade);}
	virtual void do_image_box_imp (sxsdk::image_interface* image, const sx::rectangle_class& bounds) {invert_image(image, bounds);}
};

#endif