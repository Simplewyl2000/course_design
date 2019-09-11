import readAnalysis
import cfg
import re


def findPrintf(fileinput,filename):

    program = fileinput
    functionList, functionBodyList = cfg.find_functions(filename)
    lineStart = 0
    linePrintf = 0
    argvindex = 0
    programLine = ""
    flag = 1
    warnings = []


    for i in functionList:
        program.seek(0)
        argvindex = 0
        isSafe = True
        if i.get_functionName() == "printf":
            flag = 0
            linePrintf = i.get_functionLine()
            for j in functionBodyList:
                if j.get_whereFunctionStarts() < i.get_whereFunctionStarts():
                    lineStart = j.get_functionLine()

            for i in range(linePrintf - 1):
                k = program.readline()

            programLine = program.readline()

            for i in range(len(programLine)):
                if programLine[i] == '"':
                    start = i
                    break

            for i in range(len(programLine)):
                if programLine[i] == '"':
                    end = i

            formatString = programLine[start + 1:end]
            for i in range(end, len(programLine)):
                if programLine[i] == ')':
                    endOfFunction = i

            whatLeft = programLine[end + 1:endOfFunction]
            args = whatLeft.split(",")
            del args[0]

            i = 0
            for i in range(len(formatString)):
                k = formatString[i]
                if formatString[i] == '%' and formatString[i - 1] != "\\":
                    if formatString[i + 1] == 'd' or (formatString[i+1].isdigit() and formatString[i+2] == 'd'):
                        if argvindex >= len(args):
                            warnings.append("Not safe, the number of args of printf in line %d is not matched！" %linePrintf)
                            isSafe = False
                        else:
                            if matchType(fileinput, args[argvindex].strip(), lineStart, linePrintf) == "int":
                                argvindex = argvindex + 1
                                continue
                            elif matchType(fileinput, args[argvindex].strip(), lineStart,
                                           linePrintf) == "not define the %s in line %d, it is not safe" % (
                            args[argvindex].strip(), end):
                                warnings.append("not define the %s in line %d, it is not safe" % (args[argvindex].strip(), end))
                                isSafe = False
                            else:
                                warnings.append("The argv does not match the type in line %d, it should be an int argv" %(linePrintf))
                                isSafe = False
                        argvindex = argvindex + 1
                    elif formatString[i + 1] == 's':
                        if argvindex >= len(args):
                            warnings.append("Not safe, the number of args of printf in line %d is not matched！" %linePrintf)
                            isSafe = False
                        else:
                            if matchType(fileinput, args[argvindex].strip(), lineStart, linePrintf) == "chs":
                                argvindex = argvindex + 1
                                continue
                            elif matchType(fileinput, args[argvindex].strip(), lineStart,
                                           linePrintf) == "not define the %s in line %d, it is not safe" % (
                            args[argvindex].strip(), end):
                                warnings.append( "not define the %s in line %d, it is not safe" % (args[argvindex].strip(), end))
                                isSafe = False
                            else:
                                warnings.append("The argv does not match the type in line %d, it should be an char* argv" %(linePrintf))
                                isSafe = False
                        argvindex = argvindex + 1
                    elif formatString[i + 1] == 'c':
                        if argvindex >= len(args):
                            warnings.append("Not safe, the number of args of printf in line %d is not matched！" %linePrintf)
                            isSafe = False
                        else:
                            if matchType(fileinput, args[argvindex].strip(), lineStart, linePrintf) == "cha":
                                argvindex = argvindex + 1
                                continue
                            elif matchType(fileinput, args[argvindex].strip(), lineStart,
                                           linePrintf) == "not define the %s in line %d, it is not safe" % (
                            args[argvindex].strip(), end):
                                warnings.append("not define the %s in line %d, it is not safe" % (args[argvindex].strip(), end))
                                isSafe = False
                            else:
                                warnings.append("The argv does not match the type in line %d, it should be an char argv" %(linePrintf))
                                isSafe = False
                        argvindex = argvindex + 1
            if isSafe:
                warnings.append("The printf in line %d is safe, don't worry!" % (linePrintf))



    if flag:
       warnings.append("No printf")


    return warnings


def matchType(file, arg, begin, end):
    file.seek(0)
    flag = 1

    if '(char)' in arg:
        return 'cha'
    elif "(int)" in arg:
        return 'int'
    elif "(char*)" in arg:
        return "chs"



    for i in range(begin):
        programLine = file.readline()

    for i in range(end - begin - 1):
        programLine = file.readline()

        if re.search(r'[(int)(char)][\s\S]*%s' % (arg), programLine) != None:
            flag = 0
            start, end = re.search(r'[(int)(char)][\s\S]*%s' % (arg), programLine).span()
            type = programLine[start:start+3]
            if type == 'cha':
                if '[' in programLine:
                    type = 'chs'
                    return type
                else:
                    return type

            return type
        else:
            continue

    if flag:
        return "not define the %s in line %d, it is not safe" % (arg, end)

def detectPrintf(filename):
    filepath = "sample\\" + filename
    f = open(filepath, "r", encoding="utf-8")

    warnings = findPrintf(f, filename)
    return warnings

if __name__ == "__main__":

    warnings = detectPrintf("p1.txt")



    for i in warnings:
        print(i)

