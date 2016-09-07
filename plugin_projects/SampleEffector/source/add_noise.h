//================================================================================================================
//	ノイズを加える エフェクタ 
//	
//    Copyright(C) 2015 Shade3D Co.,Ltd.
//================================================================================================================

#pragma once
#ifndef add_noise_H
#define add_noise_H

#include "SampleEffectorGlobals.h"
#include "SampleEffector.h"

struct add_noise_effector : sxsdk::effector_interface {
	static const sx::uuid_class uuid()							{return add_noize_effector_id;}
	static const char* name( sxsdk::shade_interface *shade )	{return shade->gettext("add_noise_title");}

	add_noise_effector(sxsdk::shade_interface& shade) : shade(shade) {}

	virtual int get_shade_version () const		{ return SHADE_BUILD_NUMBER; }
	virtual sx::uuid_class get_uuid (void *)	{ return uuid(); }

	virtual void ask (sxsdk::stream_interface *stream, void * = 0);

	virtual void begin (sxsdk::rendering_context_interface *rendering_context, sxsdk::stream_interface *stream, void * = 0);
	virtual void do_post_effect (void * = 0);

	virtual bool can_ask(void * = 0) { return true; }
private:
	sxsdk::shade_interface& shade;

	sxsdk::rendering_context_interface *rendering_context;
	sxsdk::stream_interface *stream;
};

struct add_noise_rip : sxsdk::rendering_image_interface {
	static const sx::uuid_class uuid()							{return add_noise_rip_id;}
	static const char* name( sxsdk::shade_interface *shade )	{return shade->gettext("add_noise_title");}

	add_noise_rip(sxsdk::shade_interface& shade) : shade(shade) {}

	virtual int get_shade_version () const		{ return SHADE_BUILD_NUMBER; }
	virtual sx::uuid_class get_uuid (void *)	{ return uuid(); }

	virtual bool do_rendering_image (sxsdk::scene_interface *scene, void * = 0);
private:
	sxsdk::shade_interface& shade;
};

struct add_noise_ibp : sxsdk::image_box_interface { 
	static const sx::uuid_class uuid()					{return add_noise_ibp_id;}
	static const char* name( sxsdk::shade_interface *shade )	{return shade->gettext("add_noise_title");}

	add_noise_ibp(sxsdk::shade_interface& shade) : shade(shade) {}

	virtual int get_shade_version () const		{ return SHADE_BUILD_NUMBER; }
	virtual sx::uuid_class get_uuid (void *)	{ return uuid(); }

	virtual bool do_image_box (sxsdk::image_interface* image, void* aux = 0);
private:
	sxsdk::shade_interface& shade;
};

#endif