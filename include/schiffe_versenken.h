/**
 * Eine Implementierung des Spiels "Schiffe versenken"
 * Schiffe können selbst oder zufällig platziert werden,
 * und haben immer eine Länge die durch SCHIFF_GROESSE definiert wird.
 */

// Include Guard setzen
#ifndef SCHIFFE_VERSENKEN_H
#define SCHIFFE_VERSENKEN_H

#include <time.h>
#include <stdio.h>
#include "input_int.h"

#include "../include/felder_2d.h"
#include "../include/oberflaeche.h"


#define SCHIFF_GROESSE 5
#define MIN_ANZ_SPALTEN 10
#define MAX_ANZ_SPALTEN 26
#define MIN_ANZ_ZEILEN 10
#define MAX_ANZ_ZEILEN 20

#define MIN_ANZ_SCHIFFE 1
#define MAX_ANZ_SCHIFFE 9

int spiel_start(void);

struct spieler init_spieler(int spieler_nummer, int anz_zeilen, int anz_spalten, int anz_schiffe);

int spieler_zug(struct spieler* spieler_ptr);

int platziere_schiff(struct matrix* spielfeld_ptr, int y1_koordinate, int x1_koordinate, int ausrichtung, int schiff_nummer);

void eingabe_schiffe_platzieren(struct spieler* spieler_ptr);

int schiffe_zufaellig_platzieren(int** spielfeld_ptr, int anz_zeilen, int anz_spalten, int anz_schiffe, int start_schiff_nummer);

int platziere_zufaellig_vertikal(int** spielfeld_ptr, int anz_zeilen, int anz_spalten, int schiff_nummer);

int platziere_zufaellig_horizontal(int** spielfeld_ptr, int anz_zeilen, int anz_spalten, int schiff_nummer);

void eingabe_spielregeln(int* anz_zeilen_ptr, int* anz_spalten_ptr, int* anz_schiffe_ptr);

void get_valid_cords(int* x_koordinate_ptr, int* y_koordinate_ptr, int anz_zeilen, int anz_spalten);

int count_not_hit_ship_parts(int** spielfeld_ptr, const int anz_zeilen, const int anz_spalten, const int anz_schiffe);

int check_and_set_getroffen(int** spielfeld_ptr, int y_koordinate, int x_koordinate, int anz_schiffe);

void check_versenkt(int** spielfeld_ptr, int anz_zeilen, int anz_spalten, int anz_schiffe);

void schiff_versenkt(int** spielfeld_ptr, int anz_zeilen, int anz_spalten, int schiff_nummer);

#endif // SCHIFFE_VERSENKEN_H

