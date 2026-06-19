import cv2
import numpy as np
import serial
import time

PORT = '/dev/ttyACM0'
ser = serial.Serial(PORT, 38400)
time.sleep(1)

AREA_MIN = 1000
AREA_FAR = 8000
AREA_NEAR = 30000
SIDE = 0.15


def red_mask(frame):
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    m1 = cv2.inRange(hsv, np.array([0, 80, 50]), np.array([10, 255, 255]))
    m2 = cv2.inRange(hsv, np.array([170, 80, 50]), np.array([180, 255, 255]))
    mask = cv2.bitwise_or(m1, m2)
    mask = cv2.medianBlur(mask, 5)
    mask = cv2.dilate(mask, None, iterations=2)
    return mask


def decide(area, cx, w):
    if area <= AREA_MIN:
        return 's'
    if cx < w * 0.5 - w * SIDE:
        return 'l'
    if cx > w * 0.5 + w * SIDE:
        return 'r'
    if area < AREA_FAR:
        return 'f'
    if area > AREA_NEAR:
        return 'b'
    return 's'


cap = cv2.VideoCapture(0)
last = None
while cap.isOpened():
    ret, frame = cap.read()
    if not ret:
        break
    h, w = frame.shape[:2]
    mask = red_mask(frame)
    mu = cv2.moments(mask, True)
    area = mu["m00"]
    cx = int(mu["m10"] / area) if area > 0 else -1
    cmd = decide(area, cx, w)
    if cmd != last:
        ser.write(bytes(cmd, 'utf-8'))
        last = cmd
    cv2.imshow("frame", frame)
    if cv2.waitKey(30) & 0xFF == ord('q'):
        break

ser.write(bytes('s', 'utf-8'))
ser.write(bytes('a', 'utf-8'))
ser.close()
cap.release()
cv2.destroyAllWindows()
