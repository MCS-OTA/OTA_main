from PyQt5.QtWidgets import (
    QApplication, QWidget, QLabel, QPushButton, QVBoxLayout, QHBoxLayout, QMessageBox, QDialog
)
import sys


class NoDialog(QDialog):
    def __init__(self, parent=None):
        super(NoDialog, self).__init__(parent)
        self.setWindowTitle("Next Update Alarm")
        self.setFixedSize(300, 200)
        self.selected_time = None

        layout = QVBoxLayout()
        label = QLabel("Next Update Time:")
        layout.addWidget(label)

        self.time_options = {
            "5 sec": 5,
            "1 Hour": 3600,
            "1 Day": 86400,
            "1 Week": 604800
        }

        for text, value in self.time_options.items():
            btn = QPushButton(text)
            btn.clicked.connect(lambda checked, v=value, t=text: self.confirm_selection(v, t))
            layout.addWidget(btn)

        self.setLayout(layout)

    def confirm_selection(self, value, text):
        self.selected_time = value
        QMessageBox.information(self, "Confirmed", f"Next alarm will be in {text}.")
        self.accept()


class OTA_GUI(QWidget):
    def __init__(self, yes_callback, no_callback):
        super().__init__()
        self.setWindowTitle("Update Alarm")
        self.setFixedSize(300, 150)
        self.yes_callback = yes_callback
        self.no_callback = no_callback
        self.init_ui()

    def init_ui(self):
        layout = QVBoxLayout()
        label = QLabel("New Update has arrived.\nUpdate Now?")
        layout.addWidget(label)

        button_layout = QHBoxLayout()

        yes_btn = QPushButton("Yes")
        yes_btn.clicked.connect(self.on_yes_click)
        button_layout.addWidget(yes_btn)

        no_btn = QPushButton("No")
        no_btn.clicked.connect(self.on_no_click)
        button_layout.addWidget(no_btn)

        layout.addLayout(button_layout)
        self.setLayout(layout)

    def on_yes_click(self):
        QMessageBox.information(self, "Start Update Now", "Start Update Now")
        if self.yes_callback:
            self.yes_callback()
        self.close()
        QApplication.quit()

    def on_no_click(self):
        dialog = NoDialog(self)
        if dialog.exec_():
            wait_time = dialog.selected_time
            if self.no_callback:
                self.no_callback(wait_time)
            self.close()
            QApplication.quit()


def show_update_gui(yes_callback, no_callback):
    app = QApplication(sys.argv)
    gui = OTA_GUI(yes_callback, no_callback)
    gui.show()
    app.exec_()

