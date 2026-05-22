import cv2

img_color = cv2.imread("./lena.bmp", cv2.IMREAD_COLOR)
img_gray = cv2.imread("./lena.bmp", cv2.IMREAD_GRAYSCALE)
cv2.imshow("Original image", img_color)
cv2.imshow("Gray image", img_gray)
print("幅: ", img_color.shape[1])
print("高さ: ", img_color.shape[0])
print("チャンネル数: ", img_color.shape[2])
print("画素数: ", (int)(img_color.size/img_color.shape[2]))
cv2.waitKey()
