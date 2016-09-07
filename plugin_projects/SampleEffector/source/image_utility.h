//================================================================================================================
//	イメージ編集実装部分 
//	
//    Copyright(C) 2015 Shade3D Co.,Ltd.
//================================================================================================================

#pragma once
#ifndef image_utility_H
#define image_utility_H

#include <utility>
#include "sxsdk/sxsdk.h"

std::pair<float, float> get_zdepth_range (sxsdk::image_interface* image, const sx::rectangle_class& bounds, float* buffer);
void extract_alpha (sxsdk::image_interface* image, const sx::rectangle_class& bounds);
void extract_zdepth (sxsdk::image_interface* image, const sx::rectangle_class& bounds);
void invert_image (sxsdk::image_interface* image, const sx::rectangle_class& bounds);

#endif