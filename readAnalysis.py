import analysis
program = ""


def get_tokenList(filename, id):
    samplePath = "sample\\" + filename
    analysisFile = "midFile\\" + "token" + str(id) + ".txt"

    analysis.processProgram(samplePath,id)
    with open(analysisFile) as f:
         program = f.read()
         tokenList = program.split(" ")

    return tokenList



if __name__ == "__main__":
    tokenList = get_tokenList("1.txt", 2)
    print(tokenList)
