/**
 * main.c Als Einstiegspunkt in das Programm
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
