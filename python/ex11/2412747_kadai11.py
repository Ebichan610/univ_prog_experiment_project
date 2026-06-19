import cv2
import numpy as np
import os
import glob
import re

# マクロの方がわかりやすい
TRAIN_DIR = "./number/train"
TEST_DIR = "./number/test"
SAMPLE_DIR = "./number"


def label_from_name(name):
    m = re.search(r'num(\d)', name)
    return int(m.group(1))


def preprocess(gray):
    gray = cv2.resize(gray, (28, 28))
    return gray.reshape(28 * 28).astype(np.float32)


def load_labeled(base):
    x, y = [], []
    for p in sorted(glob.glob(os.path.join(base, "*"))):
        label = label_from_name(os.path.basename(p))
        img = cv2.imread(p, cv2.IMREAD_GRAYSCALE)
        x.append(preprocess(img))
        y.append(label)
    return np.array(x, np.float32), np.array(y, np.int32)


train_x, train_y = load_labeled(TRAIN_DIR)
print("学習画像:", len(train_x), "枚")

knn = cv2.ml.KNearest_create()
knn.train(train_x, cv2.ml.ROW_SAMPLE, train_y)

sample_paths = glob.glob(os.path.join(SAMPLE_DIR, "*.png"))
for path in sample_paths:
    gray = cv2.imread(path, cv2.IMREAD_GRAYSCALE)
    x = gray.reshape(1, -1).astype(np.float32)
    _, result, _, _ = knn.findNearest(x, k=5)
    digit = int(result[0, 0])
    print(os.path.basename(path), "→", digit)

    img = cv2.imread(path)
    # 表示が小さかったので
    disp = cv2.resize(img, (280, 280), interpolation=cv2.INTER_NEAREST)
    cv2.imshow("result", disp)
    key = cv2.waitKey(0)
    if key & 0xFF == ord('q'):
        break

cv2.destroyAllWindows()
