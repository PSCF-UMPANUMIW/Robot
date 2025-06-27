"""
Main entry point for LIDAR visualization application
"""

import tkinter as tk
from gui import LidarApp

def main():
    """Main function to start the application"""
    root = tk.Tk()
    app = LidarApp(root)
    root.mainloop()

if __name__ == "__main__":
    main()