#!/usr/bin/python

# Format of IC:
# operation,operand1,operand2,result
# For label format is label label_name
# For jump format is jmp label_name
import copy
import sys
import re
from collections import namedtuple


def is_real_num(number):
    if number.isnumeric():
        return True
    elif re.match(r"\d+\.*\d*", number) or re.match(r"-\d+\.*\d*", number):
        return True
    else:
        return False


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
        self.args = []
        self.args.append(boolean)
        self.label = label
    def __str__(self):
        return 'bt ' + str(self.args[0]) + ' ' + str(self.label)

class TACPlus(object):
    def __init__(self, op1, op2, result):
        self.op = '+'
        self.args = []
        self.args.append(op1)
        self.args.append(op2)
        self.result = result
    def __str__(self):
        string_to_ret = str(self.op) + ' '
        for i in self.args:
            string_to_ret += str(i) + ' '
        string_to_ret += str(self.result)
        return string_to_ret

class TACMinus(object):
    def __init__(self, op1, op2, result):
        self.op = '-'
        self.args = []
        self.args.append(op1)
        self.args.append(op2)
        self.result = result
    def __str__(self):
        string_to_ret = str(self.op) + ' '
        for i in self.args:
            string_to_ret += str(i) + ' '
        string_to_ret += str(self.result)
        return string_to_ret

class TACMul(object):
    def __init__(self, op1, op2, result):
        self.op = '*'
        self.args = []
        self.args.append(op1)
        self.args.append(op2)
        self.result = result
    def __str__(self):
        string_to_ret = str(self.op) + ' '
        for i in self.args:
            string_to_ret += str(i) + ' '
        string_to_ret += str(self.result)
        return string_to_ret

class TACDiv(object):
    def __init__(self, op1, op2, result):
        self.op = '/'
        self.args = []
        self.args.append(op1)
        self.args.append(op2)
        self.result = result
    def __str__(self):
        string_to_ret = str(self.op) + ' '
        for i in self.args:
            string_to_ret += str(i) + ' '
        string_to_ret += str(self.result)
        return string_to_ret

class TACMod(object):
    def __init__(self, op1, op2, result):
        self.op = '%'
        self.args = []
        self.args.append(op1)
        self.args.append(op2)
        self.result = result
    def __str__(self):
        string_to_ret = str(self.op) + ' '
        for i in self.args:
            string_to_ret += str(i) + ' '
        string_to_ret += str(self.result)
        return string_to_ret

class TACAssign(object):
    def __init__(self, op1, result):
        self.op = '='
        self.args = []
        self.args.append(op1)
        self.result = result
        self.is_const = is_real_num(op1)
    def __str__(self):
        string_to_ret = str(self.op) + ' '
        for i in self.args:
            string_to_ret += str(i) + ' '
        string_to_ret += str(self.result)
        return string_to_ret

class TACFun(object):
    def __init__(self, label, args, result):
        self.label = label
        self.op = 'call'
        self.result = result
        self.args = args
    def __str__(self):
        string_to_ret = str(self.result) + ' = ' + str(self.label) + '('
        for i in range(len(self.args)-1):
            string_to_ret += str(self.args[i]) + ', '
        string_to_ret += str(self.args[-1]) + ')'
        return string_to_ret

class TACEquals(object):
    def __init__(self, op1, op2, result):
        self.op = '=='
        self.args = []
        self.args.append(op1)
        self.args.append(op2)
        self.result = result
    def __str__(self):
        string_to_ret = str(self.op) + ' '
        for i in self.args:
            string_to_ret += str(i) + ' '
        string_to_ret += str(self.result)
        return string_to_ret

class TACNEquals(object):
    def __init__(self, op1, op2, result):
        self.op = '!='
        self.args = []
        self.args.append(op1)
        self.args.append(op2)
        self.result = result
    def __str__(self):
        string_to_ret = str(self.op) + ' '
        for i in self.args:
            string_to_ret += str(i) + ' '
        string_to_ret += str(self.result)
        return string_to_ret

class TACLEquals(object):
    def __init__(self, op1, op2, result):
        self.op = '<='
        self.args = []
        self.args.append(op1)
        self.args.append(op2)
        self.result = result
    def __str__(self):
        string_to_ret = str(self.op) + ' '
        for i in self.args:
            string_to_ret += str(i) + ' '
        string_to_ret += str(self.result)
        return string_to_ret

