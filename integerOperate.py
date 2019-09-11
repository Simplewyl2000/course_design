import os
import re
from integerOverflowDetect import *


def detectIntegerOperateFlow(filename):
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
        localVaribal = []
        for j in variable:
            if j.get_belongToFunction() == funcindex:
                varnames.append(j.get_name())

        for n in variable:
            if n.get_belongToFunction() == funcindex:
                localVaribal.append(n)

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
                deleDecl1 = strTem.replace("int", "")
                deleDecl2 = deleDecl1.replace("char", "")
                deleDecl3 = deleDecl2.replace("unsigned", "")
                deleDecl4 = deleDecl3.replace("short","")
                ans = deleDecl4.split(" ")
                for i in localVaribal:
                    if i.get_name() == ans[0]:
                        if i.get_type() == "int" or i.get_type() == "short" or i.get_type() == "unsigned short" or i.get_type() == "unsigned int":
                            if conferValue(ans, localVaribal) == "Haven't  define the value before operate!":
                                warings.append("Line %d:\t" %(j+1) + programLine + "Using undefined variable!")
                        else:
                            continue
                    else:
                        continue

        for i in localVaribal:
            if i.get_type() == "int" or i.get_type() == "short" or i.get_type() == "unsigned short" or i.get_type() == "unsigned int":
                if detect(i) == "OK":
                    continue
                elif detect(i) == "Overflow":
                    warings.append("Line %d\t" % (i.get_line()) + "arg:%s\n" %(i.get_name()) + "Overflow!")

    return warings


def detect(variable):
    if variable.get_value() == 79916:
        return "Undefined Variable!"
    if variable.get_type() == "short":
        if -32768 < variable.get_value() < 32767:
            return "OK"
        else:
            return "Overflow"
    elif variable.get_type() == "unsigned short":
        if 0 < variable.get_value() < 65535:
            return "OK"
        else:
            return "Overflow"
    elif variable.get_type() == "int":
        if -2147483648 < variable.get_value() < 2147483647:
            return "OK"
        else:
            return "Overflow"
    elif variable.get_type() == "unsigned int":
        if 0 < variable.get_value() < 4294967295:
            return "OK"
        else:
            return "Overflow"





def conferValue(ans, varibal):
    if (ans[2].startswith('-') and ans[2][1:] or ans[2]).isdigit():
        for i in varibal:
            if i.get_name() == ans[0]:
                i.set_value(int(ans[2]))
    else:
        newans2 = ans[2]
        if re.search(r"[\w\d]\(",newans2):
            return
        for i in varibal:
            if i.get_name() in newans2:
                if i.get_value() == 79916:
                    return "Haven't  define the value before operate!"
                else:
                    newans2 = newans2.replace(i.get_name(), str(i.get_value()))

        a = eval(newans2)
        for i in varibal:
            if i.get_name() == ans[0]:
                i.set_value(a)


if __name__ == "__main__":
    print(detectIntegerOperateFlow("io.txt"))
