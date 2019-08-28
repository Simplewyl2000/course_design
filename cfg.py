import readAnalysis


functionList = []
functionBodyList = []


class Function:

    __funtionName = ""
    __functionID = 0
    __isFunctionBody = False
    __whereFunctionStarts = 0
    __whereFunctionEnds = 0
    __containFunctions = []

    childFunction = ""

    def __init__(self):
        pass

    def find_functionEnd(self,a,begin):
        lb = 0
        rb = 0
        end = begin
        for i in a[begin:]:
            if i == "{":
                lb = lb + 1
                end = end + 1
            elif i == "}":
                rb = rb + 1
                end = end + 1
            else:
                end = end + 1

            if lb == rb and lb != 0 and rb != 0:
                self.__whereFunctionEnds = end
                break

    def get_functionName(self):
        return self.__funtionName

    def get_functionID(self):
        return self.__functionID

    def get_isFunctionBody(self):
        return self.isFunctionBody

    def get_whereFunctionStarts(self):
        return self.__whereFunctionStarts

    def get_whereFunctionEnds(self):
        return self.__whereFunctionEnds

    def get_childFunction(self):
        return self.__containFunctions

    def set_functionName(self, a):
        self.__funtionName = a

    def set_functionID(self,a):
        self.__functionID = a

    def set_isFunctionBody(self, a):
         self.isFunctionBody = a

    def set_whereFunctionStarts(self, a):
         self.__whereFunctionStarts = a

    def set_whereFunctionEnds(self, a):
         self.__whereFunctionEnds = a

    def append_childFunction(self,fun):
         self.__containFunctions.append(fun)


def find_sub_list(sl, l, preresult, isBody):
    global functionList
    id = 0
    results=preresult
    sll=len(sl)
    if isBody:
        for ind in (i for i, e in enumerate(l) if e==sl[0]):
            if l[ind:ind+sll] == sl:
                functiontem = Function()
                functiontem.set_isFunctionBody(isBody)
                functiontem.set_whereFunctionStarts(ind)
                for i in functionList:
                    if i.get_whereFunctionStarts() == ind+1:
                        functiontem.set_functionID(i.get_functionID())
                functionName = "FUNC" + str(functiontem.get_functionID())
                functiontem.set_functionName(functionName)
                results.append(functiontem)
    else:
        for ind in (i for i, e in enumerate(l) if e==sl[0]):
            if l[ind:ind+sll] == sl:
                id = id + 1
                functiontem = Function()
                functiontem.set_functionID(id)
                functionName = "FUNC" + str(functiontem.get_functionID())
                functiontem.set_functionName(functionName)
                functiontem.set_isFunctionBody(isBody)
                functiontem.set_whereFunctionStarts(ind)
                results.append(functiontem)

    return results


def find_functions(tokenList):
    global functionList
    global functionBodyList

    functionList =find_sub_list(["ID", "("], tokenList, functionList, False)
    functionBodyList = find_sub_list(["INT", "ID", "("], tokenList, functionBodyList, True)
    functionBodyList = find_sub_list(["CHAR", "ID", "("], tokenList, functionBodyList, True)
    functionBodyList = find_sub_list(["VOID", "ID", "("], tokenList, functionBodyList, True)
    for i in functionBodyList:
        i.find_functionEnd(tokenList, i.get_whereFunctionStarts())


if __name__ == "__main__":
    tokenList = readAnalysis.get_tokenList("E:\pycode\sourceDetect\sample\\1.txt", 1)
    find_functions(tokenList)
    print(tokenList)
    for i in functionList:
        print(i.get_functionName() + " " + str(i.get_whereFunctionStarts()))
    print("\n")
    for i in functionBodyList:
        print(i.get_functionName() + " " + str(i.get_whereFunctionStarts())+ " " + str(i.get_whereFunctionEnds()))

    for i in functionBodyList:
        for j in functionList:
            if i.get_whereFunctionStarts() < j.get_whereFunctionStarts() < i.get_whereFunctionEnds():
                if i.get_whereFunctionStarts()+1 != j.get_whereFunctionStarts():
                    i.childFunction = i.childFunction + j.get_functionName()

    for i in functionBodyList:
            print(i.get_functionName() + " " + str(i.get_whereFunctionStarts()) + " " + str(i.get_whereFunctionEnds()) + " contains " + i.childFunction)
