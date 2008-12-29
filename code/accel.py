#!/usr/bin/python

import serial, math, pygame

w = 1000
h = 500
screen = None

def Ginit():
    pygame.init()
    global screen
    screen = pygame.display.set_mode((w,h))


x = 0
last = [0,0,0,0]
colors = [(64,64,180),(64,180,64),(180,64,64),(255,255,255)]

def AaddOnePoint(sv, i):
    global last
    yv = int(h/2 - sv / 4096.0 * h)
    if x == 0:
        pygame.draw.circle(screen, colors[i], (0,yv), 2)
    else:
        pygame.draw.line(screen, colors[i], (x-1,last[i]), (x, yv), 2)
    last[i] = yv
    

def AaddPoints(t):
    global x
    if x == 0:
        screen.fill((0,0,0))
        pygame.draw.line(screen, (127,127,127), (0,h/2), (w,h/2))
    sq = 0
    for i in range(3):
        sv = t[i]
        sq += sv*sv
        AaddOnePoint(sv, i)
    sq = math.sqrt(sq/3)
    AaddOnePoint(sq, 3)
    pygame.display.flip()
    x = x + 1
    if x == w:
        x = 0
px = w/2
py = h/2
vx = 0
vy = 0

def reduceBy(x, eps):
    if x > eps:
        return x - eps
    if x < -eps:
        return x + eps
    return 0

def addPoints(t):
    p = [ v / 2048.0 for v in t ]
#    norm = math.sqrt(sum([v*v for v in p]))
#    p = [ v / norm for v in p ]
    fx = -p[0]
    fy = p[1]
    global px, py, vx, vy
    vx = reduceBy(vx + 5*fx, 0.1)
    vy = reduceBy(vy + 5*fy, 0.1)
    px += vx
    py += vy
    if px < 0:
        vx = -vx
        px = -px
    if px > w:
        vx = -vx
        px = 2*w - px
    if py < 0:
        vy = -vy
        py = -py
    if py > h:
        vy = -vy
        py = 2*h - py
        
    screen.fill((0,0,0))
    pygame.draw.circle(screen, (200,255,200), (px, py), 10)
    pygame.display.flip()
    

Ginit()
ser = serial.Serial(0)
ser.setBaudrate(38400)
while True:
    line = ser.readline()
    line = line.strip()
    if line == "bad device":
        pygame.draw.line(screen, (255,0,0), (x, 0), (x, w))
        continue
    f = line.split(" ")
    try:
        t = [ int(q, 16) for q in f]
    except ValueError:
        continue
    if len(t) != 3:
        continue
    r = []
    for q in t:
        if q >= 0x80000000:
            r.append(q - 0x100000000)
        else:
            r.append(q)
    AaddPoints(r)
