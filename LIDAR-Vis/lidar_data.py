"""
Data structures for LIDAR visualization
"""

from dataclasses import dataclass

@dataclass
class LidarPoint:
    """
    Represents a single LIDAR data point
    """
    distance: float
    angle: float  # in degrees
    
    @property
    def angle_rad(self):
        """Convert angle to radians"""
        import math
        return math.radians(self.angle)