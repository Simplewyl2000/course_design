import sys
import copyRate
import cfg_compare
import stackOverflowTools
import heapDetect
import integerOverflowDetect
import integerSignDetect
import printfTest

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
        if sorc == "" or dest == "":
            QMessageBox.warning(self, "Warning!", self.tr("请输入文件名"))
            return

        copyRate.copyRate_count(sorc, dest, "copyResult.txt")
        result = ""
        f = open("result\\copyrate\\copyResult.txt", "r")
        for line in f:
            result = result + line
        self.copyRate.setText(result)

    def set_cfg(self):
        sorc = self.sourceFile.text()
        dest = self.DeteminateCFG.text()
        if sorc == "" or dest == "":
            QMessageBox.warning(self, "Warning!", self.tr("请输入文件名"))
            return

        distance = cfg_compare.compareMatrix(sorc, dest, 1, 2)
        if distance == 0:
            simi = 1
        else:
            simi = 1/distance
        simi =simi*100
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
        if self.StackOverFlow.isChecked():
            warnings1 = stackOverflowTools.detectStackOverflow(sorc, 1)
            result = ""
            for i in warnings1:
                result = result + '\n\n' + i
            self.DetectResult.setText(result)

        elif self.HeapOverflow.isChecked():
            warnings2 = heapDetect.detectHeapOverflow(sorc, 1)
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
            warnings4 = integerSignDetect.detectIntegerSign(sorc, 1)
            result = ""
            for i in warnings4:
                result = result + '\n\n' + i
            self.DetectResult.setText(result)

        elif self.IntegerOperate.isChecked():
            pass

        elif self.formatString.isChecked():
            warnings6 = printfTest.detectPrintf(sorc,1)
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

