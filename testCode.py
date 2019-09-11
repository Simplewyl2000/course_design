import os

filepath = "sample\\" + "100.txt"
command = "java -jar codersensor\\CodeSensor.jar " + filepath + " >midFile\\widthDetect.txt"
os.system(command)


filepath = "midfile\\widthDetect.txt"
analysis = open(filepath, "r")
functionBeginEnd = []
variables = []
functionName = []
functionLine = []
for line in analysis:
    result = line.split("\t")
    if result[0] == "func" or result[0]=="call":
        functionName.append(result[-1].replace("\n", ""))
        functionLine.append(int(result[1].split(":")[0]))

print(functionName)
print(functionLine)

