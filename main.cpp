#include "Robot.h"
#include "Lista.h"

using namespace std;

const unsigned int rozmiarMapyY = 4, rozmiarMapyX = 2;

uint8_t seg[][ROZMIAR_SEGMENTU][ROZMIAR_SEGMENTU] = {
{
	{1, 1, 0, 1, 1},
	{1, 0, 0, 1, 1},
	{0, 0, 1, 0, 0},
	{1, 0, 0, 0, 1},
	{1, 1, 0, 1, 1}
},
{
	{1, 1, 0, 0, 1},
	{0, 1, 1, 0, 1},
	{0, 0, 0, 0, 0},
	{1, 0, 0, 0, 1},
	{1, 1, 0, 0, 1}
},
{
	{0, 0, 0, 0, 0},
	{0, 0, 1, 0, 0},
	{0, 0, 1, 1, 0},
	{1, 0, 1, 1, 1},
	{1, 0, 0, 0, 0}
},
{
	{1, 1, 0, 0, 0},
	{1, 0, 0, 1, 0},
	{0, 0, 1, 0, 0},
	{0, 1, 0, 0, 1},
	{1, 0, 0, 1, 1}
},
{
	{0, 0, 0, 1, 1},
	{1, 0, 0, 0, 1},
	{0, 0, 1, 0, 0},
	{0, 0, 1, 0, 0},
	{0, 0, 0, 0, 1}
},
{
	{1, 1, 0, 1, 1},
	{1, 1, 0, 1, 1},
	{0, 0, 0, 0, 0},
	{1, 1, 0, 1, 1},
	{1, 1, 0, 1, 1}
}
};

void uzupelnijMape(Mapa_t mapa, string opis[]) {
	for (int w = 0; w < rozmiarMapyY; w++) {
		for (int k = 0; k < rozmiarMapyX; k++) {
			RodzajSeg rodzaj = (RodzajSeg)(opis[w][k] - 'A');
			for (int i = 0; i < ROZMIAR_SEGMENTU; i++) {
				for (int j = 0; j < ROZMIAR_SEGMENTU; j++) {
					mapa[w * ROZMIAR_SEGMENTU + i][k * ROZMIAR_SEGMENTU + j] = seg[rodzaj][i][j];
				}
			}
		}
	}
}

Pkt losujPkt(Mapa_t mapa) {
	Pkt A;
	do {
		A.x = rand() % (rozmiarMapyX * ROZMIAR_SEGMENTU);
		A.y = rand() % (rozmiarMapyY * ROZMIAR_SEGMENTU);
	} while (mapa[A.y][A.x] == 1);
	return A;
}

void rysujMape(Mapa_t mapa) {
	for (int i = 0; i < rozmiarMapyY * ROZMIAR_SEGMENTU; i++) {
		for (int j = 0; j < rozmiarMapyX * ROZMIAR_SEGMENTU; j++) {
			switch (mapa[i][j]) {
			case SCIANA:
				std::cout << "#";
				break;
			case ODWIEDZONE:
			case WOLNE:
				std::cout << " ";
				break;
			case PKT_B:
				std::cout << "B";
				break;
			case PKT_A:
				std::cout << "A";
				break;
			case WYNIK:
				std::cout << ".";
				break;
			}
			//std::cout<<std::setw(4)<<(int)mapa[i][j];
		}
		std::cout << "\n";
	}
}

void drukujWynik(Mapa_t wynik) {
	for (int i = 0; i < rozmiarMapyY * ROZMIAR_SEGMENTU; i++) {
		for (int j = 0; j < rozmiarMapyX * ROZMIAR_SEGMENTU; j++) {
			std::cout << "	" << (int)wynik[i][j];
		}
		std::cout << "\n";
	}
}

void wczytajMape(std::string* mapaOpis) {
	for (int i = 0; i < rozmiarMapyY; i++) {
		cin >> mapaOpis[i];
		if (mapaOpis[i].size() != rozmiarMapyX) {
			exit(1);
		}
	}
}

