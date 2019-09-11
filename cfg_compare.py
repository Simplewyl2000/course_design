import cfg
import numpy as np
import math


def buildEachMatrix(file1, file2):
    matrix1, edge1, nodes1 = cfg.martrix_build(file1)
    matrix2, edge2, nodes2 = cfg.martrix_build(file2)
    cfg.draw_cfg(edge1, nodes1, file1)
    cfg.draw_cfg(edge2, nodes2, file2)

    print(matrix1.shape)
    print(matrix2.shape)
    if matrix1.shape[1] > matrix2.shape[1]:
        newbuildMartrix = np.zeros((matrix2.shape[0], matrix1.shape[1]), dtype=int)
        print(newbuildMartrix.shape)
        for i in range(matrix2.shape[0]):
            for j in range(matrix2.shape[1]):
                newbuildMartrix[i][j] = matrix2[i][j]
        return matrix1,newbuildMartrix

    else:

        newbuildMartrix = np.zeros((matrix1.shape[0], matrix2.shape[1]), dtype=int)
        print(newbuildMartrix.shape)
        for i in range(matrix1.shape[0]):
            for j in range(matrix1.shape[1]):
                newbuildMartrix[i][j] = matrix1[i][j]
        return newbuildMartrix, matrix2


def compareMatrix(file1, file2):
    matrix1, matrix2 = buildEachMatrix(file1, file2)

    matrix1 = np.array(matrix1)
    matrix2 = np.array(matrix2)
    distance = 0
    distanceMin = 1000000
    distanceInAll = 0
    if matrix1.shape == matrix2.shape:
        return 100


    for i in range(matrix1.shape[0]):
        distanceSum = 0
        for j in range(matrix2.shape[0]):
            distance = np.linalg.norm(matrix1[i]-matrix2[j])
            distanceSum = distanceSum + distance

        distanceSum = distanceSum/matrix1.shape[0]
        distanceInAll = distanceInAll + distanceSum

    if distanceInAll>35:
        return 0

    else:
        k = (distanceInAll/35) * (math.pi/2)
        simi = math.cos(k)*90

    return simi


print(compareMatrix("1.txt", "9.txt"))
