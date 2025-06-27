"""
Visualization components for LIDAR data
"""

import matplotlib.pyplot as plt
from matplotlib.figure import Figure
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from typing import List, Optional, Tuple
import tkinter as tk
import colorsys

from lidar_data import LidarPoint

class LidarPlot:
    """
    Manages the polar plot visualization of LIDAR data
    """
    def __init__(self, master: tk.Widget, figsize: Tuple[int, int] = (8, 8)):
        """Initialize the LIDAR visualization plot"""
        self.master = master
        self.fig = Figure(figsize=figsize)
        
        # Create the polar plot
        self.ax = self.fig.add_subplot(111, projection='polar')
        self.ax.set_title("LIDAR Visualization")
        self.ax.set_theta_zero_location("N")  # 0 degrees at the top (North)
        self.ax.set_rlim(0, 1000)
        self.ax.grid(True, alpha=0.6)
        
        # Create the canvas widget
        self.canvas = FigureCanvasTkAgg(self.fig, master=master)
        self.canvas_widget = self.canvas.get_tk_widget()
        
        # For live mode
        self.scatter = None
        
        # Configuration
        self.point_size = 5
        self.point_color = "#3366cc"
        self.long_exposure_alpha = 0.05
        self.live_alpha = 0.8
        
        # Apply light theme
        self.apply_theme()
    
    def apply_theme(self) -> None:
        """Apply visual theme to plot"""
        self.fig.set_facecolor('#ffffff')
        self.ax.set_facecolor('#f8f8f8')
        self.ax.tick_params(colors='#333333')
        self.ax.set_title("LIDAR Visualization", color='#333333')
        self.ax.spines['polar'].set_color('#cccccc')
        self.ax.grid(color='#cccccc')
        self.canvas.draw_idle()
    
    def reset(self) -> None:
        """Reset the plot to empty state"""
        self.ax.clear()
        self.ax.set_theta_zero_location("N")
        self.ax.set_rlim(0, 1000)
        self.ax.set_title("LIDAR Visualization", color='#333333')
        self.ax.grid(True, alpha=0.6)
        self.ax.set_facecolor('#f8f8f8')
        self.ax.tick_params(colors='#333333')
        self.ax.grid(color='#cccccc')
        self.scatter = None
        self.canvas.draw_idle()
    
    def set_max_range(self, max_range: int) -> None:
        """Set maximum range for the visualization"""
        self.ax.set_rlim(0, max_range)
        self.canvas.draw_idle()
    
    def update_live(self, points: List[LidarPoint]) -> None:
        """Update visualization in live mode"""
        if not points:
            return
            
        angles = [point.angle_rad for point in points]
        distances = [point.distance for point in points]
        
        # Remove previous scatter if it exists
        if self.scatter:
            self.scatter.remove()
        
        # Draw new scatter plot
        self.scatter = self.ax.scatter(
            angles, 
            distances, 
            s=self.point_size,
            color=self.point_color,
            alpha=self.live_alpha
        )
        
        self.canvas.draw_idle()
    
    def update_long_exposure(self, points: List[LidarPoint]) -> None:
        """Update visualization in long exposure mode"""
        if not points:
            return
            
        angles = [point.angle_rad for point in points]
        distances = [point.distance for point in points]
        
        # Draw new points with low alpha for trail effect
        self.ax.scatter(
            angles, 
            distances, 
            s=self.point_size, 
            color=self.point_color,
            alpha=self.long_exposure_alpha
        )
        
        self.canvas.draw_idle()
    
    def set_point_color(self, color: str) -> None:
        """Set the color of data points"""
        self.point_color = color
        
    def set_point_size(self, size: int) -> None:
        """Set the size of data points"""
        self.point_size = size