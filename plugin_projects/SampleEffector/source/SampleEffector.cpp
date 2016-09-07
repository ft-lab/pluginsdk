//================================================================================================================
//	Sample Effector plugin
//
//	Copyright 2015 Shade3D Co.,Ltd.
//================================================================================================================

#include "sxsdk.cxx"

#include "SampleEffectorGlobals.h"
#include "SampleEffector.h"
#include "invert_image.h"
#include "add_noise.h"

//========================================================
/// Global Entry functions - they must declared in plugin
//========================================================

// Establish the plugin interface between Shade and the plugin. Simply define what interface this plugin going to be use.
// Creates a new plugin interface object.
// インターフェイスの構築を行う。
//
extern "C" SXSDKEXPORT void STDCALL create_interface (const IID &iid, int i, void **p, sxsdk::shade_interface *shade, void *) {
	unknown_interface *u = 0;
	if (iid == effector_iid) {
		switch (i) {
			case 0:	u = new add_noise_effector(*shade); break;
			case 1: u = new invert_image_effector;		break;
		}
	}
	else if (iid == rendering_image_iid) {
		switch (i) {
			case 0:	u = new add_noise_rip(*shade);	break;
			case 1: u = new invert_image_rip;		break;
		}
	}
	else if (iid == image_box_iid) {
		switch (i) {
			case 0:	u = new add_noise_ibp(*shade);	break;
			case 1: u = new invert_image_ibp;		break;
		}
	}

	if (u) {
		u->AddRef();
		*p = (void *)u;
	}
}

// Confirm the interface and returns the number of interface that you are going to be use.
// Returns the number of plugin interface objects.
// 使用するインターフェイスの数を返す。
//
extern "C" SXSDKEXPORT int STDCALL has_interface (const IID &iid, sxsdk::shade_interface *shade) {
	if (shade->get_version() < SHADE_BUILD_NUMBER)	return 0;

	if (iid == effector_iid)				return 2; 
	else if (iid == rendering_image_iid)	return 2;
	else if (iid == image_box_iid)			return 2;

	return 0;
}

// Returns the name of this plugin. 
// プラグインの名前を返す。
//
extern "C" SXSDKEXPORT const char * STDCALL get_name (const IID &iid, int i, sxsdk::shade_interface *shade, void *) {
	if (iid == effector_iid) {
		switch(i) {
			case 0: return add_noise_effector::name(shade);
			case 1: return invert_image_effector::name(shade);
		}
	}
	else if (iid == rendering_image_iid) {
		switch(i) {
			case 0: return add_noise_rip::name(shade);
			case 1: return invert_image_rip::name(shade);
			//case 4: return opengl_sample_rip::name(shade);
		}
	}
	else if (iid == image_box_iid) {
		switch(i) {
			case 0: return add_noise_ibp::name(shade);
			case 1: return invert_image_ibp::name(shade);
		}
	}

	return 0;
}

// Should return the plugin interface version number.
// プラグインインターフェイスバージョンを設定する。\c interface_version を返す。
//
extern "C" SXSDKEXPORT int STDCALL get_interface_version (void *) {
	return interface_version;
}

// Returns the plugin unique uuid
// プラグインIDを設定する。
//
extern "C" SXSDKEXPORT sx::uuid_class STDCALL get_uuid (const IID &iid, int i, void *) {
	if (iid == effector_iid) {
		switch(i) {
			case 0: return add_noise_effector::uuid();
			case 1: return invert_image_effector::uuid();
		}
	}
	else if (iid == rendering_image_iid) {
		switch(i) {
			case 0: return add_noise_rip::uuid();
			case 1: return invert_image_rip::uuid();
		}
	}
	else if (iid == image_box_iid) {
		switch(i) {
			case 0: return add_noise_ibp::uuid();
			case 1: return invert_image_ibp::uuid();
		}
	}
	return sx::uuid_class();
}

// Should return true if the plugin is a resident plugin. Resident means the plugin will not unload itself until Shade is quit.
// 常駐させるかどうかを設定する。
//
extern "C" SXSDKEXPORT bool STDCALL is_resident (const IID &iid, int i, void *) {
	return false;
}

// Returns various plugin info.
// バージョン・デベロッパ情報を設定する。
//
// SDK version, simply use the defined SHADE_BUILD_NUMBER
// Recommend Shade version, any Shade version below this build number will not be load
// Major version number, Minor version number, Micro version number and finally with the build number of your plugin.
extern "C" SXSDKEXPORT void STDCALL get_info (sxsdk::shade_plugin_info &info, sxsdk::shade_interface *shade, void *) {
	info.sdk_version = SHADE_BUILD_NUMBER;
	info.recommended_shade_version = 410000;
	info.major_version = 1;
	info.minor_version = 0;
	info.micro_version = 0;
	info.build_number = 0;
}

// Should return true if the plugin interfaces do not depend on each other. 
// 各インターフェイスが独立して動作可能かどうかを設定する。
//
extern "C" SXSDKEXPORT bool STDCALL has_separate_interfaces ( void * ) {
	return true;
}