void losujMape(std::string mapaOpis[]) {
	for (int i = 0; i < rozmiarMapyY; i++) {
		mapaOpis[i].resize(rozmiarMapyX);  // Zapewnia, ¿e string ma odpowiedni¹ d³ugoœæ
		for (int j = 0; j < rozmiarMapyX; j++) {
			mapaOpis[i][j] = 'A' + rand() % 6;  // Losowanie liter od 'A' do 'F'
		}
	}
}

StatusPola statusPola(Mapa_t mapa, Pkt pkt) {
	uint8_t pole = mapa[pkt.y][pkt.x];
	return (StatusPola)(pole);
}

uint8_t wynikPola(Mapa_t wyniki, Pkt pkt) {
	return wyniki[pkt.y][pkt.x];
}

void sprawdzPktNaMapie(Mapa_t mapa, Mapa_t wyniki, Lista& kolejka, Pkt biezacy, Pkt pktSprawdzany) {
	switch (statusPola(mapa, pktSprawdzany)) {
	case WOLNE:
		mapa[pktSprawdzany.y][pktSprawdzany.x] = ODWIEDZONE;
		wyniki[pktSprawdzany.y][pktSprawdzany.x] = wynikPola(wyniki, biezacy) + 1;
		kolejka.push_back(pktSprawdzany);
		break;
	case SCIANA:
		// nie przetwarzamy
		break;
	case ODWIEDZONE:
		// dojscie krotsza droga
		if (wynikPola(wyniki, pktSprawdzany) > wynikPola(wyniki, biezacy) + 1) {
			wyniki[pktSprawdzany.y][pktSprawdzany.x] = wynikPola(wyniki, biezacy) + 1;
			kolejka.push_back(pktSprawdzany);
		}
		break;
	}
}

