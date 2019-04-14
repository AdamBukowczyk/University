from fileinput import FileInput, input
from typing import Generator, NamedTuple, List, Dict

#---------------------------------------------------------------------
#----------------------------SECTION_DATA-----------------------------
#---------------------------------------------------------------------

R_TYPE_3_ARGS: List[str] = [
    'sll', 'srl', 'sra', 'sllv', 'srlv', 'srav',
    'add', 'addu', 'sub', 'subu',
    'and', 'xor', 'or', 'nor', 'slt', 'sltu',
]

R_TYPE_2_ARGS: List[str] = [
    'div', 'divu', 'mult', 'multu',
]

R_TYPE_1_ARGS: List[str] = [
    'mfhi', 'mflo', 'mtlo', 'mthi',
]

I_TYPE_3_ARGS: List[str] = [
    'addi', 'addiu', 'slti', 'sltiu', 'andi', 'ori', 'xori',
]

I_TYPE_2_ARGS: List[str] = [
    'lui',
]

MULT_T: List[str] = [
    'mtlo',
]

SHAMT: List[str] = [
    'sll', 'srl', 'sra'
]

SHAMT_V: List[str] = [
    'sllv', 'srlv', 'srav'
]

OPCODE: Dict[str, int] = {
    'lui':   0xF,
    'addi':  0x8,
    'addiu': 0x9,
    'slti':  0xA,
    'sltiu': 0xB,
    'andi':  0xC,
    'ori':   0xD,
    'xori':  0xE,
}

FUNCT: Dict[str, int] = {
    'sll':   0x0,
    'srl':   0x2,
    'sra':   0x3,
    'sllv':  0x4,
    'srlv':  0x6,
    'srav':  0x7,
    'mfhi':  0x10,
    'mthi':  0x11,
    'mflo':  0x12,
    'mtlo':  0x13,
    'mult':  0x18,
    'multu': 0x19,
    'div':   0x1A,
    'divu':  0x1B,
    'add':   0x20,
    'addu':  0x21,
    'sub':   0x22,
    'subu':  0x23,
    'and':   0x24,
    'xor':   0x25,
    'or':    0x26,
    'nor':   0x27,
    'slt':   0x2A,
    'sltu':  0x2B,
}


#---------------------------------------------------------------------
#---------------------SECTION_PREPARING_DATA--------------------------
#---------------------------------------------------------------------


class Prepared_data(NamedTuple):
    instruction_type: str
    instruction: str
    operands: List[int]


def get_type(instruction: str) -> str:
    if instruction in R_TYPE_3_ARGS: return 'R_3_ARGS'
    if instruction in R_TYPE_2_ARGS: return 'R_2_ARGS'
    if instruction in R_TYPE_1_ARGS: return 'R_1_ARGS'
    if instruction in I_TYPE_3_ARGS: return 'I_3_ARGS'
    if instruction in I_TYPE_2_ARGS: return 'I_2_ARGS'



def parse_register(reg: str) -> int:
    try:
        return int(reg)
    except ValueError:
        if reg == 'zero': return 0
        if reg == 'at':   return 1
        if reg == 'gp':   return 28
        if reg == 'sp':   return 29
        if reg == 'fp':   return 30
        if reg == 'ra':   return 31
        if reg[0] == 'v': return int(reg[1:]) + 2
        if reg[0] == 'a': return int(reg[1:]) + 4
        if reg[0] == 't': return int(reg[1:]) + (8 if int(reg[1:]) < 8 else 16)
        if reg[0] == 's': return int(reg[1:]) + 16
        if reg[0] == 'k': return int(reg[1:]) + 24


def get_operand(op: str) -> int:
    try:
        return int(op)
    except ValueError:
        return parse_register(op[1:])


def prepare_line(line: str) -> Prepared_data:
    line = line.split('#')[0]
    instruction, operands = line.split('\t')
    instruction = instruction.strip()
    operands = [operator.strip() for operator in operands.split(',')]
    parssed_operands = [get_operand(operator) for operator in operands]
    return Prepared_data(
        get_type(instruction),
        instruction,
        parssed_operands
    )


