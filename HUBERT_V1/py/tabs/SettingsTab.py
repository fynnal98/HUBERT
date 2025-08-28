import sys
import esptool
from PyQt5.QtWidgets import (QApplication, QWidget, QVBoxLayout, QFormLayout, QPushButton, QCheckBox, QLineEdit, QLabel, 
                             QHBoxLayout, QMessageBox, QGroupBox, QTextEdit)

class SettingsTab(QWidget):
    def __init__(self):
        super().__init__()
        self.initUI()

    def initUI(self):
        # Hauptlayout mit zwei Spalten (QHBoxLayout)
        main_layout = QHBoxLayout()

        # Links: Filter und PID Settings
        left_layout = QVBoxLayout()

        # Filters Section in GroupBox
        filter_group = QGroupBox("Filter Settings")
        filter_group_layout = QFormLayout()

        # LowPass Filter
        self.useLowPass = QCheckBox('Use LowPass')
        self.lowPassFrequency = QLineEdit('0.2')  # This is now Frequency instead of Alpha
        self.lowPassFrequency.setEnabled(False)  # Initially disabled
        self.useLowPass.stateChanged.connect(lambda: self.toggle_parameters(self.useLowPass, self.lowPassFrequency))
        filter_group_layout.addRow(self.useLowPass, QLabel('LowPass Frequency:'))
        filter_group_layout.addRow(self.lowPassFrequency)

        # HighPass Filter 
        self.useHighPass = QCheckBox('Use HighPass')
        self.highPassFrequency = QLineEdit('0.999')  # Frequency für HighPass
        self.highPassFrequency.setEnabled(False)  # Initially disabled
        self.useHighPass.stateChanged.connect(lambda: self.toggle_parameters(self.useHighPass, self.highPassFrequency))
        filter_group_layout.addRow(self.useHighPass, QLabel('HighPass Frequency:'))
        filter_group_layout.addRow(self.highPassFrequency)

        # Moving Average Filter
        self.useMovingAvg = QCheckBox('Use MovingAvg')
        self.movingAvgWindowSize = QLineEdit('2')
        self.movingAvgWindowSize.setEnabled(False)  # Initially disabled
        self.useMovingAvg.stateChanged.connect(lambda: self.toggle_parameters(self.useMovingAvg, self.movingAvgWindowSize))
        filter_group_layout.addRow(self.useMovingAvg, QLabel('MovingAvg Window Size:'))
        filter_group_layout.addRow(self.movingAvgWindowSize)

        # Kalman Filter
        self.useKalman = QCheckBox('Use Kalman')
        self.kalmanQ = QLineEdit('0.02')
        self.kalmanR = QLineEdit('0.2')
        self.kalmanQ.setEnabled(False)  # Initially disabled
        self.kalmanR.setEnabled(False)  # Initially disabled
        self.useKalman.stateChanged.connect(lambda: self.toggle_parameters(self.useKalman, self.kalmanQ, self.kalmanR))
        filter_group_layout.addRow(self.useKalman, QLabel('Kalman Q:'))
        filter_group_layout.addRow(self.kalmanQ)
        filter_group_layout.addRow(QLabel('Kalman R:'))
        filter_group_layout.addRow(self.kalmanR)

        # RPM Filter
        self.useRPMFilter = QCheckBox('Use RPM Filter')
        self.rpm = QLineEdit('20000')  # Default RPM
        self.bandwidth = QLineEdit('10.0')  # Default Bandwidth
        self.rpm.setEnabled(False)  # Initially disabled
        self.bandwidth.setEnabled(False)  # Initially disabled
        self.useRPMFilter.stateChanged.connect(lambda: self.toggle_parameters(self.useRPMFilter, self.rpm, self.bandwidth))
        filter_group_layout.addRow(self.useRPMFilter, QLabel('RPM:'))
        filter_group_layout.addRow(self.rpm)
        filter_group_layout.addRow(QLabel('Bandwidth:'))
        filter_group_layout.addRow(self.bandwidth)

        filter_group.setLayout(filter_group_layout)
        left_layout.addWidget(filter_group)

        # PID Section in GroupBox
        pid_group = QGroupBox("PID Settings")
        pid_group_layout = QFormLayout()

        # Roll PID
        self.pidRollP = QLineEdit('80.0')
        self.pidRollI = QLineEdit('0.0')
        self.pidRollD = QLineEdit('20')
        self.factorRoll = QLineEdit('1.0')  # Factor for Roll
        pid_group_layout.addRow(QLabel('Roll P:'), self.pidRollP)
        pid_group_layout.addRow(QLabel('Roll I:'), self.pidRollI)
        pid_group_layout.addRow(QLabel('Roll D:'), self.pidRollD)
        pid_group_layout.addRow(QLabel('Roll Factor:'), self.factorRoll)

        # Pitch PID
        self.syncRollPitch = QCheckBox('Roll & Pitch sync')
        self.syncRollPitch.stateChanged.connect(self.toggle_sync_roll_pitch)
        pid_group_layout.addRow(self.syncRollPitch)

        self.pidPitchP = QLineEdit('80.0')
        self.pidPitchI = QLineEdit('0.0')
        self.pidPitchD = QLineEdit('20')
        self.factorPitch = QLineEdit('1.0')  # Factor for Pitch
        pid_group_layout.addRow(QLabel('Pitch P:'), self.pidPitchP)
        pid_group_layout.addRow(QLabel('Pitch I:'), self.pidPitchI)
        pid_group_layout.addRow(QLabel('Pitch D:'), self.pidPitchD)
        pid_group_layout.addRow(QLabel('Pitch Factor:'), self.factorPitch)

        # Yaw PID
        self.pidYawP = QLineEdit('90.0')
        self.pidYawI = QLineEdit('0.0')
        self.pidYawD = QLineEdit('5')
        self.factorYaw = QLineEdit('1.0')  # Factor for Yaw
        pid_group_layout.addRow(QLabel('Yaw P:'), self.pidYawP)
        pid_group_layout.addRow(QLabel('Yaw I:'), self.pidYawI)
        pid_group_layout.addRow(QLabel('Yaw D:'), self.pidYawD)
        pid_group_layout.addRow(QLabel('Yaw Factor:'), self.factorYaw)

        pid_group.setLayout(pid_group_layout)
        left_layout.addWidget(pid_group)

        # Füge das left_layout zu main_layout hinzu
        main_layout.addLayout(left_layout)

        # Rechts: Advanced Settings in GroupBox
        right_layout = QVBoxLayout()

        # Gyro Offset Section in GroupBox
        gyro_group = QGroupBox("Gyro Offset")
        gyro_group_layout = QFormLayout()

        self.gyroOffsetX = QLineEdit('0.0')
        self.gyroOffsetY = QLineEdit('0.0')
        self.gyroOffsetZ = QLineEdit('0.0')
        gyro_group_layout.addRow(QLabel('Gyro Offset X:'), self.gyroOffsetX)
        gyro_group_layout.addRow(QLabel('Gyro Offset Y:'), self.gyroOffsetY)
        gyro_group_layout.addRow(QLabel('Gyro Offset Z:'), self.gyroOffsetZ)

        gyro_group.setLayout(gyro_group_layout)
        right_layout.addWidget(gyro_group)

        # Advanced Settings in GroupBox
        advanced_group = QGroupBox("Advanced Settings")
        advanced_group_layout = QFormLayout()

        # Servo Pin settings
        self.servoPinAft = QLineEdit('13')  # Aft Servo Pin
        self.servoPinLeft = QLineEdit('14')  # Left Servo Pin
        self.servoPinRight = QLineEdit('15')  # Right Servo Pin
        advanced_group_layout.addRow(QLabel('Aft Servo Pin:'), self.servoPinAft)
        advanced_group_layout.addRow(QLabel('Left Servo Pin:'), self.servoPinLeft)
        advanced_group_layout.addRow(QLabel('Right Servo Pin:'), self.servoPinRight)

        # Additional Pin settings
        self.sbusPin = QLineEdit('16')
        self.mainMotorPin = QLineEdit('5')
        self.tailMotorPin = QLineEdit('17')
        self.sdaPin = QLineEdit('21')
        self.sclPin = QLineEdit('22')
        advanced_group_layout.addRow(QLabel('SBUS Pin:'), self.sbusPin)
        advanced_group_layout.addRow(QLabel('Main Motor Pin:'), self.mainMotorPin)
        advanced_group_layout.addRow(QLabel('Tail Motor Pin:'), self.tailMotorPin)
        advanced_group_layout.addRow(QLabel('SDA Pin:'), self.sdaPin)
        advanced_group_layout.addRow(QLabel('SCL Pin:'), self.sclPin)

        # MPU Calibration Duration
        self.mpuCalibrationDuration = QLineEdit('1000')  # Calibration duration for MPU
        advanced_group_layout.addRow(QLabel('MPU Calibration Duration (ms):'), self.mpuCalibrationDuration)

        # COM Port Setting
        self.comPort = QLineEdit('COM5')  # Default COM Port
        advanced_group_layout.addRow(QLabel('COM Port:'), self.comPort)

        advanced_group.setLayout(advanced_group_layout)
        right_layout.addWidget(advanced_group)

        # Spacer hinzufügen, um alles nach oben zu schieben
        right_layout.addStretch()

        # Flash Button (in der rechten Spalte)
        flash_button = QPushButton('Flash ESP32')
        flash_button.clicked.connect(self.flash_esp)
        right_layout.addWidget(flash_button)

        # Terminal-Ausgabeanzeige
        self.output_console = QTextEdit(self)
        self.output_console.setReadOnly(True)
        right_layout.addWidget(self.output_console)

        # Füge das right_layout zu main_layout hinzu
        main_layout.addLayout(right_layout)

        self.setLayout(main_layout)
        self.setWindowTitle('Settings Tab')

    def toggle_sync_roll_pitch(self):
        """ Synchronize Roll and Pitch PID values when 'Roll & Pitch sync' is checked """
        if self.syncRollPitch.isChecked():
            # Disable Pitch PID inputs and sync with Roll
            self.pidPitchP.setEnabled(False)
            self.pidPitchI.setEnabled(False)
            self.pidPitchD.setEnabled(False)
            self.factorPitch.setEnabled(False)

            self.pidPitchP.setText(self.pidRollP.text())
            self.pidPitchI.setText(self.pidRollI.text())
            self.pidPitchD.setText(self.pidRollD.text())
            self.factorPitch.setText(self.factorRoll.text())

            # Sync Roll and Pitch when Roll inputs change
            self.pidRollP.textChanged.connect(lambda: self.pidPitchP.setText(self.pidRollP.text()))
            self.pidRollI.textChanged.connect(lambda: self.pidPitchI.setText(self.pidRollI.text()))
            self.pidRollD.textChanged.connect(lambda: self.pidPitchD.setText(self.pidRollD.text()))
            self.factorRoll.textChanged.connect(lambda: self.factorPitch.setText(self.factorRoll.text()))
        else:
            # Enable Pitch PID inputs
            self.pidPitchP.setEnabled(True)
            self.pidPitchI.setEnabled(True)
            self.pidPitchD.setEnabled(True)
            self.factorPitch.setEnabled(True)

    def toggle_parameters(self, checkbox, *widgets):
        # Enable/Disable the widgets based on checkbox state
        for widget in widgets:
            widget.setEnabled(checkbox.isChecked())




    def flash_esp(self):
        try:
            esp_port = self.comPort.text()  # Den COM-Port aus der GUI entnehmen
            firmware_file = "C:/Users/Fynn/Desktop/Git/HUBERT/.pio/build/esp32dev/firmware.bin"  # Der korrekte Pfad zur Firmware

            esptool.main([
                "--chip", "esp32",
                "--port", esp_port,
                "--baud", "115200",
                "write_flash", "-z", "0x1000", firmware_file
            ])

            self.output_console.append("ESP32 wurde erfolgreich geflasht!")
            QMessageBox.information(self, "Erfolg", "ESP32 wurde erfolgreich geflasht!")

        except Exception as e:
            self.output_console.append(f"Fehler beim Flashen: {e}")
            QMessageBox.critical(self, "Fehler", f"Fehler beim Flashen: {e}")




if __name__ == '__main__':
    app = QApplication(sys.argv)
    widget = SettingsTab()
    widget.showMaximized()  # Start the window maximized
    sys.exit(app.exec_())
