# LiDAR Visualization

Visualization tool for LiDAR sensor data.

![LiDAR Visualization](images/lidar_visualization.png)

## Features

- **Visualization** in a polar plot
- **Two visualizations mods**
    - **Real-time LiDAR data visualization** shows only the current scan data and refreshes completely with each update
    - **Long-exposure mode** accumulates points over time and creates more accurate images reducing noise
- **Fully configurable** through an GUI interface
- **Data export** with PNG saving capability

## Requirements

- matplotlib
- pyserial