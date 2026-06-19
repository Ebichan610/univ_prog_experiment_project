import cv2
import numpy as np
import os
import glob
import re

TRAIN_DIR = "./number/train"
TEST_DIR = "./number/test"
SAMPLE_DIR = "./number"


def label_from_name(name):
    m = re.search(r'num(\d)', name)
    return int(m.group(1)) if m else None


def preprocess(gray):
    if gray.mean() > 127:
        gray = 255 - gray
    gray = cv2.resize(gray, (28, 28))
    return gray.reshape(28 * 28).astype(np.float32)


def load_labeled(base):
    X, y = [], []
    for p in sorted(glob.glob(os.path.join(base, "*"))):
        label = label_from_name(os.path.basename(p))
        if label is None:
            continue
        img = cv2.imread(p, cv2.IMREAD_GRAYSCALE)
        if img is None:
            continue
        X.append(preprocess(img))
        y.append(label)
    return np.array(X, np.float32), np.array(y, np.int32)


train_x, train_y = load_labeled(TRAIN_DIR)
print("学習画像:", len(train_x), "枚")

knn = cv2.ml.KNearest_create()
knn.train(train_x, cv2.ml.ROW_SAMPLE, train_y)

test_x, test_y = load_labeled(TEST_DIR)
if len(test_x) > 0:
    if len(test_x) > 500:
        idx = np.random.default_rng(0).choice(len(test_x), 500, replace=False)
        test_x, test_y = test_x[idx], test_y[idx]
    _, pred, _, _ = knn.findNearest(test_x, k=5)
    acc = (pred.ravel().astype(int) == test_y).mean()
    print("test 正答率: {:.1f}% ({}枚で確認)".format(100 * acc, len(test_x)))

exts = ("*.png", "*.jpg", "*.jpeg", "*.bmp", "*.pgm")
sample_paths = []
for e in exts:
    sample_paths += glob.glob(os.path.join(SAMPLE_DIR, e))
sample_paths = sorted(sample_paths)
print("判定対象:", len(sample_paths), "枚")

for path in sample_paths:
    gray = cv2.imread(path, cv2.IMREAD_GRAYSCALE)
    if gray is None:
        continue
    x = preprocess(gray).reshape(1, -1)
    _, result, _, _ = knn.findNearest(x, k=5)
    digit = int(result[0, 0])
    print(os.path.basename(path), "→", digit)

    img = cv2.imread(path)
    disp = cv2.resize(img, (280, 280), interpolation=cv2.INTER_NEAREST)
    cv2.putText(disp, str(digit), (10, 45), cv2.FONT_HERSHEY_SIMPLEX,
                1.8, (0, 0, 255), 3)
    cv2.imshow("result", disp)
    key = cv2.waitKey(0)
    if key & 0xFF == ord('q'):
        break

cv2.destroyAllWindows()
