import os, wiringpi
from imutils.video import VideoStream
from module.BluetoothModule import BluetoothModule
from module.GPSModule import GPSModule
from module.RFIDModule import RFIDModule

dictOfModules = {
#    'gps' : GPSModule(),
#    'rfid' : RFIDModule(),
   'bt' : BluetoothModule(),
}

def initializationOfSerialConnection():
    ttys = os.listdir('/dev')
    counter = 0
    for tty in ttys:
        counter += 1
        if tty.find('ttyUSB') != -1:
            wiringpi.wiringPiSetup()
            print('Arduino is found: /dev/' + tty)
            serial = wiringpi.serialOpen('/dev/' + tty, 9600)
            return serial


def initializationOfWebCamConnection(source = 0):
    counter = 0
    while True:
        counter += 1
        vs = VideoStream(src=source).start()
        frame = vs.read()
        if frame is not None:
            break
        source += 1
        print("There's no sources at video{}".format(source))
        if source >= 14:
            source = 0

        if counter >= 30:
            return None

    return vs

def getDataFromQrCode():
    try:
        f = open('./qr_text.txt', 'r')
        code = f.readline()
        return code
    except FileNotFoundError:
        return None