/*
 * Spieler Struktur f�r einen Spieler, mit ben�tigten Werten
 * f�r das spielen von "Schiffe_versenken"
 */

#ifndef SPIELER_H
#define SPIELER_H

#include "../include/felder_2d.h"

/**
 * @brief Struktur f�r einen Spieler mit der Spieler Nummer, dem letzt ausf�hrten Tipp,
 * der Anzahl der bisher abgegeben Tipps und dem gegnerischem Spielfeld
*/
struct spieler
{
	// Spielregeln als const um sp�tere �nderung zu verhindern
	const int spieler_nummer; // Nummer des Spielers, 1 oder 2
	const int anz_schiffe; // Anzahl der Schiffe auf dem generischen Spielfeld
	int letzte_x_pos; // Derzeitige Position auf dem Spielfeld x-Koordinate
	int letzte_y_pos; // Derzeitige Position auf dem Spielfeld y-Koordinate
	int anz_versuche; // Bisher abgegebene Tipps 
	struct matrix gegnerisches_spielfeld; // Spielfeld mit den Schiffen des Gegners f�r das Tipps abgegeben wird
};

#endif //SPIELER_H