#---------------------------------------------------------------------
#---------------------SECTION_METADATA_CLASS--------------------------
#---------------------------------------------------------------------


class R_3_ARGS:
    def __init__(self, name: str, rd: int, rs: int, rt: int) -> None:
        self.name = name
        self.rs = rs
        self.rt = rt
        self.rd = rd
        self.shamt = 0
        self.funct = FUNCT[name]
        if name in SHAMT:
            self.shamt = self.rt
            self.rt = self.rs
            self.rs = 0
        if name in SHAMT_V:
            self.rt = rs
            self.rs = rt


    def encode(self) -> str:
        return ("%0.8X" % (
            ((self.rs & 0b11111) << 21)
             + ((self.rt & 0b11111) << 16)
             + ((self.rd & 0b11111) << 11)
             + ((self.shamt & 0b11111) << 6)
             + self.funct
        ))


class R_2_ARGS:
    def __init__(self, name: str, rs: int, rt: int) -> None:
        self.name = name
        self.rs = rs
        self.rt = rt
        self.shamt = 0
        self.funct = FUNCT[name]

    def encode(self) -> str:
        return ("%0.8X" % (
            ((self.rs & 0b11111) << 21)
             + ((self.rt & 0b11111) << 16)
             + self.funct
        ))


class R_1_ARGS:
    def __init__(self, name: str, rd: int) -> None:
        self.name = name
        self.rd = rd
        self.rs = 0
        self.funct = FUNCT[name]
        if name in MULT_T:
            self.rd = 0
            self.rs = rd


    def encode(self) -> str:
        return ("%0.8X" % (
            ((self.rs & 0b11111) << 21)
             + ((self.rd & 0b11111) << 11)
             + self.funct
        ))


class I_3_ARGS:
    def __init__(self, name: str, rt: int, rs: int, immediate: int) -> None:
        self.name = name
        self.rs = rs
        self.rt = rt
        self.immediate = immediate
        self.opcode = OPCODE[name]

    def encode(self) -> str:
        return ("%0.8X" % (
            (self.opcode << 26)
            + ((self.rs & 0b11111) << 21)
            + ((self.rt & 0b11111) << 16)
            + (self.immediate & 0xFFFF)
        ))


class I_2_ARGS:
    def __init__(self, name: str, rt: int, immediate: int) -> None:
        self.name = name
        self.rt = rt
        self.immediate = immediate
        self.opcode = OPCODE[name]

    def encode(self) -> str:
        return ("%0.8X" % (
            (self.opcode << 26)
            + ((self.rt & 0b11111) << 16)
            + (self.immediate & 0xFFFF)
        ))


#---------------------------------------------------------------------
#------------------------SECTION_MAIN_LOOP----------------------------
#---------------------------------------------------------------------


def mips_to_bin(mips: FileInput) -> Generator[str, None, None]:
    actual_address  = 0
    for line in mips:
        line = line.strip()
        Prepared_data = prepare_line(line)
        if Prepared_data.instruction_type == 'R_3_ARGS': meta = R_3_ARGS
        if Prepared_data.instruction_type == 'R_2_ARGS': meta = R_2_ARGS
        if Prepared_data.instruction_type == 'R_1_ARGS': meta = R_1_ARGS
        if Prepared_data.instruction_type == 'I_3_ARGS': meta = I_3_ARGS
        if Prepared_data.instruction_type == 'I_2_ARGS': meta = I_2_ARGS
        #tworzenie odpowiedniej klasy
        instr = meta(Prepared_data.instruction, *Prepared_data.operands)
        yield '\t'.join(["%0.8X" % actual_address, instr.encode(), line.split('#')[0]])
        actual_address += 4


if __name__ == '__main__':
    for line in mips_to_bin(input()):
        print(line)
