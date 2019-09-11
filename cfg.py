import analysis
import numpy as np
import networkx as nx
import matplotlib.pyplot as plt


class Function:

    __functionLine = 0
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
        return self.__functionLine

    def get_whereFunctionEnds(self):
        return self.__whereFunctionEnds

    def get_childFunction(self):
        return self.__containFunctions

    def get_functionLine(self):
        return self.__functionLine


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

    def set_functionLine(self,a):
        self.__functionLine = a

    def append_childFunction(self,fun):
         self.__containFunctions.append(fun)



def find_functions(filename):

    filepath = "sample\\" + filename
    functionNameList = []
    functionLineList = []
    functionBodyNameList = []
    functionBodyStartList = []
    functionBodyEndList =[]

    functionNameList,functionLineList,functionBodyNameList,functionBodyStartList,functionBodyEndList = analysis.processProgram(filepath)

    functionList = []
    functionBodyList = []

    for i in range(len(functionNameList)):
        functem = Function()
        functem.set_functionName(functionNameList[i])
        functem.set_functionLine(int(functionLineList[i]))
        functem.set_functionID(i)
        functem.set_isFunctionBody(False)
        functionList.append(functem)

    for i in range(len(functionBodyNameList)):
        functem = Function()
        functem.set_isFunctionBody(True)
        functem.set_whereFunctionEnds(int(functionBodyEndList[i]))
        functem.set_functionLine(int(functionBodyStartList[i]))
        functem.set_functionName(functionBodyNameList[i])
        functionBodyList.append(functem)

    return functionList, functionBodyList


def martrix_build(file):
    functionList, functionBodyList = find_functions(file)
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
                    a[x, functionDic[j.get_functionName()]] = a[x, functionDic[j.get_functionName()]] + 1
                    edges.append((functionDic[i.get_functionName()], functionDic[j.get_functionName()]))

    return a, edges, nodes


def draw_cfg(edges, nodes, filename):
    filepath = "result//cfg//" + filename + ".png"
    G = nx.DiGraph()
    G.add_nodes_from(nodes)
    G.add_edges_from(edges)
    nx.draw_shell(G)
    plt.savefig(filepath)
    plt.show()


def function_set(funclist):
    functiondic = {}
    functionset = []
    funcNewId = 0
    for i in funclist:
        functionset.append(i.get_functionName())
    functionset = set(functionset)
    functionset = list(functionset)
    functionset.sort()
    for i in functionset:
        functiondic[i] = funcNewId
        funcNewId = funcNewId + 1

    return functionset, functiondic


if __name__ == "__main__":
   print(martrix_build("100.txt"))







