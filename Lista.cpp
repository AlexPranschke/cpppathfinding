
#include "Lista.h"

Element::Element() :
	nastepny(NULL)
{}

Element::Element(Lista_t val) :
	wartosc(val),
	nastepny(NULL)
{}

bool Lista::empty()
{
	/*if(this->size == 0) {
		return true;
	}
	else {
		return false;
	}*/

	return (this->size == 0);
}

Lista::Lista() : size(0)
{}

void Lista::push_back(Lista_t wartosc)
{
	Element* el = &this->poczatek;
	while (el->nastepny != NULL)
	{
		el = el->nastepny;
	}
	el->nastepny = new Element(wartosc);
	this->size++;
}

void Lista::pop_front()
{
	if (this->size > 1) {
		Element* el = poczatek.nastepny;
		poczatek.nastepny = el->nastepny;
		delete el;
		this->size--;
	}
	else if (this->size == 1) {
		delete poczatek.nastepny;
		poczatek.nastepny = NULL;
		this->size--;
	}
	else {
		// nie ma nic do zrzucenia
	}
}

Lista_t Lista::front()
{
	if (this->empty() == false) {
		return poczatek.nastepny->wartosc;
	}
	else {
		throw(std::invalid_argument("Lista jest pusta"));
	}
}

void Lista::clear()
{
	while (this->empty() == false) {
		this->pop_front();
	}
}