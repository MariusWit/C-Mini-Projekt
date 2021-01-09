#include "../include/oberflaeche.h"

/**
 * @brief Stellt die Spieloberfläche und interaktive Eingabe bereit
 * @param spieler_ptr Spieler Struktur, um das Spielfeld des gegners darzustellen die
 * ausgewählten x- und y-Koordinaten werden über die Struktur zurückgegeben
 * @param y_pos_ptr Die ausgewählte y-Koordinate wird über den Pointer zurückgegeben
 * @param x_pos_ptr Die ausgewählte x-Koordinate wird über den Pointer zurückgegeben
*/
void spieler_tipp_abgabe(struct spieler* spieler_ptr, int* y_pos_ptr, int* x_pos_ptr)
{
	initscr(); // leeres Fenster erzeugen
	cbreak(); // Puffern von Eingaben deaktivieren (kein ENTER nötig)
	noecho(); // Benutzereingaben unsichtbar (sichtbar machen mit echo())
	keypad(stdscr, TRUE); // Funktions- und Pfeiltasten erlauben
	nodelay(stdscr, TRUE); // verhindert Anhalten des Programms für Eingaben

	curs_set(0); // Cursor unsichtbar (0), sichtbar (1)

	int quit = 0;

	int modus = 1;

	// Minimum und Maximum für X-Koordinaten festlegen
	int x_min = (SPIELFELD_X_OFFSET + 2);
	int x_max = (SPIELFELD_X_OFFSET + 2) + (spieler_ptr->gegnerisches_spielfeld.anz_spalten * 2) - 2;

	// Minimum und Maximum für Y-Koordinaten festlegen
	int y_min = (SPIELFELD_Y_OFFSET + 1);
	int y_max = (SPIELFELD_Y_OFFSET + 1) + (spieler_ptr->gegnerisches_spielfeld.anz_zeilen - 1);

	// Wenn bisher noch kein Tipp abgegeben wurde muss der Cursor auf die obere linke Ecke des Spielfelds setzen
	int x = spieler_ptr->letzte_x_pos == 0 ? x_min : ((spieler_ptr->letzte_x_pos * 2) + (SPIELFELD_X_OFFSET + 2));
	int y = spieler_ptr->letzte_y_pos == 0 ? y_min : (spieler_ptr->letzte_y_pos + (SPIELFELD_Y_OFFSET + 1));

	do
	{
		erase();
		zeichne_spielfeld(&spieler_ptr->gegnerisches_spielfeld, modus, spieler_ptr->spieler_nummer);
		zeichne_fadenkreuz(y, x, spieler_ptr->gegnerisches_spielfeld.anz_zeilen,
		                   spieler_ptr->gegnerisches_spielfeld.anz_spalten);
		mvprintw(1, 1, "Fadenkreuz mit Pfeiltasten bewegen, Tipp abgeben mit ENTER.");
		mvprintw(2, 1, "Mit c kann das gesamte Spielfeld offengelegt werden.");
		mvprintw(4, 1, "Spieler %d ist am Zug, dies ist der %d Tipp.", spieler_ptr->spieler_nummer,
		         spieler_ptr->anz_versuche + 1);
		refresh();

		const int pressed_key = wgetch(stdscr);


		// Cursor darf den Screen nicht verlassen
		switch (pressed_key)
		{
		case ERR:
			napms(10); // Pause in Millisekunden
			break;
		case KEY_UP:
			y--;
			y = y < y_min ? y_min : y;
			break;
		case KEY_DOWN:
			y++;
			y = y > y_max ? y_max : y;
			break;
		case KEY_LEFT:
			x = x - 2;
			x = x < x_min ? x_min : x;
			break;
		case KEY_RIGHT:
			x = x + 2;
			x = x > x_max ? x_max : x;
			break;
		case 'c':
			// Modus entweder von 1 auf 2 setzen oder von 2 auf 1
			modus = modus == 1 ? 2 : 1;
			break;
		case '\n': // Für ENTER, ENTER_KEY fängt CarriageReturn nicht ab
			*y_pos_ptr = y - (SPIELFELD_Y_OFFSET + 1); // y-Koordinate in Koordinate im Spielfeld umwandeln
			*x_pos_ptr = (x - (SPIELFELD_X_OFFSET + 1)) / 2; // x-Koordinate in Koordinate im Spielfeld umwandeln
			quit = 1;
			break;
		default: ;
		}
	}
	while (!quit);

	endwin(); // Fenster schliessen
}

