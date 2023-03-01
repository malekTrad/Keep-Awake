#Code to make the mouse click to prevent teams from going "away" status
import ctypes
import time

try:
    print("to stop the code, press CTRL+C, or simply close the window")
    while True:
        ctypes.windll.user32.SetCursorPos(0,0)
        ctypes.windll.user32.mouse_event(2, 0, 0, 0, 0)
        ctypes.windll.user32.mouse_event(4, 0, 0, 0, 0)
        time.sleep(60) 
except KeyboardInterrupt as e:
    print("exiting")
