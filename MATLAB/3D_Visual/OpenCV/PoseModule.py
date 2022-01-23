#source: https://www.youtube.com/watch?v=brwgBf6VB0I&list=PLjNZVyGQ-hJcHQqr95fWKAOWVrV2eFiIJ&index=4&t=2202s

import cv2
from cv2 import waitKey
import mediapipe as mp
import time

class poseDetector():



    def __init__(self, static_image_mode=False,
               model_complexity=1,
               smooth_landmarks=True,
               enable_segmentation=False,
               smooth_segmentation=True,
               min_detection_confidence=0.5,
               min_tracking_confidence=0.5):

        self.mode = static_image_mode
        self.modComp = model_complexity
        self.smoothLm = smooth_landmarks
        self.enSeg = enable_segmentation
        self.smoSeg = smooth_segmentation
        self.detectionCon = min_detection_confidence
        self.trackCon = min_tracking_confidence

        self.mpDraw = mp.solutions.drawing_utils
        self.mpPose = mp.solutions.pose
        self.pose = self.mpPose.Pose(self.mode, self.modComp, self.smoothLm, self.enSeg,self.smoSeg, self.detectionCon, self.trackCon)

    # def __init__(self, mode = False, upBody = False, smooth = True, detectionCon=0.5, trackCon=0.5):
    #     self.mode = mode
    #     self.upBody = upBody
    #     self.smooth = smooth
    #     self.detectionCon = detectionCon
    #     self.trackCon = trackCon

    #     self.mpDraw = mp.solutions.drawing_utils
    #     self.mpPose = mp.solutions.pose
    #     self.pose = self.mpPose.Pose(self.mode, self.upBody, self.smooth, self.detectionCon,self.trackCon)
        
    def findPose(self, img, draw=True):
        imgRGB = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
        self.results = self.pose.process(imgRGB)
        if self.results.pose_landmarks:
            if draw:
                self.mpDraw.draw_landmarks(img, self.results.pose_landmarks, self.mpPose.POSE_CONNECTIONS)    
        return img

    def findPosition(self, img, draw = True):
        lmList = []
        if self.results.pose_landmarks:
            for id, lm in enumerate(self.results.pose_landmarks.landmark):
                h,w,c = img.shape
                #print(id, lm)
                cx, cy = int(lm.x*w), int(lm.y*h)

                lmList.append([id, cx, cy])
                if draw:
                    cv2.circle(img, (cx, cy), 5, (255,0,0), cv2.FILLED)
        return lmList


def main():
    cap = cv2.VideoCapture('PoseVideos/2.mp4')
    pTime = 0

    detector = poseDetector()

    while True:
        succes, img = cap.read()
        img = detector.findPose(img)
        lmList = detector.findPosition(img)
        print(lmList)


        cTime = time.time()
        fps = 1/(cTime-pTime)
        pTime = cTime

        cv2.putText(img, str (int(fps)), (70,50), cv2.FONT_HERSHEY_COMPLEX, 3, (255,0,0),3)
    
        cv2.imshow("Image", img)
        cv2.waitKey(1)

if __name__ == "__main__":
    main()