"""
LIDAR sensor communication module
"""

import serial
import serial.tools.list_ports
import threading
import time
from typing import List, Tuple, Optional, Union

from lidar_data import LidarPoint

DISTANCES_PER_PACKET = 4

class LidarSensor:
    """
    Handles communication with the LIDAR sensor hardware
    """
    def __init__(self):
        self.serial = None
        self.port = None
        self.baudrate = None
        self.data = []
        self.lock = threading.Lock()
        self.running = False
        self.distance_threshold = 120
        self.is_long_exposure_mode = False
        
    def connect(self, port: str, baudrate: int) -> Union[bool, Tuple[bool, str]]:
        """
        Connect to LIDAR sensor on specified port with specified baudrate
        
        Returns True if successful, or (False, error_message) if failed
        """
        try:
            self.port = port
            self.baudrate = baudrate
            self.serial = serial.Serial(port, baudrate, timeout=0.1)
            return True
        except serial.SerialException as e:
            return False, str(e)
    
    def disconnect(self) -> None:
        """Disconnect from the LIDAR sensor"""
        if self.serial and self.serial.is_open:
            self.serial.close()
    
    def start_acquisition(self) -> bool:
        """Start data acquisition from LIDAR sensor"""
        with self.lock:
            if self.serial and self.serial.is_open:
                self.running = True
                return True
            return False
    
    def stop_acquisition(self) -> None:
        """Stop data acquisition from LIDAR sensor"""
        with self.lock:
            self.running = False
    
    def clear_data(self) -> None:
        """Clear all stored data points"""
        with self.lock:
            self.data = []
    
    def get_data(self) -> List[LidarPoint]:
        """
        Get a copy of the current data points
        In live mode, clears the buffer after returning data
        """
        with self.lock:
            data_copy = self.data.copy()
            if not self.is_long_exposure_mode:
                self.data = []  # Clear data in live mode
            return data_copy
    
    def set_mode(self, long_exposure_mode: bool) -> None:
        """Set visualization mode"""
        self.is_long_exposure_mode = long_exposure_mode
    
    def set_distance_threshold(self, threshold: int) -> None:
        """Set minimum distance threshold for valid measurements"""
        self.distance_threshold = threshold
    
    def read_loop(self) -> None:
        """
        Main reading loop that runs in a separate thread
        Continuously reads data from the serial port when running
        """
        while True:
            with self.lock:
                if not self.running or not self.serial or not self.serial.is_open:
                    time.sleep(0.1)
                    continue
            
            try:
                if self.serial.in_waiting > 0:
                    try:
                        sensor_data = self.serial.readline().decode().rstrip().split(' ')
                        base_angle = int(sensor_data[0])
                        
                        for i in range(DISTANCES_PER_PACKET):
                            angle = base_angle + i
                            dist = int(sensor_data[2+i])
                            if dist > self.distance_threshold:
                                with self.lock:
                                    self.data.append(LidarPoint(dist, angle))
                    except Exception:
                        continue
            except Exception:
                # Serial port might have been closed
                with self.lock:
                    self.running = False
                time.sleep(0.1)
    
    @staticmethod
    def get_available_ports() -> List[str]:
        """Get list of available serial ports"""
        return [port.device for port in serial.tools.list_ports.comports()]