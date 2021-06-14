import serial, sys, time
import wiringpi

wiringpi.wiringPiSetup()
serial = wiringpi.serialOpen('/dev/ttyUSB0',9600)

while True:
    string = input("Input key: ")
    wiringpi.serialPuts(serial, string)

#ser.write(sys.argv[1].encode('utf-8'))