/**
 * @brief Bietet Ausgabe und Eingabe um Schiffe zu platzieren
 * @param spieler_ptr Spieler Struktur für die Darstellung des Spielfelds
 * @param schiff_nummer Nummer des Schiffs das gesetzt wird
 * @param y_pos_ptr Pointer auf einen int-Wert für die y-Koordinate eines Schiffs
 * @param x_pos_ptr Pointer auf einen int-Wert für die x-Koordinate eines Schiffs
 * @param ausrichtung_ptr Pointer auf einen int-Wert für die Ausrichtung eines zu setzenden Schiffs
 * @return Auswahl, 0 für Schiff mit Koordinaten platzieren, -1 für zufällige Platzierung
*/
int spieler_schiffe_platzieren(struct spieler* spieler_ptr, const int schiff_nummer, int* y_pos_ptr, int* x_pos_ptr,
                               int* ausrichtung_ptr)
{
	initscr(); // leeres Fenster erzeugen
	cbreak(); // Puffern von Eingaben deaktivieren (kein ENTER nötig)
	noecho(); // Benutzereingaben unsichtbar (sichtbar machen mit echo())
	keypad(stdscr, TRUE); // Funktions- und Pfeiltasten erlauben
	nodelay(stdscr, TRUE); // verhindert Anhalten des Programms für Eingaben

	curs_set(0); // Cursor unsichtbar (0), sichtbar (1)

	int quit = 0;

	// Wenn ein Schiff zufällig platziert werden soll -1
	int auswahl = 0;

	// Minimum und Maximum für X-Koordinaten festlegen
	int x_min = (SPIELFELD_X_OFFSET + 2);
	int x_max = (SPIELFELD_X_OFFSET + 2) + (spieler_ptr->gegnerisches_spielfeld.anz_spalten * 2) - 2;

	// Minimum und Maximum für Y-Koordinaten festlegen
	int y_min = (SPIELFELD_Y_OFFSET + 1);
	int y_max = (SPIELFELD_Y_OFFSET + 1) + (spieler_ptr->gegnerisches_spielfeld.anz_zeilen - 1);

	// Cursor auf die obere linke Ecke setzen oder vorherige Position 
	int x = *x_pos_ptr == 0 ? x_min : ((*x_pos_ptr * 2) + (SPIELFELD_X_OFFSET + 2));
	int y = *y_pos_ptr == 0 ? y_min : (*y_pos_ptr + (SPIELFELD_Y_OFFSET + 1));

	do
	{
		erase();
		zeichne_indizes(spieler_ptr->gegnerisches_spielfeld.anz_zeilen, spieler_ptr->gegnerisches_spielfeld.anz_spalten,
		                spieler_ptr->spieler_nummer);
		zeichne_spielfeld_inhalt(spieler_ptr->gegnerisches_spielfeld.elemente,
		                         spieler_ptr->gegnerisches_spielfeld.anz_zeilen,
		                         spieler_ptr->gegnerisches_spielfeld.anz_spalten, 2);
		mvwprintw(stdscr, y, x, "X");
		mvprintw(1, 1, "Cursor mit Pfeiltasten bewegen.");
		mvprintw(2, 1, "Ein Schiff zufaellig platzieren mit 'z'");
		mvprintw(3, 1, "Platzieren beginnen mit ENTER. Danach die Ausrichtung durch Pfeiltasten bestimmen.");
		mvprintw(5, 1, "Spieler %d platziert die eigenen Schiffe, dies ist das %d Schiff von %d.",
		         spieler_ptr->spieler_nummer == 1 ? 2 : 1, schiff_nummer, spieler_ptr->anz_schiffe);
		refresh();

		const int pressed_key = getch();

		// Cursor darf den Screen nicht verlassen
		switch (pressed_key)
		{
		case ERR:
			napms(10); // Pause in Millisekunden
			break;
		case KEY_UP:
			y--;
			y = y < y_min ? y_min : y;
			erase();
			break;
		case KEY_DOWN:
			y++;
			y = y > y_max ? y_max : y;
			erase();
			break;
		case KEY_LEFT:
			x = x - 2;
			x = x < x_min ? x_min : x;
			erase();
			break;
		case KEY_RIGHT:
			x = x + 2;
			x = x > x_max ? x_max : x;
			erase();
			break;
		case 'z': // Ein Schiff zufällig platzieren
			// Auswahl auf -1 für eine zufällige Platzierung
			auswahl = -1;
			quit = 1;
			break;
		case '\n': // Für ENTER, ENTER_KEY fängt CarriageReturn nicht ab
			while (!quit)
			{
				// Position speichern
				*y_pos_ptr = y - (SPIELFELD_Y_OFFSET + 1); // y-Koordinate in Koordinate im Spielfeld umwandeln
				*x_pos_ptr = (x - (SPIELFELD_X_OFFSET + 1)) / 2; // x-Koordinate in Koordinate im Spielfeld umwandeln

				zeichne_richtungspfeile(y, x, y_max, x_max, y_min, x_min);

				// Pfeiltaste für richtung
				const int richtung = getch();
				switch (richtung)
				{
				case ERR:
					napms(10); // Pause in Millisekunden
					break;
				case KEY_UP:
					*ausrichtung_ptr = 1;
					quit = 1;
					break;
				case KEY_DOWN:
					*ausrichtung_ptr = 2;
					quit = 1;
					break;
				case KEY_LEFT:
					*ausrichtung_ptr = 3;
					quit = 1;
					break;
				case KEY_RIGHT:
					*ausrichtung_ptr = 4;
					quit = 1;
					break;
				default: ;
				}
			}
			break;
		default: ;
		}
	}
	while (!quit);

	endwin(); // Fenster schliessen

	return auswahl;
}

