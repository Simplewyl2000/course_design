import os
import sys
import threading
import copyRate
import cfg_compare
import stackOverflowTools
import heapDetect
import integerOverflowDetect
import integerSignDetect
import printfTest
import integerOperate


from UI import Ui_Form    # 注意，此模块名称为编译生成的 PyQT Python文件
from PyQt5 import QtWidgets, QtCore, QtGui
from PyQt5.QtWidgets import QApplication, QMainWindow, QWidget,  QMessageBox


class mwindow(QWidget, Ui_Form):
    def __init__(self):
        super(mwindow, self).__init__()
        self.setupUi(self)

    def set_copyRate(self):
        sorc = self.sourceFile.text()
        dest = self.DestinationFile.text()
        allsample = os.listdir("sample\\")
        if sorc not in allsample:
            QMessageBox.warning(self, "Warning!", self.tr("请检查文件名"))
            return


        if self.singleDoc.isChecked():
            if sorc == "" or dest == "":
                QMessageBox.warning(self, "Warning!", self.tr("请输入文件名"))
                return

            copyRate.copyRate_count(sorc, dest, "0.txt")
            result = ""
            f = open("result\\copyrate\\0.txt", "r")
            for line in f:
                result = result + line
            self.copyRate.setText(result)
        elif self.batchProcess.isChecked():
            if sorc == "" or dest == "":
                QMessageBox.warning(self, "Warning!", self.tr("请输入文件名"))
                return
            allfile = os.listdir(dest)
            if len(allfile) == 0:
                QMessageBox.warning(self, "Warning!", self.tr("文件夹下无内容，请检查"))
                return
            elif len(allfile) == 1:
                QMessageBox.warning(self, "Warning!", self.tr("文件夹下仅一个文件，请单个检查！"))
                return
            else:
                if len(allfile) % 2 == 0:
                    i = 0
                    while i < len(allfile):
                        t1 = threading.Thread(target=copyRate.copyRate_count, args=(sorc, allfile[i], str(i)+".txt"))
                        t2 = threading.Thread(target=copyRate.copyRate_count, args=(sorc, allfile[i+1], str(i+1)+".txt"))
                        t1.start()
                        t2.start()
                        t1.join()
                        t2.join()
                        i = i + 2

                else:
                    first = allfile[0]
                    copyRate.copyRate_count(sorc, first, "0.txt")
                    i = 1
                    while i < len(allfile):
                        t1 = threading.Thread(target=copyRate.copyRate_count, args=(sorc, allfile[i], str(i) + ".txt"))
                        t2 = threading.Thread(target=copyRate.copyRate_count, args=(sorc, allfile[i + 1], str(i+1) + ".txt"))
                        t1.start()
                        t2.start()
                        t1.join()
                        t2.join()
                        i = i + 2

            allResult = os.listdir("result\\copyrate")
            result = ""
            for i in allResult:
                f = open("result\\copyrate\\"+i,"r")
                result = result + f.read()
            self.copyRate.setText(result)

        else:
            QMessageBox.warning(self, "Warning!", self.tr("请选择一种检测方式！"))



    def set_cfg(self):
        sorc = self.sourceFile.text()
        dest = self.DeteminateCFG.text()
        allsample = os.listdir("sample\\")
        if sorc not in allsample:
            QMessageBox.warning(self, "Warning!", self.tr("请检查文件名"))
            return
        if dest not in allsample:
            QMessageBox.warning(self, "Warning!", self.tr("请检查文件名"))
            return

        if sorc == "" or dest == "":
            QMessageBox.warning(self, "Warning!", self.tr("请输入文件名"))
            return

        simi = cfg_compare.compareMatrix(sorc, dest)
        self.copyRate.setText(" CFG similarity:" + str(simi)+"%")
        pic1 = QtGui.QPixmap("result\\cfg\\"+sorc+".png")
        pic2 = QtGui.QPixmap("result\\cfg\\"+dest+".png")
        self.CFG_SORC.setScaledContents(True)
        self.DES_CFG.setScaledContents(True)
        self.CFG_SORC.setPixmap(pic1)
        self.DES_CFG.setPixmap(pic2)
    def simiintr(self):
        QMessageBox.information(self, "About Similarity", self.tr("提供自主设计的拷贝率检测以及CFG检测"))


    def detcintr(self):
        QMessageBox.information(self, "About Detect", self.tr("提供6种检测手段检测代码中的可能漏洞"))

    def detect(self):
        sorc = self.leakDetect.text()

        allsample = os.listdir("sample\\")
        if sorc not in allsample:
            QMessageBox.warning(self, "Warning!", self.tr("请检查文件名"))
            return
        if self.StackOverFlow.isChecked():
            warnings1 = stackOverflowTools.detectStackOverflow(sorc)
            result = ""
            for i in warnings1:
                result = result + '\n\n' + i
            self.DetectResult.setText(result)

        elif self.HeapOverflow.isChecked():
            warnings2 = heapDetect.detectHeapOverflow(sorc)
            result = ""
            for i in warnings2:
                result = result + '\n\n' + i
            self.DetectResult.setText(result)

        elif self.IntegerWidth.isChecked():
            warnings3 = integerOverflowDetect.detectIntegerOverflow(sorc)
            result = ""
            for i in warnings3:
                result = result + '\n\n' + i
            self.DetectResult.setText(result)

        elif self.IntegerSign.isChecked():
            warnings4 = integerSignDetect.detectIntegerSign(sorc)
            result = ""
            for i in warnings4:
                result = result + '\n\n' + i
            self.DetectResult.setText(result)

        elif self.IntegerOperate.isChecked():
            warnings5 = integerOperate.detectIntegerOperateFlow(sorc)
            result = ""
            for i in warnings5:
                result = result + '\n\n' + i
            self.DetectResult.setText(result)

        elif self.formatString.isChecked():
            warnings6 = printfTest.detectPrintf(sorc)
            result = ""
            for i in warnings6:
                result = result + '\n\n' + i
            self.DetectResult.setText(result)

        else:
            QMessageBox.warning(self, "Warning!", self.tr("请选择一种检测类型！"))


if __name__ == '__main__':
    app = QApplication(sys.argv)
    w = mwindow()
    w.show()
    w.copyRateButton.clicked.connect(w.set_copyRate)
    w.CFGButton.clicked.connect(w.set_cfg)
    w.DetectIntro.clicked.connect(w.detcintr)
    w.SimiIntro.clicked.connect(w.simiintr)
    w.startDetect.clicked.connect(w.detect)
    sys.exit(app.exec_())

