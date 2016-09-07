//================================================================================================================
//	プレビューダイアログボックスクラス 
//	
//    Copyright(C) 2015 Shade3D Co.,Ltd.
//================================================================================================================

#pragma once
#ifndef preview_image_dialog_H
#define preview_image_dialog_H

#include "SampleEffectorGlobals.h"

///ja @breif プレビュー画像を表示するダイアログボックスを扱うクラス 
///ja
///ja ダイアログボックスのインターフェイスを preview_image_dialog_id.sxul で定義しています。 
///ja 各言語のSXULファイルは、Shadeの表示言語に合わせ自動で選択されます。 
///ja
///ja sxsdk::dialog_interface::set_responder() には、通常、プラグイン自身のinterfaceを渡しますが、 
///ja sxsdk::plugin_interface から派生したクラスであればコールバックを受けることが可能です。 
///ja これはダイアログボックスに関する操作を一つのクラスに纏めたい場合に有効です。
///ja 
///ja この場合、sxsdk::dialog_interfaceが破棄されるときに参照カウンタが減るので、 
///ja set_responder() と合わせて、必ず AddRef() を呼び、オブジェクトが破棄されないようにします。 
///ja
struct preview_image_dialog : sxsdk::plugin_interface
{
	virtual int get_shade_version () const { return SHADE_BUILD_NUMBER; }

	struct preview_window : sxsdk::window_interface
	{
		preview_window (sxsdk::shade_interface &shade, sxsdk::dialog_interface &dialog, sxsdk::dialog_item_class &item, sxsdk::image_interface* image) : stored_image(image), sxsdk::window_interface(shade, dialog, item, 0)	{}

		virtual int get_shade_version () const { return SHADE_BUILD_NUMBER; }

		virtual void resize (int x, int y, bool remake, void *aux);
		virtual void initialize (void *aux=0);
		virtual void paint (sxsdk::graphic_context_interface &gc, const sx::rectangle_class *visible_rectangle, void *);
		void setup_preview_image ();

	private:
		sxsdk::image_interface* stored_image;
	};

	preview_image_dialog (sxsdk::shade_interface& shade) : stored_image(NULL), shade(shade)	{}

	bool ask (sxsdk::image_interface* src_image);

	virtual void initialize_dialog (sxsdk::dialog_interface &dialog, void *aux=0);
	//virtual bool respond (sxsdk::dialog_interface &dialog, sxsdk::dialog_item_class &item, int action, void *aux);

private:
	sxsdk::shade_interface& shade;

	preview_window* preview;
	sxsdk::image_interface* stored_image;
};

#endif
