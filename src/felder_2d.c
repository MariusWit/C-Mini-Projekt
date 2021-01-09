/**
 * Sammlung von Funktionen für 2D-Felder mit int-Werten
 */

#include "../include/felder_2d.h"

/**
 * @brief Funktion für das reservieren von Speicher für ein 2D-Feld mit int-Werten
 * @param anz_zeilen Anzahl der Zeilen des 2D-Felds
 * @param anz_spalten Anzahl der Spalten des 2D-Felds
 * @return Adresse des reservierten Speichers,  NULL bei Fehlern beim reservieren des Speichers
*/
int** speicher_reservieren_2d_feld(const int anz_zeilen, const int anz_spalten)
{
	int** matrix = (int**)malloc(anz_zeilen * sizeof(int*));

	// Speicher für das äußere Feld reservieren
	if (matrix != NULL)
	{
		// Speicher für die inneren Felder reservieren
		for (int index_zeilen = 0; index_zeilen < anz_zeilen; index_zeilen++)
		{
			matrix[index_zeilen] = (int*)malloc(anz_spalten * sizeof(int));

			// Fehler bei reservierung von Speicher für die inneren Felder
			if (matrix[index_zeilen] == NULL)
			{
				// Bisher reservierten Speicher für die inneren Felder freigeben
				for (int i = 0; i < index_zeilen; i++)
				{
					free(matrix[i]);
				}
				// Speicher des äußeren Felds freigeben und als ungültig setzen
				free(matrix);
				matrix = NULL;
			}
		}
	}
	return matrix;
}

/**
 * @brief Funktion um Speicher von einem 2D-Feld freizugeben 
 * @param feld_ptr Adresse des Felds, dessen speicher freigegeben werden soll
 * @param anz_zeilen Anzahl der inneren Felder
*/
void speicher_freigeben_2d_feld(int** feld_ptr, const int anz_zeilen)
{
	if (feld_ptr != NULL)
	{
		// Speicher der innere Felder freigeben 
		for (int index_zeilen = 0; index_zeilen < anz_zeilen; index_zeilen++)
		{
			free(feld_ptr[index_zeilen]);
		}
		free(feld_ptr);
	}
}

/**
 * @brief Setzt alle Werte eines 2D-Felds auf den übergebenen Wert
 * @param feld_ptr Adresse des Feldes, dessen Werte gesetzt werden sollen
 * @param anz_zeilen Anzahl der Zeilen des 2D-Felds
 * @param anz_spalten Anzahl der Spalten des 2D-Felds
 * @param wert Wert der gesetzt werden soll
*/
void setze_wert_2d_feld(int** feld_ptr, const int anz_zeilen, const int anz_spalten, const int wert)
{
	// Iteration über alle Elemente und Wert setzen
	for (int index_zeilen = 0; index_zeilen < anz_zeilen; index_zeilen++)
	{
		for (int index_spalten = 0; index_spalten < anz_spalten; index_spalten++)
		{
			feld_ptr[index_zeilen][index_spalten] = wert;
		}
	}
}

/**
 * @brief Funktion zur Ausgabe von 2D-Feldern auf der Konsole
 * @param feld_ptr Adresse dessen Werte ausgegeben werden sollen 
 * @param anz_zeilen Anzahl der Zeilen des 2D-Felds
 * @param anz_spalten Anzahl der Spalten des 2D-Felds
*/
void print_2d_feld(int** feld_ptr, const int anz_zeilen, const int anz_spalten)
{
	// Iteration über alle Elemente als Tabelle bzw. Matrix
	for (int index_zeilen = 0; index_zeilen < anz_zeilen; index_zeilen++)
	{
		for (int index_spalten = 0; index_spalten < anz_spalten; index_spalten++)
		{
			printf("%i\t", feld_ptr[index_zeilen][index_spalten]);
		}
		printf("\n\n");
	}
}
