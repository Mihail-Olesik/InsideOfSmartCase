import serial
from module.module import Module


class SerialModule(Module):
    def __init__(self, nameOfModule = 'serialModule', port = None, baudrate = 9600):
        super().__init__(nameOfModule)
        self.port = port
        self.baudrate = baudrate

    def closeConnection(self):
        self.serial.close()

    def initializeConnection(self):
        try:
            self.serial = serial.Serial(self.port, self.baudrate, timeout=0.5)
            return True
        except serial.SerialException:
            return False

    def getData(self):
        return self.serial.readline()

    def sendData(self, data):
        self.serial.write(bytes(data, 'utf-8'))