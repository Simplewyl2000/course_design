import readAnalysis
import cfg
import re

sensitiveFunctions = ["memcpy", "memncpy", "strcat", "strncat", "sprintf", "vsprintf", "gets", "getchar", "fgetc", "getc", "read", "sscanf", "fscanf", "vfscanf", "vscanf", "vsscanf"]

def findStrcpy(fileinput,tokenList,id):

    program = fileinput
    functionList, functionBodyList = cfg.find_functions(tokenList, id)
    lineStart = 0
    lineStrcpy = 0
    programLine = ""
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

    if a != "not define the %s in line %d, it is not safe" % (argv1, lineStrcpy):
        sizeOfargv1 = a
    else:
        return a

    if "\"" in argv2:
        sizeOfargv2 = len(argv2)-2

    else:
        b = findTheSize(program, argv2, lineStart, lineStrcpy)
        if b != "not define the %s in line %d, it is not safe" % (argv2, lineStrcpy):
            sizeOfargv2 = b
        else:
            return b

    if sizeOfargv1 >= sizeOfargv2:
        return "The strcpy is safe in line %d!" %(lineStrcpy)
    else:
        return "This strcpy in line %d is not safe! The size of argv2 is bigger than argv1!" %(lineStrcpy)


def findStrncpy(fileinput,tokenList,id):

    program = fileinput
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

    if a != "not define the %s in line %d, it is not safe" % (argv1, lineStrncpy):
        sizeOfargv1 = a
    else:
        return a

    sizeOfargv3 = int(argv3)


    if sizeOfargv1 >= sizeOfargv3:
        return "The strncpy is safe in line %d!" % (lineStrncpy)
    else:
        return "This strncpy in line %d is not safe! The size of argv2 is bigger than argv1!" %(lineStrncpy)




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
        return "not define the %s in line %d, it is not safe" % (arg, end)
    else:
        return int(size)



def findTheSensitive(fileInput,tokenList,id):

    program = fileInput
    functionList, functionBodyList = cfg.find_functions(tokenList, id)
    warning =[]
    for i in functionList:
        if i.get_functionName() in sensitiveFunctions:
            warning.append((i.get_functionName(), i.get_functionLine()))

    if len(warning) != 0:
        for i in warning:
            print("%s may be dangerous in line %d" % (i[0], i[1]))
    else:
        print("no sensitive function")


def detectStackOverflow(filename, id):
    filepath = "sample\\" + filename
    fileinput = open(filepath, "r",encoding="utf-8")
    tokenList = readAnalysis.get_tokenList(filename, id)
    print(findStrcpy(fileinput, tokenList, id))
    print(findStrncpy(fileinput, tokenList, id))
    findTheSensitive(fileinput, tokenList, id)


if __name__ == "__main__":
    detectStackOverflow("1.txt", 1)
