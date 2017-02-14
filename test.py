import time
import serial

ser = serial.Serial(
    port='/dev/cu.usbserial-FTVPDIG5',
    baudrate=1200)

ser.isOpen()

text = "\x02\x0aADCO 049801282083 D\x0d\x0aOPTARIF BASE 0\x0d\x0aISOUSC 30 9\x0d\x0aBASE 040302197 %\x0d\x0aPTEC TH.. $\x0d\x0aIINST 003 Z\x0d\x0aIMAX 023 D\x0d\x0aPAPP 03310 (\x0d\x0aMOTDETAT 000000 B\x0d\x03"

print text

while 1 :
  ser.write(text)

  time.sleep(5)
