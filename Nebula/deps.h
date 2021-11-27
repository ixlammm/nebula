#pragma once

#include <string>
#include <sstream>
#include <iomanip>

std::string hexify(int b, char fill = 2) {
	std::stringstream bt;
	bt << "0x" << std::setfill('0') << std::setw(fill) << std::uppercase << std::hex << b;
	return bt.str();
}

template<class B, class C>
B* as(C* cls) {
	return dynamic_cast<B*>(cls);
}