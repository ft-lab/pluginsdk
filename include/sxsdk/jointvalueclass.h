#pragma once

namespace sxsdk {
	class joint_value_class {
	public:
		joint_value_class () = default;
		joint_value_class (const joint_value_class&) = default;
		joint_value_class& operator= (const joint_value_class&)SXLREF = default;
		virtual ~joint_value_class () = default;
		VTABLE_PADDING
#pragma clang diagnostics push
#pragma clang diagnostic ignored "-Wold-style-cast"
	virtual float get_scalar (void* = nullptr) const = 0; // 0
#pragma clang diagnostics pop
	};
}
