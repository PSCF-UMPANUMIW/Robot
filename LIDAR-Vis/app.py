import serial
import matplotlib.pyplot as plt
import math
from dataclasses import dataclass

DISTANCES_PER_PACKET = 4

@dataclass
class LidarPoint:
    distance: int
    angle: int


class LidarVis:
    def __init__(self, port, baudrate):
        self.datalen = 6
        self.port = port
        self.baudrate = baudrate
        self.data : list[LidarPoint] = []
        self.MAX_POINTS = 360
    
    def connect_serial(self):
        try:
            self.serial = serial.Serial(self.port, self.baudrate)
            print(f"Connected to {self.port} at {self.baudrate} baud.")
        except serial.SerialException as e:
            print(f"Error connecting to serial port: {e}")
            return False
        return True

    def read_and_visualize(self):
        # Create the polar plot
        fig = plt.figure(figsize=(8, 8))
        ax = fig.add_subplot(111, projection='polar')
        ax.set_title("LIDAR Visualization")
        ax.set_theta_zero_location("N")  # 0 degrees at the top (North)
        scatter = ax.scatter([], [], s=10)
        plt.ion()  # Interactive mode on
        plt.show(block=False)
        
        try:
            while True:
                print(self.serial.in_waiting)
                if self.serial.in_waiting > 0:
                    try:
                        sensor_data = self.serial.readline().decode().rstrip().split(' ')
                        for i in range(DISTANCES_PER_PACKET):
                            angle = int(sensor_data[0]) + i
                            dist = int(sensor_data[2+i])
                            self.data.append(LidarPoint(dist, angle))
                    except:
                        print("No newline in buffer?")
                        continue
                
                if len(self.data) >= self.MAX_POINTS:
                    self._update_plot(ax, scatter)
                    plt.pause(0.1)
                    self.data = []

        except KeyboardInterrupt:
            print("Data collection interrupted by user.")
        
        
    def _update_plot(self, ax, scatter):
        if not self.data:
            return
            
        # Extract angles and distances
        angles = [math.radians(point.angle) for point in self.data]
        distances = [point.distance for point in self.data]
        
        # Clear previous data
        scatter.set_offsets([(a, r) for a, r in zip(angles, distances)])
        
        ax.set_rlim(0, 4000)  # Add 10% margin
        
        plt.draw()


def main():
    port = "/dev/ttyUSB0"
    baudrate = 115200
    
    lidar_vis = LidarVis(port, baudrate)
    
    if lidar_vis.connect_serial():
        lidar_vis.read_and_visualize()
    else:
        print("Failed to connect to LIDAR device.")

if __name__ == "__main__":
    main()