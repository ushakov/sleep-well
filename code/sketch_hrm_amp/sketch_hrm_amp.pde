/**
 * Simple Read
 * 
 * Read data from the serial port and change the color of a rectangle
 * when a switch connected to a Wiring or Arduino board is pressed and released.
 * This example works with the Wiring / Arduino program that follows below.
 */


import processing.serial.*;

Serial myPort;  // Create object from Serial class

void setup() 
{
  size(1000, 600);
  // I know that the first port in the serial list on my mac
  // is always my  FTDI adaptor, so I open Serial.list()[0].
  // On Windows machines, this generally opens COM1.
  // Open whatever port is the one you're using.
  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 38400);
  stroke(255);
  x = 0;
  frameRate(100);
  PFont font = loadFont("font.vlw");
  textFont(font,14);
  frame();
}

boolean need_sync = true;

int readNum() {
  if (need_sync) {
    int k;
    while (myPort.available() > 0) {
      k = myPort.read();
      if (k == 10) break;
    }
  }
  if (myPort.available() < 5) {
    return -5000;
  }
  int t = 0;
  for (int i = 0; i < 3; i++) {
    t *= 16;
    int k = myPort.read();
    if (k >= '0' && k <= '9') {
      t += k - '0';
    } else {
      t += k - 'a';
    }
  }
  
//  if  ((t & 0x20) != 0) {
//    t = ~t & 0x3f;
//    t +=1;
//    t = -t;
//  } 
  
  if (myPort.read() != 13) need_sync = true;
  if (myPort.read() != 10) need_sync = true;
  return t;
}

float m = 0.5;
int Y(float val) {
  return int(height - val * m);
}

int x;
float last;
void outputSample(float t) {
  stroke(255);
  line(x, Y(last), x+1, Y(t));
  last = t;
  x = x + 1;
  if (x + 1 == width) {
    x = 0;
    frame();
  }
}

void frame() {
  background(0);
  stroke(255);
  line(0, Y(0), width, Y(0)); text("0", 0, Y(0));
  line(0, Y(100), width, Y(100)); text("100", 0, Y(100));
  line(0, Y(500), width, Y(500)); text("500", 0, Y(500));
  line(0, Y(1000), width, Y(1000)); text("1000", 0, Y(1000));
}

float cap = 0;
float cap2 = 0;
int c = 0;
float acc = 0;
float acc2 = 0;

int ss = -1;
int sx = 0;
float mx = 0;

int s = 0;

int NBEATS = 20;
int[] beats = new int[NBEATS];
int nbeats;

void draw()
{
  int samples = 0;
  while (true) {
    int n = readNum();
    if (n == -5000) { // no data available
      break;
    }
    samples ++;
    s++;
    if (s % 10 == 0) {
      line(x, height, x, height - 10);
    }
    if (s % 50 == 0) {
      line(x, height, x, height - 20);
    }
    if (s % 100 == 0) {
      line(x, height, x, height - 30);
    }
    
//    float out = n + cap;
//    cap -= 0.001 * out;
    cap = 0.99 * cap + 0.01*n;
    cap2 = 0.999 * cap2 + 0.001 * n;
    acc += n;
    acc2 += cap2;
    c ++;
    if (c == 1) {
      // beat detection
  
      float thr = 30;
      if (cap > cap2 + thr) {
        stroke(#808080);
        line(x, Y(cap2), x, Y(cap2 + thr));
        if (ss == -1) {
          ss = s;
          sx = x;
          mx = 0;
        }
        if (cap - cap2 > mx) {
          mx = cap - cap2;
        }
      } else {
        if (ss != -1) {
          int len = s - ss;
          if (len > 100 && len < 400) {
            stroke(#ff8080); 
            fill (#ff8080);
            if (x < sx) {
              sx = 0;
            }
            rect (sx, height, x - sx, - 20);
            stroke(255);
            text(nf(len, 2,0), sx, Y(cap2 + thr/2));
            
            // record beat
            if (nbeats < NBEATS) {
              beats[nbeats] = s;
              nbeats++;
            } else {
              for (int t = 0; t < NBEATS-1; t++) {
                beats[t] = beats[t+1];
              }
              beats[NBEATS-1] = s;
            }
            // calculate rates
            if (nbeats > 2) {
              float mindelta = -1;
              float maxdelta = -1;
              float sum = 0;
              for (int t = 0; t < nbeats-1; t++) {
                float delta = 750.0 * 60 / (beats[t+1] - beats[t]);
                if (mindelta == -1 || mindelta > delta) {
                  mindelta = delta;
                }
                if (maxdelta == -1 || maxdelta < delta) {
                  maxdelta = delta;
                }
                sum += delta;
              }
              float avg = sum / nbeats;
              if (mindelta * 2 < maxdelta) {
                stroke(#ff8080); 
                nbeats = 0;
              } else {
                stroke(255);
              }
              text(nf(avg, 3,2), sx, Y(cap2 + thr/2)+15);
              text(nf(mindelta, 3,2), sx, Y(cap2 + thr/2)+30);
              text(nf(maxdelta, 3,2), sx, Y(cap2 + thr/2)+45);
            }
          }
          ss = -1;
        }
      }
      outputSample(acc/c);
      stroke(#80ff80);
      line(x, Y(acc2/c), x+1, Y(acc2/c));
      acc = 0;
      acc2 = 0;
      c = 0;
    }
  }
    
  float time = millis()/1000.0;
  int k = int(time / 100 - 0.5);
  time -= k * 100;
  text(nf(time, 3, 2), x, 10);
  text(nf(s*1.0/time, 4, 2), x, 20);
}

