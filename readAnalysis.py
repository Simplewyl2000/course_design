import os

program = ""




def get_tokenList(filename, id):

    analysisFile = "token" + str(id) + ".txt"
    if os.path.exists(analysisFile):
        with open(analysisFile) as f:
            program = f.read()
            tokenList = program.split(" ")
    else:
        command = "E:\\PY2.7.13\\python.exe E:\\pycode\\sourceDetect\\analysis.py" + " " + filename + " " + str(id)
        os.system(command)
        with open(analysisFile) as f:
            program = f.read()
            tokenList = program.split(" ")

    return tokenList



if __name__ == "__main__":
    tokenList = get_tokenList("1.txt", 2)
    print(tokenList)