/**
 * @brief Dekodiert einen Wert im Spielfeld zu einem char für die Anzeige
 * @details Kodierung des Spielfelds:
 *	0: Wasser (~)
 *	-1 bis -333 Treffer für den jeweilig getroffenen Teil eines Schiffs (T)
 *	-333: kein Treffer (O)
 *	-1000: versenkt (V)
 *
 * @param value Kodierter Wert im Spielfeld
 * @param modus 2 für Debug-Modus, um Schiffe etc. für den Spieler sofort sichtbar zu machen
 * @return Anzuzeigendes Zeichen für die Ausgabe
*/
char spielfeld_index_dekodieren(const int value, const int modus)
{
	char to_ret;

	if (value == -1000)
	{
		to_ret = 'V';
	}
	else if (value < 0 && value > -333)
	{
		to_ret = 'T';
	}
	else if (value == -333)
	{
		to_ret = 'O';
	}
	else
	{
		to_ret = '~';
	}

	//Cheat-Modus, zeigt alle Schiffe sofort an
	if (modus == 2)
	{
		if (value > 0)
		{
			//Funktioniert nur bis max. 9 Schiffen
			to_ret = (char)(value + '0');
		}
	}
	return to_ret;
}

/**
 * @brief Stellt die Indizierung des Spielfelds dar
 * @param anz_zeilen Anzahl der Zeilen des Spielfelds
 * @param anz_spalten Anzahl der Spalten des Spielfelds
 * @param spieler_nummer Nummer des Spielers der gerade eine Eingabe machen darf
*/
void zeichne_indizes(const int anz_zeilen, const int anz_spalten, const int spieler_nummer)
{
	//Anzeige mit Spalten und Zeilen- und Spaltennummern
	for (int index_spalten = 0; index_spalten < anz_spalten; index_spalten++)
	{
		// Spaltenkennung über oberen Spielfeldrand schreiben
		mvprintw((SPIELFELD_Y_OFFSET - 1), (index_spalten * 2) + (SPIELFELD_X_OFFSET + 2), "%c", ('a' + index_spalten));
	}

	for (int index_zeilen = 0; index_zeilen < anz_zeilen; index_zeilen++)
	{
		// Zeilennummer links neben das Spielfeld schreiben
		mvprintw(index_zeilen + (SPIELFELD_Y_OFFSET + 1), SPIELFELD_X_OFFSET - 2, "%d", index_zeilen + 1);
	}


	// Um die Spieler differenzieren zu können, Grün: Spieler 1, Rot: Spieler 2
	start_color();
	init_pair(1, COLOR_BLACK, COLOR_RED);
	init_pair(2, COLOR_BLACK, COLOR_GREEN);

	if (spieler_nummer == 1)
	{
		attron(COLOR_PAIR(1));
	}
	else
	{
		attron(COLOR_PAIR(2));
	}


	// Spielfeld Grenzen zeichnen
	mvwvline(stdscr, SPIELFELD_Y_OFFSET, SPIELFELD_X_OFFSET, ACS_VLINE, anz_zeilen + 1); // linke Seite
	mvwvline(stdscr, SPIELFELD_Y_OFFSET, (anz_spalten * 2) + (SPIELFELD_X_OFFSET + 2), ACS_VLINE, anz_zeilen + 1);
	// rechte Seite
	mvwhline(stdscr, SPIELFELD_Y_OFFSET, SPIELFELD_X_OFFSET, ACS_HLINE, (anz_spalten * 2) + 2); // obere Seite
	mvwhline(stdscr, anz_zeilen + (SPIELFELD_Y_OFFSET + 1), SPIELFELD_X_OFFSET, ACS_HLINE, (anz_spalten * 2) + 2);
	// untere Seite

	// Ecken zeichnen
	mvwvline(stdscr, SPIELFELD_Y_OFFSET, SPIELFELD_X_OFFSET, ACS_ULCORNER, 1); // oben links
	mvwvline(stdscr, SPIELFELD_Y_OFFSET, (anz_spalten * 2) + (SPIELFELD_X_OFFSET + 2), ACS_URCORNER, 1); // oben rechts
	mvwvline(stdscr, anz_zeilen + (SPIELFELD_Y_OFFSET + 1), (anz_spalten * 2) + (SPIELFELD_X_OFFSET + 2), ACS_LRCORNER,
	         1); // unten rechts
	mvwvline(stdscr, anz_zeilen + (SPIELFELD_Y_OFFSET + 1), SPIELFELD_X_OFFSET, ACS_LLCORNER, 1); // unten links

	if (spieler_nummer == 1)
	{
		attroff(COLOR_PAIR(1));
	}
	else
	{
		attroff(COLOR_PAIR(2));
	}
}

