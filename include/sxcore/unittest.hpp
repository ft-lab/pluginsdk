#pragma once
SXMODULE_SXCORE

#define SXUNITTEST(TEST) do { if (!(TEST)) { sx::unittest::testfailed(__FILE__, __LINE__); } } while (false)

namespace sx {
	enum unittest_enum : int {
		skip_unittest,
		unittest_and_exit,
		unittest_and_continue
	};

	namespace unittest {
		bool nearlyequal (float a, float b);
		void testfailed (const char *file, int line);
		extern bool failed;
		void test (void (*)());
	}
	namespace core {
		namespace unittest {
			void test ();
		}
	}
}
