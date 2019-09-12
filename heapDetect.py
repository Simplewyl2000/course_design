import cfg
import re


def findStrcpy(fileinput, filename):
    program = fileinput
    functionList, functionBodyList = cfg.find_functions(filename)
    firstFunc = functionBodyList[0].get_whereFunctionStarts()
    glvar = findGlobal(fileinput, firstFunc)


    lineStart = 0
    lineStrcpy = 0
    programLine = ""
    warnings = []

    for i in functionList:
        if i.get_functionName() == "strcpy":

            lineStrcpy = i.get_functionLine()
            for j in functionBodyList:
                if j.get_whereFunctionStarts() < i.get_whereFunctionStarts():
                    lineStart = j.get_functionLine()

            program.seek(0)
            for i in range(lineStrcpy - 1):
                program.readline()

            programLine = program.readline()
            start1, end1 = re.search(r"\bstrcpy\([0-9a-zA-Z_]*,\"*[0-9a-zA-Z_]*\"*\)", programLine).span()
            arguement = programLine[start1 + 7:end1 - 1]
            argv1 = arguement[:arguement.index(",")]
            argv2 = arguement[arguement.index(",") + 1:]
            program.seek(0)
            a = findTheSize(program, argv1, glvar, lineStart, lineStrcpy)

            if a != "not define the %s in line %d, it is not safe" % (argv1, lineStrcpy):
                sizeOfargv1 = a
            else:
                warnings.append(a)

            if "\"" in argv2:
                sizeOfargv2 = len(argv2) - 2

            else:
                b = findTheSize(program, argv2, glvar, lineStart, lineStrcpy)
                if b != "not define the %s in line %d, it is not safe" % (argv2, lineStrcpy):
                    sizeOfargv2 = b
                    if sizeOfargv1 >= sizeOfargv2:
                        warnings.append("The strcpy is safe in line %d!" % (lineStrcpy))
                    else:
                        warnings.append("Line %d" % (lineStrcpy) + programLine + "\n" +
                            "This strcpy in line %d is not safe! The size of argv2 is bigger than argv1!" % (
                                lineStrcpy))
                else:
                    warnings.append(b)

    return warnings


def findTheSize(file, arg, glvar, begin, ends):
    file.seek(0)
    flag = 1


    for i in glvar:
        if i[0] == arg:
            size = i[1]
            return int(size)
    i = 0

    for i in range(begin):
        programLine = file.readline()

    for i in range(ends - begin - 1):
        programLine = file.readline()
        if re.search(r'%s\[\d+\]' % (arg), programLine) != None:
            flag = 0
            size = programLine[programLine.index("[") + 1:programLine.index("]")]

    file.seek(0)





    for programLine in file:
        i = i+1
        a= re.search(r'%s[\s\S]*=[\s\S]*\(*[a-zA-Z]*\)*[\s\S]*(malloc|HeapAlloc)\([\s\S]*[a-zA-Z]*[\s\S]*,[\s\S]*\d*[\s\S]*,[\s\S]*\d*[\s\S]*\)' % (arg), programLine)
        if a != None:
            flag = 0
            start, end = re.search(r'%s[\s\S]*=[\s\S]*\(*[a-zA-Z]*\)*[\s\S]*(malloc|HeapAlloc)\([\s\S]*[a-zA-Z]*[\s\S]*,[\s\S]*\d*[\s\S]*,[\s\S]*\d*[\s\S]*\)' % (arg), programLine).span()
            left = programLine[:end-1]
            size = left[left.rfind(",")+1:]
    if flag:
        return "not define the %s in line %d, it is not safe" % (arg, ends)
    else:
        return int(size)


def findGlobal(fileinput,firstFunc):
    k = 0
    glvar = []
    for k in range(firstFunc):
        programLine = fileinput.readline()
        a = re.search(r"\[\d+\]", programLine)
        if a:
            ans = programLine.strip().split(" ")
            ans2 = ans[1].split("[")
            glname = ans2[0]
            ans3 = ans2[1].split("]")
            glsize = ans3[0]
            glvar.append((glname, glsize))
    return glvar


def detectHeapOverflow(filename):
    filepath = "sample\\" + filename
    fileinput = open(filepath, "r", encoding="utf-8")

    warnings = findStrcpy(fileinput, filename)
    return warnings


if __name__ == "__main__":
    print(detectHeapOverflow("s4.txt"))
