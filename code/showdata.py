#!/usr/bin/python

import sys, pickle
import math, pygame

w = 1000
h = 500
screen = None

def Ginit():
    pygame.init()
    global screen
    screen = pygame.display.set_mode((w,h))

def show_data(data):
    Ginit()
    all = [ s for b in data for s in b['data'] ]
    N = len(all)
    for x in range(w):
        low = int(1.0 * x * N / w)
        high = int(1.0 * (x+1) * N / w)
        inrange = [ (all[i] * h / 2048) for i in range(low, high)]
        pygame.draw.line(screen, (0,255,0), (x,min(inrange)), (x, max(inrange)), 1)
    pygame.draw.line(screen, (0,0,255), (0,h/2), (w, h/2), 1)
    pygame.display.flip()

if __name__ == "__main__":
    data = pickle.load(open(sys.argv[1]))
    print len(data)
    show_data(data)
    while True:
        pass
