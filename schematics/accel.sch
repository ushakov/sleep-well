v 20080127 1
C 40000 40000 0 0 0 title-B.sym
C 46000 45600 1 0 0 attiny44.sym
{
T 47700 49300 5 10 1 1 0 6 1
refdes=U1
T 46300 49500 5 10 0 0 0 0 1
device=attiny44
T 46300 49700 5 10 0 0 0 0 1
footprint=SO14
}
C 44500 48300 1 0 0 crystal-1.sym
{
T 44700 48800 5 10 0 0 0 0 1
device=CRYSTAL
T 44700 48600 5 10 1 1 0 0 1
refdes=U2
T 44700 49000 5 10 0 0 0 0 1
symversion=0.1
T 44500 48300 5 10 0 0 0 0 1
footprint=CRYSTAL 300
}
C 44300 47000 1 0 0 gnd-1.sym
C 45200 47000 1 0 0 gnd-1.sym
C 44600 47500 1 90 0 capacitor-1.sym
{
T 43900 47700 5 10 0 0 90 0 1
device=CAPACITOR
T 44100 47700 5 10 1 1 90 0 1
refdes=C1
T 43700 47700 5 10 0 0 90 0 1
symversion=0.1
T 44600 47500 5 10 0 0 90 0 1
footprint=1206
}
C 41500 48400 1 0 0 pad_gnd.sym
{
T 42000 49231 5 10 1 1 0 0 1
refdes=P1
T 42400 48500 5 8 0 0 0 0 1
device=pad
T 41500 48400 5 10 0 0 0 0 1
footprint=CONNECTOR 1 1
}
C 41500 49400 1 0 0 pad_vdd.sym
{
T 42000 50231 5 10 1 1 0 0 1
refdes=P2
T 42400 49500 5 8 0 0 0 0 1
device=pad
T 41500 49400 5 10 0 0 0 0 1
footprint=CONNECTOR 1 1
}
C 45500 47500 1 90 0 capacitor-1.sym
{
T 44800 47700 5 10 0 0 90 0 1
device=CAPACITOR
T 45000 47700 5 10 1 1 90 0 1
refdes=C2
T 44600 47700 5 10 0 0 90 0 1
symversion=0.1
T 45500 47500 5 10 0 0 90 0 1
footprint=1206
}
N 44500 48400 44400 48400 4
N 44400 47500 44400 47300 4
N 45300 47300 45300 47500 4
N 45200 48400 46000 48400 4
N 44400 48400 44400 48800 4
N 44400 48800 46000 48800 4
C 45800 49200 1 90 0 resistor-2.sym
{
T 45450 49600 5 10 0 0 90 0 1
device=RESISTOR
T 45500 49400 5 10 1 1 90 0 1
refdes=R1
T 45800 49200 5 10 0 0 90 0 1
footprint=1206
}
C 49000 48700 1 0 0 resistor-2.sym
{
T 49400 49050 5 10 0 0 0 0 1
device=RESISTOR
T 49200 49000 5 10 1 1 0 0 1
refdes=R2
T 49000 48700 5 10 0 0 0 0 1
footprint=1206
}
N 45700 49200 45700 47600 4
N 45700 47600 46000 47600 4
N 45700 50100 45700 50300 4
C 50100 48600 1 0 0 led-1.sym
{
T 50900 49200 5 10 0 0 0 0 1
device=LED
T 50900 49000 5 10 1 1 0 0 1
refdes=LED1
T 50900 49400 5 10 0 0 0 0 1
symversion=0.1
T 50100 48600 5 10 0 0 0 0 1
footprint=1206
}
N 49900 48800 50100 48800 4
N 51000 48800 51400 48800 4
N 43000 49800 43000 50300 4
C 52200 47500 1 180 0 connector5-1.sym
{
T 50400 46000 5 10 0 0 180 0 1
device=CONNECTOR_5
T 52100 45800 5 10 1 1 180 0 1
refdes=CONN1
T 52200 47500 5 10 0 0 0 0 1
footprint=CONNECTOR 1 5
}
C 50100 45600 1 0 0 gnd-1.sym
N 50500 46100 50200 46100 4
N 50200 46100 50200 45900 4
N 48300 49600 46000 49600 4
N 46000 49600 46000 49100 4
N 46000 49100 45700 49100 4
C 45500 50300 1 0 0 vcc-1.sym
N 43000 49800 42600 49800 4
N 42600 48800 43000 48800 4
N 43000 48500 43000 48800 4
C 42800 50300 1 0 0 vcc-1.sym
C 42900 48200 1 0 0 gnd-1.sym
C 52300 43400 1 180 0 connector3-1.sym
{
T 50500 42500 5 10 0 0 180 0 1
device=CONNECTOR_3
T 52300 42300 5 10 1 1 180 0 1
refdes=CONN2
T 52300 43400 5 10 0 0 0 0 1
footprint=CONNECTOR 1 3
}
C 50300 42200 1 0 0 gnd-1.sym
C 50100 43400 1 0 0 vcc-1.sym
N 50600 43200 50300 43200 4
N 50300 43200 50300 43400 4
N 50600 42600 50400 42600 4
N 50400 42600 50400 42500 4
N 48300 46600 48300 49600 4
N 50500 46700 49600 46700 4
N 49600 47200 48000 47200 4
N 50500 47000 48600 47000 4
N 48600 47000 48600 46800 4
N 48600 46800 48000 46800 4
N 48300 46600 49500 46600 4
N 49500 46600 49500 46400 4
N 49500 46400 50500 46400 4
N 50500 47300 49000 47300 4
N 49000 46400 48000 46400 4
C 51200 49400 1 0 0 vcc-1.sym
N 51400 49400 51400 48800 4
N 48000 48800 49000 48800 4
C 52500 43500 1 0 0 lis3lv02.sym
{
T 54300 45700 5 10 1 1 0 6 1
refdes=U3
T 52900 45900 5 10 0 0 0 0 1
device=lis3lv02
T 52900 46100 5 10 0 0 0 0 1
footprint=LIS3LV02
}
N 48000 46000 48700 46000 4
N 48700 42900 48700 46000 4
N 48700 42900 50600 42900 4
N 52600 44000 49900 44000 4
N 49900 44000 49900 47000 4
N 52600 44400 49000 44400 4
N 49000 44400 49000 47300 4
N 52600 44800 49600 44800 4
N 49600 44800 49600 47200 4
N 52600 45200 52500 45200 4
N 52500 45200 52500 48400 4
N 52500 48400 48000 48400 4
C 45900 41000 1 0 0 microsd.sym
{
T 47000 44800 5 10 1 1 0 6 1
refdes=U?
T 46300 45000 5 10 0 0 0 0 1
device=microsd
T 46300 45200 5 10 0 0 0 0 1
footprint=MICROSD
}
