# -*- coding: utf-8 -*-
'''
Created on 2012-10-18

@author: zouliping
'''
import string
import re

_key = ("auto","break","case","char","const","continue","default",

"do","double","else","enum","extern","float","for",

"goto","if","int","long","register","return","short",

"signed","static","sizeof","struct","switch","typedef","union",

"unsigned","void","volatile","while")  # c���Ե�32���ؼ���

_abnormalChar = '@#$%^&*~' #��ʶ���п��ܳ��ֵķǷ��ַ�

_syn = ''  #���ʵ��ֱ���
_p = 0  #�±�
_value = ''  #��Ŵʷ��������ĵ���
_content = '' #��������
_mstate = 0 #�ַ�����״̬
_cstate = 0 #�ַ���״̬
_dstate = 0 #�����͸�������״̬
_line = 1 #����ĵڼ���
_mysymbol = [] #���ű�
_functionName = []
_flag = 1


def outOfComment():
    '''ȥ�������е�ע��'''
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
                _content = _content.replace(comment,'') #��ע���滻Ϊ�գ����ҽ��±��ƶ�
                index = startIndex - 1
                state = 0

            elif c == '*':
                pass
            else:
                state = 2

def getMyProm(filename):
    '''���ļ��л�ȡ����Ƭ��'''
    global _content
    flag = 0
    myPro = open(filename,'r',encoding="utf-8")

    for line in myPro:
        flag = 0
        for i in line:
            if i == '\n' or i == ' ':
                continue
            else:
                flag = 1
                break

        if flag:
            _content = "%s%s" % (_content, line.lstrip()) #Ч�ʸ��ߵ��ַ���ƴ�ӷ���
        else:
            _content = "%s%s" % (_content, line)
    myPro.close()

def analysis(mystr):
    '''����Ŀ����룬����token'''
    global _p,_value,_syn,_mstate,_dstate,_line,_cstate,_flag

    _flag = 1


    if _p == len(mystr):
        _flag = 0
        return

    _value = ''
    ch = mystr[_p]
    _p += 1

    while ch == ' ':
        if _p == len(mystr):
            _flag = 0
            return
        ch = mystr[_p]
        _p += 1
        if _p == len(mystr):
            _flag = 0

    if ch in string.ascii_letters or ch == '_':    ###############letter(letter|digit)*
        _funcname = ''
        while ch in string.ascii_letters or ch in string.digits or ch == '_' or ch in _abnormalChar:
            _value += ch
            _funcname = _funcname + ch
            ch = mystr[_p]
            _p += 1
        _p -= 1

        for abnormal in _abnormalChar:
            if abnormal in _value:
                _syn = '@-6' #������룬��ʶ���к��зǷ��ַ�
                break
            else:
                _syn = 'ID'

        for s in _key:
            if s == _value:
                _syn = _value.upper()               #############�ؼ���
                break
        if _syn == 'ID':
            if mystr[_p] == '(':
                _functionName.append((_funcname,_line))
            inSymbolTable(_value)

    elif ch == '\"':                        #############�ַ���
        while ch in string.ascii_letters or ch in '\"% ' :
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
            _syn = '@-2'     #������룬�ַ��������
            _mstate = 0

        elif _mstate == 2:
            _mstate = 0
            _syn = 'STRING'

        _p -= 1

    elif ch in string.digits:
        while ch in string.digits or ch == '.' or ch in string.ascii_letters:
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

        for char in string.ascii_letters:
            if char in _value:
                _syn = '@-7' #������룬���ֺ���ĸ��ϣ���12AB56��
                _dstate = 0


        if _syn != '@-7':
            if _dstate == 5:
                _syn = '@-3' #������룬������0��ͷ
                _dstate = 0
            else:
                _dstate = 0
                if '.' not in _value:
                    _syn = 'DIGIT'               ##################digit digit*
                else:
                    if _value.count('.') == 1:
                        _syn = 'FRACTION'           ################## ������
                    else:
                        _syn = '@-5' #������룬�������а�������㣬��1.2.3
        _p -= 1


    elif ch == '\'':                    ################## �ַ�
        while ch in string.ascii_letters or ch in '@#$%&*\\\'\"':
            _value += ch
            if _cstate == 0:
                if ch == '\'':
                    _cstate = 1

            elif _cstate == 1:
                if ch == '\\':
                    _cstate = 2
                elif ch in string.ascii_letters or ch in '@#$%&*':
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
            _syn = '@-4'   #������룬�ַ������
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
    '''���ؼ��ֺͱ�ʶ��������ű�'''
    global _mysymbol
    if token not in _mysymbol:
        _mysymbol.append(token)

def processProgram(filepath, id):
    global _line
    getMyProm(filepath)
    outOfComment()
    filepath2 = 'midFile\\token' + str(id) + ".txt"
    filepath3 = 'midFile\\functionName' + str(id) + ".txt"
    filepath4 = 'midFile\\functionLine' + str(id) + ".txt"

    tokenFile = open(filepath2, 'w')
    funcnameFile = open(filepath3, 'w')
    functionLine = open(filepath4, 'w')
    _flag = 1
    while _p != len(_content) and _flag:
        analysis(_content)
        if _syn == '@-1':
            _line += 1  # ��¼���������

        else:  # ���������޴ʷ���������
            tokenFile.write(str(_syn) + " ")

    tokenFile.close()

    for func in _functionName:
        funcnameFile.write(func[0] + " ")

    for func in _functionName:
        functionLine.write(str(func[1]) + " ")

    funcnameFile.close()
    functionLine.close()



if __name__ == '__main__':
    processProgram("1.txt", 1)
