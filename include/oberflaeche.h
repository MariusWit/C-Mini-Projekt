/**
 * @file oberflaeche.h
 * @brief Bietet eine interaktive Oberfläche für das Spiel "Schiffe versenken"
 * @version 0.1
 * @date 2021-01-02
 * 
 * @copyright Copyright (c) 2021
 * 
 */

// Include Guard setzen
#ifndef OBERFLAECHE_H
#define OBERFLAECHE_H

#include "../include/spieler.h" // Spieler Struktur enthält alle benötigten Informationen für die Darstellung
#include <ncurses.h>

/**
 * Koordinaten für die obere rechte Ecke des Spielfelds
 * Zeilennummer und Spaltenbezeichnung bei Änderung beachten
 */
#define SPIELFELD_X_OFFSET 4 // x-Koordinate der linken oberen Ecke des Spielfelds
#define SPIELFELD_Y_OFFSET 8 // y-Koordinate der linken oberen Ecke des Spielfelds

void spieler_tipp_abgabe(struct spieler* spieler_ptr, int* y_pos_ptr, int* x_pos_ptr);

int spieler_schiffe_platzieren(struct spieler* spieler_ptr, int schiff_nummer, int* y_pos_ptr, int* x_pos_ptr,
                               int* ausrichtung_ptr);

char spielfeld_index_dekodieren(const int value, const int modus);

void zeichne_indizes(const int anz_zeilen, const int anz_spalten, const int spieler_nummer);

void zeichne_fadenkreuz(const int y, const int x, const int anz_zeilen, const int anz_spalten);

void zeichne_spielfeld_inhalt(int** spielfeld_ptr, const int anz_zeilen, const int anz_spalten, const int modus);

void zeichne_spielfeld(struct matrix* spielfeld_ptr, const int modus, const int spieler_nummer);

void zeichne_richtungspfeile(int y_pos, int x_pos, int y_max, int x_max, int y_min, int x_min);

#endif
