#ifndef MANCHESTER_H
#define MANCHESTER_H

void manchester_init();
void manchester_send(int t);
int manchester_read();
int manchester_wait_bit();

#endif /* MANCHESTER_H */
