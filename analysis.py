# -*- coding: utf-8 -*-
'''
Created on 2012-10-18

@author: zouliping
'''
import string
import sys

_key = ("auto","break","case","char","const","continue","default",

"do","double","else","enum","extern","float","for",

"goto","if","int","long","register","return","short",

"signed","static","sizeof","struct","switch","typedef","union",

"unsigned","void","volatile","while")  # c语言的32个关键字

_abnormalChar = '@#$%^&*~' #标识符中可能出现的非法字符

_syn = ''  #单词的种别码
_p = 0  #下标
_value = ''  #存放词法分析出的单词
_content = '' #程序内容
_mstate = 0 #字符串的状态
_cstate = 0 #字符的状态
_dstate = 0 #整数和浮点数的状态
_line = 1 #代码的第几行
_mysymbol = [] #符号表

def outOfComment():
    '''去除代码中的注释'''
    global _content
    state = 0
    index = -1

    for c in _content:
        index = index + 1

        if state == 0:
            if c == '/':
                state = 1
                startIndex = index

        elif state == 1:
            if c == '*':
                state = 2
            else:
                state = 0

        elif state == 2:
            if c == '*':
                state = 3
            else:
                pass

        elif state == 3:
            if c == '/':
                endIndex = index + 1
                comment = _content[startIndex:endIndex]
                _content = _content.replace(comment,'') #将注释替换为空，并且将下标移动
                index = startIndex - 1
                state = 0

            elif c == '*':
                pass
            else:
                state = 2

def getMyProm(filename):
    '''从文件中获取代码片段'''
    global _content
    myPro = open(filename,'r')

    for line in myPro:
        if line != '\n':
            _content = "%s%s" %(_content,line.lstrip()) #效率更高的字符串拼接方法
        else:
            _content = "%s%s" %(_content,line)
    myPro.close()

def analysis(mystr):
    '''分析目标代码，生成token'''
    global _p,_value,_syn,_mstate,_dstate,_line,_cstate

    _value = ''
    ch = mystr[_p]
    _p += 1
    while ch == ' ':
        ch = mystr[_p]
        _p += 1
    if ch in string.letters or ch == '_':    ###############letter(letter|digit)*
        while ch in string.letters or ch in string.digits or ch == '_' or ch in _abnormalChar:
            _value += ch
            ch = mystr[_p]
            _p += 1
        _p -= 1

        for abnormal in _abnormalChar:
            if abnormal in _value:
                _syn = '@-6' #错误代码，标识符中含有非法字符
                break
            else:
                _syn = 'ID'

        for s in _key:
            if cmp(s,_value) == 0:
                _syn = _value.upper()               #############关键字
                break
        if _syn == 'ID':
            inSymbolTable(_value)

    elif ch == '\"':                        #############字符串
        while ch in string.letters or ch in '\"% ' :
            _value += ch
            if _mstate == 0:
                if ch == '\"':
                    _mstate = 1
            elif _mstate == 1:
                if ch == '\"':
                    _mstate = 2

            ch = mystr[_p]
            _p += 1

        if _mstate == 1:
            _syn = '@-2'     #错误代码，字符串不封闭
            _mstate = 0

        elif _mstate == 2:
            _mstate = 0
            _syn = 'STRING'

        _p -= 1

    elif ch in string.digits:
        while ch in string.digits or ch == '.' or ch in string.letters:
            _value += ch
            if _dstate == 0:
                if ch == '0':
                    _dstate = 1
                else:
                    _dstate = 2

            elif _dstate == 1:
                if ch == '.':
                    _dstate = 3
                else:
                    _dstate = 5

            elif _dstate == 2:
                if ch == '.':
                    _dstate = 3

            ch = mystr[_p]
            _p += 1

        for char in string.letters:
            if char in _value:
                _syn = '@-7' #错误代码，数字和字母混合，如12AB56等
                _dstate = 0


        if _syn != '@-7':
            if _dstate == 5:
                _syn = '@-3' #错误代码，数字以0开头
                _dstate = 0
            else:
                _dstate = 0
                if '.' not in _value:
                    _syn = 'DIGIT'               ##################digit digit*
                else:
                    if _value.count('.') == 1:
                        _syn = 'FRACTION'           ################## 浮点数
                    else:
                        _syn = '@-5' #错误代码，浮点数中包含多个点，如1.2.3
        _p -= 1


    elif ch == '\'':                    ################## 字符
        while ch in string.letters or ch in '@#$%&*\\\'\"':
            _value += ch
            if _cstate == 0:
                if ch == '\'':
                    _cstate = 1

            elif _cstate == 1:
                if ch == '\\':
                    _cstate = 2
                elif ch in string.letters or ch in '@#$%&*':
                    _cstate = 3

            elif _cstate == 2:
                if ch in 'nt':
                    _cstate = 3

            elif _cstate == 3:
                if ch == '\'':
                    _cstate = 4
            ch = mystr[_p]
            _p += 1

        _p -= 1
        if _cstate == 4:
            _syn = 'CHARACTER'
            _cstate = 0
        else:
            _syn = '@-4'   #错误代码，字符不封闭
            _cstate = 0

    elif ch == '<':
        _value = ch
        ch = mystr[_p]

        if ch == '=':           ###########  '<='
            _value += ch
            _p += 1
            _syn = '<='
        else:                   ###########  '<'
            _syn = '<'

    elif ch == '>':
        _value = ch
        ch = mystr[_p]

        if ch == '=':           ########### '>='
            _value += ch
            _p += 1
            _syn = '>='
        else:                   ########## '>'
            _syn = '>'

    elif ch == '!':
        _value = ch
        ch = mystr[_p]

        if ch == '=':           ########## '!='
            _value += ch
            _p += 1
            _syn = '!='
        else:                   ########## '!'
            _syn = '!'


    elif ch == '+':
        _value = ch
        ch = mystr[_p]

        if ch =='+':            ############ '++'
            _value += ch
            _p += 1
            _syn = '++'
        else :                  ############ '+'
            _syn = '+'

    elif ch == '-':
        _value = ch
        ch = mystr[_p]

        if ch =='-':            ########### '--'
            _value += ch
            _p += 1
            _syn = '--'
        else :                  ########### '-'
            _syn = '-'

    elif ch == '=':
        _value = ch
        ch = mystr[_p]

        if ch =='=':            ########### '=='
            _value += ch
            _p += 1
            _syn = '=='
        else :                  ########### '='
            _syn = '='

    elif ch == '&':
        _value = ch
        ch = mystr[_p]

        if ch == '&':           ########### '&&'
            _value += ch
            _p += 1
            _syn = '&&'
        else:                   ########### '&'
            _syn = '&'

    elif ch == '|':
        _value = ch
        ch = mystr[_p]

        if ch == '|':           ########## '||'
            _value += ch
            _p += 1
            _syn = '||'
        else:                   ########## '|'
            _syn = '|'

    elif ch == '*':             ########## '*'
        _value = ch
        _syn = '*'

    elif ch == '/':             ########## '/'
        _value = ch
        _syn = '/'

    elif ch ==';':              ########## ';'
        _value = ch
        _syn = ';'

    elif ch == '(':             ##########  '('
        _value = ch
        _syn = '('

    elif ch == ')':             ########### ')'
        _value = ch
        _syn = ')'

    elif ch == '{':             ########### '{'
        _value = ch
        _syn = '{'

    elif ch == '}':             ########### '}'
        _value = ch
        _syn = '}'

    elif ch == '[':             ########### '['
        _value = ch
        _syn = '['

    elif ch == ']':             ########### ']'
        _value = ch
        _syn = ']'

    elif ch == ',':             ########## ','
        _value = ch
        _syn = ','
    elif ch == '\n':
        _syn = '@-1'

