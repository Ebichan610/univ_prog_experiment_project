import cv2
import numpy as np

cap = cv2.VideoCapture('./Test.wmv')
while(cap.isOpened()):
    ret, frame = cap.read()
    bgrUpper = np.array([10, 10, 255])
    bgrLower = np.array([0, 0, 40])
    mv_threshold = cv2.inRange(frame, bgrLower, bgrUpper)
    cv2.imshow("frame", frame)
    cv2.imshow("RedExtract", mv_threshold)
    cv2.waitKey(25)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
