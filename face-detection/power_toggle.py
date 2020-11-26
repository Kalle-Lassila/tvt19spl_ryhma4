import RPi.GPIO as GPIO
import time

def pwr_btn():
    #set pinmode for pin 7
    GPIO.setmode(GPIO.BOARD)
    GPIO.setup(7, GPIO.OUT)

    #toggle pin on for a short peroid
    GPIO.output(7,True)
    print("power btn")
    time.sleep(.50)
    GPIO.output(7,False)
    print("power btn")
    GPIO.cleanup()
