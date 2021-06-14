from module.module import Module
import mfrc522

class RFIDModule(Module):
    def __init__(self):
        super().__init__('rfid')

    def initializeConnection(self):
        self.MIFAREReader = mfrc522.SimpleMFRC522()
        return True

    def getData(self):
        id, text = self.MIFAREReader.read()
        return "RFID id: {} text: {}".format(id, text)