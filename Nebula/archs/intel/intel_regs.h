#pragma once

#define MODRM(mod, ro, rm) ((rm) & (ro << 3) & (mod << 6))
#define SIB(s, i, b) ((b) & (i << 3) & (s << 6))


// 8bit Registers
enum REG8		{ AL = 0, CL, DL, BL, AH, CH, DH, BH };
// 16bit Registers
enum REG16		{ AX = 0, CX, DX, BX, SP, BP, SI, DI };
// 32bit Registers
enum REG32		{ EAX = 0, ECX, EDX, EBX, ESP, EBP, ESI, EDI };
enum REG64		{ RAX = 0, RCX, RDX, RBX, RSP, RBP, RSI, RDI };
enum MM_		{ MM0 = 0, MM1, MM2, MM3, MM4, MM5, MM6, MM7 };
enum Xmm_		{ XMM0 = 0, XMM1, XMM2, XMM3, XMM4, XMM5, XMM6, XMM7};
// 8bit Registers with any REX prefix
enum REG8_REX	{ SPL = 4, BPL, SIL, DIL };