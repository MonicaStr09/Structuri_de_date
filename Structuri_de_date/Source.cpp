#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int an;
	int luna;
	int zi;
} data_calendaristica;

typedef struct {
	char* cod;
	char* nume_solicitant;
	char* nume_doctor;
	int ora;
	data_calendaristica data_calendaristica;
	int varsta;
	int test_covid;
} Programare;

void afisare_programare_doctor(Programare p) {
	printf("Cod Programare %s === Nume solicitant %s === Nume doctor %s === Varsta %d === Test COVID(0/1): %d === Data: %d.%d.%d - %d:00\n",
		p.cod,
		p.nume_solicitant,
		p.nume_doctor,
		p.varsta,
		p.test_covid,
		p.data_calendaristica.zi,
		p.data_calendaristica.luna,
		p.data_calendaristica.an,
		p.ora);
}

void dezalocare_programare(Programare* programare) {
	free(programare->cod);
	free(programare->nume_solicitant);
	free(programare->nume_doctor);
}

Programare citire_fisier_programare(FILE* fisier) {
	Programare p;

	char buffer[500];

	fgets(buffer, 500, fisier);
	fgets(buffer, 500, fisier);
	buffer[strlen(buffer) - 1] = 0;
	p.cod = (char*)malloc(sizeof(char) * strlen(buffer) + 1);
	strcpy(p.cod, buffer);

	fgets(buffer, 500, fisier);
	buffer[strlen(buffer) - 1] = 0;
	p.nume_solicitant = (char*)malloc(sizeof(char) * strlen(buffer) + 1);
	strcpy(p.nume_solicitant, buffer);

	fgets(buffer, 500, fisier);
	buffer[strlen(buffer) - 1] = 0;
	p.nume_doctor = (char*)malloc(sizeof(char) * strlen(buffer) + 1);
	strcpy(p.nume_doctor, buffer);

	fscanf(fisier, "%d", &p.ora);

	fscanf(fisier, "%d %d %d", &p.data_calendaristica.zi, &p.data_calendaristica.luna, &p.data_calendaristica.an);

	fscanf(fisier, "%d", &p.varsta);
	fscanf(fisier, "%d", &p.test_covid);

	return p;
}


void copie_programare(Programare* original, Programare* copie) {
	copie->cod = (char*)malloc(strlen(original->cod) + 1);
	strcpy(copie->cod, original->cod);
	copie->nume_solicitant = (char*)malloc(strlen(original->nume_solicitant) + 1);
	strcpy(copie->nume_solicitant, original->nume_solicitant);
	copie->nume_doctor = (char*)malloc(strlen(original->nume_doctor) + 1);
	strcpy(copie->nume_doctor, original->nume_doctor);
	copie->ora = original->ora;
	copie->varsta = original->varsta;
	copie->test_covid = original->test_covid;

	copie->data_calendaristica = original->data_calendaristica;
}


typedef struct lista_dubla lista_dubla;

struct lista_dubla {
	Programare programare;
	lista_dubla* next;
	lista_dubla* prev;
};

lista_dubla* insert_lista_dubla(lista_dubla* cap, Programare* programare) {
	if (cap == NULL) {
		lista_dubla* nou = (lista_dubla*)malloc(sizeof(lista_dubla));
		copie_programare(programare, &nou->programare);

		nou->next = NULL;
		nou->prev = NULL;
		cap = nou;
	}
	else if (cap->next == NULL) {
		lista_dubla* nou = (lista_dubla*)malloc(sizeof(lista_dubla));
		copie_programare(programare, &nou->programare);

		nou->next = NULL;
		nou->prev = NULL;
		cap->next = nou;
		nou->prev = cap;
	}
	else {
		insert_lista_dubla(cap->next, programare);
	}

	return cap;
}

void print_lista_dubla(lista_dubla* cap) {
	if (cap == NULL) {
		return;
	}
	afisare_programare_doctor(cap->programare);
	print_lista_dubla(cap->next);
}


