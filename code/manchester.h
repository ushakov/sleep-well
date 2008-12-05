#ifndef MANCHESTER_H
#define MANCHESTER_H

void manchester_init();
void manchester_send(int t);
int manchester_read();

// low level, for debug
int manchester_wait_bit();
void manchester_send_bit(int b);

#endif /* MANCHESTER_H */
