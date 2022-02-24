


# ------------------- imports ------------------
from asyncio.windows_events import NULL
from cgi import test
from cgitb import text
from ctypes import alignment
from select import select
from sys import maxsize
from textwrap import fill
import tkinter as tk
from tkinter import ttk
from tkinter import font
from tkinter.font import BOLD, ITALIC
from tkinter import *
from PIL import Image, ImageTk
from turtle import bgcolor, left, right, screensize, width
from numpy import left_shift, size







# ------------- global variables -------------

root = tk.Tk()
list_sel = tk.StringVar()
list_sel.set("No ESP selected")





def key_left(event):
    print("Left key")
root.bind("<Left>", key_left)

def key_right(event):
    print("Right key")
root.bind("<Right>", key_right)

def key_up(event):
    print("Up key")
root.bind("<Up>", key_up)

def key_down(event):
    print("Down key")
root.bind("<Down>", key_down)

# ------------- window properties -------------

root.title("ESP32 - Cam controller")
root.geometry("400x400")
root.minsize(width=600, height=300)
root.iconbitmap("img/icon.ico")

# --------------- window layout --------------

titlebar = tk.Label(text="ESP32 Cam Controller", font=('Arial', 20, BOLD,), fg="#424242")
titlebar.pack()



MainFrame = tk.Frame(root, bg="red")
MainFrame.pack(fill=BOTH, expand=TRUE)


left_frame = tk.Frame(MainFrame, bg="lightcyan")
left_frame.pack(side=LEFT, fill="both", expand=True)

right_frame = tk.Frame(MainFrame, bg="cyan", width=100)
right_frame.pack(side=LEFT, fill="both", expand=True)


#------------------ images -------------------

image_controller = Image.open("img/controller.png").resize((30, 30))
icon_controller = ImageTk.PhotoImage(image_controller)

image_gimbal = Image.open("img/gimbal.png").resize((30, 30))
icon_gimbal = ImageTk.PhotoImage(image_gimbal)

#----------------- left frame ----------------

title_lf = tk.Label(left_frame, text="ESP32 in Network", image=icon_controller, compound="left").pack(anchor=W, fill=X)

Lb1 = tk.Listbox(left_frame, activestyle='none', selectbackground="darkgrey", font=('Arial', 12, BOLD), width=15)
for i in range(0, 6):
    Lb1.insert(i, "ESP32.0" + str(i))
Lb1.pack(anchor=W, fill=X)


#---------------- right frame ----------------
title_rf = tk.Label(right_frame, text="Selected CAM:", image=icon_gimbal, compound="left").pack(anchor=W, fill=X) 
sel_lable = ttk.Label(right_frame, textvariable=list_sel, font=("Arial", 10, BOLD)).pack()



# Control Frame
control_frame = tk.Frame(right_frame, bg="darkcyan")
control_frame.pack(fill=BOTH, expand=TRUE)

button_left = tk.Button(control_frame, text="Left", command= lambda: key_left(NULL), width=5, height=2)
button_left.grid(row=1, column=0)

button_up = tk.Button(control_frame, text="Up", command= lambda: key_up(NULL), width=5, height=2)
button_up.grid(row=0, column=1)

button_right = tk.Button(control_frame, text="Right", command= lambda: key_right(NULL), width=5, height=2)
button_right.grid(row=1, column=2)

button_down = tk.Button(control_frame, text="Down", command=key_down, width=5, height=2)
button_down.grid(row=2, column=1)





# ------------------ functions -----------------
def lb_callback(event):
    print("Selection detected")
    selection = event.widget.get(event.widget.curselection())
    print(str(selection))
    list_sel.set(selection)
Lb1.bind("<<ListboxSelect>>", lb_callback)








root.mainloop()