import os


def processProgram(filepath):
    command = "java -jar codersensor\\CodeSensor.jar " + filepath + " >midFile\\haha.txt"
    filepath = "midfile\\haha.txt"
    os.system(command)
    analyse = open(filepath, "r")
    functionNamehaha = []
    functionLinehaha = []
    functionBodyhaha = []
    functionBodyEndhaha = []
    functionBodyStarthaha = []

    for line in analyse:
        result = line.split("\t")
        if result[0] == "func" or result[0] == "call":
            functionNamehaha.append(result[-1].replace("\n", ""))
            functionLinehaha.append(int(result[1].split(":")[0]))
        if result[0] == "func":
            functionBodyhaha.append(result[-1].replace("\n", ""))
            functionBodyStarthaha.append(result[-5].split(":")[0])
            functionBodyEndhaha.append(result[-4].split(":")[0])


    return functionNamehaha,functionLinehaha,functionBodyhaha,functionBodyStarthaha,functionBodyEndhaha

if __name__ == '__main__':
    processProgram("sample\\100.txt")
