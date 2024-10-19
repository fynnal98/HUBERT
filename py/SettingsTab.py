import sys
from PyQt5.QtWidgets import (QApplication, QWidget, QVBoxLayout, QFormLayout, QPushButton, QCheckBox, QLineEdit, QLabel, QHBoxLayout)

class SettingsTab(QWidget):
    def __init__(self):
        super().__init__()
        self.initUI()

    def initUI(self):
        layout = QVBoxLayout()

        # Filters Section
        form_layout = QFormLayout()
        filter_label = QLabel("<b>Filter Settings</b>")
        form_layout.addRow(filter_label)

        # LowPass Filter
        self.useLowPass = QCheckBox('Use LowPass')
        self.lowPassAlpha = QLineEdit('0.2')
        self.lowPassAlpha.setEnabled(False)  # Initially disabled
        self.useLowPass.stateChanged.connect(lambda: self.toggle_parameters(self.useLowPass, self.lowPassAlpha))
        form_layout.addRow(self.useLowPass, QLabel('LowPass Alpha:'))
        form_layout.addRow(self.lowPassAlpha)

        # HighPass Filter
        self.useHighPass = QCheckBox('Use HighPass')
        self.highPassAlpha = QLineEdit('0.999')
        self.highPassAlpha.setEnabled(False)  # Initially disabled
        self.useHighPass.stateChanged.connect(lambda: self.toggle_parameters(self.useHighPass, self.highPassAlpha))
        form_layout.addRow(self.useHighPass, QLabel('HighPass Alpha:'))
        form_layout.addRow(self.highPassAlpha)

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

        # Add the form layout to the main layout
        layout.addLayout(form_layout)

        # Advanced Settings Section
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
        self.sbusPin = QLineEdit('21')
        self.mainMotorPin = QLineEdit('22')
        self.tailMotorPin = QLineEdit('23')
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

        # Add advanced settings layout to the main layout
        layout.addLayout(advanced_layout)

        # Save Button
        save_button = QPushButton('Save Parameters')
        save_button.clicked.connect(self.save_parameters)
        layout.addWidget(save_button)

        self.setLayout(layout)
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
        # Save the parameters (for now, just print them)
        print(f"Use LowPass: {self.useLowPass.isChecked()}")
        print(f"LowPass Alpha: {self.lowPassAlpha.text()}")
        print(f"Use HighPass: {self.useHighPass.isChecked()}")
        print(f"HighPass Alpha: {self.highPassAlpha.text()}")
        print(f"Use MovingAvg: {self.useMovingAvg.isChecked()}")
        print(f"MovingAvg Window Size: {self.movingAvgWindowSize.text()}")
        print(f"Use Kalman: {self.useKalman.isChecked()}")
        print(f"Kalman Q: {self.kalmanQ.text()}")
        print(f"Kalman R: {self.kalmanR.text()}")

        # PID and Factors
        print(f"Roll P: {self.pidRollP.text()}, Roll I: {self.pidRollI.text()}, Roll D: {self.pidRollD.text()}, Roll Factor: {self.factorRoll.text()}")
        print(f"Pitch P: {self.pidPitchP.text()}, Pitch I: {self.pidPitchI.text()}, Pitch D: {self.pidPitchD.text()}, Pitch Factor: {self.factorPitch.text()}")
        print(f"Yaw P: {self.pidYawP.text()}, Yaw I: {self.pidYawI.text()}, Yaw D: {self.pidYawD.text()}, Yaw Factor: {self.factorYaw.text()}")
        print(f"Gyro Offset X: {self.gyroOffsetX.text()}, Y: {self.gyroOffsetY.text()}, Z: {self.gyroOffsetZ.text()}")

        # Advanced settings
        print(f"Aft Servo Pin: {self.servoPinAft.text()}")
        print(f"Left Servo Pin: {self.servoPinLeft.text()}")
        print(f"Right Servo Pin: {self.servoPinRight.text()}")
        print(f"SBUS Pin: {self.sbusPin.text()}")
        print(f"Main Motor Pin: {self.mainMotorPin.text()}")
        print(f"Tail Motor Pin: {self.tailMotorPin.text()}")
        print(f"SDA Pin: {self.sdaPin.text()}")
        print(f"SCL Pin: {self.sclPin.text()}")
        print(f"MPU Calibration Duration: {self.mpuCalibrationDuration.text()}")

if __name__ == '__main__':
    app = QApplication(sys.argv)
    widget = SettingsTab()
    widget.showMaximized()  # Start the window maximized
    sys.exit(app.exec_())
