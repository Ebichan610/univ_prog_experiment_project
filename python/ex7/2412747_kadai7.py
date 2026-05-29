import cv2
import numpy as np

def frame_sub(img1, img2, img3):
    diff1 = cv2.absdiff(img1, img2)
    diff2 = cv2.absdiff(img2, img3)
    diff = cv2.bitwise_and(diff1, diff2)
    _, mv_th = cv2.threshold(diff, 10, 255, cv2.THRESH_BINARY)
    mv_th = cv2.dilate(mv_th, None, iterations=2)
    
    return  mv_th


cap = cv2.VideoCapture('./Test2(320x240).wmv')
frame1 = cv2.cvtColor(cap.read()[1], cv2.COLOR_RGB2GRAY)
frame2 = cv2.cvtColor(cap.read()[1], cv2.COLOR_RGB2GRAY)
frame3 = cv2.cvtColor(cap.read()[1], cv2.COLOR_RGB2GRAY)
prev_x, prev_y = None, None
while(cap.isOpened()):
    mv_th = frame_sub(frame1, frame2, frame3)
    mu = cv2.moments(mv_th, True)
    if mu["m00"] > 3500:
        x,y = int(mu["m10"]/mu["m00"]), int(mu["m01"]/mu["m00"])
        if prev_x is not None:
            dx = x - prev_x
            dy = y - prev_y
            if np.abs(dx) > np.abs(dy):
                if dx > 0:
                    print("右に動きました")
                else:
                    print("左に動きました")
            else:
                if dy > 0:
                    print("下に動きました")
                else:
                    print("上に動きました")
        prev_x, prev_y = x, y
    else:
        prev_x, prev_y = None, None
    _, frame = cap.read()
    cv2.imshow("frame", frame)
    frame1 = frame2
    frame2 = frame3
    frame3 = cv2.cvtColor(cap.read()[1], cv2.COLOR_RGB2GRAY)
    cv2.waitKey(25)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
