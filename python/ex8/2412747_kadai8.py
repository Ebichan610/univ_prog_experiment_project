import cv2
import numpy as np

cap = cv2.VideoCapture('./ObjectMovement.mp4')
while(cap.isOpened()):
    ret, frame = cap.read()
    
    
cap.release()
cv2.destroyAllWindows()
