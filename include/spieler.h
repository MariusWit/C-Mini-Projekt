/**
 * @file spieler.h
 * @brief  Spieler Struktur für einen Spieler
 * @details Hält für den Spielablauf benötigte Werte
 * @version 0.1
 * @date 2021-01-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef SPIELER_H
#define SPIELER_H

#include "../include/felder_2d.h"

/**
 * @brief Struktur für einen Spieler mit der Spieler Nummer, dem letzt ausführten Tipp,
 * der Anzahl der bisher abgegeben Tipps und dem gegnerischem Spielfeld
*/
struct spieler
{
	// Spielregeln als const um spätere Änderung zu verhindern
	const int spieler_nummer; // Nummer des Spielers, 1 oder 2
	const int anz_schiffe; // Anzahl der Schiffe auf dem generischen Spielfeld
	int letzte_x_pos; // Derzeitige Position auf dem Spielfeld x-Koordinate
	int letzte_y_pos; // Derzeitige Position auf dem Spielfeld y-Koordinate
	int anz_versuche; // Bisher abgegebene Tipps 
	struct matrix gegnerisches_spielfeld; // Spielfeld mit den Schiffen des Gegners für das Tipps abgegeben wird
};

#endif //SPIELER_H
