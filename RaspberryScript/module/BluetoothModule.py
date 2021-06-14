from sys import modules
import bluetooth
from module.module import Module

uuid = "94f39d29-7d6d-437d-973b-fba39e49d4ee"


class BluetoothModule(Module):
    def __init__(self, nameOfModule="Bluetooth"):
        super().__init__(nameOfModule=nameOfModule)
        self.isWorking = False

    def initializeConnection(self):
        self.server_sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
        self.server_sock.bind(("", bluetooth.PORT_ANY))
        self.server_sock.listen(1)

        self.port = self.server_sock.getsockname()[1]

        bluetooth.advertise_service(self.server_sock, "SmartCaseServer", service_id=uuid,
                                    service_classes=[
                                        uuid, bluetooth.SERIAL_PORT_CLASS],
                                    profiles=[bluetooth.SERIAL_PORT_PROFILE]
                                    )

        print("Waiting for connection")
        self.client_sock, self.address = self.server_sock.accept()
        # self.server_sock2 = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
        # self.server_sock2.connect(self.address)
        self.isWorking = True
        print("Accepted connection from " + str(self.address))
        return True
        
    def getData(self):
        try:
            data = self.client_sock.recv(1024)
            return data
        except OSError:
            self.isWorking = False
            self.initializeConnection()

    def sendData(self, data: str):
        # print(data)
        self.client_sock.send(data)

    def closeConnection(self):
        self.isWorking = False
        self.client_sock.close()
        self.server_sock.close()
        # self.server_sock2.close()
