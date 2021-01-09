/**
 * Sammlung von Funktionen für die Eingabe von int-Werten über die Konsole
 */

#include "../include/input_int.h"

/**
 * @brief Funktion zum einlesen für eine int-Variable
 * @param save_loc Adresse einer int-Variable in die gespeichert wird
 * @return Anzahl erfolgreich eingelesener int-Werte
*/
int get_int_input(int *save_loc) {
	char input[BUFSIZ];
	int status = 0;

	fgets(input, BUFSIZ, stdin);
	const unsigned long length = strlen(input);

	if (check_int_input(input, length) == 0) {
		// Eingabe gültig
		*save_loc = atoi(input);
		status = 1;
	}

	return status;
}

/**
 * @brief Funktion zur Überprüfung eines Strings auf einen gültigen int-Wert
 * @param input_string String der überprüft werden soll
 * @param length Länge des Strings
 * @return Anzahl gefundener Fehler, bzw. Zeichen die keine Zahl sind 0 wenn String gültig ist
*/
int check_int_input(const char* input_string, const unsigned long length) {
	int fehler = 0;
	
	for (unsigned long index = 0; index < length; index++) {
		// isdigit gibt für Zeichen != [0-9] 0 zurück
		const int auswertung = isdigit((unsigned char)input_string[index]);
		if (auswertung == 0) {
			fehler = fehler + 1;
		}
	}
	//Bei negativer Zahl liegt an Index 0 '-', daher muss ein Fehler wieder abgezogen werden
	if (input_string[0] == '-' || input_string[0] == '+') {
		fehler = fehler - 1;
	}

	if(input_string[length-1] == '\n')
	{
		fehler = fehler - 1;
	}

	return fehler;
}
