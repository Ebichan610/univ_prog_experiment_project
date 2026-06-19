import cv2
import numpy as np


def skin_mask(frame):
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    mask = cv2.inRange(hsv, np.array([0, 20, 60]), np.array([25, 150, 255]))
    mask = cv2.medianBlur(mask, 5)
    kernel = np.ones((3, 3), np.uint8)
    mask = cv2.morphologyEx(mask, cv2.MORPH_CLOSE, kernel)
    return mask


cap = cv2.VideoCapture('./Janken1.mp4')

while cap.isOpened():
    ret, frame = cap.read()
    if not ret:
        break

    mask = skin_mask(frame)

    contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL,
                                   cv2.CHAIN_APPROX_SIMPLE)
    result = ""
    if len(contours) > 0:
        cnt = max(contours, key=cv2.contourArea)

        if cv2.contourArea(cnt) > 3000:
            m = cv2.moments(cnt)
            cy = m["m01"] / m["m00"]
            hull = cv2.convexHull(cnt, returnPoints=False)
            valleys = 0
            if hull is not None and len(hull) > 3:
                defects = cv2.convexityDefects(cnt, hull)
                if defects is not None:
                    for i in range(defects.shape[0]):
                        s, e, f, d = defects[i, 0]
                        start = cnt[s][0]
                        end = cnt[e][0]
                        far = cnt[f][0]
                        a = np.linalg.norm(end - start)
                        b = np.linalg.norm(far - start)
                        c = np.linalg.norm(far - end)
                        angle = np.arccos((b ** 2 + c ** 2 - a ** 2) / (2 * b * c + 1e-6))
                        if d > 3000 and angle < np.pi * 2 / 3 and far[1] < cy:
                            valleys += 1
                            # cv2.circle(frame, tuple(far), 8, (0, 0, 255), -1)

            if valleys == 0:
                result = "グー"
            elif valleys == 1:
                result = "チョキ"
            else:
                result = "パー"

            # cv2.drawContours(frame, [cnt], -1, (0, 255, 0), 2)
            # cv2.line(frame, (0, int(cy)), (frame.shape[1], int(cy)), (255, 0, 0), 1)

    print(result)
    # label = {"グー": "Gu", "チョキ": "Choki", "パー": "Paa"}.get(result, "?")
    # cv2.putText(frame, label, (20, 50), cv2.FONT_HERSHEY_SIMPLEX, 1.5, (0, 0, 255), 3)
    cv2.imshow("frame", frame)
    if cv2.waitKey(25) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
