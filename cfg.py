import readAnalysis
import numpy as np
import networkx as nx
import matplotlib.pyplot as plt


functionList = []
functionBodyList = []
colors = ["blue","red","green","black","pink","purple","orange","yellow"]


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


def find_sub_list(sl, l, preresult, isBody,id):
    global functionList
    functionNameInputFile = open("midFile\\functionName" + str(id) + ".txt")
    functionNameInput = functionNameInputFile.read()
    functionNameList = functionNameInput.split(" ")
    functionNameInputFile.close()


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
                        functiontem.set_functionName(functionNameList[i.get_functionID() - 1])

                results.append(functiontem)
    else:
        for ind in (i for i, e in enumerate(l) if e==sl[0]):
            if l[ind:ind+sll] == sl:
                id = id + 1
                functiontem = Function()
                functiontem.set_functionID(id)
                functiontem.set_functionName(functionNameList[id - 1])
                functiontem.set_isFunctionBody(isBody)
                functiontem.set_whereFunctionStarts(ind)
                results.append(functiontem)

    return results


def find_functions(tokenList):
    global functionList
    global functionBodyList

    functionList =find_sub_list(["ID", "("], tokenList, functionList, False, 1)
    functionBodyList = find_sub_list(["INT", "ID", "("], tokenList, functionBodyList, True, 1)
    functionBodyList = find_sub_list(["CHAR", "ID", "("], tokenList, functionBodyList, True, 1)
    functionBodyList = find_sub_list(["VOID", "ID", "("], tokenList, functionBodyList, True, 1)
    for i in functionBodyList:
        i.find_functionEnd(tokenList, i.get_whereFunctionStarts())


def martrix_build(file, id):
    tokenList = readAnalysis.get_tokenList(file, id)
    find_functions(tokenList)
    functionSet, functionDic = function_set(functionList)
    a = np.zeros([len(functionBodyList), len(functionSet)], dtype=np.int)
    x = -1
    edges = []
    nodes = []
    for i in functionDic:
        nodes.append(functionDic[i])


    for i in functionBodyList:
        x = x + 1
        for j in functionList:
            if i.get_whereFunctionStarts() < j.get_whereFunctionStarts() < i.get_whereFunctionEnds():
                if i.get_whereFunctionStarts() + 1 != j.get_whereFunctionStarts():
                    a[x, functionDic[j.get_functionName()]] = a[x, functionDic[j.get_functionName()]] + 1
                    edges.append((functionDic[i.get_functionName()], functionDic[j.get_functionName()]))

    return a, edges, nodes


def draw_cfg(edges, nodes):
    G = nx.DiGraph()
    G.add_nodes_from(nodes)
    G.add_edges_from(edges)
    nx.draw(G)
    plt.savefig("youxiangtu.png")
    plt.show()


def function_set(funclist):
    functiondic = {}
    functionset = []
    funcNewId = 0
    for i in funclist:
        functionset.append(i.get_functionName())
    functionset = set(functionset)
    functionset = list(functionset)
    for i in functionset:
        functiondic[i] = funcNewId
        funcNewId = funcNewId + 1

    return functionset, functiondic


if __name__ == "__main__":
    tokenList = readAnalysis.get_tokenList("1.txt", 1)
    find_functions(tokenList)
    functionSet, functionDic = function_set(functionList)
    print(functionDic)
    print(functionSet)









