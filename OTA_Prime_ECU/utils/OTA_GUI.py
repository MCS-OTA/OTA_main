from PyQt5.QtWidgets import (
    QApplication, QWidget, QLabel, QPushButton,
    QVBoxLayout, QHBoxLayout, QMessageBox, QDialog
)
from PyQt5.QtCore import QTimer
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
    def __init__(self, app):
        super().__init__()
        self.app = app
        self.selected_time = None
        self.init_ui()

    def init_ui(self):
        self.setWindowTitle("Update Alarm")
        self.setFixedSize(300, 150)

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
        print("Update Now!")
        self.app.quit()

    def on_no_click(self):
        dialog = NoDialog(self)
        if dialog.exec_():
            wait_time = dialog.selected_time
            print(f"Wait {wait_time} sec until next alarm...")
            QTimer.singleShot(wait_time * 1000, self.show_again)
            self.close()

    def show_again(self):
        new_gui = OTA_GUI(self.app)
        new_gui.show()


def main():
    app = QApplication(sys.argv)
    gui = OTA_GUI(app)
    gui.show()
    sys.exit(app.exec_())


if __name__ == "__main__":
    main()
