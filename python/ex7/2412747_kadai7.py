import cv2
import numpy as np

def frame_sub(img1, img2, img3):
    diff1 = cv2.absdiff(img1, img2)
    diff2 = cv2.absdiff(img2, img3)
    diff = cv2.bitwise_and(diff1, diff2)
    ret, mv_th = cv2.threshold(diff, 10, 255, cv2.THRESH_BINARY)
    
    return  mv_th

cap = cv2.VideoCapture('./Test2(320x240).wmv')
frame1 = cv2.cvtColor(cap.read()[1], cv2.COLOR_RGB2GRAY)
frame2 = cv2.cvtColor(cap.read()[1], cv2.COLOR_RGB2GRAY)
frame3 = cv2.cvtColor(cap.read()[1], cv2.COLOR_RGB2GRAY)
while(cap.isOpened()):
    mv_th = frame_sub(frame1, frame2, frame3)
    cv2.imshow("frame", frame2)
    cv2.imshow("Mask", mv_th)
    frame1 = frame2
    frame2 = frame3
    frame3 = cv2.cvtColor(cap.read()[1], cv2.COLOR_RGB2GRAY)
    cv2.waitKey(25)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
