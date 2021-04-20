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
        self.boolean = boolean
        self.label = label
    def __str__(self):
        return 'bt ' + str(self.boolean) + ' ' + str(self.label)

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
        string_to_ret += self.args[-1] + ')'
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
        n = self.fresh()
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
        inst = tacinst.split(',')
        if inst[0] == 'call':
            n = int(inst[2].strip())
            label = inst[1].strip()
            result = inst[3].strip()
            args = [param_list[i] for i in range(len(param_list)-n, len(param_list))]
            inst_list.append(TACFun(label, args, result))
            param_list = param_list[:len(param_list)-n]
        elif inst[0] == 'param':
            param_list.append(inst[1].strip())
        elif inst[0] == 'label':
            label = inst[-1].strip()
            inst_list.append(TACLabel(label))
        elif inst[0] == 'goto':
            label = inst[-1].strip()
            inst_list.append(TACJmp(label))
        elif inst[0] == 'return':
            label = inst[-1].strip()
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
            elif inst[0] == '~':
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
            if instr.op == 'call':
                if instr.result not in used and len(used.intersection(set(instr.args))) == 0:
                    code.remove(instr)
            elif instr.result not in used:
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
            if hasattr(instr, 'args'):
                for i in instr.args:
                    if i in last_def:
                        del last_def[i]
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


def read_before_write(block):
    read_set = set()
    write_set = set()

    for instr in block.instructions:
        if hasattr(instr, 'args'):
            read.update(set(instr.args) - write_set)
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
    if hasattr(block.instructions, 'args'):
        for var in read_before_write(block.instructions.args):
            num = var2num.add(var)
            num2var[num] = var
        
        for instr, last_write in zip(block.instructions, last_writes(block)):
            argvars = instr.args
            argnums = tuple(var2num[var] for var in argvars)

            val = None
            if hasattr(instr, 'result') and hasattr(instr, 'args') and not isinstance(instr, TACFun):
                val = canonicalize(Value(instr.op, argnums))

                num = lookup(value2num, val)
                if num is not None:
                    var2num[instr.result] = num
                    
                    # constant propagation
                    if num in num2const:
                        new_instr = TACConst(value, instr.result)
                        instr = new_instr

                    else:
                        new_instr = TACId(num2var[num], instr.result)
                        instr = new_instr
                        # change this part also
                        instr.op = 'id'
                        instr.args = [num2var[num]]
                    continue
            
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

                if val is not None:
                    # constant folding
                    const = fold(num2const, val)
                    if const is not None:
                        num2const[newnum] = const
                        # change this part too
                        new_instr = TACConst(const, instr.result)
                        instr = new_instr
                        continue
                    value2num[val] = newnum

            if hasattr(instr, 'args'):
                instr.args = [num2var[n] for n in argnums]


def lookup(value2num, val):
    if val.op == 'id':
        return val.args[0]
    else:
        return value2num.get(val)


FOLDABLE_OPS = {
    '+': lambda a, b: a + b,
    '*': lambda a, b: a * b,
    '-': lambda a, b: a - b,
    '/': lambda a, b: a / b,
    '>': lambda a, b: a > b,
    '<': lambda a, b: a < b,
    '>=': lambda a, b: a >= b,
    '<=': lambda a, b: a <= b,
    '!=': lambda a, b: a != b,
    '==': lambda a, b: a == b,
    '||': lambda a, b: a or b,
    '&&': lambda a, b: a and b,
    '~': lambda a: not a,
    '&': lambda a, b: a & b,
    '|': lambda a, b: a | b,
    '^': lambda a, b: a ^ b
}


def fold(num2const, val):
    if val.op in FOLDABLE_OPS:
        try:
            const_args = [num2const[n] for n in val.args]
            return FOLDABLE_OPS[value.op](*const_args)
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
    if value.op in ('+', '*'):
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


def loop_opt(code):
    # Mark "invariant" those statements whose operands are either
    #   - constant
    #   - have all reaching definitions outside of L
    # Repeat until a fixed point is reached: mark "invariant" those unmarked statements whose operands are either 
    #   - constant
    #   - have all reaching definitions outside of L
    #   - have exactly one reaching definition and that definition is in the set marked "invariant"

    # First: Identify a loop
    
    return code


def optimize(code):
    code = dead_code_elimination(code)
    # LVN performs common subexpression elimination, constant folding and constant propagation
    code = local_value_numbering(code)
    code = loop_opt(code)
    with open("opt_code.out", "w") as file:
        sys.stdout = file
        for bb in code:
            print(bb)


with open("inp1.icg", "r") as file:
    lines = file.readlines()

if __name__ == "__main__":
    optimize(lines)