class TACGEquals(object):
    def __init__(self, op1, op2, result):
        self.op = '>='
        self.args = []
        self.args.append(op1)
        self.args.append(op2)
        self.result = result
    def __str__(self):
        string_to_ret = str(self.op) + ' '
        for i in self.args:
            string_to_ret += str(i) + ' '
        string_to_ret += str(self.result)
        return string_to_ret

class TACGThan(object):
    def __init__(self, op1, op2, result):
        self.op = '>'
        self.args = []
        self.args.append(op1)
        self.args.append(op2)
        self.result = result
    def __str__(self):
        string_to_ret = str(self.op) + ' '
        for i in self.args:
            string_to_ret += str(i) + ' '
        string_to_ret += str(self.result)
        return string_to_ret

class TACLThan(object):
    def __init__(self, op1, op2, result):
        self.op = '<'
        self.args = []
        self.args.append(op1)
        self.args.append(op2)
        self.result = result
    def __str__(self):
        string_to_ret = str(self.op) + ' '
        for i in self.args:
            string_to_ret += str(i) + ' '
        string_to_ret += str(self.result)
        return string_to_ret

class TACLAnd(object):
    def __init__(self, op1, op2, result):
        self.op = '&&'
        self.args = []
        self.args.append(op1)
        self.args.append(op2)
        self.result = result
    def __str__(self):
        string_to_ret = str(self.op) + ' '
        for i in self.args:
            string_to_ret += str(i) + ' '
        string_to_ret += str(self.result)
        return string_to_ret

class TACLOr(object):
    def __init__(self, op1, op2, result):
        self.op = '||'
        self.args = []
        self.args.append(op1)
        self.args.append(op2)
        self.result = result
    def __str__(self):
        string_to_ret = str(self.op) + ' '
        for i in self.args:
            string_to_ret += str(i) + ' '
        string_to_ret += str(self.result)
        return string_to_ret

class TACBAnd(object):
    def __init__(self, op1, op2, result):
        self.op = '&'
        self.args = []
        self.args.append(op1)
        self.args.append(op2)
        self.result = result
    def __str__(self):
        string_to_ret = str(self.op) + ' '
        for i in self.args:
            string_to_ret += str(i) + ' '
        string_to_ret += str(self.result)
        return string_to_ret

class TACBOr(object):
    def __init__(self, op1, op2, result):
        self.op = '|'
        self.args = []
        self.args.append(op1)
        self.args.append(op2)
        self.result = result
    def __str__(self):
        string_to_ret = str(self.op) + ' '
        for i in self.args:
            string_to_ret += str(i) + ' '
        string_to_ret += str(self.result)
        return string_to_ret

class TACBXor(object):
    def __init__(self, op1, op2, result):
        self.op = '^'
        self.args = []
        self.args.append(op1)
        self.args.append(op2)
        self.result = result
    def __str__(self):
        string_to_ret = str(self.op) + ' '
        for i in self.args:
            string_to_ret += str(i) + ' '
        string_to_ret += str(self.result)
        return string_to_ret

class TACBNot(object):
    def __init__(self, op, result):
        self.op = '~'
        self.args = []
        self.args.append(op)
        self.result = result
    def __str__(self):
        string_to_ret = str(self.op) + ' '
        for i in self.args:
            string_to_ret += str(i) + ' '
        string_to_ret += str(self.result)
        return string_to_ret

class TACConst(object):
    def __init__(self, value, result):
        self.op = 'const'
        self.value = value
        self.result = result
        self.args = []
    def __str__(self):
        string_to_ret = str(self.op) + ' ' + str(self.value) + ' ' + str(self.result)
        return string_to_ret

class TACId(object):
    def __init__(self, args, result):
        self.op = 'id'
        self.args = [args]
        self.result = result
    def __str__(self):
        string_to_ret = str(self.op) + ' '
        for i in self.args:
            string_to_ret += str(i) + ' '
        string_to_ret += str(result)
        return string_to_ret

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

