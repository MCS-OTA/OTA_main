import tkinter as tk
from OTA_GUI import OTA_GUI

root = tk.Tk()
app = OTA_GUI(root)
root.mainloop()

print(f"slelelf, {app.selected_time}")
