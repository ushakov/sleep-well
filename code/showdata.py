#!/usr/bin/python

import sys, pickle
import math, pygame
from pygame.locals import *

def Ginit(w, h):
    pygame.init()
    pygame.font.init()
    return pygame.display.set_mode((w,h))

class Data:
    def __init__(self, data):
        self.d = []
        self.times = []
        n = 0
        for d in data:
            self.times += [(n, d['timestamp'])]
            self.d += d['data']
            n += len(d['data'])

    def data(self, k):
        return self.d[k]

    def datalen(self):
        return len(self.d)

    def times(self, k):
        return self.times[k]

    def itertimes(self):
        return self.times.__iter__()

    def timeslen(self):
        return len(self.times)

class DataViewer:
    def __init__(self, screen, data, timeshift):
        self.w = screen.get_width()
        self.h = screen.get_height()
        self.screen = screen
        self.timeshift = timeshift
        self.x0 = 0
        self.font = pygame.font.Font(None, 12)
        self.d = Data(data)
        self.scale = self.d.datalen() * 1.0 / self.w
        self.max_scale = self.d.datalen() * 1.0 / self.w

    def set_scale(self, scale, x):
        if scale < .125:
            scale = .125
        if scale > self.max_scale:
            scale = self.max_scale
        if scale != self.scale:
            sh = x * (self.scale - scale)
            self.scale = scale
            self.set_x0(self.x0 + sh)

    def zoom_in(self, x):
        self.set_scale(self.scale / 2.0, x)

    def zoom_out(self, x):
        self.set_scale(self.scale * 2, x)

    def set_x0(self, x0):
        if x0 < 0:
            x0 = 0
        if x0 + self.w * self.scale > self.d.datalen():
            x0 = self.d.datalen() - (self.scale * self.w)
        self.x0 = x0

    def shift_view(self, dx):
        self.set_x0(self.x0 + dx * self.scale)
    
    def update(self):
        self.screen.fill((0,0,0))
        for x in range(self.w):
            low = int(x * self.scale + self.x0)
            high = int((x + 1) * self.scale + self.x0)
            if high <= low: high = low+1
            inrange = [ (self.d.data(i) * self.h / 2048) for i in range(low, high)]
            pygame.draw.line(self.screen, (0,255,0), (x,min(inrange)), (x, max(inrange)), 1)
        min_x = -1
        for t in self.d.itertimes():
            x = (t[0] - self.x0) / self.scale
            if x >= 0 and x <= self.w:
                pygame.draw.line(self.screen, (128,128,128), (x, self.h), (x, self.h-10), 1)
                pygame.draw.line(self.screen, (64,64,64), (x, self.h-10), (x, 0), 1)
                if x >= min_x:
                    rt = t[1] + self.timeshift
                    hours = (rt / 3600) % 24
                    minutes = (rt % 3600) / 60
                    seconds = rt % 60
                    tstr = "%02d:%02d:%02d" % (hours, minutes, seconds)
                    tsurf = self.font.render(tstr, 1, (255,255,255))
                    trect = tsurf.get_rect()
                    trect.bottomleft = (x, self.h - 10)
                    min_x = trect.right + 24
                    self.screen.blit(tsurf, trect)

        pygame.draw.line(screen, (0,0,255), (0,self.h/2), (self.w, self.h/2), 1)
        pygame.display.flip()
        
def handle_event(ev):
    if ev.type == QUIT:
        sys.exit()
    if ev.type == KEYDOWN:
        if ev.unicode == 'q':
            sys.exit()
    if ev.type == MOUSEBUTTONDOWN:
        if ev.button == 4:
            # wheel forward
            dv.zoom_in(ev.pos[0])
        if ev.button == 5:
            # wheel back
            dv.zoom_out(ev.pos[0])
        # print "button down:", ev.pos, ev.button
    if ev.type == MOUSEMOTION:
        if ev.buttons[0] != 0:
            dv.shift_view(-ev.rel[0])
        # print "motion:", ev.pos, ev.rel, ev.buttons

if __name__ == "__main__":
    data = pickle.load(open(sys.argv[1]))
    print len(data)
    screen = Ginit(1000, 500)
    dv = DataViewer(screen, data, 0)
    dragging = False
    prev = None

    dv.update()
    clock = pygame.time.Clock()
    while True:
        clock.tick(60)
        for ev in pygame.event.get():
            handle_event(ev)
        dv.update()