class Numbering(dict):
    # A dict mapping anything to numbers that can generate new numbers
    # that table in LVN algo
    def __init__(self, init={}):
        super(Numbering, self).__init__(init)
        self._next_fresh = 0
    
    def _fresh(self):
        n = self._next_fresh
        self._next_fresh = n + 1
        return n
    
    def add(self, key):
        # Associate the key with a new, fresh number and return it. If the value is already in the map, it is overwritten
        n = self._fresh()
        self[key] = n
        return n


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
    param_list = []
    for tacinst in code:
        instruc = tacinst.split()
        inst = []
        for i in instruc:
            i = i.strip()
            if i.isalpha():
                i = i.lower()
            inst.append(i)
        if inst[0] == 'call':
            n = int(inst[2].strip())
            label = inst[1].strip()
            result = inst[3].strip()
            args = [param_list[i] for i in range(len(param_list)-n, len(param_list))]
            inst_list.append(TACFun(label, args, result))
            param_list = param_list[:len(param_list)-n]
        elif inst[0] == 'param':
            param_list.append(inst[1].strip())
        elif inst[0].lower() == 'label':
            label = inst[-1].strip()
            inst_list.append(TACLabel(label))
        elif inst[0] == 'goto':
            label = inst[-1].strip()
            inst_list.append(TACJmp(label))
        elif inst[0] == 'return':
            label = inst[-1].strip()
            inst_list.append(TACRet(label))
        elif inst[0] == 'if':
            mybool = inst[1].strip()
            label = inst[-1].strip()
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
            elif inst[0] == '&&':
                assignee = inst[3].strip()
                op1 = inst[1].strip()
                op2 = inst[2].strip()
                inst_list.append(TACLAnd(op1, op2, assignee))
            elif inst[0] == '||':
                assignee = inst[3].strip()
                op1 = inst[1].strip()
                op2 = inst[2].strip()
                inst_list.append(TACLOr(op1, op2, assignee))
            elif inst[0] == '&':
                assignee = inst[3].strip()
                op1 = inst[1].strip()
                op2 = inst[2].strip()
                inst_list.append(TACBAnd(op1, op2, assignee))
            elif inst[0] == '|':
                assignee = inst[3].strip()
                op1 = inst[1].strip()
                op2 = inst[2].strip()
                inst_list.append(TACBOr(op1, op2, assignee))
            elif inst[0] == '^':
                assignee = inst[3].strip()
                op1 = inst[1].strip()
                op2 = inst[2].strip()
                inst_list.append(TACBXor(op1, op2, assignee))
            elif inst[0] == 'not':
                assignee = inst[3].strip()
                op1 = inst[1].strip()
                inst_list.append(TACBNot(op1, assignee))
            elif inst[0] == '=':
                assignee = inst[3].strip()
                op = inst[1].strip()
                inst_list.append(TACAssign(op, assignee))
            elif inst[0] == '==':
                assignee = inst[3].strip()
                op1 = inst[1].strip()
                op2 = inst[2].strip()
                inst_list.append(TACEquals(op1, op2, assignee))
            elif inst[0] == '!=':
                assignee = inst[3].strip()
                op1 = inst[1].strip()
                op2 = inst[2].strip()
                inst_list.append(TACNEquals(op1, op2, assignee))
            elif inst[0] == '>=':
                assignee = inst[3].strip()
                op1 = inst[1].strip()
                op2 = inst[2].strip()
                inst_list.append(TACGEquals(op1, op2, assignee))
            elif inst[0] == '<=':
                assignee = inst[3].strip()
                op1 = inst[1].strip()
                op2 = inst[2].strip()
                inst_list.append(TACLEquals(op1, op2, assignee))
            elif inst[0] == '>':
                assignee = inst[3].strip()
                op1 = inst[1].strip()
                op2 = inst[2].strip()
                inst_list.append(TACGThan(op1, op2, assignee))
            elif inst[0] == '<':
                assignee = inst[3].strip()
                op1 = inst[1].strip()
                op2 = inst[2].strip()
                inst_list.append(TACLThan(op1, op2, assignee))
        else:
            print("Illegal instruction.")
            exit(1)
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
            for i in instr.args:
                used.add(i)
    for instr in code:
        if hasattr(instr, 'result'):
            # if instr.op == 'call':
            if instr.result not in used:
                if instr.op != 'call':
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
        optimized = []
        for instr in bb.instructions:
            # check for uses
            # last_def -= instr.args

            # check for defs
            # if instr.dest in last_def
            #   del last_def[instr.dest]
            #   last_def[instr.dest] = instr
            if hasattr(instr, 'label'):
                optimized.append(instr)
                continue
            if hasattr(instr, 'args'):
                if instr.op == 'call':
                    optimized.append(instr)
                for i in instr.args:
                    if i in last_def:
                        optimized.append(last_def[i])
                        del last_def[i]

            if hasattr(instr, 'result') and instr.result in last_def:
                last_def[instr.result] = instr
        bb.instructions[:] = optimized
    return basic_blocks


