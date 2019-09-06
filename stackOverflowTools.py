import readAnalysis
import cfg
import re

sensitiveFunctions = ["memcpy", "memncpy", "strcat", "strncat", "sprintf", "vsprintf", "gets", "getchar", "fgetc", "getc", "read", "sscanf", "fscanf", "vfscanf", "vscanf", "vsscanf"]

def findStrcpy(filename, id):
    programpath = "sample//" + filename
    program = open(programpath, "r", encoding="UTF-8")
    tokenList = readAnalysis.get_tokenList(filename,id)
    functionList, functionBodyList = cfg.find_functions(tokenList,id)
    lineStart = 0
    lineStrcpy = 0
    programLine =""
    flag = 1

    for i in functionList:
        if i.get_functionName() == "strcpy":
            flag = 0
            lineStrcpy = i.get_functionLine()
            for j in functionBodyList:
                if j.get_whereFunctionStarts() < i.get_whereFunctionStarts():
                    lineStart = j.get_functionLine()

    if flag:
        return "No strcpy"



    for i in range(lineStrcpy-1):
        program.readline()

    programLine = program.readline()
    start1, end1 = re.search(r"\bstrcpy\([0-9a-zA-Z_]*,\"*[0-9a-zA-Z_]*\"*\)", programLine).span()
    arguement = programLine[start1+7:end1-1]
    argv1 = arguement[:arguement.index(",")]
    argv2 = arguement[arguement.index(",")+1:]
    program.seek(0)
    a = findTheSize(program, argv1, lineStart, lineStrcpy)

    if a != "not define the argv, it is not safe":
        sizeOfargv1 = a
    else:
        return a

    if "\"" in argv2:
        sizeOfargv2 = len(argv2)-2

    else:
        b = findTheSize(program, argv2, lineStart, lineStrcpy)
        if b != "not define the argv, it is not safe":
            sizeOfargv2 = b
        else:
            return b

    if sizeOfargv1 >= sizeOfargv2:
        return "It is safe!"
    else:
        return "Not safe! The size of argv2 is bigger than argv1!"


def findStrncpy(filename,id):
    programpath = "sample//" + filename
    program = open(programpath, "r", encoding="UTF-8")
    tokenList = readAnalysis.get_tokenList(filename, id)
    functionList, functionBodyList = cfg.find_functions(tokenList, id)
    lineStart = 0
    lineStrncpy = 0
    programLine = ""
    flag = 1

    for i in functionList:
        if i.get_functionName() == "strncpy":
            flag = 0
            lineStrncpy = i.get_functionLine()
            for j in functionBodyList:
                if j.get_whereFunctionStarts() < i.get_whereFunctionStarts():
                    lineStart = j.get_functionLine()

    if flag:
        return "No strncpy"

    for i in range(lineStrncpy - 1):
        program.readline()

    programLine = program.readline()
    start, end = re.search(r"\bstrncpy\([0-9a-zA-Z_]*,\"*[0-9a-zA-Z_]*\"*,\d+\)", programLine).span()
    arguement = programLine[start+8:end-1]
    argv1 = arguement[:arguement.index(",")]
    argv2Andargv3 = arguement[arguement.index(",") + 1:]
    argv2 = argv2Andargv3[:argv2Andargv3.index(",")]
    argv3 = argv2Andargv3[argv2Andargv3.index(",")+1:]


    program.seek(0)
    a = findTheSize(program, argv1, lineStart, lineStrncpy)

    if a != "not define the argv, it is not safe":
        sizeOfargv1 = a
    else:
        return a

    sizeOfargv3 = int(argv3)


    if sizeOfargv1 >= sizeOfargv3:
        return "It is safe!"
    else:
        return "Not safe! The size of argv2 is bigger than argv1!"




def findTheSize(file, arg, begin, end):
    file.seek(0)
    flag = 1
    for i in range(begin):
        programLine = file.readline()

    for i in range(end - begin - 1):
        programLine = file.readline()
        if re.search(r'\b%s\[\d\b' % (arg), programLine) != None:
            flag = 0
            start, end = re.search(r'\b%s\[\d\b' % (arg), programLine).span()
            size = programLine[start+len(arg)+1]
    if flag:
        return "not define the argv, it is not safe"
    else:
        return int(size)


if __name__ == "__main__":
    print(findStrncpy("7.txt", 7))
