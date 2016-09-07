#pragma once
SXMODULE_SXCORE
#include "sxcore/setting.hpp"
#include "sxcore/types.hpp"

namespace sx {
	enum unittest_enum : int;

	namespace core {
		namespace settings {
			#if SXAPI_WIN32
				void	langid (LANGID);
				LANGID	langid ();
			#endif
			extern sx::setting<bool>			help;
			extern sx::setting<bool>			mappedimage;
			extern sx::setting<bool>			forcemappedimage;
			extern sx::setting<bool>			strippedimage;
			extern sx::setting<bool>			log_exr;
			extern sx::setting<bool>			log_mmap;
			extern sx::setting<bool>			log_mappedimage;
			extern sx::setting<bool>			log_timer;
			extern sx::setting<bool>			log_action;
			extern sx::setting<bool>			verbose;
			extern sx::setting<bool>			break_on_error;
			extern sx::setting<unittest_enum>	unittest;
			extern sx::setting<bool>			assert_invariant;
			extern sx::setting<bool>			check_address;
			extern sx::setting<std::string>		locale;
			extern sx::setting<bool>			backtrace;
			extern sx::setting<bool>			log_glyph;
			extern sx::setting<bool>			temp_file;			// use temporary file to back sx::datastream_class.
			extern sx::setting<bool>			timing;
			extern sx::setting<sx::size_t>		file_buffer_size;
			extern sx::setting<sx::size_t>		tcp_buffer_size;
			extern sx::setting<float>			display_scaling;
			extern sx::setting<float>			ui_scaling;
			extern sx::setting<float>			timer_granularity;

			void set (const std::string& option);
		}
	}
}

