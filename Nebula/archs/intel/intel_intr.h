#pragma once

#include "../../detatypes.h"
#include <vector>

enum class LABEL_TYPE { AUTO, IMM8, IMM16, IMM32, IMM_AUTO, REL8, REL16, REL32, REL_AUTO };

enum class VALUE_TYPE { IMMEDIATE, LABEL };

class SectionValue {
public:
	virtual ~SectionValue() {}
};

class Value {
public:
	virtual ~Value() {}
};

class BYTE : public Value {
public:
	::byte b;
	BYTE(::byte b) : b(b) {}
};

class LABEL : public Value {
public:
	uint32 id;
	LABEL(uint32 id = 0) : id(id) {}
};

struct Section {
	std::vector<SectionValue*> data;
	std::vector<::byte> raw_data;
};

struct LABEL_DEF {
	Section* section;
	uint32 position;
};

class IntelInstruction: public SectionValue {
public:
	bool rex_used = false;
	::byte rex;

	::byte opcode_size = 1;
	::byte opcode[4];

	bool modrm_used = false;
	::byte modrm;

	bool sib_used = false;
	::byte sib;
	
	VALUE_TYPE displacement_type = VALUE_TYPE::IMMEDIATE;
	// FOR IMMEDIATE TYPE
	::byte displacement_size = 0;
	::byte displacement[4];
	// FOR LABAL TYPE
	uint32 displacement_label_id;
	LABEL_TYPE displacement_label_usage;

	VALUE_TYPE immediate_type = VALUE_TYPE::IMMEDIATE;
	// FOR IMMEDIATE TYPE
	::byte immediate_size = 0;
	::byte immediate[4];
	// FOR LABEL TYPE
	uint32 immediate_label_id;
	LABEL_TYPE immediate_label_usage;
};

class SECTION_BYTE : public SectionValue {
public:
	::byte b;
};

class SECTION_WORD : public SectionValue {
public:
	word w;
};

class SECTION_DWORD : public SectionValue {
public:
	dword dw;
};
