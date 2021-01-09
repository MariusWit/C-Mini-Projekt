/**
 * @file felder_2d.h
 * @brief Sammlung von Funktionen für 2D-Felder
 * @version 0.1
 * @date 2020-12-20
 * 
 * @copyright Copyright (c) 2020
 * 
 */

// Include Guard setzen
#ifndef FELDER_2D_H
#define FELDER_2D_H

#include <stdio.h>
#include <stdlib.h>

struct matrix
{
	int anz_zeilen;
	int anz_spalten;
	int** elemente;
};

int** speicher_reservieren_2d_feld(const int anz_zeilen, const int anz_spalten);

void speicher_freigeben_2d_feld(int** feld_ptr, const int anz_zeilen);

void setze_wert_2d_feld(int** feld_ptr, const int anz_zeilen, const int anz_spalten, const int wert);

void print_2d_feld(int** feld_ptr, int anz_zeilen, int anz_spalten);

#endif// FELDER_2D_H
