import face_recognition
import cv2
import numpy as np
import power_toggle
import time



locations = []
encoding = []
camera_on = False
sleeptime = (20)

while True:
    #open camera
    if not camera_on:
        video_capture = cv2.VideoCapture(0)
        camera_on = True
    #capture a frame/image
    ret, frame = video_capture.read()

    #simpler image for easier processing
    image = cv2.resize(frame, (0, 0),fx = 0.25, fy = 0.25)

    #convert into proper color space(not quite sure how this works but it does)
    rgb_image = image[:, :, ::-1]
    #try to find a face from the image
    locations = face_recognition.face_locations(rgb_image)
    #encodings not needed
    #encodings = face_recognition.face_encodings(rgb_image, locations)
    #check if one or more faces was found
    if locations != []:
        print(locations) #just print where the faces are for testing
        locations = []
        #call for power button action
        power_toggle.pwr_btn()
        #turn off the camera
        if camera_on:
            video_capture.release()
            camera_on = False
        print("sleep seconds", sleeptime)
        time.sleep(sleeptime)
        #To turn the displaly off after a delay
        power_toggle.pwr_btn()

    #display captured image
    #cv2.imshow('ikkuna', frame)

    #exit
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
#these are here just to be safe
video_capture.release()
cv2.destroyAllWindows()