void znajdzDroge(Pkt a, Pkt b, Mapa_t mapa) {
	Mapa_t wyniki = new uint8_t * [rozmiarMapyY * ROZMIAR_SEGMENTU];
	for (unsigned int i = 0; i < rozmiarMapyY * ROZMIAR_SEGMENTU; i++) {
		wyniki[i] = new uint8_t[rozmiarMapyX * ROZMIAR_SEGMENTU];
		for (unsigned int j = 0; j < rozmiarMapyX * ROZMIAR_SEGMENTU; j++) {
			wyniki[i][j] = 0;  // Wype³nienie zerem
		}
	}

	Lista kolejkaDoPrzetworzenia;
	kolejkaDoPrzetworzenia.push_back(a);

	while (kolejkaDoPrzetworzenia.empty() == false) {
		Pkt przetwarzany = kolejkaDoPrzetworzenia.front();
		if (przetwarzany.x == b.x && przetwarzany.y == b.y) { break; } // dotarliœmy do celu!
		mapa[przetwarzany.y][przetwarzany.x] = ODWIEDZONE;
		if (przetwarzany.x > 0) {
			Pkt nowy(przetwarzany.x - 1, przetwarzany.y);
			sprawdzPktNaMapie(mapa, wyniki, kolejkaDoPrzetworzenia, przetwarzany, nowy);
		}
		if (przetwarzany.y > 0) {
			Pkt nowy(przetwarzany.x, przetwarzany.y - 1);
			sprawdzPktNaMapie(mapa, wyniki, kolejkaDoPrzetworzenia, przetwarzany, nowy);
		}
		if (przetwarzany.x < rozmiarMapyX * ROZMIAR_SEGMENTU - 1) {
			Pkt nowy(przetwarzany.x + 1, przetwarzany.y);
			sprawdzPktNaMapie(mapa, wyniki, kolejkaDoPrzetworzenia, przetwarzany, nowy);
		}
		if (przetwarzany.y < rozmiarMapyY * ROZMIAR_SEGMENTU - 1) {
			Pkt nowy(przetwarzany.x, przetwarzany.y + 1);
			sprawdzPktNaMapie(mapa, wyniki, kolejkaDoPrzetworzenia, przetwarzany, nowy);
		}
		kolejkaDoPrzetworzenia.pop_front();
	}

	kolejkaDoPrzetworzenia.clear();
	kolejkaDoPrzetworzenia.push_back(b);

	while (kolejkaDoPrzetworzenia.empty() == false) {
		Pkt przetwarzany = kolejkaDoPrzetworzenia.front();
		if (przetwarzany.x == a.x && przetwarzany.y == a.y) { break; } // dotarliœmy do celu!
		mapa[przetwarzany.y][przetwarzany.x] = WYNIK;

		if (przetwarzany.x > 0) {
			Pkt nowy(przetwarzany.x - 1, przetwarzany.y);
			if (wynikPola(wyniki, nowy) + 1 == wynikPola(wyniki, przetwarzany)) {
				kolejkaDoPrzetworzenia.push_back(nowy);
				kolejkaDoPrzetworzenia.pop_front();
				continue;
			}
		}
		if (przetwarzany.y > 0) {
			Pkt nowy(przetwarzany.x, przetwarzany.y - 1);
			if (wynikPola(wyniki, nowy) + 1 == wynikPola(wyniki, przetwarzany)) {
				kolejkaDoPrzetworzenia.push_back(nowy);
				kolejkaDoPrzetworzenia.pop_front();
				continue;
			}
		}
		if (przetwarzany.x < rozmiarMapyX * ROZMIAR_SEGMENTU - 1) {
			Pkt nowy(przetwarzany.x + 1, przetwarzany.y);
			if (wynikPola(wyniki, nowy) + 1 == wynikPola(wyniki, przetwarzany)) {
				kolejkaDoPrzetworzenia.push_back(nowy);
				kolejkaDoPrzetworzenia.pop_front();
				continue;
			}
		}
		if (przetwarzany.y < rozmiarMapyY * ROZMIAR_SEGMENTU - 1) {
			Pkt nowy(przetwarzany.x, przetwarzany.y + 1);
			if (wynikPola(wyniki, nowy) + 1 == wynikPola(wyniki, przetwarzany)) {
				kolejkaDoPrzetworzenia.push_back(nowy);
				kolejkaDoPrzetworzenia.pop_front();
				continue;
			}
		}
		kolejkaDoPrzetworzenia.pop_front();
	}

	// to nie istotne
	wyniki[a.y][a.x] = PKT_A;
	wyniki[b.y][b.x] = PKT_B;

	mapa[a.y][a.x] = PKT_A;
	mapa[b.y][b.x] = PKT_B;

	rysujMape(mapa);

	// to nie potrzebne docelowo
	drukujWynik(wyniki);

	for (unsigned int i = 0; i < rozmiarMapyY * ROZMIAR_SEGMENTU; i++) {
		delete[] wyniki[i];
	}
	delete[] wyniki;
}

int main(int argc, char** argv)
{
	srand(time(NULL));
	std::string mapaOpis[rozmiarMapyY];
	losujMape(mapaOpis);

	// alokacja pamiêci dla mapy
	uint8_t** mapa = new uint8_t * [rozmiarMapyY * ROZMIAR_SEGMENTU];
	for (unsigned int i = 0; i < rozmiarMapyY * ROZMIAR_SEGMENTU; i++) {
		mapa[i] = new uint8_t[rozmiarMapyX * ROZMIAR_SEGMENTU];
	}
	
	// tworzymy tablice zer i 1 na podstawie segmentów
	uzupelnijMape(mapa, mapaOpis);

	Pkt A = losujPkt(mapa),
		B = losujPkt(mapa);

	std::cout << A.x << " " << A.y << "\n" << B.x << " " << B.y << "\n";
	znajdzDroge(A, B, mapa);

	for (unsigned int i = 0; i < rozmiarMapyY * ROZMIAR_SEGMENTU; i++) {
		delete[] mapa[i];
	}
	delete[] mapa;
	return 0;
}

