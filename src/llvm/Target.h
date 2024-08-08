#ifndef LLVMPYM_TARGET_H
#define LLVMPYM_TARGET_H

#include <nanobind/nanobind.h>

#define PYM_TARGET_INIT_TARGET_INFO(TargetName, LowerCase) \
  m.def("init_" #LowerCase "_target_info", &LLVMInitialize##TargetName##TargetInfo);

#define PYM_TARGET_INIT_TARGET(TargetName, LowerCase) \
  m.def("init_" #LowerCase, &LLVMInitialize##TargetName##Target);

#define PYM_TARGET_INIT_TARGET_MC(TargetName, LowerCase) \
  m.def("init_" #LowerCase "_mc", &LLVMInitialize##TargetName##TargetMC);

#define PYM_ASM_PRINTER_INIT_ASM_PRINTER(TargetName, LowerCase) \
  m.def("init_" #LowerCase "_asm_printer", &LLVMInitialize##TargetName##AsmPrinter);

#define PYM_ASM_PARSER_INIT_ASM_PARSER(TargetName, LowerCase) \
  m.def("init_" #LowerCase "_asm_parser", &LLVMInitialize##TargetName##AsmParser);

#define PYM_DISASSEMBLER_INIT_DISASSEMBLER(TargetName, LowerCase) \
  m.def("init_" #LowerCase "_disassembler", &LLVMInitialize##TargetName##Disassembler);

#define PYM_TARGET_FOR_EACH(macro) \
  macro(AArch64, aarch64) \
  macro(AMDGPU, amdgpu) \
  macro(ARM, arm) \
  macro(AVR, avr) \
  macro(BPF, bpf) \
  macro(Hexagon, hexagon) \
  macro(Lanai, lanai) \
  macro(LoongArch, loongarch) \
  macro(Mips, mips) \
  macro(MSP430, msp430) \
  macro(NVPTX, nvptx) \
  macro(PowerPC, powerpc) \
  macro(RISCV, riscv) \
  macro(Sparc, sparc) \
  macro(SystemZ, systemz) \
  macro(VE, ve) \
  macro(WebAssembly, webassembly) \
  macro(X86, x86) \
  macro(XCore, xcore)


#define PYM_ASM_PRINTER_FOR_EACH(macro) \
  macro(AArch64, aarch64) \
  macro(AMDGPU, amdgpu) \
  macro(ARM, arm) \
  macro(AVR, avr) \
  macro(BPF, bpf) \
  macro(Hexagon, hexagon) \
  macro(Lanai, lanai) \
  macro(LoongArch, loongarch) \
  macro(Mips, mips) \
  macro(MSP430, msp430) \
  macro(NVPTX, nvptx) \
  macro(PowerPC, powerpc) \
  macro(RISCV, riscv) \
  macro(Sparc, sparc) \
  macro(SystemZ, systemz) \
  macro(VE, ve) \
  macro(WebAssembly, webassembly) \
  macro(X86, x86) \
  macro(XCore, xcore)


#define PYM_DISASSEMBLER_FOR_EACH(macro) \
  macro(AArch64, aarch64) \
  macro(AMDGPU, amdgpu) \
  macro(ARM, arm) \
  macro(AVR, avr) \
  macro(BPF, bpf) \
  macro(Hexagon, hexagon) \
  macro(Lanai, lanai) \
  macro(LoongArch, loongarch) \
  macro(Mips, mips) \
  macro(MSP430, msp430) \
  macro(NVPTX, nvptx) \
  macro(PowerPC, powerpc) \
  macro(RISCV, riscv) \
  macro(Sparc, sparc) \
  macro(SystemZ, systemz) \
  macro(VE, ve) \
  macro(WebAssembly, webassembly) \
  macro(X86, x86) \
  macro(XCore, xcore)

#define PYM_ASM_PARSER_FOR_EACH(macro) \
  macro(AArch64, aarch64) \
  macro(AMDGPU, amdgpu) \
  macro(ARM, arm) \
  macro(AVR, avr) \
  macro(BPF, bpf) \
  macro(Hexagon, hexagon) \
  macro(Lanai, lanai) \
  macro(LoongArch, loongarch) \
  macro(Mips, mips) \
  macro(MSP430, msp430) \
  macro(PowerPC, powerpc) \
  macro(RISCV, riscv) \
  macro(Sparc, sparc) \
  macro(SystemZ, systemz) \
  macro(VE, ve) \
  macro(WebAssembly, webassembly) \
  macro(X86, x86)


#define PYM_TARGET_FOR_EACH_INIT_TARGET_INFO() \
  PYM_TARGET_FOR_EACH(PYM_TARGET_INIT_TARGET_INFO)

#define PYM_TARGET_FOR_EACH_INIT_TARGET() \
  PYM_TARGET_FOR_EACH(PYM_TARGET_INIT_TARGET_INFO)

#define PYM_TARGET_FOR_EACH_INIT_TARGET_MC() \
  PYM_TARGET_FOR_EACH(PYM_TARGET_INIT_TARGET_MC)

#define PYM_ASM_PRINTER_FOR_EACH_INIT_ASM_PRINTER() \
  PYM_ASM_PRINTER_FOR_EACH(PYM_ASM_PRINTER_INIT_ASM_PRINTER)

#define PYM_TARGET_FOR_EACH_INIT_ASM_PARSER() \
  PYM_ASM_PARSER_FOR_EACH(PYM_ASM_PARSER_INIT_ASM_PARSER)

#define PYM_DISASSEMBLER_FOR_EACH_INIT_DISASSEMBLER() \
  PYM_ASM_PARSER_FOR_EACH(PYM_DISASSEMBLER_INIT_DISASSEMBLER)


void populateTarget(nanobind::module_ &m);


#endif
