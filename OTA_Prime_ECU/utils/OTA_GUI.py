import tkinter as tk
from tkinter import messagebox

class OTA_GUI:
    def __init__(self, root):
        self.root = root
        self.root.title("Update Alarm")
        self.root.geometry("300x150")
        self.selected_time = None 

        tk.Label(root, text="New Update has arrived.\nUpdate Now?", font=("Arial", 12)).pack(pady=10)

        tk.Button(root, text="Yes", command=self.on_yes_click, width=10).pack(side="left", padx=20, pady=10)
        tk.Button(root, text="No", command=self.on_no_click, width=10).pack(side="right", padx=20, pady=10)

    def on_yes_click(self):
        messagebox.showinfo("Start Update Now", "Start Update Now")
        self.selected_time = 0
        self.root.destroy()

    def on_no_click(self):
        no_window = tk.Toplevel(self.root)
        no_window.title("Next Update Alarm")
        no_window.geometry("300x200")

        tk.Label(no_window, text="Next Update Time:", font=("Arial", 10)).pack(pady=10)

        time_options = {"5 sec": 5, "1 Hour": 3600, "1 Day": 86400, "1 Week": 604800}

        def confirm_selection(value):
            self.selected_time = value
            messagebox.showinfo("Confirmed", f"Next alarm will be in {list(time_options.keys())[list(time_options.values()).index(value)]}.")
            no_window.destroy()
            self.root.destroy()
        for text, value in time_options.items():
            tk.Button(no_window, text=text, command=lambda v=value: confirm_selection(v), width=15).pack(pady=5)
        tk.Button(no_window, text="확인", command=confirm_selection).pack(pady=10)