void vector_programari_cerinta2(lista_dubla* cap, Programare* vector, int* size, int luna) {
	if (cap == NULL) {
		return;
	}
	if (cap->programare.data_calendaristica.luna == luna) {
		copie_programare(&cap->programare, &vector[*size]);
		*size = *size + 1;
	}

	vector_programari_cerinta2(cap->next, vector, size, luna);
}

typedef struct {
	int size;
	lista_dubla** liste;
} HashTable;

void afisare_tabela_dispersie(HashTable tabela) {
	printf("Tabela de dispersie este\n");
	for (int i = 0; i < tabela.size; i++) {
		printf("Bucket-ul pentru luna %d\n", i);
		print_lista_dubla(tabela.liste[i]);
	}
}

void lista_hashtable(lista_dubla* cap, HashTable* tabela) {
	if (cap == NULL) {
		return;
	}
	Programare copie;
	int index = cap->programare.data_calendaristica.luna % tabela->size;
	tabela->liste[index] = insert_lista_dubla(tabela->liste[index], &cap->programare);

	lista_hashtable(cap->next, tabela);
}

void stergere_vector_cerinta2(Programare* vector, int n) {
	for (int i = 0; i < n; i++) {
		dezalocare_programare(&vector[i]);
	}
	free(vector);
}

void stergere_lista(lista_dubla* cap) {
	if (cap == NULL) {
		return;
	}
	stergere_lista(cap->next);
	dezalocare_programare(&cap->programare);
	free(cap);
}

void dezalocare_tabela_dispersie(HashTable* tabela) {
	for (int i = 0; i < tabela->size; i++) {
		stergere_lista(tabela->liste[i]);
	}
	free(tabela->liste);
}

int numar_programari_luna(lista_dubla* lista, int luna) {
	if (lista == NULL) {
		return 0;
	}

	if (lista->programare.data_calendaristica.an == 2021 && lista->programare.data_calendaristica.luna == luna) {
		return 1 + numar_programari_luna(lista->next, luna);
	}
	else {
		return 0 + numar_programari_luna(lista->next, luna);
	}
}

void cautare_luna_max(lista_dubla* lista, int* luna, int* nr) {
	*luna = 0;
	*nr = 0;
	for (int l = 1; l <= 12; l++) {
		if (*nr < numar_programari_luna(lista, l)) {
			*nr = numar_programari_luna(lista, l);
			*luna = l;
		}
	}
}

int main() {
	int n, i;
	lista_dubla* cap = NULL;
	FILE* fisier = fopen("1058_Stirbescu_Monica.txt", "r");

	Programare prog;
	fscanf(fisier, "%d", &n);
	for (i = 0; i < n; i++) {
		prog = citire_fisier_programare(fisier);
		cap = insert_lista_dubla(cap, &prog);
		dezalocare_programare(&prog);
	}
	fclose(fisier);

	printf("\n Cerinta 1: \n");

	print_lista_dubla(cap);

	printf("\n Cerinta 2: \n");

	int size = 0;
	Programare* vector = (Programare*)malloc(sizeof(Programare) * n);
	vector_programari_cerinta2(cap, vector, &size, 3);
	printf("\n Programarile din luna specificata sunt:\n");
	for (i = 0; i < size; i++) {
		afisare_programare_doctor(vector[i]);
	}

	printf("\n Cerinta 3: \n");
	HashTable hashtable;
	hashtable.size = 12;
	hashtable.liste = (lista_dubla**)malloc(sizeof(lista_dubla*) * 12);
	for (i = 0; i < 12; i++) {
		hashtable.liste[i] = NULL;
	}
	lista_hashtable(cap, &hashtable);
	afisare_tabela_dispersie(hashtable);

	printf("\n Cerinta 4: \n");
	int luna, nr;
	cautare_luna_max(cap, &luna, &nr);
	printf("Luna %d are numar maxim de aparitii (%d)", luna, nr);


	printf("\n Cerinta 5: \n");

	stergere_vector_cerinta2(vector, size);
	stergere_lista(cap);
	dezalocare_tabela_dispersie(&hashtable);

	return 0;
}
