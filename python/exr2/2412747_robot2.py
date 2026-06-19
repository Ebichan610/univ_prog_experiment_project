import cv2
import numpy as np
import serial
import time
from collections import deque

PORT = '/dev/ttyACM0'
ser = serial.Serial(PORT, 38400)
time.sleep(1)

AREA_MIN = 3000
EXT_TH = 1.8
DEPTH_RATIO = 0.5

CMD = {"パー": "f", "グー": "b", "指1本": "r", "チョキ": "l"}


def skin_mask(frame):
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    mask = cv2.inRange(hsv, np.array([0, 20, 60]), np.array([25, 150, 255]))
    mask = cv2.medianBlur(mask, 5)
    mask = cv2.morphologyEx(mask, cv2.MORPH_CLOSE, np.ones((3, 3), np.uint8))
    return mask


def analyze_hand(cnt, shape):
    hand = np.zeros(shape, np.uint8)
    cv2.drawContours(hand, [cnt], -1, 255, -1)
    dist = cv2.distanceTransform(hand, cv2.DIST_L2, 5)
    _, palm_r, _, palm_c = cv2.minMaxLoc(dist)
    px, py = palm_c

    pts = cnt.reshape(-1, 2)
    upper = pts[pts[:, 1] < py]
    reach = np.sqrt(((upper - [px, py]) ** 2).sum(axis=1)).max() \
        if len(upper) > 0 else palm_r
    ext = reach / (palm_r + 1e-6)

    valleys = 0
    hull = cv2.convexHull(cnt, returnPoints=False)
    if hull is not None and len(hull) > 3:
        try:
            defects = cv2.convexityDefects(cnt, hull)
        except cv2.error:
            defects = None
        if defects is not None:
            for i in range(defects.shape[0]):
                s, e, f, dd = defects[i, 0]
                start = cnt[s][0]
                end = cnt[e][0]
                far = cnt[f][0]
                a = np.linalg.norm(end - start)
                b = np.linalg.norm(far - start)
                c = np.linalg.norm(far - end)
                angle = np.arccos((b ** 2 + c ** 2 - a ** 2) / (2 * b * c + 1e-6))
                if dd / 256.0 > palm_r * DEPTH_RATIO and angle < np.pi * 5 / 9 and far[1] < py:
                    valleys += 1
    return ext, valleys


def classify(cnt, shape):
    if cv2.contourArea(cnt) < AREA_MIN:
        return None
    ext, valleys = analyze_hand(cnt, shape)
    if ext < EXT_TH:
        return "グー"
    if valleys == 0:
        return "指1本"
    if valleys == 1:
        return "チョキ"
    return "パー"


cap = cv2.VideoCapture(0)
history = deque(maxlen=7)
last = None

while cap.isOpened():
    ret, frame = cap.read()
    if not ret:
        break

    mask = skin_mask(frame)
    contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    gesture = None
    if len(contours) > 0:
        cnt = max(contours, key=cv2.contourArea)
        gesture = classify(cnt, mask.shape)
        if gesture is not None:
            cv2.drawContours(frame, [cnt], -1, (0, 255, 0), 2)

    history.append(gesture)
    valid = [g for g in history if g is not None]
    if len(valid) == 0:
        stable = None
    else:
        stable = max(set(valid), key=valid.count)

    cmd = CMD.get(stable, "s")
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
