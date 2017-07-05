import random
import time

# parameters that subject to change
frame_sizex = 640
frame_sizey = 480
while True:
    x, y = random.randint(0, frame_sizex), random.randint(0, frame_sizey)
    timestamp, uid = random.getrandbits(64), random.getrandbits(64)
    print str(timestamp) + ',' + str(x) + ',' + str(y) + ',' + str(uid)
    time.sleep(1)