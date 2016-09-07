//================================================================================================================
//	Image utility functions
//
//	Copyright 2015 Shade3D Co.,Ltd.
//================================================================================================================

#include "sxsdk.cxx"
#include "image_utility.h"
#include "boost/scoped_array.hpp"

void extract_alpha(sxsdk::image_interface* image, const sx::rectangle_class& bounds)
{
	boost::scoped_array<sxsdk::rgba_class> lineBuffer(new sxsdk::rgba_class[(bounds.max.x-bounds.min.x)]);

	for (int line = bounds.min.y ; line < bounds.max.y ; ++line) {
		image->get_pixels_rgba_float(bounds.min.x, line, (bounds.max.x-bounds.min.x), 1, lineBuffer.get()); 
		for (int i = 0; i < (bounds.max.x-bounds.min.x) ; ++i) {
			float  val = lineBuffer[i].alpha;
			lineBuffer[i] = sxsdk::rgba_class(val, val, val, val);
		}
		image->set_pixels_rgba_float(bounds.min.x, line, (bounds.max.x-bounds.min.x), 1, lineBuffer.get()); 
	}
}

std::pair<float, float> get_zdepth_range(sxsdk::image_interface* image, const sx::rectangle_class& bounds, float* buffer)
{
	float z_max = 0.0f;
	float z_min = 1.0f;

	for (int line = bounds.min.y ; line < bounds.max.y ; ++line) {
		image->get_zs(bounds.min.x, line, (bounds.max.x-bounds.min.x), 1, buffer); 
		for (int i = 0; i < (bounds.max.x-bounds.min.x) ; ++i) {
			const float z = buffer[i];
			if(z < 1.0f) {
				z_min = z < z_min ? z : z_min;
				z_max = z > z_max ? z : z_max;
			}
		}
	}

	return std::make_pair(z_min, z_max);
}

void extract_zdepth(sxsdk::image_interface* image, const sx::rectangle_class& bounds)
{
	boost::scoped_array<float> zlineBuffer(new float[(bounds.max.x-bounds.min.x)]);;
	std::pair<float, float> z_range = get_zdepth_range(image, bounds, zlineBuffer.get());

	float extract_ratio = 1.0f / (z_range.second - z_range.first);
	boost::scoped_array<sxsdk::rgba_class> lineBuffer(new sxsdk::rgba_class[(bounds.max.x-bounds.min.x)]);

	for (int line = bounds.min.y ; line < bounds.max.y ; ++line) {
		image->get_pixels_rgba_float(bounds.min.x, line, (bounds.max.x-bounds.min.x), 1, lineBuffer.get()); 
		image->get_zs(bounds.min.x, line, (bounds.max.x-bounds.min.x), 1, zlineBuffer.get()); 
		for (int i = 0; i < (bounds.max.x-bounds.min.x) ; ++i) {
			const float z = (zlineBuffer[i] < 1.0f) ? (zlineBuffer[i] - z_range.first) * extract_ratio : 1.0f;
			lineBuffer[i].red   = z;
			lineBuffer[i].green = z;
			lineBuffer[i].blue  = z;
		}
		image->set_pixels_rgba_float(bounds.min.x, line, (bounds.max.x-bounds.min.x), 1, lineBuffer.get()); 
	}
}

void invert_image(sxsdk::image_interface* image, const sx::rectangle_class& bounds)
{
	boost::scoped_array<sxsdk::vec4> lineBuffer(new sxsdk::vec4[(bounds.max.x-bounds.min.x)]);

	for (int line = bounds.min.y ; line < bounds.max.y ; ++line) {
		image->get_pixels_rgba_float(bounds.min.x, line, (bounds.max.x-bounds.min.x), 1, (sxsdk::rgba_class*)lineBuffer.get()); 
		for (int i = 0; i < (bounds.max.x-bounds.min.x) ; ++i) {
			lineBuffer[i].x = 1.0 - lineBuffer[i].x;
			lineBuffer[i].y = 1.0 - lineBuffer[i].y;
			lineBuffer[i].z = 1.0 - lineBuffer[i].z;
		}
		image->set_pixels_rgba_float(bounds.min.x, line, (bounds.max.x-bounds.min.x), 1, (sxsdk::rgba_class*)lineBuffer.get()); 
	}
}