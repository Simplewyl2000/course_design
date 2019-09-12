import cfg
import re
import integerOverflowDetect
import os

sensitiveFunctions = ["memcpy", "memncpy", "strcat", "strncat", "sprintf", "vsprintf", "gets", "getchar", "fgetc",
                      "getc", "read", "sscanf", "fscanf", "vfscanf", "vscanf", "vsscanf"]


def findMemcpy(fileinput, filename):
    program = fileinput
    functionList, functionBodyList = cfg.find_functions(filename)
    lineStart = 0
    lineMemcpy = 0
    programLine = ""
    warnings = []

    for i in functionList:
        if i.get_functionName() == "memcpy":
            flag = 0
            lineMemcpy = i.get_functionLine()
            for j in functionBodyList:
                if j.get_whereFunctionStarts() < i.get_whereFunctionStarts():
                    lineStart = j.get_functionLine()

            for i in range(lineMemcpy - 1):
                program.readline()

            programLine = program.readline()
            start, end = re.search(r"\bmemcpy\([0-9a-zA-Z_]*,\"*[0-9a-zA-Z_]*\"*,\w+\)", programLine).span()
            arguement = programLine[start + 7:end - 1]
            argv1 = arguement[:arguement.index(",")]
            argv2Andargv3 = arguement[arguement.index(",") + 1:]
            argv2 = argv2Andargv3[:argv2Andargv3.index(",")]
            argv3 = argv2Andargv3[argv2Andargv3.index(",") + 1:]

            filepath = "sample\\" + filename
            command = "java -jar codersensor\\CodeSensor.jar " + filepath + " >midFile\\signDetect.txt"
            os.system(command)
            variables = integerOverflowDetect.analyseCodersensor("signDetect.txt")[1]
            for i in variables:
                if i.get_name() == argv3:
                    if "unsigned" in i.get_type():
                        pass
                    else:
                        warnings.append("Line %d" %(lineMemcpy) + programLine+ "\n" + "In line %d the memcpy is not safe!" % lineMemcpy)
    return warnings



def detectIntegerSign(filename):
    filepath = "sample\\" + filename
    fileinput = open(filepath, "r", encoding="utf-8")

    warnings = findMemcpy(fileinput, filename) + integerOverflowDetect.detectIntegerOverflow(filename)
    return warnings


if __name__ == "__main__":
    print(detectIntegerSign("is1.txt"))
