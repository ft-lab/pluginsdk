#pragma once
#include "sxcore/uuid.hpp"

namespace sxsdk {
	static sx::uuid_class uuid_from_pluginid (unsigned pluginid) {
		sx::uuid_class uuid(0u);
		reinterpret_cast<unsigned*>(&uuid)[0] = pluginid;
		reinterpret_cast<unsigned*>(&uuid)[1] = 0x53B811D9;
		reinterpret_cast<unsigned*>(&uuid)[2] = 0xBCFF000A;
		reinterpret_cast<unsigned*>(&uuid)[3] = 0x95BACEB2;
		return uuid;
	}
	static unsigned pluginid (const sx::uuid_class &uuid) {
		return reinterpret_cast<const unsigned&>(uuid);
	}
}
static bool operator== (const sx::uuid_class &uuid, unsigned id) {
	if (id == 0) return (uuid == sxsdk::uuid_from_pluginid(0)) || (uuid == sx::uuid_class(0u));
	return (uuid.n() == id);
}
static bool operator!= (const sx::uuid_class &uuid, unsigned id) {
	return !(uuid == id);
}
