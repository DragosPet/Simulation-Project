#include<iostream>
#include<stdlib.h>
#include<fstream>
using namespace std;
int stoc, CSt = 40, CL = 1000, Ca = 3000, Csup = 30, Cpen = 250;// variabile globale pentru a utiliza mai usor costurile suportate
float* generare(int n) {
	float*vector;
	vector = new float[n];
	for (int i = 0; i < n; i++)
		vector[i] = (rand() % 100)*0.01;

	return vector;
}//functie pentru a genera un sir de numere pseudo aleatoare pe intervalul [0-1]

float generare_unica() {//functie pentru a genera o singura valoare aleatoare
	return (rand() % 100)*0.01;
}

float medie(float*vect) {
	float sum = 0, med = 0;;
	if (vect != NULL)
		for (int i = 0; i < 30; i++)
			sum += vect[i];
	med = sum / 30;
	return med;
}

int verificare_cerere(float n) {
	int cerere;
	if (n < 0.3)
		cerere = 5;
	else if (n >= 0.3&&n < 0.35)
		cerere = 10;
	else if (n >= 0.35&&n < 0.8)
		cerere = 30;
	else
		cerere = 40;
	return cerere;
}

class Tabel {
public:
	int st[30];
	int aprov[30];
	int cer[30];
	bool com[30];
	int cant[30];
	float cs[30];
	float cp[30];
	float cpl[30];
	float csup[30];
	float ct[30];
	Tabel() {
		for (int i = 0; i < 30; i++) {
			this->st[i] = 0;
			this->aprov[i] = 0;
			this->cer[i] = 0;
			this->com[i] = false;
			this->cant[i] = 0;
			this->cs[i] = 0;
			this->cp[i] = 0;
			this->cpl[i] = 0;
			this->csup[i] = 0;
			this->ct[i] = 0;
		}
	}
	float calculmedie() {
		float sum = 0;
		for (int i = 0; i < 30; i++)
			sum += this->ct[i];
		return sum / 30;
	}
};

void afisare(Tabel t) {
	for (int i = 0; i < 30; i++)
		cout << i << "," << t.st[i] << "," << t.aprov[i] << "," << t.cer[i] << "," << t.com[i] << "," << t.cant[i] << "," << t.cs[i] << "," << t.cp[i] << "," << t.cpl[i] << "," << t.csup[i] << "," << t.ct[i] << endl;
}
void scrie_fisier(ofstream &o, Tabel t) {
	o << "Nrcurent;Stoc;Cantitate_aprovizionata;Cerere;comanda_part(0/1);cantitate_partener;Costuri_stocare;Costuri_penalizare;CosturiL+ach;Costuri_suplimentare;COST(ZI)" << endl;
	for (int i = 0; i < 30; i++)
		o << i + 1 << ";" << t.st[i] << ";" << t.aprov[i] << ";" << t.cer[i] << ";" << t.com[i] << ";" << t.cant[i] << ";" << t.cs[i] << ";" << t.cp[i] << ";" << t.cpl[i] << ";" << t.csup[i] << ";" << t.ct[i] << endl;
}

float niteratii(int n) {
	float**vector, *medii;
	stoc = 40;
	medii = new float[n];
	for (int i = 0; i < n; i++)
		medii[i] = 0;
	vector = new float*[n];
	for (int i = 0; i < n; i++)
		vector[i] = generare(30);
	Tabel *tab;
	tab = new Tabel[n];
	for (int j = 0; j < n; j++)
	{
		stoc = 40;
		for (int i = 0; i < 30; i++)
		{
			if (i != 0 && i % 5 == 0) {
				tab[j].ct[i] += (CL + (20 * Ca));
				tab[j].cpl[i] += (CL + (20 * Ca));
				tab[j].aprov[i] += 20;
			}
			if (i != 0 && i != 2 && i % 5 == 2)
				stoc += 20;
			tab[j].st[i] = stoc;
			tab[j].cer[i] = verificare_cerere(vector[j][i]);

			if (tab[j].st[i] > tab[j].cer[i]) {
				tab[j].aprov[i] = 0;
				tab[j].ct[i] = CSt*(tab[j].st[i] - tab[j].cer[i]);
				tab[j].cs[i] = CSt*(tab[j].st[i] - tab[j].cer[i]);
				stoc = tab[j].st[i] - tab[j].cer[i];
				tab[j].csup[i] = 0;
				tab[j].cp[i] = 0;
			}
			else if (tab[j].st[i] == tab[j].cer[i]) {
				tab[j].cs[i] += 0;
				stoc = 0;
			}
			else {
				float pcs = generare_unica();
				if (pcs < 0.4)
					tab[j].com[i] = true;
				else
					tab[j].com[i] = false;

				if (tab[j].com[i] == true) {
					tab[j].csup[i] = Csup*(tab[j].cer[i] - tab[j].st[i]);
					tab[j].ct[i] += Csup*(tab[j].cer[i] - tab[j].st[i]);
					tab[j].cant[i] = tab[j].cer[i] - tab[j].st[i];
					stoc = 0;

				}
				else {
					tab[j].cp[i] += Cpen*(tab[j].cer[i] - tab[j].st[i]);
					tab[j].ct[i] += Cpen*(tab[j].cer[i] - tab[j].st[i]);
					stoc = 0;

				}
			}

		}

	}

	for (int i = 0; i < n; i++) {
		medii[i] = tab[i].calculmedie();
	}
	ofstream f, g;
	g.open("Desfasurare1.csv");
	f.open("Rez1.csv");
	if (f.is_open()) {
		f << "valorile medii:;" << endl;
		for (int i = 0; i < n; i++)
			f << medii[i] << endl;
	}
	f.close();

	if (g.is_open()) {
		for (int i = 0; i < n; i++)
			scrie_fisier(g, tab[i]);
		g << endl;
	}
	g.close();

	float suma = 0;
	for (int i = 0; i < n; i++)
		suma += medii[i];
	delete[]medii;
	return suma / n;


}

