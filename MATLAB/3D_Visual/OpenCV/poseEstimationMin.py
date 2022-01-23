#source: https://www.youtube.com/watch?v=brwgBf6VB0I&list=PLjNZVyGQ-hJcHQqr95fWKAOWVrV2eFiIJ&index=4&t=2202s


import cv2
from cv2 import waitKey
import mediapipe as mp
import time

mpDraw = mp.solutions.drawing_utils
mpPose = mp.solutions.pose
pose = mpPose.Pose()


cap = cv2.VideoCapture('PoseVideos/2.mp4')

pTime = 0

while True:
    succes, img = cap.read()
    imgRGB = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    results = pose.process(imgRGB)
    #print(results.pose_landmarks)

    if results.pose_landmarks:
        mpDraw.draw_landmarks(img, results.pose_landmarks, mpPose.POSE_CONNECTIONS)
        for id, lm in enumerate(results.pose_landmarks.landmark):
            h,w,c = img.shape
            print(id, lm)
            cx, cy = int(lm.x*w), int(lm.y*h)
            cv2.circle(img, (cx, cy), 5, (255,0,0), cv2.FILLED)
    
    cTime = time.time()
    fps = 1/(cTime-pTime)
    pTime = cTime

    cv2.putText(img, str (int(fps)), (70,50), cv2.FONT_HERSHEY_COMPLEX, 3, (255,0,0),3)
    
    cv2.imshow("Image", img)
    cv2.waitKey(1)