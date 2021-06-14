from module.module import Module
from modules import Modules
from config import *
from pyzbar import pyzbar
import cv2, time, os
import imutils, wiringpi

from config import *

width = 480
height = 360
blocking_opportunity = False

def get_coords_from_qr_code(frame, qr_text):
    barcodes = pyzbar.decode(frame)

    for barcode in barcodes:
        # extract the bounding box location of the barcode and draw
        # the bounding box surrounding the barcode on the image
        (x, y, w, h) = barcode.rect
        print('X: ', x, ' Y: ', y, 'W: ', w, 'H: ', h)
        cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 0, 255), 2)

        barcodeData = barcode.data.decode("utf-8")
        if barcodeData == qr_text:
            print(barcodeData)
            return dict(zip(tuple(['x', 'y', 'w', 'h']), barcode.rect))
    return False

# serial = initializationOfSerialConnection()
# if not serial:
#     print("Arduino is not connected")
#     exit(1)

# vs = initializationOfWebCamConnection()
# if not vs:
#     print("WebCam is not connected")
#     exit(1)

# time.sleep(2.0)
# code = getDataFromQrCode()
# if not code:
#     print('QrCode is incorrect')
#     exit(1)

try:
    res = Modules.initializeModules()

    if res != None:
        print("Error in module {}".format(res))
        exit(1)
        
except:
    print("Some error in modules")
    Modules.closeConnections()
    exit(1)

counter_of_failed_frames = 0
isMoving = False


try:
    while True:
        Modules.getModule('bt').sendData('dir: l')
        frame = vs.read()
        frame = imutils.resize(frame, width=width)

        data = Modules.getData()
        print(data)

        if(data['bt'] == 'Lock: lock'):
            isMoving = False
        elif(data['bt'] == 'Lock: unlock'):
            isMoving = True
        
        if isMoving:
            coords_of_qr = get_coords_from_qr_code(frame, code)
            if coords_of_qr == False:
                counter_of_failed_frames += 1
            else:
                counter_of_failed_frames = 0

                if coords_of_qr['w'] > width // 4:
                    print("I need stop: ", coords_of_qr['w'])
                    wiringpi.serialPuts(serial, 's')

                    Modules.getModule('bt').sendData('dir: s')

                elif coords_of_qr['w'] < width // 4:
                    print("I need go: ", coords_of_qr['w'])
                    wiringpi.serialPuts(serial, 'g')

                    Modules.getModule('bt').sendData('dir: g')
                
                center = (coords_of_qr['x'] + coords_of_qr['w'] // 2)
                
                if  center < width // 2 - coords_of_qr['w']:
                    print("I need left: ", center)
                    wiringpi.serialPuts(serial, 'l')

                    Modules.getModule('bt').sendData('dir: l')
                elif center > width // 2 + coords_of_qr['w']:
                    print("I need right: ", center)
                    wiringpi.serialPuts(serial, 'r')

                    Modules.getModule('bt').sendData('dir: r')

            cv2.imshow("Barcode Scanner", frame)
        key = cv2.waitKey(1)

        if counter_of_failed_frames == 100 and blocking_opportunity:
            print("Module is blocked. Please RFID or unlock with mobile.")
            isMoving = False

            Modules.getModule('bt').sendData('Lock: lock')

        # if the `q` key was pressed, break from the loop
        if key == ord('q'):
            break
except KeyboardInterrupt:
    Modules.closeConnections()
    print("[INFO] cleaning up...")
    wiringpi.serialPuts(serial, 's')
    wiringpi.serialPuts(serial, '2')
    cv2.destroyAllWindows()
    vs.stop()
