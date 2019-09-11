import cfg
import numpy as np


def buildEachMatrix(file1, file2, id1, id2):
    matrix1, edge1, nodes1 = cfg.martrix_build(file1, id1)
    matrix2, edge2, nodes2 = cfg.martrix_build(file2, id2)
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


def compareMatrix(file1, file2, id1, id2):
    matrix1, matrix2 = buildEachMatrix(file1, file2, id1, id2)
    distance = 0
    distanceMin = 1000000
    distanceInAll = 0

    for i in range(matrix1.shape[0]):
        for j in range(matrix2.shape[0]):
            distance = np.linalg.norm(matrix1[i]-matrix2[j])
            if distance < distanceMin:
                distanceMin = distance

        distanceInAll = distanceInAll + distanceMin
    return distanceInAll







