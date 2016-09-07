#pragma once

namespace sxsdk {
	class physical_sky_class {
	public:
		virtual ~physical_sky_class () { }
		VTABLE_PADDING
#pragma clang diagnostics push
#pragma clang diagnostic ignored "-Wold-style-cast"
	virtual physical_sky_class &set_direction (const sxsdk::vec3 &direction_param, void* = nullptr) = 0; // 0
	virtual sxsdk::vec3 get_direction (void* = nullptr) const = 0; // 1
	virtual physical_sky_class &set_color (const sxsdk::rgb_class &color_param, void* = nullptr) = 0; // 2
	virtual sxsdk::rgb_class get_color (void* = nullptr) const = 0; // 3
	virtual physical_sky_class &set_north_angle (float north_angle_param, void* = nullptr) = 0; // 6
	virtual float get_north_angle (void* = nullptr) const = 0; // 7
	virtual physical_sky_class &set_intensity (float intensity_param, void* = nullptr) = 0; // 8
	virtual float get_intensity (void* = nullptr) const = 0; // 9
	virtual physical_sky_class &set_utc (float utc_param, void* = nullptr) = 0; // 10
	virtual float get_utc (void* = nullptr) const = 0; // 11
	virtual void set_datetime (int month, int day, int hour, int minutes, int second, void* aux = 0) = 0; // 12
	virtual void set_datetime_end (int month, int day, int hour, int minutes, int second, void* aux = 0) = 0; // 13
	virtual physical_sky_class &set_datetime_month (int datetime_month_param, void* = nullptr) = 0; // 14
	virtual int get_datetime_month (void* = nullptr) const = 0; // 15
	virtual physical_sky_class &set_datetime_day (int datetime_day_param, void* = nullptr) = 0; // 16
	virtual int get_datetime_day (void* = nullptr) const = 0; // 17
	virtual physical_sky_class &set_datetime_hour (int datetime_hour_param, void* = nullptr) = 0; // 18
	virtual int get_datetime_hour (void* = nullptr) const = 0; // 19
	virtual physical_sky_class &set_datetime_minutes (int datetime_minutes_param, void* = nullptr) = 0; // 20
	virtual int get_datetime_minutes (void* = nullptr) const = 0; // 21
	virtual physical_sky_class &set_datetime_second (int datetime_second_param, void* = nullptr) = 0; // 22
	virtual int get_datetime_second (void* = nullptr) const = 0; // 23
	virtual physical_sky_class &set_use_sun (bool use_sun_param, void* = nullptr) = 0; // 24
	virtual bool get_use_sun (void* = nullptr) const = 0; // 25
	virtual physical_sky_class &set_update_light_direction (bool update_light_direction_param, void* = nullptr) = 0; // 28
	virtual bool get_update_light_direction (void* = nullptr) const = 0; // 29
	virtual physical_sky_class &set_update_light_color (bool update_light_color_param, void* = nullptr) = 0; // 30
	virtual bool get_update_light_color (void* = nullptr) const = 0; // 31
	virtual physical_sky_class &set_use_background (bool use_background_param, void* = nullptr) = 0; // 32
	virtual bool get_use_background (void* = nullptr) const = 0; // 33
	virtual physical_sky_class &set_enable_animation_end (bool enable_animation_end_param, void* = nullptr) = 0; // 34
	virtual bool get_enable_animation_end (void* = nullptr) const = 0; // 35
	virtual physical_sky_class &set_background_auto_color (bool background_auto_color_param, void* = nullptr) = 0; // 36
	virtual bool get_background_auto_color (void* = nullptr) const = 0; // 37
	virtual physical_sky_class &set_datetime_month_end (int datetime_month_end_param, void* = nullptr) = 0; // 38
	virtual int get_datetime_month_end (void* = nullptr) const = 0; // 39
	virtual physical_sky_class &set_datetime_day_end (int datetime_day_end_param, void* = nullptr) = 0; // 40
	virtual int get_datetime_day_end (void* = nullptr) const = 0; // 41
	virtual physical_sky_class &set_datetime_hour_end (int datetime_hour_end_param, void* = nullptr) = 0; // 42
	virtual int get_datetime_hour_end (void* = nullptr) const = 0; // 43
	virtual physical_sky_class &set_datetime_minutes_end (int datetime_minutes_end_param, void* = nullptr) = 0; // 44
	virtual int get_datetime_minutes_end (void* = nullptr) const = 0; // 45
	virtual physical_sky_class &set_datetime_second_end (int datetime_second_end_param, void* = nullptr) = 0; // 46
	virtual int get_datetime_second_end (void* = nullptr) const = 0; // 47
	virtual physical_sky_class &set_turbidity (float turbidity_param, void* = nullptr) = 0; // 48
	virtual float get_turbidity (void* = nullptr) const = 0; // 49
	virtual physical_sky_class &set_latitude (float latitude_param, void* = nullptr) = 0; // 52
	virtual float get_latitude (void* = nullptr) const = 0; // 53
	virtual physical_sky_class &set_longitude (float longitude_param, void* = nullptr) = 0; // 54
	virtual float get_longitude (void* = nullptr) const = 0; // 55
	virtual sxsdk::rgb_class get_sky_color (sxsdk::vec3 v, void* aux = 0) = 0; // 56
	virtual void set_now_datetime (void* aux = 0) = 0; // 57
	virtual void set_now_datetime_end (void* aux = 0) = 0; // 58
	virtual physical_sky_class &set_gamma (float gamma_param, void* = nullptr) = 0; // 59
	virtual float get_gamma (void* = nullptr) const = 0; // 60
#pragma clang diagnostics pop
	};
}
