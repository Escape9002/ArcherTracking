import cv2

img = cv2.imread('ChessPatterns/chess1.jpg', 0 )
img = cv2.resize(img, (500, 500))

cv2.imshow('Image', img)
cv2.waitKey(0)
cv2.destroyAllWindows()