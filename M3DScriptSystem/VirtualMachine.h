#pragma once
#include <vector>



static struct M3DS_ModuleVM {
	unsigned int lastIP = 0;
	unsigned int IP = 0;
	bool IPJumped = false;
	std::vector<unsigned> specialRegs;
	std::vector<int> integerRegs;
	std::vector<float> floatRegs;
	std::vector<double> doubleRegs;
	std::vector<char> charRegs;
	std::vector<long> longRegs;
	std::vector<long long> longlongRegs;
	std::vector<long long> templonglongRegs;
};

static std::vector<M3DS_ModuleVM*> vmInstances = { NULL };
static unsigned int currentVMInstanceID = 0;

enum M3DS_InstructionSet {
	M3DS_TYPE_INT_IMM,
	M3DS_TYPE_SPEC_REG,
	M3DS_TYPE_CHAR_REG,
	M3DS_TYPE_INT_REG,
	M3DS_TYPE_LONG_REG,
	M3DS_TYPE_LONGLONG_REG,
	M3DS_TYPE_TEMPLONGLONG_REG,
	M3DS_TYPE_FLOAT_REG,
	M3DS_TYPE_DOUBLE_REG,
	M3DS_OP_NOP,
	M3DS_OP_RET,
	M3DS_OP_SAVE,
	M3DS_OP_MOV,
	M3DS_OP_ISET,
	M3DS_OP_CSET,
	M3DS_OP_LSET,
	M3DS_OP_LLSET,
	M3DS_OP_FSET,
	M3DS_OP_DSET,
	M3DS_OP_ADD,
	M3DS_OP_SUB,
	M3DS_OP_MUL,
	M3DS_OP_DIV,
	M3DS_OP_MOD,
	M3DS_OP_POW,
	M3DS_OP_SQRT,
	M3DS_OP_LOOP,
	M3DS_OP_JMP,
	M3DS_OP_JEQ,
	M3DS_OP_JNE,
	M3DS_OP_JLE,
	M3DS_OP_JLT,
	M3DS_OP_JGE,
	M3DS_OP_JGT
};

void M3DS_ExecuteModuleInstructions(unsigned int vm_instance_ID, std::vector<int> instructions);