float niteratii_var2(int n) {
	float**vector, *medii;
	stoc = 40;
	medii = new float[n];
	for (int i = 0; i < n; i++)
		medii[i] = 0;
	vector = new float*[n];
	for (int i = 0; i < n; i++)
		vector[i] = generare(30);
	Tabel *tab = new Tabel[n];
	int k[30];
	for (int i = 0; i < 30; i++)
		k[i] = 0;
	for (int j = 0; j < n; j++)
	{
		stoc = 40;
		for (int i = 0; i < 30; i++)
		{

			if (stoc < 30) {
				tab[j].ct[i] += (CL + (20 * Ca));
				tab[j].cpl[i] += (CL + (20 * Ca));
				tab[j].aprov[i] = 20;
				k[i] = i;
			}
			for (int l = 0; l < 30; l++)
				if (k[l] != 0 && i == (k[l] + 2) && i != 0)
				{
					stoc += 20;
					k[l] = 0;
				}
			if (stoc < 30) {
				tab[j].ct[i] += (CL + (20 * Ca));
				tab[j].cpl[i] += (CL + (20 * Ca));
				tab[j].aprov[i] = 20;
				k[i] = i;
			}
			tab[j].st[i] = stoc;
			tab[j].cer[i] = verificare_cerere(vector[j][i]);

			if (tab[j].st[i] > tab[j].cer[i]) {
				tab[j].aprov[i] = 0;
				tab[j].ct[i] = CSt*(tab[j].st[i] - tab[j].cer[i]);
				tab[j].cs[i] = CSt*(tab[j].st[i] - tab[j].cer[i]);
				stoc = tab[j].st[i] - tab[j].cer[i];
				tab[j].csup[i] = 0;
				tab[j].cp[i] = 0;
				tab[j].cpl[i] = 0;
			}
			else if (tab[j].st[i] == tab[j].cer[i]) {
				tab[j].cs[i] += 0;
				stoc = 0;
			}
			else if (tab[j].st[i] < tab[j].cer[i]) {
				float pcs = generare_unica();
				if (pcs < 0.4)
					tab[j].com[i] = true;
				else
					tab[j].com[i] = false;

				if (tab[j].com[i] == true) {
					tab[j].csup[i] = Csup*(tab[j].cer[i] - tab[j].st[i]);
					tab[j].ct[i] += Csup*(tab[j].cer[i] - tab[j].st[i]);
					tab[j].cant[i] = tab[j].cer[i] - tab[j].st[i];
					stoc = 0;
				}
				else {
					tab[j].cp[i] += Cpen*(tab[j].cer[i] - tab[j].st[i]);
					tab[j].ct[i] += Cpen*(tab[j].cer[i] - tab[j].st[i]);
					stoc = 0;
				}
			}
		}
	}
	for (int i = 0; i < n; i++) {
		medii[i] = tab[i].calculmedie();
	}

	float suma = 0;
	for (int i = 0; i < n; i++)
		suma += medii[i];
	ofstream file, gen;
	file.open("Desfasurare2.csv", ios::out);
	if (file.is_open()) {
		for (int i = 0; i < n; i++)
			scrie_fisier(file, tab[i]);
		file << endl;
	}
	file.close();
	gen.open("Rez2.csv");
	if (gen.is_open()) {
		gen << "valorile medii:;" << endl;
		for (int i = 0; i < n; i++)
			gen << medii[i] << endl;
	}
	gen.close();

	delete[]medii;
	delete[]tab;
	return suma / n;
}

void main() {
	Tabel t;
	int n;
	cout << endl << "Care este numarul de simulari dorite pentru problema enuntata: ";
	cin >> n;
	float medie, medie2;
	medie = niteratii(n);
	medie2 = niteratii_var2(n);
	cout << endl << "medie: " << medie;
	cout << endl << "medie prin a 2-a strategie: " << medie2;
}
