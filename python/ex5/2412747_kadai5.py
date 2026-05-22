import cv2
import numpy as np

cap = cv2.VideoCapture('./Test.wmv')
while(cap.isOpened()):
    ret, frame = cap.read()
    gray = cv2.cvtColor(frame, cv2.COLOR_RGB2GRAY)
    sobelx64 = cv2.Sobel(gray, cv2.CV_64F, 1, 0, ksize=3)
    sobely64 = cv2.Sobel(gray, cv2.CV_64F, 0, 1, ksize=3)
    laplacian64 = cv2.Laplacian(gray, cv2.CV_64F)
    abs_sobelx64 = np.absolute(sobelx64)
    abs_sobely64 = np.absolute(sobely64)
    abs_laplacian64 = np.absolute(laplacian64)
    sobelx_8u = np.uint8(abs_sobelx64)
    sobely_8u = np.uint8(abs_sobely64)
    laplacian_8u = np.uint8(abs_laplacian64)    
    cv2.imshow('frame', frame)
    cv2.imshow('Sobel_x', sobelx_8u)
    cv2.imshow('Sobel_y', sobely_8u)
    cv2.imshow('Laplacian', laplacian_8u)
    cv2.waitKey(25)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()