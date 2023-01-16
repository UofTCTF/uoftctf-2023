import cv2
import os

vidcap = cv2.VideoCapture('source.mp4')
success, image = vidcap.read()
count=0
while True:
        success, image = vidcap.read()
        if not success:
            break
        cv2.imwrite(os.path.join('output/', "{:d}.png".format(count)), image)
        count += 1