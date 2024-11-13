#pragma once



#ifndef LISTA_H
#define LISTA_H

#include <stdlib.h>
#include "Robot.h"

typedef Pkt Lista_t;
typedef struct Element Element;

struct Element {
	Element* nastepny;
	Lista_t wartosc;

	Element();
	Element(Lista_t);
};

struct Lista {
public:
	Lista();

	void push_back(Lista_t);
	Lista_t front();
	bool empty();
	void pop_front();
	void clear();
private:
	Element poczatek;
	unsigned int size;
};


#endif /* LISTA_H */

