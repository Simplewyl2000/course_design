#!/usr/bin/python
# -*- coding: UTF-8 -*-

import re


'''
words_count函数用于统计代码中出现的各类符号的次数
实现了对宏定义进行替换的功能

'''


def words_conut(programLine, base, preDefine, sequence):
    programLine = re.sub('\"([^\"]*)\"', ' ', programLine, 0, 0)              # 去双引号
    programLine = re.sub('/\*(?<=/\*).+?(?=\*/)\*/', ' ', programLine, 0, 0)  # 去注释/**/
    programLine = re.sub('//.*', ' ', programLine, 0, 0)                      # 去注释//
    programLine = programLine.replace('"', ' ')
    programLine = programLine.replace(')', ' ')
    programLine = programLine.replace('(', ' ')
    programLine = programLine.replace(',', ' ')
    programLine = programLine.replace(':', ' ')
    programLine = programLine.replace('}', ' ')
    programLine = programLine.replace('{', ' ')                               # 对统计产生影响的特殊符号
    li = programLine.split()  # 以空格划分
    if len(li) >= 3 and (li[0] == '#define' or li[0] == '#DEFINE'):
        preDefine.append([li[2], li[1]])
        for keyWord in base:
            if keyWord[0] == li[2]:
                keyWord[1] = keyWord[1] + 1
                
    elif len(li) >= 3 and li[0] == 'typedef':
        for keyWord in base:
            if keyWord[0] == li[1]:
                keyWord[1] = keyWord[1] + 1
        preDefine.append([li[1], li[2]])                                 #此时对已经有了对于变量进行替换的list，可以进行宏替换提高准确率
    
    for word1 in li:
        flag = 0
        for wordPreDefined in preDefine:  # 宏替换
            if wordPreDefined[1] == word1:
                word1 = wordPreDefined[0]
        for word2 in base:
            if word2[0] == word1:
                word2[1] = word2[1] + 1
                sequence.append(word2[0])
                flag = 1
        if flag == 0 :
            sequence.append("variable")
                                           
    return base


'''
copyRate_count
对统计好的两个文件进行拷贝率的计算
'''


def copyRate_count(filename1, filename2, outfile):

    countBase = [['int', 0], ['long', 0], ['short', 0], ['float', 0], ['double', 0], ['char', 0], ['unsigned', 0], ['signed', 0], ['const', 0], ['void', 0], ['volatile', 0], ['enum', 0], ['struct', 0], ['union', 0],
           ['if', 0], ['else', 0], ['goto', 0], ['switch', 0], ['case', 0], ['do', 0], ['while', 0], ['for', 0], ['continue', 0], ['break', 0], ['return', 0], ['default', 0], ['auto', 0], ['register', 0], ['extern', 0],
           ['static', 0], ['sizeof', 0], ['printf', 0], ['scanf', 0], ['fopen', 0], ['fclose', 0], ]
    
    preDefine1 = []
    preDefine2 = []
    sequence1 = []
    sequence2 = []
    file = open(filename1)

    while 1:
        programLine = file.readline()
        if not programLine:
            break
        pass
        countBase = words_conut(programLine, countBase, preDefine1, sequence1)
        
    file.close()

    res2 = [['int', 0], ['long', 0], ['short', 0], ['float', 0], ['double', 0], ['char', 0], ['unsigned', 0],
           ['signed', 0], ['const', 0], ['void', 0], ['volatile', 0], ['enum', 0], ['struct', 0], ['union', 0],
           ['if', 0], ['else', 0], ['goto', 0], ['switch', 0], ['case', 0], ['do', 0], ['while', 0], ['for', 0],
           ['continue', 0], ['break', 0], ['return', 0], ['default', 0], ['auto', 0], ['register', 0], ['extern', 0],
           ['static', 0], ['sizeof', 0], ['printf', 0], ['scanf', 0], ['fopen', 0], ['fclose', 0], ]

    preDefine2 = []


    file = open(filename2)
    while 1:
        programLine = file.readline()
        if not programLine:
            break
        pass
        res2 = words_conut(programLine, res2, preDefine2,sequence2)
        # print countBase
    file.close()
 #至此，统计结束，开始进行拷贝率的计算
    i = 0
    count = 0
    sum = 0
    repeatRate = 0.0
    while i < len(countBase):
        if not (countBase[i][1] == 0 and res2[i][1] == 0):
            sum = sum + 1
            if abs((countBase[i][1] - res2[i][1]) / (countBase[i][1] + res2[i][1])) < 0.25:
                count = count + 1
        i = i + 1
    if sum != 0:
        repeatRate = 1.0 * count / sum * 100

    #计算最长相似字串的

    str1 = "-"
    programSequence1 = str1.join(sequence1)
    programSequence2 = str1.join(sequence2)
    lcsubstr = find_lcsubstr(programSequence1,programSequence2)
    print(lcsubstr[1])






    copyrateDocument = open(outfile, 'a')
    copyrateDocument.write(filename2 + '  ' + 'copyRate:'+ str(repeatRate) + '%  ' + "sequence similarity" + " " + str(100*(lcsubstr[1])/min(len(programSequence1), len(programSequence2))) + "%")
    if repeatRate >= 70.0:
        copyrateDocument.write('  *\n')
    else:
        copyrateDocument.write('\n')
    copyrateDocument.close()


def find_lcsubstr(s1, s2):
    m=[[0 for i in range(len(s2)+1)]  for j in range(len(s1)+1)]  #生成0矩阵，为方便后续计算，比字符串长度多了一列
    mmax=0   #最长匹配的长度
    p=0  #最长匹配对应在s1中的最后一位
    for i in range(len(s1)):
        for j in range(len(s2)):
            if s1[i]==s2[j]:
                m[i+1][j+1]=m[i][j]+1
                if m[i+1][j+1]>mmax:
                    mmax=m[i+1][j+1]
                    p=i+1
    return s1[p-mmax:p],mmax   #返回最长子串及其长度





if __name__ == '__main__':
    copyRate_count("1.txt", "1.txt", "3.txt");
