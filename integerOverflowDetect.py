import os


class Variable:

    _type = 0
    _value = 79916
    _name = ""
    _belongToFunction = 0
    _line = 0

    def __init__(self, a, b, c):
        self._type = a
        self._name = b
        self._belongToFunction = c

    def set_type(self, a):
        self._type = a

    def set_belongToFunction(self, a):
        self._belongToFunction = a

    def set_name(self, a):
        self._name = a

    def set_value(self, a):
        self._value = a

    def set_line(self, a):
        self._line = a

    def get_type(self):
        return self._type

    def get_belongToFunction(self):
        return self._belongToFunction

    def get_name(self):
        return self._name

    def get_value(self):
        return self._value

    def get_line(self):
        return self._line


def detectIntegerOverflow(filename):
    filepath = "sample\\" + filename
    command = "java -jar codersensor\\CodeSensor.jar " + filepath + " >midFile\\widthDetect.txt"
    os.system(command)
    linesToAnalyse, variable = analyseCodersensor("widthDetect.txt")
    program = open(filepath, "r", encoding="utf-8")
    warings = []
    funcindex = 0

    for i in linesToAnalyse:
        funcindex = funcindex + 1
        varnames = []
        for j in variable:
            if j.get_belongToFunction() == funcindex:
                varnames.append(j.get_name())
        program.seek(0)
        for k in range(i[0]):
            programLine = program.readline()
        for j in range(i[0], i[1]):
            programLine = program.readline()
            if "=" in programLine:
                deleteBlank = programLine.replace(" ", "")
                deleten = deleteBlank.replace("\n", "")
                deleteSemi = deleten.replace(";", "")
                deleteTable = deleteSemi.replace("\t", "")
                toinsert = list(deleteTable)
                toinsert.insert(toinsert.index("="), " ")
                toinsert.insert(toinsert.index("=")+1, " ")
                strTem = "".join(toinsert)
                ans = strTem.split(" ")
                deleDecl1 = strTem.replace("int", "")
                deleDecl2 = deleDecl1.replace("char", "")
                deleDecl3 = deleDecl2.replace("unsigned", "")
                ans = deleDecl3.split(" ")

                if (ans[0] in varnames) and ((ans[2] in varnames) or (ans[2].startswith('-') and ans[2][1:] or ans[2]).isdigit()):
                    detectResult = detect(findVariable(ans[0], variable), findVariable(ans[2], variable))
                    if detectResult == "Overflow":
                        warings.append(("Line %d:\t" % (j+1) + programLine.replace("\n", "").strip() + "\n" + detectResult))
                else:
                    continue
    return warings


def findVariable(name, variable):
    if (name.startswith('-') and name[1:] or name).isdigit():
        return name
    else:
        for i in variable:
            if i.get_name() == name:
                return i


def detect(var1, var2):
    types = {"char": 1, "short": 2, "unsigned short": -2, "int": 3, "unsigned int": -3}
    if var1.get_type() == "char":
        if type(var2) != Variable:
            if int(var2)> 127 or int(var2) < -128:
                return "Overflow"
            else:
                return "OK"
        else:
             if abs(types[var2.get_type()]) >= abs(types[var1.get_type()]):
                   if types[var2.get_type()] == types[var1.get_type()]:
                         return "OK"
                   else:
                        return "Overflow"
             else:
                 return "OK"


    elif var1.get_type() == "short":
        if type(var2) != Variable:
            if int(var2)> 32767 or int(var2)< -32768:
                return "Overflow"
            else:
                return "OK"
        else:
            if abs(types[var2.get_type()]) >= abs(types[var1.get_type()]):
                if types[var2.get_type()] == types[var1.get_type()]:
                    return "OK"
                else:
                    return "Overflow"
            else:
                return "OK"

    elif var1.get_type() == "unsigned short":
        if type(var2) != Variable:
            if int(var2) > 65535 or int(var2) < 0:
                return "Overflow"
            else:
                return "OK"
        else:
            if abs(types[var2.get_type()]) >= abs(types[var1.get_type()]):
                if types[var2.get_type()] == types[var1.get_type()]:
                    return "OK"
                else:
                    return "Overflow"
            else:
                return "OK"

    elif var1.get_type() == "int":
        if type(var2) != Variable:
            if int(var2) > 2147483647 or int(var2) < -2147483648:
                return "Overflow"
            else:
                return "OK"
        else:
            if abs(types[var2.get_type()]) >= abs(types[var1.get_type()]):
                if types[var2.get_type()] == types[var1.get_type()]:
                    return "OK"
                else:
                    return "Overflow"
            else:
                return "OK"

    elif var1.get_type() == "unsigned int":
        if type(var2) != Variable:
            if int(var2) > 4294967295 or int(var2) < 0:
                return "Overflow"
            else:
                return "OK"
        else:
            if abs(types[var2.get_type()]) >= abs(types[var1.get_type()]):
                if types[var2.get_type()] == types[var1.get_type()]:
                    return "OK"
                else:
                    return "Overflow"
            else:
                return "OK"


def analyseCodersensor(filename):
    filepath = "midfile\\"+filename
    analysis = open(filepath, "r")
    functionBeginEnd = []
    variables = []
    funcindex = 0
    for line in analysis:
        result = line.split("\t")
        if result[0] == "func":
            funcindex = funcindex + 1
            start = int(result[1].split(":")[0])
            end = int(result[2].split(":")[0])
            functionBeginEnd.append((start, end))
            continue
        elif result[0] == 'param' or result[0] == 'decl':
            name = result[-1].replace("\n", "")
            line = int(result[-5].split(":")[0])
            var = Variable(result[-2], name, funcindex)
            var.set_line(line)
            variables.append(var)
    return functionBeginEnd, variables


if __name__ == "__main__":
    warnings = detectIntegerOverflow("it1.txt")
    for i in warnings:
        print(i)
