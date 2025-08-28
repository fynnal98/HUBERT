import sys
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FigureCanvas
from PyQt5.QtWidgets import QWidget, QVBoxLayout, QPushButton, QFileDialog, QLabel, QLineEdit, QHBoxLayout, QSpacerItem, QSizePolicy


class FreqPlotCanvas(FigureCanvas):
    def __init__(self, parent=None, width=5, height=4, dpi=100):
        fig, (self.ax1, self.ax2) = plt.subplots(2, 1, figsize=(width, height), dpi=dpi)
        super(FreqPlotCanvas, self).__init__(fig)
        self.setParent(parent)

    def plot_freq(self, freq, magnitude_x, magnitude_y, phase_x, phase_y, title, max_freq):
        self.ax1.clear()
        self.ax2.clear()

        # Magnitude plot
        self.ax1.set_title(f'{title} - Magnitude')
        self.ax1.plot(freq, 20 * np.log10(magnitude_x), label='X-axis')
        self.ax1.plot(freq, 20 * np.log10(magnitude_y), label='Y-axis')
        self.ax1.set_ylabel('Magnitude (dB)')
        self.ax1.set_xscale('log')
        self.ax1.grid(True)
        self.ax1.legend()
        self.ax1.set_xlim([0.1, max_freq])  # Dynamische Anpassung der X-Achse

        # Phase plot
        self.ax2.set_title(f'{title} - Phase')
        self.ax2.plot(freq, np.degrees(phase_x), label='X-axis')
        self.ax2.plot(freq, np.degrees(phase_y), label='Y-axis')
        self.ax2.set_ylabel('Phase (degrees)')
        self.ax2.set_xlabel('Frequency (Hz)')
        self.ax2.set_xscale('log')
        self.ax2.grid(True)
        self.ax2.legend()
        self.ax2.set_xlim([0.1, max_freq])  # Dynamische Anpassung der X-Achse

        self.draw()


class FreqPlotTab(QWidget):
    def __init__(self):
        super().__init__()

        self.raw_x = None
        self.raw_y = None
        self.filtered_x = None
        self.filtered_y = None
        self.sampling_rate = 1000.0  # Standard-Abtastrate

        # UI Elements
        self.label = QLabel("Load a CSV file to generate plots", self)
        self.load_button = QPushButton("Load CSV", self)
        self.load_button.clicked.connect(self.load_csv)

        # Create canvases for raw and filtered data
        self.raw_canvas = FreqPlotCanvas(self, width=5, height=10)
        self.filtered_canvas = FreqPlotCanvas(self, width=5, height=10)

        # Abtastrate Eingabe
        self.sampling_rate_label = QLabel(f"Sampling Rate (Hz):", self)
        self.sampling_rate_input = QLineEdit(str(self.sampling_rate), self)
        self.sampling_rate_input.editingFinished.connect(self.update_sampling_rate)

        # Nyquist-Frequenz Anzeige
        self.nyquist_label = QLabel(f"Max Frequency (Nyquist): {self.sampling_rate / 2} Hz", self)

        # Layout setup
        layout = QVBoxLayout()

        # Abtastrate und Nyquist-Frequenz Layout
        sampling_layout = QHBoxLayout()
        sampling_layout.addWidget(self.sampling_rate_label)
        sampling_layout.addWidget(self.sampling_rate_input)
        sampling_layout.addWidget(self.nyquist_label)

        layout.addLayout(sampling_layout)
        layout.addWidget(self.label)
        layout.addWidget(self.load_button)

        plot_layout = QVBoxLayout()
        plot_layout.addWidget(self.raw_canvas)
        plot_layout.addWidget(self.filtered_canvas)

        # Spacer einfügen
        spacer = QSpacerItem(20, 40, QSizePolicy.Minimum, QSizePolicy.Expanding)
        plot_layout.addItem(spacer)

        layout.addLayout(plot_layout)
        self.setLayout(layout)

    def update_sampling_rate(self):
        try:
            # Abtastrate aktualisieren und Nyquist-Frequenz berechnen
            self.sampling_rate = float(self.sampling_rate_input.text())
            nyquist_freq = self.sampling_rate / 2
            self.nyquist_label.setText(f"Max Frequency (Nyquist): {nyquist_freq:.2f} Hz")

            if self.raw_x is not None and self.raw_y is not None:
                self.process_data()
        except ValueError:
            self.label.setText("Invalid sampling rate entered")

    def load_csv(self):
        options = QFileDialog.Options()
        file_name, _ = QFileDialog.getOpenFileName(self, "Open CSV File", "", "CSV Files (*.csv)", options=options)
        if file_name:
            self.label.setText(f"Loaded: {file_name}")
            self.process_data(file_name)

    def process_data(self, file_name=None):
        if file_name:
            df = pd.read_csv(file_name)
            self.raw_x = df['gx'].values  # Gyro X-Achse
            self.raw_y = df['gy'].values  # Gyro Y-Achse
            self.filtered_x = df['rollFiltered'].values
            self.filtered_y = df['pitchFiltered'].values


        # Anzahl der Datenpunkte
        N = len(self.raw_x)

        # Frequenzachse berechnen basierend auf der Abtastrate
        freq = np.fft.fftfreq(N, d=1/self.sampling_rate)

        # Nur die positive Frequenzhälfte behalten (da FFT symmetrisch ist)
        freq = freq[:N // 2]

        # FFT Berechnungen (nur die positive Hälfte)
        magnitude_x_raw = np.abs(np.fft.fft(self.raw_x))[:N // 2]
        phase_x_raw = np.angle(np.fft.fft(self.raw_x))[:N // 2]

        magnitude_y_raw = np.abs(np.fft.fft(self.raw_y))[:N // 2]
        phase_y_raw = np.angle(np.fft.fft(self.raw_y))[:N // 2]

        magnitude_x_filtered = np.abs(np.fft.fft(self.filtered_x))[:N // 2]
        phase_x_filtered = np.angle(np.fft.fft(self.filtered_x))[:N // 2]

        magnitude_y_filtered = np.abs(np.fft.fft(self.filtered_y))[:N // 2]
        phase_y_filtered = np.angle(np.fft.fft(self.filtered_y))[:N // 2]

        # Max Frequenz (Nyquist) berechnen
        max_freq = self.sampling_rate / 2

        # Plot aktualisieren
        self.raw_canvas.plot_freq(freq, magnitude_x_raw, magnitude_y_raw, phase_x_raw, phase_y_raw, "Raw Data (X, Y)", max_freq)
        self.filtered_canvas.plot_freq(freq, magnitude_x_filtered, magnitude_y_filtered, phase_x_filtered, phase_y_filtered, "Filtered Data (X, Y)", max_freq)
