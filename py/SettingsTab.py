import sys
import serial
from PyQt5.QtWidgets import (QApplication, QWidget, QVBoxLayout, QFormLayout, QPushButton, QCheckBox, QLineEdit, QLabel, QHBoxLayout, QMessageBox)

class SettingsTab(QWidget):
    def __init__(self):
        super().__init__()
        self.initUI()

    def initUI(self):
        # Hauptlayout mit zwei Spalten (QHBoxLayout)
        main_layout = QHBoxLayout()

        # Links: Filter und PID Settings
        left_layout = QVBoxLayout()

        # Filters Section
        form_layout = QFormLayout()
        filter_label = QLabel("<b>Filter Settings</b>")
        form_layout.addRow(filter_label)

        # LowPass Filter 
        self.useLowPass = QCheckBox('Use LowPass')
        self.lowPassFrequency = QLineEdit('0.2')  # This is now Frequency instead of Alpha
        self.lowPassFrequency.setEnabled(False)  # Initially disabled
        self.useLowPass.stateChanged.connect(lambda: self.toggle_parameters(self.useLowPass, self.lowPassFrequency))
        form_layout.addRow(self.useLowPass, QLabel('LowPass Frequency:'))
        form_layout.addRow(self.lowPassFrequency)


        # HighPass Filter 
        self.useHighPass = QCheckBox('Use HighPass')
        self.highPassFrequency = QLineEdit('0.999')  # Frequency für HighPass
        self.highPassFrequency.setEnabled(False)  # Initially disabled
        self.useHighPass.stateChanged.connect(lambda: self.toggle_parameters(self.useHighPass, self.highPassFrequency))
        form_layout.addRow(self.useHighPass, QLabel('HighPass Frequency:'))
        form_layout.addRow(self.highPassFrequency)

        # Moving Average Filter
        self.useMovingAvg = QCheckBox('Use MovingAvg')
        self.movingAvgWindowSize = QLineEdit('2')
        self.movingAvgWindowSize.setEnabled(False)  # Initially disabled
        self.useMovingAvg.stateChanged.connect(lambda: self.toggle_parameters(self.useMovingAvg, self.movingAvgWindowSize))
        form_layout.addRow(self.useMovingAvg, QLabel('MovingAvg Window Size:'))
        form_layout.addRow(self.movingAvgWindowSize)

        # Kalman Filter
        self.useKalman = QCheckBox('Use Kalman')
        self.kalmanQ = QLineEdit('0.02')
        self.kalmanR = QLineEdit('0.2')
        self.kalmanQ.setEnabled(False)  # Initially disabled
        self.kalmanR.setEnabled(False)  # Initially disabled
        self.useKalman.stateChanged.connect(lambda: self.toggle_parameters(self.useKalman, self.kalmanQ, self.kalmanR))
        form_layout.addRow(self.useKalman, QLabel('Kalman Q:'))
        form_layout.addRow(self.kalmanQ)
        form_layout.addRow(QLabel('Kalman R:'))
        form_layout.addRow(self.kalmanR)

        # PID Section
        pid_label = QLabel("<b>PID Settings</b>")
        form_layout.addRow(pid_label)

        # Roll PID
        self.pidRollP = QLineEdit('80.0')
        self.pidRollI = QLineEdit('0.0')
        self.pidRollD = QLineEdit('20')
        self.factorRoll = QLineEdit('1.0')  # Factor for Roll
        form_layout.addRow(QLabel('Roll P:'), self.pidRollP)
        form_layout.addRow(QLabel('Roll I:'), self.pidRollI)
        form_layout.addRow(QLabel('Roll D:'), self.pidRollD)
        form_layout.addRow(QLabel('Roll Factor:'), self.factorRoll)

        # Pitch PID
        self.syncRollPitch = QCheckBox('Roll & Pitch sync')
        self.syncRollPitch.stateChanged.connect(self.toggle_sync_roll_pitch)
        form_layout.addRow(self.syncRollPitch)

        self.pidPitchP = QLineEdit('80.0')
        self.pidPitchI = QLineEdit('0.0')
        self.pidPitchD = QLineEdit('20')
        self.factorPitch = QLineEdit('1.0')  # Factor for Pitch
        form_layout.addRow(QLabel('Pitch P:'), self.pidPitchP)
        form_layout.addRow(QLabel('Pitch I:'), self.pidPitchI)
        form_layout.addRow(QLabel('Pitch D:'), self.pidPitchD)
        form_layout.addRow(QLabel('Pitch Factor:'), self.factorPitch)

        # Yaw PID
        self.pidYawP = QLineEdit('90.0')
        self.pidYawI = QLineEdit('0.0')
        self.pidYawD = QLineEdit('5')
        self.factorYaw = QLineEdit('1.0')  # Factor for Yaw
        form_layout.addRow(QLabel('Yaw P:'), self.pidYawP)
        form_layout.addRow(QLabel('Yaw I:'), self.pidYawI)
        form_layout.addRow(QLabel('Yaw D:'), self.pidYawD)
        form_layout.addRow(QLabel('Yaw Factor:'), self.factorYaw)

        # Gyro Offset
        gyro_offset_label = QLabel("<b>Gyro Offset</b>")
        form_layout.addRow(gyro_offset_label)
        self.gyroOffsetX = QLineEdit('0.0')
        self.gyroOffsetY = QLineEdit('0.0')
        self.gyroOffsetZ = QLineEdit('0.0')
        form_layout.addRow(QLabel('Gyro Offset X:'), self.gyroOffsetX)
        form_layout.addRow(QLabel('Gyro Offset Y:'), self.gyroOffsetY)
        form_layout.addRow(QLabel('Gyro Offset Z:'), self.gyroOffsetZ)

        # Füge das form_layout zu left_layout hinzu
        left_layout.addLayout(form_layout)

        # Rechts: Advanced Settings
        right_layout = QVBoxLayout()

        advanced_layout = QFormLayout()
        advanced_label = QLabel("<b>Advanced Settings</b>")
        advanced_layout.addRow(advanced_label)

        # Servo Pin settings
        self.servoPinAft = QLineEdit('13')  # Aft Servo Pin
        self.servoPinLeft = QLineEdit('14')  # Left Servo Pin
        self.servoPinRight = QLineEdit('15')  # Right Servo Pin
        advanced_layout.addRow(QLabel('Aft Servo Pin:'), self.servoPinAft)
        advanced_layout.addRow(QLabel('Left Servo Pin:'), self.servoPinLeft)
        advanced_layout.addRow(QLabel('Right Servo Pin:'), self.servoPinRight)

        # Additional Pin settings
        self.sbusPin = QLineEdit('16')
        self.mainMotorPin = QLineEdit('5')
        self.tailMotorPin = QLineEdit('17')
        self.sdaPin = QLineEdit('21')
        self.sclPin = QLineEdit('22')
        advanced_layout.addRow(QLabel('SBUS Pin:'), self.sbusPin)
        advanced_layout.addRow(QLabel('Main Motor Pin:'), self.mainMotorPin)
        advanced_layout.addRow(QLabel('Tail Motor Pin:'), self.tailMotorPin)
        advanced_layout.addRow(QLabel('SDA Pin:'), self.sdaPin)
        advanced_layout.addRow(QLabel('SCL Pin:'), self.sclPin)

        # MPU Calibration Duration
        self.mpuCalibrationDuration = QLineEdit('1000')  # Calibration duration for MPU
        advanced_layout.addRow(QLabel('MPU Calibration Duration (ms):'), self.mpuCalibrationDuration)

        # COM Port Setting
        self.comPort = QLineEdit('COM5')  # Default COM Port
        advanced_layout.addRow(QLabel('COM Port:'), self.comPort)

        # Füge das advanced_layout zu right_layout hinzu
        right_layout.addLayout(advanced_layout)

        # Save Button (in der rechten Spalte)
        save_button = QPushButton('Save Parameters')
        save_button.clicked.connect(self.save_parameters)
        right_layout.addWidget(save_button)

        # Füge die beiden Layouts (links und rechts) zum Hauptlayout hinzu
        main_layout.addLayout(left_layout)
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

    def save_parameters(self):
        # Verbindung über USB herstellen
        try:
            ser = serial.Serial(self.comPort.text(), 115200, timeout=1)
        except serial.SerialException as e:
            QMessageBox.critical(self, "Error", f"Failed to open serial port: {e}")
            return

        # Sammle die Parameter
        parameter_string = f"rollPID={self.pidRollP.text()};pitchPID={self.pidPitchP.text()};yawPID={self.pidYawP.text()};" \
                           f"lowPassFrequency={self.lowPassFrequency.text()};highPassFrequency={self.highPassFrequency.text()};" \
                           f"movingAvgWindowSize={self.movingAvgWindowSize.text()};kalmanQ={self.kalmanQ.text()};" \
                           f"kalmanR={self.kalmanR.text()};gyroOffsetX={self.gyroOffsetX.text()};gyroOffsetY={self.gyroOffsetY.text()};" \
                           f"gyroOffsetZ={self.gyroOffsetZ.text()};servoPinAft={self.servoPinAft.text()};servoPinLeft={self.servoPinLeft.text()};" \
                           f"servoPinRight={self.servoPinRight.text()};sbusPin={self.sbusPin.text()};mainMotorPin={self.mainMotorPin.text()};" \
                           f"tailMotorPin={self.tailMotorPin.text()};sdaPin={self.sdaPin.text()};sclPin={self.sclPin.text()};" \
                           f"mpuCalibrationDuration={self.mpuCalibrationDuration.text()};" \
                           f"useLowPass={self.useLowPass.isChecked()};useHighPass={self.useHighPass.isChecked()};" \
                           f"useMovingAvg={self.useMovingAvg.isChecked()};useKalman={self.useKalman.isChecked()}\n"


        # Senden der Parameter
        ser.write(parameter_string.encode())
        ser.close()

        QMessageBox.information(self, "Success", "Parameters were sent successfully!")

if __name__ == '__main__':
    app = QApplication(sys.argv)
    widget = SettingsTab()
    widget.showMaximized()  # Start the window maximized
    sys.exit(app.exec_())
