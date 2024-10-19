import sys
from PyQt5.QtWidgets import QApplication, QTabWidget, QWidget, QVBoxLayout, QLabel, QHBoxLayout
from PyQt5.QtGui import QPainter, QPen
from PyQt5.QtCore import Qt, QRect


class SystemTab(QWidget):
    def __init__(self):
        super().__init__()
        self.initUI()

    def initUI(self):
        layout = QVBoxLayout()

        # Coming soon label
        self.coming_soon_label = QLabel("Coming Soon!")
        self.coming_soon_label.setAlignment(Qt.AlignCenter)
        layout.addWidget(self.coming_soon_label)

        self.setLayout(layout)