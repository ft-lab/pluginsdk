#pragma once
SXMODULE_SXCORE
#include "sxcore/property.hpp"

namespace sx {
	class list_item_value_class {
	public:
		property<bool> check; 
		property<std::string> label;
	};
	class list_item_xvalue_class {  // to identify items in unfixed and multilingual list
	public:
		property<bool> check;
		property<std::string> label;
		property<int> id;
	};
	class list_items_class : std::vector<list_item_value_class> {
	public:
		list_items_class () {}
	};
	class xlist_items_class : std::vector<list_item_xvalue_class> {
	public:
		xlist_items_class () {}
	};
}
