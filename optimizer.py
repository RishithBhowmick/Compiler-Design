#!/usr/bin/python

# Format of IC:
# operation,operand1,operand2,result
# For label format is label label_name
# For jump format is jmp label_name
import copy
import sys

class TACLabel(object):
    def __init__(self, label):
        self.label = label
    def __str__(self):
        return 'label ' + str(self.label)

class TACRet(object):
    def __init__(self, label):
        self.label = label
    def __str__(self):
        return 'return ' + str(self.label)

class TACJmp(object):
    def __init__(self, label):
        self.label = label
    def __str__(self):
        return 'jmp ' + str(self.label)

class TACBt(object):
    def __init__(self, boolean, label):
        self.boolean = boolean
        self.label = label
    def __str__(self):
        return 'bt ' + str(self.boolean) + ' ' + str(self.label)

class TACPlus(object):
    def __init__(self, op1, op2, result):
        self.op1 = op1
        self.op2 = op2
        self.result = result
    def __str__(self):
        return '+ ' + str(self.op1) + ' ' + str(self.op2) + ' ' + str(self.result)

class TACMinus(object):
    def __init__(self, op1, op2, result):
        self.op1 = op1
        self.op2 = op2
        self.result = result
    def __str__(self):
        return '- ' + str(self.op1) + ' ' + str(self.op2) + ' ' + str(self.result)

class TACMul(object):
    def __init__(self, op1, op2, result):
        self.op1 = op1
        self.op2 = op2
        self.result = result
    def __str__(self):
        return '* ' + str(self.op1) + ' ' + str(self.op2) + ' ' + str(self.result)

class TACDiv(object):
    def __init__(self, op1, op2, result):
        self.op1 = op1
        self.op2 = op2
        self.result = result
    def __str__(self):
        return '/ ' + str(self.op1) + ' ' + str(self.op2) + ' ' + str(self.result)

class TACMod(object):
    def __init__(self, op1, op2, result):
        self.op1 = op1
        self.op2 = op2
        self.result = result
    def __str__(self):
        return '% ' + str(self.op1) + ' ' + str(self.op2) + ' ' + str(self.result)

class TACAssign(object):
    def __init__(self, op, result):
        self.op = op
        self.result = result
    def __str__(self):
        return ':= ' + str(self.op) + ' ' + str(self.result)

class TACFun(object):
    def __init__(self, label, args):
        self.label = label
        self.args = args
    def __str__(self):
        return str(self.label) + ' ' + str(self.args)

class TACBasicBlock(object):
    def __init__(self, instructions):
        self.instructions = instructions
        self.label = self.instructions[0].label

        self.children = []
        self.parents = []

        self.child_labels = [ins.label for ins in self.instructions if isinstance(ins, TACJmp) or isinstance(ins, TACBt)]
    
    def __str__(self):
        s = 'Label : ' + str(self.label) + '\n'
        s += 'Parents : ' + str([parent.label for parent in self.parents]) + '\n'
        for index, item in enumerate(self.instructions):
            s += str(index) + '. ' + str(item) + '\n'
        s += 'Children : ' + str([child.label for child in self.children]) + '\n'
        return s


def make_bbs(inst):
    blocks = []
    blocked_instructions = []
    for instruction in inst:
        if isinstance(instruction, TACLabel):
            blocked_instructions += [[]]
        blocked_instructions[-1] += [instruction]
    block_dict = {}
    for block in blocked_instructions:
        newblock = TACBasicBlock(block)
        blocks += [newblock]
        block_dict[newblock.label] = newblock
    for block in blocks:
        for child_label in block.child_labels:
            block.children += [block_dict[child_label]]
            # now have forward edges
        del block.child_labels
    for block in blocks:
        for child in block.children:
            child.parents += [block]
        # now have backward edges
    
    return blocks


def create_CFG(code):
    inst_list = []
    for tacinst in code:
        inst = tacinst.split(',')
        if inst[0] == 'label':
            label = inst[1].strip()
            inst_list.append(TACLabel(label))
        elif inst[0] == 'jmp':
            label = inst[1].strip()
            inst_list.append(TACJmp(label))
        elif inst[0] == 'return':
            label = inst[1].strip()
            inst_list.append(TACRet(label))
        elif inst[0] == 'bt':
            mybool = inst[1].strip()
            label = inst[2].strip()
            inst_list.append(TACBt(mybool, label))
        elif inst[3] != '\n':
            if inst[0] == '+':
                assignee = inst[3].strip()
                op1 = inst[1].strip()
                op2 = inst[2].strip()
                inst_list.append(TACPlus(op1, op2, assignee))
            elif inst[0] == '-':
                assignee = inst[3].strip()
                op1 = inst[1].strip()
                op2 = inst[2].strip()
                inst_list.append(TACMinus(op1, op2, assignee))
            elif inst[0] == '*':
                assignee = inst[3].strip()
                op1 = inst[1].strip()
                op2 = inst[2].strip()
                inst_list.append(TACMul(op1, op2, assignee))
            elif inst[0] == '/':
                assignee = inst[3].strip()
                op1 = inst[1].strip()
                op2 = inst[2].strip()
                inst_list.append(TACDiv(op1, op2, assignee))
            elif inst[0] == '%':
                assignee = inst[3].strip()
                op1 = inst[1].strip()
                op2 = inst[2].strip()
                inst_list.append(TACMod(op1, op2, assignee))
            else:
                assignee = inst[3].strip()
                op = inst[1].strip()
                inst_list.append(TACAssign(op, assignee))
        else:
            label = inst[0].strip()
            arg = inst[1].strip()
            inst_list.append(TACFun(label, arg))
    # global dead code elimination before makinig it into basic blocks
    instructions = global_dead_code_elimination(inst_list)
    basic_blocks = make_bbs(instructions)
    return basic_blocks


