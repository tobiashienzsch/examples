import random
import sys

from PySide2 import QtCore, QtGui, QtWidgets
import pyaudio


def query_audio_devices():
    p = pyaudio.PyAudio()
    for device in range(p.get_device_count()):
        info = p.get_device_info_by_index(device)
        name = info['name']
        max_input = info['maxInputChannels']
        max_output = info['maxOutputChannels']
        print(f'name: {name} input: {max_input} output: {max_output}')
    p.terminate()


class AudioDeviceSettings(QtWidgets.QWidget):
    def __init__(self):
        super(AudioDeviceSettings, self).__init__()

        layout = QtWidgets.QVBoxLayout(self)

        self.label = QtWidgets.QLabel()
        self.label.setAlignment(QtCore.Qt.AlignHCenter | QtCore.Qt.AlignTop)
        self.label.setText("test")
        layout.addWidget(self.label)

        self.setLayout(layout)
        query_audio_devices()


class MainWindow(QtWidgets.QMainWindow):
    def __init__(self):
        super(MainWindow, self).__init__()
        self.setWindowTitle("MC DAW")
        self.setFixedSize(1280, 720)
        self.createLayout()
        self.createMenus()
        self.createStatusBar()

    def createLayout(self):
        self._main = QtWidgets.QWidget()
        self.setCentralWidget(self._main)
        layout = QtWidgets.QVBoxLayout(self._main)

        self.audio_settings = AudioDeviceSettings()
        layout.addWidget(self.audio_settings)

        self.model = QtWidgets.QFileSystemModel()
        path = QtCore.QDir.currentPath()
        print(path)
        self.model.setRootPath(path)
        self.tree = QtWidgets.QTreeView()
        self.tree.setModel(self.model)

        self.tree.setAnimated(False)
        self.tree.setIndentation(20)
        self.tree.setSortingEnabled(True)
        layout.addWidget(self.tree)

    def createMenus(self):
        self.fileMenu = self.menuBar().addMenu("File")

    def createStatusBar(self):
        self.statusBar().showMessage("Ready")

    def paintEvent(self, e):
        qp = QtGui.QPainter()
        qp.begin(self)
        qp.end()


if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)
    mainWin = MainWindow()
    mainWin.show()
    sys.exit(app.exec_())
