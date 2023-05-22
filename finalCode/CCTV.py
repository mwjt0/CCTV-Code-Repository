#! /usr/bin/python

import socket
import datetime as dt
from picamera import PiCamera
from time import sleep

# declare target IP and port
UDP_IP = "10.42.0.1"
UDP_PORT = 55056

# declare target path and file name
path = '/home/piIoT/IoT_CCTV/Videos/CCTV_'
fileEnding = '.h264'

# declare time to record in seconds
recTime = 10

# declare default door state
doorOpen = False

# open and connect socket
udpSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
udpSocket.bind((UDP_IP, UDP_PORT))

# definition of cam-method
def cam():
    # setting up camera
    camera = PiCamera()
    camera.resolution = (640, 480)

    # setting the final file path
    time = dt.datetime.now().strftime('%Y-%m-%d_%H.%M')
    finalPath = path + time + fileEnding

    # recording for specified time
    camera.start_recording(finalPath)
    camera.wait_recording(recTime)
    camera.stop_recording()
    camera.close()

while True:
    # receive data
    data, addr = udpSocket.recvfrom(1024)
    print (data)

    # check for opened door
    if doorOpen == False:
        if data == b'Door Opened':
            print("calling subprocess")
            cam()
            doorOpen = True
    # check for closed door
    if doorOpen == True:
        if data == b'Door Closed':
            doorOpen = False