def basic_dead_code_elimination(code):
    # used = {} # set
    # for instr in func:
    #   used += instr.args
    # for instr in func:
    #   if instr.dest and instr.dest not in used:
    #        delete instr
    used = set()
    for instr in code:
        if hasattr(instr, 'args'):
            used.add(instr.args)
        if hasattr(instr, 'op1'):
            used.add(instr.op1)
        if hasattr(instr, 'op2'):
            used.add(instr.op2)
    for instr in code:
        if hasattr(instr, 'result') and instr.result not in used:
            code.remove(instr)
    return code


def is_equal(basic_block1, basic_block2):
    for bb1 in basic_block1:
        for bb2 in basic_block2:
            if len(bb1.instructions) != len(bb2.instructions):
                return False
    return True


def global_dead_code_elimination(code):

    print("---------------------------Before global dead code elimination---------------------------")
    for instr in code:
        print(instr)

    code_copy = copy.deepcopy(code)
    opt_code = []
    opt_code = basic_dead_code_elimination(code)

    while len(opt_code) != len(code_copy):
        code_copy = copy.deepcopy(opt_code)
        code = copy.deepcopy(code_copy)
        opt_code = []
        opt_code = basic_dead_code_elimination(code)

    print("---------------------------After global dead code elimination---------------------------")
    for instr in opt_code:
        print(instr)
    return opt_code


def local_dead_code_elimination(basic_blocks):
    last_def = dict()   # var: instr; used to keep track of all the variables that have been defined but not used
    for bb in basic_blocks:
        for instr in bb.instructions:
            # check for uses
            # last_def -= instr.args

            # check for defs
            # if instr.dest in last_def
            #   del last_def[instr.dest]
            #   last_def[instr.dest] = instr
            if hasattr(instr, 'args') and instr.args in last_def:
                del last_def[instr.args]
            if hasattr(instr, 'op1') and instr.op1 in last_def:
                del last_def[instr.op1]
            if hasattr(instr, 'op2') and instr.op2 in last_def:
                del last_def[instr.op2]
            if hasattr(instr, 'result') and instr.result in last_def:
                del last_def[instr.result]
                last_def[instr.result] = instr
    return basic_blocks


def dead_code_elimination(code):
    basic_blocks = create_CFG(code)
    print("---------------------------Before local dead code elimination---------------------------")
    for bb in basic_blocks:
        print(bb)

    basic_blocks_copy = copy.deepcopy(basic_blocks)
    opt_basic_blocks = local_dead_code_elimination(basic_blocks)

    while not is_equal(opt_basic_blocks, basic_blocks_copy):
        basic_blocks_copy = copy.deepcopy(opt_basic_blocks)
        basic_blocks = copy.deepcopy(basic_blocks_copy)
        opt_basic_blocks = local_dead_code_elimination(basic_blocks)

    print("---------------------------After local dead code elimination---------------------------")
    for bb in opt_basic_blocks:
        print(bb)
    return opt_basic_blocks


def local_value_numbering(basic_block):
    # table = mapping from value tuples to avoid canonical variables, with each row numbered
    # var2num = mapping from variable names to their current row in the table
    # for instr in basic_block.instructions:
    #    value = (instr.op, var2num[instr.args[0]], ...)
    #    if value in table:
    #        // reuse the value that has been computed before
    #        num, var = table[value]
    #        replace instr with copy of var
    #    else:
    #        num = fresh value number
    #        dest = instr.dest
    #        if instr.dest will be overwritten later:
    #            dest = fresh variable name
    #            instr.dest = dest
    #        else:
    #            dest = instr.dest
    #        table[value] = num, dest
    #        for a in instr.args:
    #            replace a with table[var2num[a]].var
    #    var2num[instr.dest] = num


def common_sub_exp_elimination(code):
    return code


def constant_folding(code):
    return code


def constant_propogation(code):
    return code


def loop_opt(code):
    return code


def optimize(code):
    code = dead_code_elimination(code)
    code = common_sub_exp_elimination(code)
    code = constant_folding(code)
    code = constant_propogation(code)
    code = loop_opt(code)
    with open("opt_code.out", "w") as file:
        sys.stdout = file
        for bb in code:
            print(bb)


with open("inp1.icg", "r") as file:
    lines = file.readlines()

if __name__ == "__main__":
    optimize(lines)
