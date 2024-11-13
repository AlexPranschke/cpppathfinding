
#include <cstdlib>
#include <vector>
#include <iostream>
#include <inttypes.h>
#include <list>
#include <stdio.h>
#include <iomanip>

#ifndef ROBOT_H
#define ROBOT_H

#define ROZMIAR_SEGMENTU 5

typedef uint8_t** Mapa_t;

enum StatusPola {
	WOLNE = 0,
	SCIANA = 1,
	ODWIEDZONE = 2,
	WYNIK = 253,
	PKT_A = 254,
	PKT_B = 255
};


enum RodzajSeg {
	A = 0,
	B,
	C,
	D,
	E,
	F
};

struct Pkt {
	unsigned int x, y;
	// konstruktor struktury
	Pkt() {
		x = y = 0;
	}
	Pkt(unsigned int a, unsigned int b) {
		x = a;
		y = b;
	}
};


#endif /* ROBOT_H */

