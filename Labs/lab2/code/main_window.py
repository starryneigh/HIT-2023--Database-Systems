# -*- coding: utf-8 -*-

from PyQt5 import QtCore, QtGui, QtWidgets


class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(800, 600)
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")

        # 设置背景渐变色
        gradient = QtGui.QLinearGradient(0, 0, 0, 600)
        gradient.setColorAt(0, QtGui.QColor(52, 152, 219))
        gradient.setColorAt(1, QtGui.QColor(44, 62, 80))

        self.centralwidget.setStyleSheet(f"background: {gradient};")

        self.label = QtWidgets.QLabel(self.centralwidget)
        self.label.setGeometry(QtCore.QRect(230, 60, 311, 101))
        font = QtGui.QFont()
        font.setFamily("楷体")
        font.setPointSize(26)
        self.label.setFont(font)
        self.label.setObjectName("label")

        self.pushButton_3 = QtWidgets.QPushButton(self.centralwidget)
        self.pushButton_3.setGeometry(QtCore.QRect(280, 420, 201, 61))
        font = QtGui.QFont()
        font.setFamily("楷体")
        font.setPointSize(16)
        self.pushButton_3.setFont(font)
        self.pushButton_3.setObjectName("pushButton_3")
        self.setup_button_style(self.pushButton_3)

        self.pushButton_4 = QtWidgets.QPushButton(self.centralwidget)
        self.pushButton_4.setGeometry(QtCore.QRect(280, 320, 201, 61))
        font = QtGui.QFont()
        font.setFamily("楷体")
        font.setPointSize(16)
        self.pushButton_4.setFont(font)
        self.pushButton_4.setObjectName("pushButton_4")
        self.setup_button_style(self.pushButton_4)

        self.pushButton_5 = QtWidgets.QPushButton(self.centralwidget)
        self.pushButton_5.setGeometry(QtCore.QRect(280, 230, 201, 61))
        font = QtGui.QFont()
        font.setFamily("楷体")
        font.setPointSize(16)
        self.pushButton_5.setFont(font)
        self.pushButton_5.setObjectName("pushButton_5")
        self.setup_button_style(self.pushButton_5)

        MainWindow.setCentralWidget(self.centralwidget)

        self.menubar = QtWidgets.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 800, 30))
        self.menubar.setObjectName("menubar")
        MainWindow.setMenuBar(self.menubar)

        self.statusbar = QtWidgets.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "学院管理系统"))
        self.label.setText(_translate("MainWindow", "学院管理系统"))
        self.pushButton_3.setText(_translate("MainWindow", "建立视图"))
        self.pushButton_4.setText(_translate("MainWindow", "修改"))
        self.pushButton_5.setText(_translate("MainWindow", "查询"))

    def setup_button_style(self, button):
        button.setStyleSheet("QPushButton { background-color: #3498db; color: white; border: 2px solid #3498db; border-radius: 10px; }"
                             "QPushButton:hover { background-color: #2980b9; }"
                             "QPushButton:pressed { background-color: #21618c; }")
