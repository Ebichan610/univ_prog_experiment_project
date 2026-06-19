import cv2
import numpy as np


def red_mask(frame):
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    mask1 = cv2.inRange(hsv, np.array([0, 80, 50]),   np.array([10, 255, 255]))
    mask2 = cv2.inRange(hsv, np.array([170, 80, 50]), np.array([180, 255, 255]))
    mask = cv2.bitwise_or(mask1, mask2)
    mask = cv2.dilate(mask, None, iterations=2)
    return mask


def direction(dx, dy, th):
    if np.abs(dx) < th and np.abs(dy) < th:
        return "静止"
    if np.abs(dx) > np.abs(dy):
        return "右" if dx > 0 else "左"
    else:
        return "下" if dy > 0 else "上"


cap = cv2.VideoCapture('./ObjectMovement.mp4')
frame_prev = cv2.cvtColor(cap.read()[1], cv2.COLOR_BGR2GRAY)
prev_x, prev_y = None, None
while cap.isOpened():
    ret, frame = cap.read()
    if not ret:
        break
    frame_now = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    flow = cv2.calcOpticalFlowFarneback(frame_prev, frame_now, None, 0.5, 3, 50, 3, 5, 1.2, 0)
    cam_x = np.median(flow[..., 0])
    cam_y = np.median(flow[..., 1])
    cam_speed = np.sqrt(cam_x ** 2 + cam_y ** 2)
    camera_moving = cam_speed > 0.1

    mask = red_mask(frame)
    mu = cv2.moments(mask, True)
    object_moving = False
    obj_dx, obj_dy = 0.0, 0.0
    if mu["m00"] > 500:
        x = int(mu["m10"] / mu["m00"])
        y = int(mu["m01"] / mu["m00"])
        if prev_x is not None:
            obj_dx = x - prev_x
            obj_dy = y - prev_y
            if np.sqrt(obj_dx ** 2 + obj_dy ** 2) > 3.0:
                object_moving = True
        prev_x, prev_y = x, y
    else:
        prev_x, prev_y = None, None

    if camera_moving and object_moving:
        scene = "カメラも物体も動いている"
    elif camera_moving:
        scene = "カメラが動いていて、物体は動いていない"
    elif object_moving:
        scene = "カメラは動いておらず、物体が動いている"
    else:
        scene = "カメラも物体も動いていない"

    cam_dir = direction(-cam_x, -cam_y, 0.1)
    obj_dir = direction(obj_dx, obj_dy, 3.0)
    print(scene + " （カメラ:" + cam_dir + " / 物体:" + obj_dir + "）")

    cv2.imshow("frame", frame)
    frame_prev = frame_now
    if cv2.waitKey(25) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
