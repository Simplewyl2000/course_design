# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'UI.ui'
#
# Created by: PyQt5 UI code generator 5.13.0
#
# WARNING! All changes made in this file will be lost!


from PyQt5 import QtCore, QtGui, QtWidgets


class Ui_Form(object):
    def setupUi(self, Form):
        Form.setObjectName("Form")
        Form.resize(700, 500)
        self.tabWidget = QtWidgets.QTabWidget(Form)
        self.tabWidget.setGeometry(QtCore.QRect(5, 5, 690, 490))
        self.tabWidget.setObjectName("tabWidget")
        self.tab_3 = QtWidgets.QWidget()
        self.tab_3.setObjectName("tab_3")
        self.label = QtWidgets.QLabel(self.tab_3)
        self.label.setGeometry(QtCore.QRect(185, 90, 330, 50))
        font = QtGui.QFont()
        font.setFamily("Microsoft YaHei")
        font.setPointSize(20)
        font.setBold(False)
        font.setWeight(50)
        self.label.setFont(font)
        self.label.setObjectName("label")
        self.label_2 = QtWidgets.QLabel(self.tab_3)
        self.label_2.setGeometry(QtCore.QRect(275, 180, 150, 30))
        font = QtGui.QFont()
        font.setFamily("Microsoft YaHei")
        font.setPointSize(12)
        self.label_2.setFont(font)
        self.label_2.setObjectName("label_2")
        self.groupBox = QtWidgets.QGroupBox(self.tab_3)
        self.groupBox.setGeometry(QtCore.QRect(110, 270, 480, 130))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.groupBox.setFont(font)
        self.groupBox.setObjectName("groupBox")
        self.SimiIntro = QtWidgets.QPushButton(self.groupBox)
        self.SimiIntro.setGeometry(QtCore.QRect(40, 40, 141, 51))
        self.SimiIntro.setObjectName("SimiIntro")
        self.DetectIntro = QtWidgets.QPushButton(self.groupBox)
        self.DetectIntro.setGeometry(QtCore.QRect(290, 40, 131, 51))
        self.DetectIntro.setObjectName("DetectIntro")
        self.tabWidget.addTab(self.tab_3, "")
        self.tab_2 = QtWidgets.QWidget()
        self.tab_2.setObjectName("tab_2")
        self.label_5 = QtWidgets.QLabel(self.tab_2)
        self.label_5.setGeometry(QtCore.QRect(30, 30, 72, 20))
        self.label_5.setObjectName("label_5")
        self.label_6 = QtWidgets.QLabel(self.tab_2)
        self.label_6.setGeometry(QtCore.QRect(1, 60, 101, 20))
        self.label_6.setObjectName("label_6")
        self.sourceFile = QtWidgets.QLineEdit(self.tab_2)
        self.sourceFile.setGeometry(QtCore.QRect(120, 30, 350, 20))
        self.sourceFile.setObjectName("sourceFile")
        self.DestinationFile = QtWidgets.QLineEdit(self.tab_2)
        self.DestinationFile.setGeometry(QtCore.QRect(120, 60, 350, 20))
        self.DestinationFile.setObjectName("DestinationFile")
        self.copyRateButton = QtWidgets.QPushButton(self.tab_2)
        self.copyRateButton.setGeometry(QtCore.QRect(500, 35, 93, 40))
        self.copyRateButton.setObjectName("copyRateButton")
        self.label_7 = QtWidgets.QLabel(self.tab_2)
        self.label_7.setGeometry(QtCore.QRect(90, 110, 81, 16))
        self.label_7.setObjectName("label_7")
        self.label_8 = QtWidgets.QLabel(self.tab_2)
        self.label_8.setGeometry(QtCore.QRect(170, 110, 72, 15))
        self.label_8.setAlignment(QtCore.Qt.AlignCenter)
        self.label_8.setObjectName("label_8")
        self.groupBox_2 = QtWidgets.QGroupBox(self.tab_2)
        self.groupBox_2.setGeometry(QtCore.QRect(10, 260, 660, 200))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.groupBox_2.setFont(font)
        self.groupBox_2.setObjectName("groupBox_2")
        self.DES_CFG = QtWidgets.QLabel(self.groupBox_2)
        self.DES_CFG.setGeometry(QtCore.QRect(480, 60, 180, 140))
        self.DES_CFG.setAlignment(QtCore.Qt.AlignCenter)
        self.DES_CFG.setObjectName("DES_CFG")
        self.CFG_SORC = QtWidgets.QLabel(self.groupBox_2)
        self.CFG_SORC.setGeometry(QtCore.QRect(290, 60, 180, 140))
        font = QtGui.QFont()
        font.setPointSize(10)
        self.CFG_SORC.setFont(font)
        self.CFG_SORC.setAlignment(QtCore.Qt.AlignCenter)
        self.CFG_SORC.setObjectName("CFG_SORC")
        self.label_22 = QtWidgets.QLabel(self.groupBox_2)
        self.label_22.setGeometry(QtCore.QRect(290, 30, 72, 15))
        font = QtGui.QFont()
        font.setPointSize(9)
        self.label_22.setFont(font)
        self.label_22.setObjectName("label_22")
        self.label_23 = QtWidgets.QLabel(self.groupBox_2)
        self.label_23.setGeometry(QtCore.QRect(480, 30, 72, 15))
        font = QtGui.QFont()
        font.setPointSize(9)
        self.label_23.setFont(font)
        self.label_23.setObjectName("label_23")
        self.DeteminateCFG = QtWidgets.QLineEdit(self.groupBox_2)
        self.DeteminateCFG.setGeometry(QtCore.QRect(20, 70, 241, 20))
        self.DeteminateCFG.setObjectName("DeteminateCFG")
        self.label_24 = QtWidgets.QLabel(self.groupBox_2)
        self.label_24.setGeometry(QtCore.QRect(20, 30, 131, 20))
        font = QtGui.QFont()
        font.setPointSize(9)
        self.label_24.setFont(font)
        self.label_24.setObjectName("label_24")
        self.CFGButton = QtWidgets.QPushButton(self.groupBox_2)
        self.CFGButton.setGeometry(QtCore.QRect(60, 110, 161, 51))
        self.CFGButton.setObjectName("CFGButton")
        self.copyRate = QtWidgets.QTextBrowser(self.tab_2)
        self.copyRate.setGeometry(QtCore.QRect(120, 160, 470, 80))
        self.copyRate.setObjectName("copyRate")
        self.label_27 = QtWidgets.QLabel(self.tab_2)
        self.label_27.setGeometry(QtCore.QRect(30, 190, 72, 20))
        self.label_27.setObjectName("label_27")
        self.checkBox = QtWidgets.QCheckBox(self.tab_2)
        self.checkBox.setGeometry(QtCore.QRect(120, 90, 16, 19))
        self.checkBox.setText("")
        self.checkBox.setObjectName("checkBox")
        self.checkBox_2 = QtWidgets.QCheckBox(self.tab_2)
        self.checkBox_2.setGeometry(QtCore.QRect(200, 90, 16, 19))
        self.checkBox_2.setText("")
        self.checkBox_2.setObjectName("checkBox_2")
        self.tabWidget.addTab(self.tab_2, "")
        self.tab = QtWidgets.QWidget()
        self.tab.setObjectName("tab")
        self.startDetect = QtWidgets.QPushButton(self.tab)
        self.startDetect.setGeometry(QtCore.QRect(500, 25, 93, 28))
        self.startDetect.setObjectName("startDetect")
        self.label_13 = QtWidgets.QLabel(self.tab)
        self.label_13.setGeometry(QtCore.QRect(30, 30, 72, 20))
        self.label_13.setObjectName("label_13")
        self.leakDetect = QtWidgets.QLineEdit(self.tab)
        self.leakDetect.setGeometry(QtCore.QRect(120, 30, 350, 20))
        self.leakDetect.setObjectName("leakDetect")
        self.DetectResult = QtWidgets.QTextBrowser(self.tab)
        self.DetectResult.setGeometry(QtCore.QRect(120, 130, 470, 200))
        self.DetectResult.setObjectName("DetectResult")
        self.HeapOverflow = QtWidgets.QRadioButton(self.tab)
        self.HeapOverflow.setGeometry(QtCore.QRect(240, 60, 115, 19))
        self.HeapOverflow.setObjectName("HeapOverflow")
        self.IntegerWidth = QtWidgets.QRadioButton(self.tab)
        self.IntegerWidth.setGeometry(QtCore.QRect(360, 60, 115, 19))
        self.IntegerWidth.setObjectName("IntegerWidth")
        self.IntegerOperate = QtWidgets.QRadioButton(self.tab)
        self.IntegerOperate.setGeometry(QtCore.QRect(240, 90, 115, 19))
        self.IntegerOperate.setObjectName("IntegerOperate")
        self.IntegerSign = QtWidgets.QRadioButton(self.tab)
        self.IntegerSign.setGeometry(QtCore.QRect(360, 90, 115, 19))
        self.IntegerSign.setObjectName("IntegerSign")
        self.StackOverFlow = QtWidgets.QRadioButton(self.tab)
        self.StackOverFlow.setGeometry(QtCore.QRect(120, 60, 115, 19))
        self.StackOverFlow.setObjectName("StackOverFlow")
        self.formatString = QtWidgets.QRadioButton(self.tab)
        self.formatString.setGeometry(QtCore.QRect(120, 90, 115, 19))
        self.formatString.setObjectName("formatString")
        self.tabWidget.addTab(self.tab, "")

        self.retranslateUi(Form)
        self.tabWidget.setCurrentIndex(2)
        QtCore.QMetaObject.connectSlotsByName(Form)

    def retranslateUi(self, Form):
        _translate = QtCore.QCoreApplication.translate
        Form.setWindowTitle(_translate("Form", "Form"))
        self.label.setText(_translate("Form", "软 件 安 全 课 程 设 计"))
        self.label_2.setText(_translate("Form", "IS1701 吴与伦"))
        self.groupBox.setTitle(_translate("Form", "功能"))
        self.SimiIntro.setText(_translate("Form", "同源性检测"))
        self.DetectIntro.setText(_translate("Form", "漏洞检测"))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab_3), _translate("Form", "封面"))
        self.label_5.setText(_translate("Form", "源文件"))
        self.label_6.setText(_translate("Form", "文件/文件目录"))
        self.copyRateButton.setText(_translate("Form", "开始检测"))
        self.label_7.setText(_translate("Form", "单文件检测"))
        self.label_8.setText(_translate("Form", "批处理"))
        self.groupBox_2.setTitle(_translate("Form", "CFG比对"))
        self.DES_CFG.setText(_translate("Form", "还未生成CFG"))
        self.CFG_SORC.setText(_translate("Form", "还未生成CFG"))
        self.label_22.setText(_translate("Form", "源文件："))
        self.label_23.setText(_translate("Form", "目标文件："))
        self.label_24.setText(_translate("Form", "文件名："))
        self.CFGButton.setText(_translate("Form", "开始检测"))
        self.label_27.setText(_translate("Form", "结果："))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab_2), _translate("Form", "同源性检测"))
        self.startDetect.setText(_translate("Form", "开始检测"))
        self.label_13.setText(_translate("Form", "源文件"))
        self.HeapOverflow.setText(_translate("Form", "堆溢出检测"))
        self.IntegerWidth.setText(_translate("Form", "整数宽度检测"))
        self.IntegerOperate.setText(_translate("Form", "整数运算检测"))
        self.IntegerSign.setText(_translate("Form", "整数符号检测"))
        self.StackOverFlow.setText(_translate("Form", "栈溢出检测"))
        self.formatString.setText(_translate("Form", "格式化字符串"))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab), _translate("Form", "漏洞检测"))
