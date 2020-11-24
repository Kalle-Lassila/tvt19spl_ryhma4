import face_recognition
import cv2
import numpy as np

#video capture object from default camera
video_capture = cv2.VideoCapture(0)

locations = []
encoding = []

while(True):
    #capture a frame/image
    ret, frame = video_capture.read()

    #simpler image for easier processing
    image = cv2.resize(frame, (0, 0),fx = 0.25, fy = 0.25)

    #convert into proper color space(not quite sure how this works but it does)
    rgb_image = image[:, :, ::-1]
    #try to find a face from the image
    locations = face_recognition.face_locations(rgb_image)
    encodings = face_recognition.face_encodings(rgb_image, locations)

    #check for one or more faces
    if locations != []:
        print(locations) #just print where the faces are for testing
        locations = []
    #display captured image
    cv2.imshow('ikkuna', frame)


    #exit
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

video_capture.release()
cv2.destroyAllWindows()