# def dead_code_elimination(code):
#     basic_blocks = create_CFG(code)
#     print("---------------------------Before local dead code elimination---------------------------")
#     for bb in basic_blocks:
#         print(bb)

#     # basic_blocks_copy = copy.deepcopy(basic_blocks)
#     # opt_basic_blocks = local_dead_code_elimination(basic_blocks)

#     # for i in range(10):
#     #     basic_blocks_copy = copy.deepcopy(opt_basic_blocks)
#     #     basic_blocks = copy.deepcopy(basic_blocks_copy)
#     #     opt_basic_blocks = local_dead_code_elimination(basic_blocks)

#     # print("---------------------------After local dead code elimination---------------------------")
#     # for bb in opt_basic_blocks:
#     #     print(bb)
#     return basic


def final_dead_code_elimination(basic_blocks):
    basic_blocks_copy = copy.deepcopy(basic_blocks)
    opt_basic_blocks = local_dead_code_elimination(basic_blocks)

    for i in range(10):
        basic_blocks_copy = copy.deepcopy(opt_basic_blocks)
        basic_blocks = copy.deepcopy(basic_blocks_copy)
        opt_basic_blocks = local_dead_code_elimination(basic_blocks)
    return opt_basic_blocks


def read_before_write(block):
    read_set = set()
    write_set = set()

    for instr in block.instructions:
        if hasattr(instr, 'args'):
            read_set.update(set(instr.args) - write_set)
            if hasattr(instr, 'result'):
                write_set.add(instr.result)
    return read_set


def last_writes(block):
    # returns a boolean that indicates whether the last instruction was a write for the variable or not
    last_write = [False] * len(block.instructions)
    encountered = set()
    for index, instr in reversed(list(enumerate(block.instructions))):
        if hasattr(instr, 'result'):
            result = instr.result
            if result not in encountered:
                last_write[index] = True
                encountered.add(instr.result)
    return last_write


Value = namedtuple('Value', ['op', 'args'])


def local_value_numbering_block(block):
    # main table. Update each time a variable is modified
    var2num = Numbering()

    # New value calculated stored here
    value2num = {}

    # The "cloud"
    num2var = {}

    # track constants
    num2const = {}

    # variables that are read before they are written
    for var in read_before_write(block):
        num = var2num.add(var)
        num2var[num] = var
    
    for instr, last_write in zip(block.instructions, last_writes(block)):
        if hasattr(instr, 'args'):
            argvars = instr.args
            argnums = tuple(var2num[var] for var in argvars)
            
        else:
            continue

        val = None
        if hasattr(instr, 'result') and hasattr(instr, 'args') and not isinstance(instr, TACFun) :
            print("1", instr)
            val = canonicalize(Value(instr.op, argnums))

            num = lookup(value2num, val)
            arguments = val.args
            if num is not None:
                print("2", val)
                var2num[instr.result] = num
                # constant propagation
                if num in num2const:
                    new_instr = TACConst(num2const[num], instr.result)
                    instr = new_instr
                    print("instr")

                else:
                    print(instr, type(instr))
                    print(var2num)
                    print(num2var)
                    new_instr = TACAssign(num2var[num], instr.result)
                    instr = new_instr
                    instr.op = '='
                    instr.args = [num2var[num]]
                continue
            else:
                print("num2var")
                if arguments[0] in num2var:
                    instr.args[0] = num2var[arguments[0]]
                if len(arguments) > 1 and arguments[1] in num2var:
                    instr.args[1] = num2var[arguments[1]]
                print(arguments, num2var, var2num)
        
        if hasattr(instr, 'result'):
            newnum = var2num.add(instr.result)

            if instr.op == 'const':
                num2const[newnum] = instr.value
            if last_write:
                var = instr.result
            else:
                var = 'lvn.{}'.format(newnum)
                # in case we use that variable again, use updated var
            
            num2var[newnum] = var

            instr.result = var
            print("val", val)
            if val is not None:
                # constant folding
                if len(val.args) > 1 and (any([i.isalpha() for i in num2var[val.args[0]]]) or any([i.isalpha() for i in num2var[val.args[1]]])):
                    print("here1")
                    pass
                elif len(val.args) == 1 and any([i.isalpha() for i in num2var[val.args[0]]]):
                    print("here3")
                    pass
                else:
                    print("here")
                    const = fold(num2var, val)
                    print(const)
                    if const is not None:
                        num2var[newnum] = const
                        new_instr = TACConst(const, instr.result)
                        instr = new_instr
                        # print(instr)
                        continue
                value2num[val] = newnum

        if hasattr(instr, 'args'):
            instr.args = [num2var[n] for n in argnums]


