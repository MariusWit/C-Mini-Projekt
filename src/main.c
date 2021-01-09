/**
 * @file main.c
 * @brief Eintrittspunkt für den Compiler, startet das Spiel
 * @version 0.1
 * @date 2021-01-02
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "../include/schiffe_versenken.h"



int main()
{
	// Generator für Zufallszahlen initialisieren
	const time_t zeit = time(0);
	srand((unsigned int)zeit);

	// Spiel starten
	spiel_start();
	
	return 0;
}