def inSymbolTable(token):
    '''将关键字和标识符存进符号表'''
    global _mysymbol
    if token not in _mysymbol:
        _mysymbol.append(token)

if __name__ == '__main__':
    filenameFromCMD = sys.argv[1]
    getMyProm(filenameFromCMD)
    outOfComment()
    filepath1 = 'E:\\pycode\\sourceDetect\\symbol_table' + str(sys.argv[2]) + ".txt"
    filepath2 = 'E:\\pycode\\sourceDetect\\token' + str(sys.argv[2]) + ".txt"


    symbolTableFile = open(filepath1,'w')
    tokenFile = open(filepath2,'w')
    while _p != len(_content):
        analysis(_content)
        if _syn == '@-1':
            _line += 1 #记录程序的行数
        elif _syn == '@-2':
            print '字符串 ' + _value + ' 不封闭! Error in line ' + str(_line)
        elif _syn == '@-3':
            print '数字 ' + _value + ' 错误，不能以0开头! Error in line ' + str(_line)
        elif _syn == '@-4':
            print '字符 ' + _value + ' 不封闭! Error in line ' + str(_line)
        elif _syn == '@-5':
            print '数字 ' + _value + ' 不合法! Error in line ' + str(_line)
        elif _syn == '@-6':
            print '标识符' + _value + ' 不能包含非法字符!Error in line ' + str(_line)
        elif _syn == '@-7':
            print '数字 ' + _value + ' 不合法,包含字母! Error in line ' + str(_line)
        else: #若程序中无词法错误的情况
            #print (_syn,_value)
            tokenFile.write(str(_syn)+ " ")

    tokenFile.close()
    symbolTableFile.write('入口地址\t变量名\n')
    i = 0
    for symbolItem in _mysymbol:
        symbolTableFile.write(str(i)+'\t\t\t'+symbolItem+'\n')
        i += 1
    symbolTableFile.close()