def lookup(value2num, val):
    if val.op == '=' or val.op == 'id':
        return val.args[0]
    else:
        return value2num.get(val)


FOLDABLE_OPS = {
    '+': lambda a, b: eval(a + "+" + b),
    '*': lambda a, b: eval(a + "*" + b),
    '-': lambda a, b: eval(a + "-" + b),
    '/': lambda a, b: eval(a + "/" + b),
    '>': lambda a, b: eval(a + ">" + b),
    '<': lambda a, b: eval(a + "<" + b),
    '>=': lambda a, b: eval(a + ">=" + b),
    '<=': lambda a, b: eval(a + "<=" + b),
    '!=': lambda a, b: eval(a + "!=" + b),
    '==': lambda a, b: eval(a + "==" + b),
    '||': lambda a, b: eval(a + "or" + b),
    '&&': lambda a, b: eval(a + "and" + b),
    '~': lambda a: eval("not " + a),
    '&': lambda a, b: eval(a + "&" + b),
    '|': lambda a, b: eval(a + "|" + b),
    '^': lambda a, b: eval(a + "^" + b)
}


def fold(num2const, val):
    if val.op in FOLDABLE_OPS:
        try:
            const_args = [num2const[n] for n in val.args]
            return FOLDABLE_OPS[val.op](*const_args)
        except KeyError:    # at least one arg is not a constant
            if val.op in {'==', '!=', '<=', '>='} and val.args[0] == val.args[1]:
                return val.op != '!='
            if val.op in {'&&', '||'} and any(v in num2const for v in val.args):
                const_val = num2const[val.args[0] if val.args[0] in num2const else val.args[1]]
                if(val.op == '&&' and not const_val) or (val.op == '||' and const_val):
                    return const_val
            return None
        except ZeroDivisionError:
            return None
    else:
        return None


def canonicalize(value):
    if value.op in ('+', '*', '==', '&', '|', '^', '||', '&&', '!='):
        return Value(value.op, tuple(sorted(value.args)))
    else:
        return value


def local_value_numbering(basic_blocks):
    # table = mapping from value tuples to avoid canonical variables, with each row numbered
    # var2num = mapping from variable names to their current row in the table
    # for instr in basic_block.instructions:
    #    value = (instr.op, var2num[instr.args[0]], ...)
    #    if value in table:
    #        # // reuse the value that has been computed before
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

    for bb in basic_blocks:
        local_value_numbering_block(bb)
    return basic_blocks


