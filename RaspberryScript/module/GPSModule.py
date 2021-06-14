import serial
from module.serialModule import SerialModule
from gps import *


class GPSModule(SerialModule):
    def __init__(self, port = None, baudrate = 9600):
        super().__init__('gps', port, baudrate)
    
    def getData(self):
        data = self.serial.readline()
        message = data[0:6]
        if (message == "$GPRMC"):
            parts = data.split(",")
            if parts[2] == 'V':
                print("GPS receiver warning")
            else:
                longitude = self._formatDegreesMinutes(parts[5], 3)
                latitude = self._formatDegreesMinutes(parts[3], 2)
                print("Your position: lon = ") + str(longitude) + ", lat = " + str(latitude)
                return 'GPS lon: {} lat: {}'.format(longitude, latitude)
        else:
            pass

    def _formatDegreesMinutes(self, coordinates, digits):

        parts = coordinates.split(".")

        if (len(parts) != 2):
            return coordinates

        if (digits > 3 or digits < 2):
            return coordinates
        
        left = parts[0]
        right = parts[1]
        degrees = str(left[:digits])
        minutes = str(right[:3])

        return degrees + "." + minutes