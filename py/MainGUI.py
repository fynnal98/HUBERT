import sys
from PyQt5.QtWidgets import QApplication, QMainWindow, QTabWidget
from PyQt5.QtGui import QIcon
from tabs.FreqTab import FreqPlotTab
from tabs.LoggerTab import LoggerTab
from tabs.SettingsTab import SettingsTab
from tabs.SystemTab import SystemTab

class MainGUI(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("HUBERT | Flightcontroller")
        self.setWindowIcon(QIcon('py/icons/icon.png'))  

        self.tabs = QTabWidget()

        self.logger_tab = LoggerTab()
        self.freq_plot_tab = FreqPlotTab()
        self.setting_tab = SettingsTab()
        self.system_tab = SystemTab()

        self.tabs.addTab(self.setting_tab, "Settings")
        self.tabs.addTab(self.logger_tab, "Logger")
        self.tabs.addTab(self.freq_plot_tab, "Frequenz Plot")
        self.tabs.addTab(self.system_tab, "System")

        self.setCentralWidget(self.tabs)

if __name__ == '__main__':
    app = QApplication(sys.argv)


    with open('py/qss/style.qss', 'r') as file:
        style_sheet = file.read()
        app.setStyleSheet(style_sheet)


    widget = MainGUI()
    widget.showMaximized()  # Start the window maximized
    sys.exit(app.exec_())
