import cv2
import numpy as np
from collections import deque


def skin_mask(frame):
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    mask = cv2.inRange(hsv, np.array([0, 20, 60]), np.array([25, 150, 255]))
    mask = cv2.medianBlur(mask, 5)
    kernel = np.ones((3, 3), np.uint8)
    mask = cv2.morphologyEx(mask, cv2.MORPH_CLOSE, kernel)
    return mask


cap = cv2.VideoCapture('./janken2.mp4')
history = deque(maxlen=11)
while cap.isOpened():
    ret, frame = cap.read()
    if not ret:
        break
    mask = skin_mask(frame)
    contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
    result = None
    if len(contours) > 0:
        cnt = max(contours, key=cv2.contourArea)

        if cv2.contourArea(cnt) > 3000:
            hand = np.zeros(mask.shape, np.uint8)
            cv2.drawContours(hand, [cnt], -1, 255, -1)
            dist = cv2.distanceTransform(hand, cv2.DIST_L2, 5)
            _, palm_r, _, palm_c = cv2.minMaxLoc(dist)
            px, py = palm_c[0], palm_c[1]
            pts = cnt.reshape(-1, 2)
            upper = pts[pts[:, 1] < py]
            reach = np.sqrt(((upper - [px, py]) ** 2).sum(axis=1)).max() \
                if len(upper) > 0 else palm_r
            ext = reach / (palm_r + 1e-6)
            hull = cv2.convexHull(cnt, returnPoints=False)
            defects = None
            if hull is not None and len(hull) > 3:
                try:
                    defects = cv2.convexityDefects(cnt, hull)
                except cv2.error:
                    defects = None
            valleys = 0
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
                    depth = dd / 256.0
                    if depth > palm_r * 0.5 and angle < np.pi * 5 / 9 and far[1] < py:
                        valleys += 1
            if ext < 1.8:
                result = "グー"
            elif valleys >= 2:
                result = "パー"
            else:
                result = "チョキ"
    if result is not None:
        history.append(result)
    display = max(set(history), key=history.count)
    print(display)
    cv2.imshow("frame", frame)
    if cv2.waitKey(25) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
