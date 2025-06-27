"""
Main GUI application for LIDAR visualization
"""

import tkinter as tk
from tkinter import ttk, messagebox, StringVar, BooleanVar, IntVar
import threading
from pathlib import Path
import time
from typing import Optional, List

from lidar_sensor import LidarSensor
from lidar_visualization import LidarPlot

# Constants
PLOT_INTERVAL_MS = 100  # GUI update interval
DEFAULT_DISTANCE_THRESHOLD = 120
DEFAULT_MAX_RANGE = 1000
DEFAULT_BAUDRATES = ["9600", "19200", "38400", "57600", "115200"]
COLOR_OPTIONS = {
    "Blue": "#3366cc",
    "Red": "#cc3333",
    "Green": "#33cc33",
    "Purple": "#9933cc",
    "Orange": "#ff9900",
    "Cyan": "#33cccc"
}

class LidarApp:
    """
    Main GUI application for LIDAR visualization
    """
    def __init__(self, master: tk.Tk):
        """Initialize the LIDAR visualization application"""
        self.master = master
        master.title("LIDAR Visualization")
        master.geometry("1200x800")
        master.configure(bg='#f0f0f0')
        
        # Set theme
        self.style = ttk.Style()
        try:
            self.style.theme_use('clam')  # More modern theme
        except:
            pass  # Fall back to default if not available
        
        # Configure styles
        self.style.configure('TFrame', background='#f0f0f0')
        self.style.configure('TLabel', background='#f0f0f0')
        self.style.configure('TLabelframe', background='#f0f0f0')
        self.style.configure('TLabelframe.Label', background='#f0f0f0', font=('Arial', 10, 'bold'))
        self.style.configure('TButton', font=('Arial', 10))
        
        # Create main frames
        self.top_frame = ttk.Frame(master)
        self.top_frame.pack(side=tk.TOP, fill=tk.X, padx=10, pady=10)
        
        self.main_frame = ttk.Frame(master)
        self.main_frame.pack(side=tk.TOP, fill=tk.BOTH, expand=True, padx=10, pady=5)
        
        # Initialize LIDAR sensor
        self.sensor = LidarSensor()
        
        # Create components
        self.create_control_panel()
        self.create_visualization_panel()
        self.create_status_bar()
        
        # Variables
        self.is_connected = False
        self.is_running = False
        
        # Start reader thread
        self.reader_thread = threading.Thread(target=self.sensor.read_loop, daemon=True)
        self.reader_thread.start()
        
        # Setup plot update
        self.master.after(PLOT_INTERVAL_MS, self.update_visualization)
        
        # Bind window close event
        self.master.protocol("WM_DELETE_WINDOW", self.on_close)
        
    def create_control_panel(self) -> None:
        """Create connection and visualization controls"""
        # Connection panel
        conn_frame = ttk.LabelFrame(self.top_frame, text="Connection")
        conn_frame.pack(side=tk.LEFT, fill=tk.Y, padx=10, pady=5)
        
        ttk.Label(conn_frame, text="COM Port:").grid(row=0, column=0, padx=8, pady=(10, 5), sticky="w")
        self.port_var = StringVar()
        self.port_combo = ttk.Combobox(conn_frame, textvariable=self.port_var, width=15)
        self.port_combo.grid(row=0, column=1, padx=8, pady=(10, 5), sticky="w")
        self.refresh_ports()
        
        ttk.Button(conn_frame, text="Refresh", command=self.refresh_ports).grid(row=0, column=2, padx=8, pady=(10, 5))
        
        ttk.Label(conn_frame, text="Baudrate:").grid(row=1, column=0, padx=8, pady=5, sticky="w")
        self.baud_var = StringVar(value="115200")
        baud_combo = ttk.Combobox(conn_frame, textvariable=self.baud_var, values=DEFAULT_BAUDRATES, width=15)
        baud_combo.grid(row=1, column=1, padx=8, pady=5, sticky="w")
        
        self.connect_btn = ttk.Button(conn_frame, text="Connect", command=self.toggle_connection)
        self.connect_btn.grid(row=1, column=2, padx=8, pady=5)
        
        # Separator
        ttk.Separator(self.top_frame, orient="vertical").pack(side=tk.LEFT, fill=tk.Y, padx=5, pady=5)
        
        # Visualization settings panel
        viz_frame = ttk.LabelFrame(self.top_frame, text="Visualization Settings")
        viz_frame.pack(side=tk.LEFT, fill=tk.Y, padx=10, pady=5)
        
        # Mode selector
        ttk.Label(viz_frame, text="Mode:").grid(row=0, column=0, padx=8, pady=(10, 5), sticky="w")
        self.mode_var = StringVar(value="Live")
        self.mode_combo = ttk.Combobox(viz_frame, textvariable=self.mode_var, values=["Live", "Long Exposure"], 
                                      width=15, state="readonly")
        self.mode_combo.grid(row=0, column=1, padx=8, pady=(10, 5), sticky="w")
        self.mode_combo.bind("<<ComboboxSelected>>", self.on_mode_change)
        
        # Distance threshold
        ttk.Label(viz_frame, text="Min Distance:").grid(row=1, column=0, padx=8, pady=5, sticky="w")
        self.min_dist_var = StringVar(value=str(DEFAULT_DISTANCE_THRESHOLD))
        min_dist_entry = ttk.Entry(viz_frame, textvariable=self.min_dist_var, width=10)
        min_dist_entry.grid(row=1, column=1, padx=8, pady=5, sticky="w")
        self.min_dist_var.trace_add("write", self.on_min_distance_change)
        
        # Max range
        ttk.Label(viz_frame, text="Max Range:").grid(row=2, column=0, padx=8, pady=5, sticky="w")
        self.max_range_var = StringVar(value=str(DEFAULT_MAX_RANGE))
        max_range_entry = ttk.Entry(viz_frame, textvariable=self.max_range_var, width=10)
        max_range_entry.grid(row=2, column=1, padx=8, pady=5, sticky="w")
        self.max_range_var.trace_add("write", self.on_max_range_change)
        
        # Point color
        ttk.Label(viz_frame, text="Point Color:").grid(row=3, column=0, padx=8, pady=5, sticky="w")
        self.color_var = StringVar(value="Blue")
        color_combo = ttk.Combobox(viz_frame, textvariable=self.color_var, values=list(COLOR_OPTIONS.keys()),
                                  width=10, state="readonly")
        color_combo.grid(row=3, column=1, padx=8, pady=5, sticky="w")
        self.color_var.trace_add("write", self.on_color_change)
        
        # Separator
        ttk.Separator(self.top_frame, orient="vertical").pack(side=tk.LEFT, fill=tk.Y, padx=5, pady=5)
        
        # Control buttons panel
        ctrl_frame = ttk.LabelFrame(self.top_frame, text="Controls")
        ctrl_frame.pack(side=tk.LEFT, fill=tk.Y, padx=10, pady=5)
        
        # Action buttons
        self.start_btn = ttk.Button(ctrl_frame, text="Start", command=self.start, state=tk.DISABLED, width=10)
        self.start_btn.grid(row=0, column=0, padx=8, pady=(10, 5))
        
        self.stop_btn = ttk.Button(ctrl_frame, text="Stop", command=self.stop, state=tk.DISABLED, width=10)
        self.stop_btn.grid(row=0, column=1, padx=8, pady=(10, 5))
        
        self.reset_btn = ttk.Button(ctrl_frame, text="Reset", command=self.reset, state=tk.DISABLED, width=10)
        self.reset_btn.grid(row=1, column=0, padx=8, pady=5)
        
        self.save_btn = ttk.Button(ctrl_frame, text="Save PNG", command=self.save_as_png, state=tk.DISABLED, width=10)
        self.save_btn.grid(row=1, column=1, padx=8, pady=5)
        
    def create_visualization_panel(self) -> None:
        """Create the visualization panel with the LIDAR plot"""
        # Create the plot in the main frame
        self.plot = LidarPlot(self.main_frame)
        self.plot.canvas_widget.pack(fill=tk.BOTH, expand=True)
        
    def create_status_bar(self) -> None:
        """Create status bar at the bottom of the window"""
        status_frame = ttk.Frame(self.master)
        status_frame.pack(side=tk.BOTTOM, fill=tk.X)
        
        ttk.Separator(status_frame, orient="horizontal").pack(fill=tk.X)
        
        self.status_var = StringVar(value="Disconnected")
        self.status_label = ttk.Label(status_frame, textvariable=self.status_var, padding=(5, 2))
        self.status_label.pack(side=tk.LEFT)
        
        self.points_var = StringVar(value="Points: 0")
        self.points_label = ttk.Label(status_frame, textvariable=self.points_var, padding=(5, 2))
        self.points_label.pack(side=tk.RIGHT)
        
    def refresh_ports(self) -> None:
        """Update the list of available serial ports"""
        ports = LidarSensor.get_available_ports()
        self.port_combo['values'] = ports
        if ports and not self.port_var.get() in ports:
            self.port_var.set(ports[0])
    
    def toggle_connection(self) -> None:
        """Connect to or disconnect from the LIDAR sensor"""
        if self.is_connected:
            self.disconnect()
        else:
            self.connect()
    
    def connect(self) -> None:
        """Establish connection to LIDAR sensor"""
        port = self.port_var.get()
        if not port:
            messagebox.showwarning("Connection Error", "No COM port selected.")
            return
            
        try:
            baudrate = int(self.baud_var.get())
        except ValueError:
            messagebox.showwarning("Connection Error", "Invalid baudrate.")
            return
        
        # Disconnect if already connected
        self.sensor.disconnect()
        
        result = self.sensor.connect(port, baudrate)
        if isinstance(result, tuple) and not result[0]:
            messagebox.showerror("Connection Error", f"Could not connect to {port}: {result[1]}")
            return
            
        self.is_connected = True
        self.connect_btn.config(text="Disconnect")
        self.start_btn.config(state=tk.NORMAL)
        self.reset_btn.config(state=tk.NORMAL)
        
        self.status_var.set(f"Connected to {port} at {baudrate} baud")
        self.status_label.configure(foreground="#006600")
    
    def disconnect(self) -> None:
        """Disconnect from the LIDAR sensor"""
        self.stop()  # Stop acquisition first
        self.sensor.disconnect()
        
        self.is_connected = False
        self.connect_btn.config(text="Connect")
        self.start_btn.config(state=tk.DISABLED)
        self.stop_btn.config(state=tk.DISABLED)
        self.reset_btn.config(state=tk.DISABLED)
        self.save_btn.config(state=tk.DISABLED)
        
        self.status_var.set("Disconnected")
        self.status_label.configure(foreground="#990000")
    
    def on_mode_change(self, event=None) -> None:
        """Handle visualization mode change"""
        is_long_exposure = self.mode_var.get() == "Long Exposure"
        self.sensor.set_mode(is_long_exposure)
        
        if not is_long_exposure:
            # Reset the plot when switching to live mode
            self.reset()
    
    def on_min_distance_change(self, *args) -> None:
        """Handle minimum distance threshold change"""
        try:
            threshold = int(self.min_dist_var.get())
            if threshold >= 0:
                self.sensor.set_distance_threshold(threshold)
        except ValueError:
            pass  # Ignore invalid input
    
    def on_max_range_change(self, *args) -> None:
        """Handle maximum range change"""
        try:
            max_range = int(self.max_range_var.get())
            if max_range > 0:
                self.plot.set_max_range(max_range)
        except ValueError:
            pass  # Ignore invalid input
    
    def on_color_change(self, *args) -> None:
        """Handle point color change"""
        color_name = self.color_var.get()
        if color_name in COLOR_OPTIONS:
            color = COLOR_OPTIONS[color_name]
            self.plot.set_point_color(color)
    
    def start(self) -> None:
        """Start data acquisition"""
        if self.sensor.start_acquisition():
            self.is_running = True
            self.start_btn.config(state=tk.DISABLED)
            self.stop_btn.config(state=tk.NORMAL)
            self.save_btn.config(state=tk.NORMAL)
            self.status_var.set(f"{self.status_var.get()} - Running")
        else:
            messagebox.showerror("Error", "Could not start data acquisition. Check connection.")
    
    def stop(self) -> None:
        """Stop data acquisition"""
        self.sensor.stop_acquisition()
        self.is_running = False
        self.start_btn.config(state=tk.NORMAL)
        self.stop_btn.config(state=tk.DISABLED)
        
        # Update status
        status_text = self.status_var.get()
        if " - Running" in status_text:
            self.status_var.set(status_text.replace(" - Running", " - Stopped"))
    
    def reset(self) -> None:
        """Reset visualization"""
        self.sensor.clear_data()
        self.plot.reset()
    
    def save_as_png(self) -> None:
        """Save visualization as PNG"""
        try:
            downloads_path = Path.home() / "Downloads"
            filename = downloads_path / f"Lidar_Visualization_{time.strftime('%Y%m%d_%H%M%S')}.png"
            self.plot.fig.savefig(filename)
            messagebox.showinfo("Save Successful", f"Saved to {filename}")
        except Exception as e:
            messagebox.showerror("Save Error", f"Could not save file: {e}")
    
    def update_visualization(self) -> None:
        """Update the visualization with new data"""
        data = self.sensor.get_data()
        
        if data:
            self.points_var.set(f"Points: {len(data)}")
            
            # Update visualization based on mode
            if self.mode_var.get() == "Long Exposure":
                self.plot.update_long_exposure(data)
            else:
                self.plot.update_live(data)
        
        self.master.after(PLOT_INTERVAL_MS, self.update_visualization)
    
    def on_close(self) -> None:
        """Handle window close event"""
        if self.is_connected:
            self.disconnect()
        self.master.destroy()