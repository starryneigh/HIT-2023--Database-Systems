# --*-- coding:utf-8 --*--
"""
@Filename: view_.py
@Author: Keyan Xu
@Time: 2023-11-13
"""
import sys

import pymysql

from view import Ui_MainWindow
from PyQt5.QtWidgets import QMainWindow, QApplication, QTableWidgetItem, QMessageBox


class View(QMainWindow, Ui_MainWindow):
    def __init__(self, parent=None):
        super(View, self).__init__(parent)
        self.setupUi(self)
        self.pushButton.clicked.connect(self.create_view)

    def create_view(self):
        clno = self.comboBox.currentText()
        print(clno)
        db = pymysql.connect(host='localhost', user='root', passwd='mysql', database='stu_sys')
        cursor = db.cursor()
        sql = 'create view class' + clno + ('_stu as (select sno, sname, deno, '
                                            'dono from class natural join student where clno=%s)')
        cnt = cursor.execute(sql, clno)
        QMessageBox.information(self, '提示', '创建成功')


if __name__ == '__main__':
    app = QApplication(sys.argv)
    MainWindow = View()
    MainWindow.show()
    sys.exit(app.exec_())