/**
 * @brief Zeichnet das Fadenkreuz für die Auswahl der Koordinaten durch einen Spieler
 * @param y Derzeitige ausgewählte y-Koordinate
 * @param x Derzeitige ausgewählte x-Koordinate
 * @param anz_zeilen Anzahl der Zeilen des Spielfelds
 * @param anz_spalten Anzahl der Spalten des Spielfelds
*/
void zeichne_fadenkreuz(const int y, const int x, const int anz_zeilen, const int anz_spalten)
{
	// Kreuz mit Cursor Position als Mittelpunkt zeichnen

	// Linien des Fadenkreuz zeichnen
	//mvwhline(stdscr, y, 0 + (SPIELFELD_X_OFFSET + 1), ACS_HLINE, (anz_spalten * 2) + 1);
	//mvwvline(stdscr, 0 + (SPIELFELD_Y_OFFSET + 1), x, ACS_VLINE, anz_zeilen);
	
	mvwprintw(stdscr, y, x, "X");
}

void zeichne_spielfeld_inhalt(int** spielfeld_ptr, const int anz_zeilen, const int anz_spalten, const int modus)
{
	for (int index_zeilen = 0; index_zeilen < anz_zeilen; index_zeilen++)
	{
		for (int index_spalten = 0; index_spalten < anz_spalten; index_spalten++)
		{
			// Spielfeld ist um 4 Zeilen nach unten geschoben um Platz für Ausgaben darüber zu haben
			mvprintw(index_zeilen + (SPIELFELD_Y_OFFSET + 1), (index_spalten * 2) + (SPIELFELD_X_OFFSET + 2), "%c",
			         spielfeld_index_dekodieren(spielfeld_ptr[index_zeilen][index_spalten], modus));
		}
	}
}

/**
 * @brief Zeichnet das gesamte Spielfeld also Indizierung und Inhalt
 * @param spielfeld_ptr Spielfeld das darstellt werden soll
 * @param modus Für verdeckte Schiffe 1 für offen gelegte Schiffe 2
*/
void zeichne_spielfeld(struct matrix* spielfeld_ptr, const int modus, const int spieler_nummer)
{
	zeichne_indizes(spielfeld_ptr->anz_zeilen, spielfeld_ptr->anz_spalten, spieler_nummer);
	zeichne_spielfeld_inhalt(spielfeld_ptr->elemente, spielfeld_ptr->anz_zeilen, spielfeld_ptr->anz_spalten, modus);
	zeichne_indizes(spielfeld_ptr->anz_zeilen, spielfeld_ptr->anz_spalten, spieler_nummer);
}

/**
 * @brief Zeichnen der möglichen Richtungen für die Ausrichtung eines Schiffs
 * @param y_pos Derzeitige y-Koordinate
 * @param x_pos Derzeitige x-Koordinate
 * @param y_max Maximum für die y-Koordinate
 * @param x_max Maximum für die x-Koordinate
 * @param y_min Minimum für die y-Koordinate
 * @param x_min Maximum für die x-Koordinate
*/
void zeichne_richtungspfeile(const int y_pos, const int x_pos, const int y_max, const int x_max, const int y_min,
                             const int x_min)
{
	// Zeichnen der Richtungspfeile sofern sie nicht auf dem Spielfeldrand liegen
	if (x_pos < x_max)
	{
		mvwvline(stdscr, y_pos, x_pos + 2, ACS_RARROW, 1);
	}
	if (x_pos > x_min)
	{
		mvwvline(stdscr, y_pos, x_pos - 2, ACS_LARROW, 1);
	}
	if (y_pos < y_max)
	{
		mvwvline(stdscr, y_pos + 1, x_pos, ACS_DARROW, 1);
	}
	if (y_pos > y_min)
	{
		mvwvline(stdscr, y_pos - 1, x_pos, ACS_UARROW, 1);
	}
}
