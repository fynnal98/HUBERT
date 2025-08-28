import serial
import csv
import threading
import time
from PyQt5.QtWidgets import QWidget, QPushButton, QLabel, QVBoxLayout, QLineEdit, QMessageBox, QSpinBox, QHBoxLayout
from datetime import datetime
import os

class LoggerTab(QWidget):
    def __init__(self):
        super().__init__()

        # Variables
        self.serial_port = None
        self.is_logging = False
        self.filename = ""
        self.start_time = 0
        self.timer_thread = None
        self.timer_set = False

        # GUI elements
        self.start_button = QPushButton("Start Recording")
        self.start_button.clicked.connect(self.start_recording)

        self.stop_button = QPushButton("Stop Recording")
        self.stop_button.setEnabled(False)
        self.stop_button.clicked.connect(self.stop_recording)

        self.status_label = QLabel("Status: Not Recording")

        self.timer_label = QLabel("Timer: 00:00:00")

        # Text field to customize file name
        self.file_name_edit = QLineEdit(self)
        self.file_name_edit.setPlaceholderText("Enter custom name (optional)")

        # Timer input (in seconds)
        self.timer_spinbox = QSpinBox(self)
        self.timer_spinbox.setRange(0, 3600)  # Set max timer limit to 3600 seconds (1 hour)
        self.timer_spinbox.setPrefix("Stop after: ")
        self.timer_spinbox.setSuffix(" sec")
        self.timer_spinbox.setValue(0)  # Default value is 0 (no timer)

        # Layout setup
        layout = QVBoxLayout()
        layout.addWidget(self.start_button)
        layout.addWidget(self.stop_button)
        layout.addWidget(self.file_name_edit)
        layout.addWidget(self.status_label)
        layout.addWidget(self.timer_label)
        layout.addWidget(self.timer_spinbox)

        layout.addStretch()  # Fügt einen Spacer hinzu, der den Platz nach oben drückt

        self.setLayout(layout)

    def start_timer(self):
        self.start_time = time.time()
        self.timer_thread = threading.Thread(target=self.update_timer)
        self.timer_thread.start()

    def update_timer(self):
        while self.is_logging:
            elapsed_time = time.time() - self.start_time
            hours, rem = divmod(elapsed_time, 3600)
            minutes, seconds = divmod(rem, 60)
            self.timer_label.setText(f"Timer: {int(hours):02}:{int(minutes):02}:{int(seconds):02}")

            # Check if timer is set and time has expired
            if self.timer_set and elapsed_time >= self.timer_spinbox.value():
                self.stop_recording()
                break

            time.sleep(1)

    def start_recording(self):
        # Get current date and time
        now = datetime.now().strftime("%Y-%m-%d_%H-%M-%S")
        custom_name = self.file_name_edit.text()

        # Construct file name
        if custom_name:
            file_name = f"{now}_{custom_name}.csv"
        else:
            file_name = f"{now}.csv"

        # Create the CSV folder inside the 'py' folder if it doesn't exist
        py_folder = os.path.join(os.getcwd(), "py")
        csv_folder = os.path.join(py_folder, "log")
        if not os.path.exists(csv_folder):
            os.makedirs(csv_folder)

        # Full file path
        self.filename = os.path.join(csv_folder, file_name)

        try:
            self.serial_port = serial.Serial('COM5', 115200, timeout=1)
        except serial.SerialException as e:
            QMessageBox.critical(self, "Error", f"Failed to open serial port: {e}")
            return

        self.is_logging = True
        self.start_button.setEnabled(False)
        self.stop_button.setEnabled(True)
        self.status_label.setText(f"Status: Recording... Saving to {self.filename}")

        # Start the logging and timer
        self.thread = threading.Thread(target=self.log_data)
        self.thread.start()
        self.start_timer()

        # Check if a timer is set (greater than 0 seconds)
        if self.timer_spinbox.value() > 0:
            self.timer_set = True
        else:
            self.timer_set = False

    def log_data(self):
        with open(self.filename, mode='w', newline='') as csv_file:
            csv_writer = csv.writer(csv_file)
            csv_writer.writerow(['gx', 'gy', 'rollFiltered', 'pitchFiltered'])

            while self.is_logging:
                if self.serial_port.in_waiting > 0:
                    line = self.serial_port.readline().decode('utf-8').strip()
                    if line.startswith("Raw:"):
                        raw_data, filtered_data = line.split(" | Filtered: ")
                        raw_values = raw_data.replace("Raw: ", "").split(",")
                        filtered_values = filtered_data.split(",")
                        csv_writer.writerow(raw_values + filtered_values)
                time.sleep(0.1)

        self.serial_port.close()

    def stop_recording(self):
        self.is_logging = False
        self.start_button.setEnabled(True)
        self.stop_button.setEnabled(False)
        self.status_label.setText(f"Status: Not Recording - Saved to {self.filename}")
        self.timer_label.setText("Timer: 00:00:00")
