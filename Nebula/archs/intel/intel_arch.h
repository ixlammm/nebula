#pragma once

#include <vector>
#include "intel_regs.h"
#include "intel_intr.h"
#include "../../detatypes.h"
#include "../../deps.h"

enum class CPU_MODE { BIT32, BIT64 };
enum class SEC { CODE, DATA, OTHER };

bool nonullptr(void* ptr) {
	return ptr != nullptr;
}

class ProgramHolder {
	std::vector<Section*> sections;
	Section* code;
	Section* data;

	Section* current_section;

	std::vector<LABEL_DEF> labels;

	CPU_MODE _mode = CPU_MODE::BIT32;

	uint8 getInstructionSize(IntelInstruction* instruction) {
		uint32 size(0);
		if (instruction->rex_used) size++;
		if (instruction->modrm_used) size++;
		if (instruction->sib_used) size++;
		size += instruction->opcode_size;
		size += instruction->displacement_size;
		size += instruction->immediate_size;
		return size;
	}

	void pushValue(SectionValue *value) {
		current_section->data.push_back(value);
	}

	void checkSectionLabelType(Section* section, VALUE_TYPE &type, ::byte &size, ::byte* table, uint32 label_id, LABEL_TYPE label_usage) {
		if (size > 0) switch (type)
		{
		case VALUE_TYPE::IMMEDIATE:
			section->raw_data.insert(section->raw_data.end(), table, table + size);
			break;
		case VALUE_TYPE::LABEL:
			switch (label_usage) {
			case LABEL_TYPE::IMM8:
				section->raw_data.push_back(labels[label_id].position & 0xff);
				break;
			case LABEL_TYPE::REL8:
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}

	void compileSection(Section* section) {
		for (auto v : section->data) {
			if (auto i = as<IntelInstruction>(v); i != nullptr) {
				if (i->rex_used) section->raw_data.push_back(i->rex);
				section->raw_data.insert(section->raw_data.end(), i->opcode, i->opcode + i->opcode_size);
				if (i->modrm_used) section->raw_data.push_back(i->modrm);
				if (i->sib_used) section->raw_data.push_back(i->sib);
				// DISPLACEMENT
				checkSectionLabelType(section, i->displacement_type, i->displacement_size, i->displacement, i->displacement_label_id, i->displacement_label_usage);
				// IMMEDIATE
				checkSectionLabelType(section, i->immediate_type, i->immediate_size, i->immediate, i->immediate_label_id, i->immediate_label_usage);
			}
		}
	}

	void r8_r8(::byte opcode, REG8 d, REG8 r) {
		IntelInstruction *instr = new IntelInstruction();
		instr->opcode[0] = opcode;

		instr->modrm_used = true;
		instr->modrm = MODRM(0b11, d, r);
		pushValue(instr);
	}

public:
	void setCpuMode(CPU_MODE mode) { _mode = mode; }
	uint8 addSection(SEC type) 
	{ 
		Section* section = new Section();
		sections.push_back(section); 
		switch(type) {
		case SEC::CODE:
			code = section;
			break;
		case SEC::DATA:
			data = section;
			break;
		}
		return sections.size() - 1;
	}
	void setSection(uint8 section) { current_section = sections[section]; }

	LABEL createLabel() { labels.push_back({}); return labels.size() - 1; }

	void setLabelHere(LABEL& l) {
		labels[l.id] = { current_section, current_section->data.size() - 1 };
	}

	void logSection(uint8 id) {

		Section* section = sections[id];

		int col = 0;
		std::cout << "         ";
		for (int i(0); i < 16; i++) {
			std::cout << hexify(i) << " | ";
		}
		std::cout << std::endl;
		for (int i(0); i < section->raw_data.size(); i++) {
			col++;
			if (col == 1) std::cout << hexify(i, 4) << " | ";
			std::cout << hexify(section->raw_data[i]) << " | ";
			if (col == 16) { std::cout << std::endl; col = 0; }
		}
		std::cout << std::endl;
	}

	void compileSections() {
		for (auto s : sections) compileSection(s);
	}

	// INSTRUCTION IMPLEMENTATION HERE >>>>>

	void add(REG8 d, REG8 r) { r8_r8(0x00, d, r); }
	void add_al(Value &v) {
		IntelInstruction *instr = new IntelInstruction();
		instr->opcode[0] = 0x04;
		instr->immediate_size = 1;
		if (auto l = as<LABEL>(&v); l != nullptr) {
			instr->immediate_type = VALUE_TYPE::LABEL;
			instr->immediate_label_id = l->id;
			instr->immediate_label_usage = LABEL_TYPE::IMM8;
		}
		if (auto b = as<BYTE>(&v); b != nullptr) {
			instr->immediate[0] = b->b;
		}
		pushValue(instr);
	}
};

