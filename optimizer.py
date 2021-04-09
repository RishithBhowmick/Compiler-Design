#!/usr/bin/python

# Format of IC:
# operation,operand1,operand2,result
# For label format is label label_name
# For jump format is jmp label_name


class TACLabel(object):
    def __init__(self, label):
        self.label = label
    def __str__(self):
        return 'label ' + str(self.label)

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


def create_CFG(code):
    inst_list = []
    for TACinst in code:
        inst = TACinst.split(',')
        if inst[0] == 'label':
            label = inst[1]
            inst_list.append(TACLabel(label))
        elif inst[0] == 'jmp':
            label = inst[1]
            inst_list.append(TACJmp(label))
        # to be continued...
    return cfg

def dead_code_elimination(code):
    cfg = create_CFG(code)
    return code


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
        for line in code:
            file.write(line)


with open("icg.quad", "r") as file:
    lines = file.readlines()

if __name__ == "__main__":
    optimize(lines)
