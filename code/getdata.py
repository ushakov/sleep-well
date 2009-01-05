#!/usr/bin/python

import sys, pickle

def get_data(fname):
    f = open(fname, "r")
    alldata = []
    blocks = 0
    seq = None
    while True:
        b = f.read(512)
        b = [ ord(i) for i in b]
        if b[5] != 0 or b[6] != 0 or b[7] != 0:
#            print "bad block #%d" % blocks
            return alldata
        if seq == None:
            seq = b[0]
        elif seq != b[0]:
#            print "bad sequence in block #%d: %d, not %d" % (blocks+1, b[0], seq+1)
            return alldata
        seq += 1
        seq = seq & 0xff
        vals = []
        t = b[1] + 0x100 * b[2] + 0x10000 * b[3] + 0x1000000 * b[4]
        for off in range(8, 512, 2):
            vals += [b[off] + 0x100*b[off+1]]
        alldata += [{ 'timestamp' : t, 'data' : vals }]
        blocks += 1

if __name__ == "__main__":
    data = get_data(sys.argv[1])
    pickle.dump(data, open(sys.argv[2], "w"))
