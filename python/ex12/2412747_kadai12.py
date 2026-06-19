import cv2
import numpy as np
import os
import glob
import re

TRAIN_DIR = "./number/train"
TEST_DIR = "./number/test"
SAMPLE_DIR = "./number"

hog = cv2.HOGDescriptor((28, 28), (14, 14), (7, 7), (14, 14), 9)


def label_from_name(name):
    m = re.search(r'num(\d)', name)
    return int(m.group(1))


def deskew(img):
    m = cv2.moments(img)
    if abs(m['mu02']) < 1e-2:
        return img.copy()
    skew = m['mu11'] / m['mu02']
    M = np.float32([[1, skew, -0.5 * 28 * skew], [0, 1, 0]])
    return cv2.warpAffine(img, M, (28, 28), flags=cv2.WARP_INVERSE_MAP | cv2.INTER_LINEAR)


def preprocess(gray):
    gray = cv2.resize(gray, (28, 28))
    return deskew(gray)


def feature(gray):
    return hog.compute(preprocess(gray)).ravel()


def load_labeled(base):
    X, y = [], []
    for p in sorted(glob.glob(os.path.join(base, "*"))):
        label = label_from_name(os.path.basename(p))
        img = cv2.imread(p, cv2.IMREAD_GRAYSCALE)
        X.append(feature(img))
        y.append(label)
    return np.array(X, np.float32), np.array(y, np.int32)


train_x, train_y = load_labeled(TRAIN_DIR)
print("学習画像:", len(train_x), "枚")
svm = cv2.ml.SVM_create()
svm.setType(cv2.ml.SVM_C_SVC)
svm.setKernel(cv2.ml.SVM_RBF)
svm.setC(12.5)
svm.setGamma(0.50625)
svm.train(train_x, cv2.ml.ROW_SAMPLE, train_y)
sample_paths = glob.glob(os.path.join(SAMPLE_DIR, "*.png"))
for path in sample_paths:
    gray = cv2.imread(path, cv2.IMREAD_GRAYSCALE)
    x = feature(gray).reshape(1, -1)
    digit = int(svm.predict(x)[1][0, 0])
    print(os.path.basename(path), "→", digit)
    img = cv2.imread(path)
    disp = cv2.resize(img, (280, 280), interpolation=cv2.INTER_NEAREST)
    cv2.imshow("result", disp)
    key = cv2.waitKey(0)
    if key & 0xFF == ord('q'):
        break

cv2.destroyAllWindows()