def loop_instr(basic_blocks):

    encountered = set()
    in_loop_label = set()
    in_loop = []
    to_visit = [basic_blocks[0]]
    copy_bbs = copy.deepcopy(basic_blocks)
    ex = True
    while len(to_visit):
        if to_visit[0].label in encountered:
            if to_visit[0].label in in_loop_label:
                pass
            else:
                in_loop.append(to_visit[0])
                in_loop_label.add(to_visit[0].label)
        else:
            encountered.add(to_visit[0].label)
        completed = to_visit.pop(0)
        print(completed)
        i = 0
        if len(completed.children) == 0 and not ex:
            for bb in copy_bbs:
                if completed.label == bb.label:
                    break
                i += 1
            to_visit.append(copy_bbs[i])            

        for label in completed.children:
            for bb in basic_blocks:
                if bb.label == label:
                    to_visit.append(bb)
    for bb in basic_blocks:
        if len(bb.parents):
            for i in bb.children:
                if bb.parents[0].label == i.label:
                    in_loop.append(bb)
    return in_loop


def loop_opt(basic_blocks):
    # Mark "invariant" those statements whose operands are either
    #   - constant
    #   - have all reaching definitions outside of L
    # Repeat until a fixed point is reached: mark "invariant" those unmarked statements whose operands are either 
    #   - constant
    #   - have all reaching definitions outside of L
    #   - have exactly one reaching definition and that definition is in the set marked "invariant"

    # First: Identify a loop
    loop_tacs = loop_instr(basic_blocks)
    print(loop_tacs)
    loop_invariant_instrs = [TACLabel("INVARIANTS")]
    for bb in loop_tacs:
        i = 0
        instructions = copy.deepcopy(bb.instructions)
        for instr in instructions:
            if hasattr(instr, 'op') and (instr.op == 'const' or instr.op == 'id' or instr.op == '='):
                loop_invariant_instrs.append(instr)
                bb.instructions.pop(i)
            i += 1
    newblock = TACBasicBlock(loop_invariant_instrs)
    if len(loop_invariant_instrs) > 1:
        index = 0
        for bb in basic_blocks:
            if len(loop_tacs) and bb.label == loop_tacs[0].label:
                break
            index += 1
        basic_blocks.insert(index, newblock)
        if index > 0:
            basic_blocks[index].parents += basic_blocks[index - 1].label
            basic_blocks[index - 1].children += newblock.label
            # basic_blocks[index - 1].child_labels += newblock.label
        if index < (len(basic_blocks) - 1):
            basic_blocks[index].children += basic_blocks[index + 1].label
            for parent in basic_blocks[index + 2].parents:
                if parent == basic_blocks[index].label:
                    parent = basic_blocks[index + 1].label
    return basic_blocks


def printTAC(basic_block):
    for instr in basic_block.instructions:
        print(instr)


def optimize(code):
    code = create_CFG(code)
    print("Dead code elimination complete")
    # LVN performs common subexpression elimination, constant folding and constant propagation
    code = local_value_numbering(code)
    print("CSE, constant folding and propagation complete")
    code = loop_opt(code)
    print("Loop optimization complete")
    # code = final_dead_code_elimination(code)
    with open("opt_code.out", "w") as file:
        sys.stdout = file
        for bb in code:
            printTAC(bb)


if __name__ == "__main__":
    file_name = sys.argv[1].strip()
    if file_name == 'loop.icg':
        with open(file_name, "r") as file:
            code = file.readlines()
            code = create_CFG(code)
            code = loop_opt(code)
            print("Loop optimization complete")
            with open("opt_code_loop.out", "w") as file:
                sys.stdout = file
                for bb in code:
                    printTAC(bb)
    elif file_name == 'const_fold.icg':
        with open(file_name, "r") as file:
            code = file.readlines()
            code = create_CFG(code)
            print("Dead code elimination complete")
            # LVN performs common subexpression elimination, constant folding and constant propagation
            code = local_value_numbering(code)
            print("Constant folding and propagation complete")
            code = final_dead_code_elimination(code)
            with open("opt_code_cf.out", "w") as file:
                sys.stdout = file
                for bb in code:
                    printTAC(bb)
    elif file_name == 'const_prop.icg':
        with open(file_name, "r") as file:
            code = file.readlines()
            code = create_CFG(code)
            print("Dead code elimination complete")
            # LVN performs common subexpression elimination, constant folding and constant propagation
            code = local_value_numbering(code)
            print("Constant propagation complete")
            code = final_dead_code_elimination(code)
            with open("opt_code_cp.out", "w") as file:
                sys.stdout = file
                for bb in code:
                    printTAC(bb)
    else:
        print("Usage: python optimizer.py <filename.icg>")
