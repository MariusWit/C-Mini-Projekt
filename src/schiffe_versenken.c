/**
 * @file schiffe_versenken.h
 * @brief Eine Implementierung des Spiels "Schiffe versenken"
 * @details Schiffe können selbst oder zufällig platziert werden,
 * und haben immer eine Länge die durch SCHIFF_GROESSE definiert wird.
 * @version 0.1
 * @date 2020-12-20
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "../include/schiffe_versenken.h"

/**
 * @brief Steuert den Spielablauf von Beginn bis Ende
 * @return Exit-Status
*/
int spiel_start()
{
	int anz_zeilen = 0;
	int anz_spalten = 0;
	int anz_schiffe = 0;

	// Eingabe der Spielregeln durch den Spieler
	eingabe_spielregeln(&anz_zeilen, &anz_spalten, &anz_schiffe);

	// Konsole leeren, damit das Spielfeld ohne Scrollen angezeigt wird
	system("clear");


	/**
	 * Spieler anlegen die abwechselnd Tipps abgeben
	 * Schritte die dabei durchgeführt werden:
	 *	- Speichern der Spielregeln für jeden Spieler (Anzahl der Zeilen, Anzahl der Spalten und Anzahl der Schiffe)
	 *	- Reservieren des Speichers für das Spielfeld
	 */
	struct spieler spieler1 = init_spieler(1, anz_zeilen, anz_spalten, anz_schiffe);
	struct spieler spieler2 = init_spieler(2, anz_zeilen, anz_spalten, anz_schiffe);

	// Beide Spieler platzieren ihre Schiffe
	eingabe_schiffe_platzieren(&spieler1);
	eingabe_schiffe_platzieren(&spieler2);

	// Für benötige Treffer bis zum Sieg
	int spieler1_treffer_bis_sieg = 0;
	int spieler2_treffer_bis_sieg = 0;

	//Bis alle Schiffe eines Spielers versenkt wurden, wechseln sich die Spieler ab
	do
	{
		spieler1_treffer_bis_sieg = spieler_zug(&spieler1);

		// Wenn keine Treffer mehr benötigt hat Spieler 1 gewonnen
		if (spieler1_treffer_bis_sieg == 0)
		{
			printf("Spieler 1 hat nach %i Versuchen gewonnen.\n", spieler1.anz_versuche);
			break;
		}

		spieler2_treffer_bis_sieg = spieler_zug(&spieler2);

		// Wenn keine Treffer mehr benötigt hat Spieler 2 gewonnen
		if (spieler2_treffer_bis_sieg == 0)
		{
			printf("Spieler 2 hat nach %i Versuchen gewonnen.\n", spieler2.anz_versuche);
			break;
		}
	}
	while (spieler1_treffer_bis_sieg != 0 && spieler2_treffer_bis_sieg != 0);

	// Speicher der angelegten Spielfelder freigeben
	speicher_freigeben_2d_feld(spieler1.gegnerisches_spielfeld.elemente, anz_zeilen);
	speicher_freigeben_2d_feld(spieler2.gegnerisches_spielfeld.elemente, anz_zeilen);

	return EXIT_SUCCESS;
}

/**
 * @brief Eingabe der Schiffe durch den Gegner über die Oberfläche
 * @param spieler_ptr Spieler Struktur für die Schiffe gesetzt wird
*/
void eingabe_schiffe_platzieren(struct spieler* spieler_ptr)
{
	int ausrichtung = 0; // 1, 2, 3 oder 4 für die Ausrichtung eines Schiffs
	int x_koordinate = 0; // x-Koordinate von wo das erstellen eines Schiffs startet
	int y_koordinate = 0; // y-Koordinate von wo das erstellen eines Schiffs startet
	int ret_val = 0; // Getroffene Auswahl auf der Oberfläche

	// Schiffe platzieren bis die benötige Anzahl gesetzt wurde
	for (int i = 1; i <= spieler_ptr->anz_schiffe; i++)
	{
		// Oberfläche für die Eingabe aufrufen
		const int auswahl = spieler_schiffe_platzieren(spieler_ptr, i, &y_koordinate, &x_koordinate, &ausrichtung);
		if (auswahl == -1)
		{
			ret_val = schiffe_zufaellig_platzieren(spieler_ptr->gegnerisches_spielfeld.elemente,
			                                       spieler_ptr->gegnerisches_spielfeld.anz_zeilen,
			                                       spieler_ptr->gegnerisches_spielfeld.anz_spalten, 1, i);
			if (ret_val != 1)
			{
				printf("Es konnte nicht alle %d Schiffe platziert werden.\n", spieler_ptr->anz_schiffe);
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			ret_val = platziere_schiff(&spieler_ptr->gegnerisches_spielfeld, y_koordinate, x_koordinate, ausrichtung,
			                           i);
			if (ret_val != 0)
			{
				i--;
			}
		}
	}
}


/**
 * @brief Führt einen Zug für den übergebenen Spieler durch, dieser gibt seinen Tipp ab, welcher dann überprüft wird
 * @param spieler_ptr Spieler der sein Tipp abgeben soll
 * @return Anzahl der noch benötigten Treffer auf bisher nicht getroffene Schiffsteile
*/
int spieler_zug(struct spieler* spieler_ptr)
{
	int y_pos_tipp = 0;
	int x_pos_tipp = 0;

	// Der Zug beginnt mit der Abgabe eines Tipps über die Oberfläche
	spieler_tipp_abgabe(spieler_ptr, &y_pos_tipp, &x_pos_tipp);

	// Mit den Koordinaten die ausgewählt wurden auf Treffer überprüfen und ggf. setzen
	check_and_set_getroffen(spieler_ptr->gegnerisches_spielfeld.elemente, y_pos_tipp, x_pos_tipp, spieler_ptr->anz_schiffe);

	// Überprüfen und setzen falls ein Schiff als versenkt markiert werden muss
	check_versenkt(spieler_ptr->gegnerisches_spielfeld.elemente, spieler_ptr->gegnerisches_spielfeld.anz_zeilen,
	               spieler_ptr->gegnerisches_spielfeld.anz_spalten, spieler_ptr->anz_schiffe);

	// Benötige Treffer bis zum Sieg bestimmen, wenn dies 0 ist hat der Spieler gewonnen
	const int anz_treffer_bis_sieg = count_not_hit_ship_parts(spieler_ptr->gegnerisches_spielfeld.elemente,
	                                                          spieler_ptr->gegnerisches_spielfeld.anz_zeilen,
	                                                          spieler_ptr->gegnerisches_spielfeld.anz_spalten,
	                                                          spieler_ptr->anz_schiffe);

	// Anzahl der bisherig abgegebenen Tipps
	spieler_ptr->anz_versuche++;

	// Letzte Position aktualisieren damit der Spieler seine Position auf dem Spielfeld nicht erneut ansteuern muss
	spieler_ptr->letzte_x_pos = x_pos_tipp;
	spieler_ptr->letzte_y_pos = y_pos_tipp;

	return anz_treffer_bis_sieg;
}


/**
 * @brief Initialisiert einen Spieler soweit, dass Schiffe platziert werden können
 * @param spieler_nummer Nummer des Spielers, 1 oder 2
 * @param anz_zeilen Anzahl der Zeilen des Spielfelds
 * @param anz_spalten Anzahl der Spalten des Spielfelds
 * @param anz_schiffe Anzahl der Schiffe die zufällig gesetzt werden sollen
 * @return spieler Rückgabe einer initialisierten spieler Struktur
*/
struct spieler init_spieler(const int spieler_nummer, const int anz_zeilen, const int anz_spalten,
                            const int anz_schiffe)
{
	const struct spieler neuer_spieler = {

		.spieler_nummer = spieler_nummer,
		.anz_versuche = 0,
		.letzte_x_pos = 0,
		.letzte_y_pos = 0,
		.anz_schiffe = anz_schiffe,

		// Gegnerisches Spielfeld vorbereiten
		.gegnerisches_spielfeld.anz_spalten = anz_spalten,
		.gegnerisches_spielfeld.anz_zeilen = anz_zeilen,
		// Speicher für das Spielfeld reservieren
		.gegnerisches_spielfeld.elemente = speicher_reservieren_2d_feld(anz_zeilen, anz_spalten)

	};

	setze_wert_2d_feld(neuer_spieler.gegnerisches_spielfeld.elemente, anz_zeilen, anz_spalten, 0);

	return neuer_spieler;
}

/**
 * @brief Versucht ein Schiff auf dem Spielfeld zu platzieren, durch 2 Koordinatenpaare wird die Richtung vorgegeben
 * @param spielfeld_ptr Adresse des Spielfelds auf das ein Schiff platziert werden soll
 * @param y1_koordinate y-Koordinate des Schiffs (Heck) (Hinterseite)
 * @param x1_koordinate x-Koordinate des Schiffs (Heck) (Hinterseite)
 * @param ausrichtung Ausrichtung in die platziert werden soll: 1 (nach oben); 2 (nach unten); 3 (nach links); 4 (nach rechts)
 * @param schiff_nummer Nummerierung des Schiffs
 * @return Bei erfolgreichem platzieren 0, sonst 1 für eine Kollision oder das verlassen des Spielfelds
*/
int platziere_schiff(struct matrix* spielfeld_ptr, const int y1_koordinate, const int x1_koordinate, const int ausrichtung,
                     const int schiff_nummer)
{
	int kollision = 0;
	// Bei gültigen Koordinaten sind x oder y gleich, Schiffe können nicht quer liegen
	int test_x_koordinate = x1_koordinate;
	int test_y_koordinate = y1_koordinate;

	switch (ausrichtung)
	{
	case 1:
		// nach oben platzieren

		// Testen ob das Schiff ohne Kollision platziert werden kann
		while (test_y_koordinate > y1_koordinate - SCHIFF_GROESSE && kollision == 0)
		{
			// Kollision mit einem anderen Schiff oder das verlassen des Spielfelds abfangen
			if (test_y_koordinate < 0 || spielfeld_ptr->elemente[test_y_koordinate][x1_koordinate] != 0)
			{
				kollision = 1;
			}
			test_y_koordinate--;
		}

		//Keine Kollision, Schiff kann platziert werden
		if (kollision == 0)
		{
			for (int cur_index_y = y1_koordinate; cur_index_y > y1_koordinate - SCHIFF_GROESSE; cur_index_y--)
			{
				spielfeld_ptr->elemente[cur_index_y][x1_koordinate] = schiff_nummer;
			}
		}
		break;

	case 2:
		// nach unten platzieren

		// Testen ob das Schiff ohne Kollision platziert werden kann
		while (test_y_koordinate < y1_koordinate + SCHIFF_GROESSE && kollision == 0)
		{
			// Kollision mit einem anderen Schiff oder das verlassen des Spielfelds abfangen
			if (test_y_koordinate >= spielfeld_ptr->anz_zeilen || spielfeld_ptr->elemente[test_y_koordinate][x1_koordinate] != 0
			)
			{
				kollision = 1;
			}
			test_y_koordinate++;
		}

		//Keine Kollision, Schiff kann platziert werden
		if (kollision == 0)
		{
			for (int cur_index_y = y1_koordinate; cur_index_y < y1_koordinate + SCHIFF_GROESSE; cur_index_y++)
			{
				spielfeld_ptr->elemente[cur_index_y][x1_koordinate] = schiff_nummer;
			}
		}
		break;

	case 3:
		// nach links platzieren

		// Kollision mit einem anderen Schiff oder das verlassen des Spielfelds abfangen
		while (test_x_koordinate > x1_koordinate - SCHIFF_GROESSE && kollision == 0)
		{
			if (test_x_koordinate < 0 || spielfeld_ptr->elemente[y1_koordinate][test_x_koordinate] != 0)
			{
				kollision = 1;
			}
			test_x_koordinate--;
		}

		//Keine Kollision, Schiff kann platziert werden
		if (kollision == 0)
		{
			for (int cur_index_x = x1_koordinate; cur_index_x > x1_koordinate - SCHIFF_GROESSE; cur_index_x--)
			{
				spielfeld_ptr->elemente[y1_koordinate][cur_index_x] = schiff_nummer;
			}
		}

		break;

	case 4:
		// nach rechts platzieren

		// Kollision mit einem anderen Schiff oder das verlassen des Spielfelds abfangen
		while (test_x_koordinate < x1_koordinate + SCHIFF_GROESSE && kollision == 0)
		{
			if (test_x_koordinate >= spielfeld_ptr->anz_spalten || spielfeld_ptr->elemente[y1_koordinate][test_x_koordinate] !=
				0)
			{
				kollision = 1;
			}
			test_x_koordinate++;
		}

		//Keine Kollision, Schiff kann platziert werden
		if (kollision == 0)
		{
			for (int cur_index_x = x1_koordinate; cur_index_x < x1_koordinate + SCHIFF_GROESSE; cur_index_x++)
			{
				spielfeld_ptr->elemente[y1_koordinate][cur_index_x] = schiff_nummer;
			}
		}

		break;
	default:
		kollision = 1;
	}


	return kollision;
}


/**
 * @brief Zufällige Platzierung von Schiffen auf dem Spielfeld
 * @param spielfeld_ptr Adresse eines 2D-Felds als Spielfeld
 * @param anz_zeilen Anzahl der Zeilen des Spielfelds
 * @param anz_spalten Anzahl der Spalten des Spielfelds
 * @param anz_schiffe Anzahl der zu platzierenden Schiffe
 * @param start_schiff_nummer Schiffsnummer von der aus die weiteren Schiffe nummeriert werden
 * @return Anzahl der erfolgreich platzierten Schiffe, -1 bei zu vielen Versuchen Schiffe zu platzieren
*/
int schiffe_zufaellig_platzieren(int** spielfeld_ptr, const int anz_zeilen, const int anz_spalten, const int anz_schiffe,
                                 const int start_schiff_nummer)
{
	int schiffe_platziert = 0;
	int schiff_nummerierung = start_schiff_nummer;
	int platzierungs_versuche = 0;
	int kollision;

	// Maximal werden anz_zeilen*anz_spalten Versuche gemacht Schiffe zu platzieren
	while (schiffe_platziert != anz_schiffe && platzierungs_versuche < (anz_zeilen * anz_spalten))
	{
		//Zufaellige Ausrichtung generieren, vertikal/horizontal
		const int ausrichtung = rand() % 2;

		//Vertikale Positionierung
		if (ausrichtung == 0)
		{
			kollision = platziere_zufaellig_vertikal(spielfeld_ptr, anz_zeilen, anz_spalten, schiff_nummerierung);
		}
			//Horizontale Positionierung
		else
		{
			kollision = platziere_zufaellig_horizontal(spielfeld_ptr, anz_zeilen, anz_spalten, schiff_nummerierung);
		}

		//Schiff konnte platziert werden, ansonsten wird erneut versucht zu platzieren
		if (kollision == 0)
		{
			schiffe_platziert++;
			schiff_nummerierung++;
		}
		platzierungs_versuche++;
	}

	return schiffe_platziert;
}

/**
 * @brief Hilfsfunktion zum platzieren eines Schiffs in vertikaler Ausrichtung
 * @param spielfeld_ptr Adresse eines 2D-Felds als Spielfeld
 * @param anz_zeilen Anzahl der Zeilen des Spielfelds
 * @param anz_spalten Anzahl der Spalten des Spielfelds
 * @param schiff_nummer Nummer des zu platzierenden Schiffs
 * @return Rückgabewert für eine eventuell vorhandene Kollision, 0 bei erfolgreichem Platzieren sonst 1
*/
int platziere_zufaellig_vertikal(int** spielfeld_ptr, const int anz_zeilen, const int anz_spalten, const int schiff_nummer)
{
	//Schiffe werden vom Startpunkt aus nach oben platziert, daher y-index --
	const int start_x = rand() % anz_spalten; // Zufällige x-Koordinate
	const int start_y = rand() % anz_zeilen; // Zufällige y-Koordinate
	int kollision = 0; // Für Überprüfung auf Spielfeldrand oder auf anderes Schiff

	// Überprüfung ob das Schiff vom Startpunkt nach oben platziert werden kann
	int test_index_y = start_y;
	while (test_index_y > start_y - SCHIFF_GROESSE && kollision == 0)
	{
		// Wenn ein Index auf dem geplanten Weg nicht 0 ist oder der Index den Spielfeldrand überläuft, liegt eine Kollision vor
		if (test_index_y < 0 || spielfeld_ptr[test_index_y][start_x] != 0)
		{
			kollision = 1;
		}
		// nächsten Index überprüfen
		test_index_y--;
	}

	//Wenn das Schiff nicht in einem Sicherheitsbereich oder Schiff lieft, kann es platziert werden
	if (kollision == 0)
	{
		// Schiff platzieren
		for (int cur_index_y = start_y; cur_index_y > start_y - SCHIFF_GROESSE; cur_index_y--)
		{
			spielfeld_ptr[cur_index_y][start_x] = schiff_nummer;
		}
	}
	return kollision;
}


/**
 * @brief Hilfsfunktion zum platzieren eines Schiffs in horizontaler Ausrichtung
 * @param spielfeld_ptr Adresse eines 2D-Felds als Spielfeld
 * @param anz_zeilen Anzahl der Zeilen des Spielfelds
 * @param anz_spalten Anzahl der Spalten des Spielfelds
 * @param schiff_nummer Nummer des zu platzierenden Schiffs
 * @return Rückgabewert für eine eventuell vorhandene Kollision, 0 bei erfolgreichem Platzieren sonst 1
*/
int platziere_zufaellig_horizontal(int** spielfeld_ptr, const int anz_zeilen, const int anz_spalten,
                                   const int schiff_nummer)
{
	// X/Y-Koordinaten zufällig wählen
	const int start_x = rand() % anz_spalten; // Zufällige x-Koordinate
	const int start_y = rand() % anz_zeilen; // Zufällige y-Koordinate
	int kollision = 0; // // Für Überprüfung auf Spielfeldrand oder auf anderes Schiff

	//Horizontal müssen x-koordinaten auf der Schiffslänge überprüft werden
	int test_index_x = start_x;

	while (test_index_x < start_x + SCHIFF_GROESSE && kollision == 0)
	{
		// Wenn ein Index auf dem geplanten Weg nicht 0 ist oder der Index den Spielfeldrand überläuft, liegt eine Kollision vor
		if (test_index_x >= anz_spalten || spielfeld_ptr[start_y][test_index_x] != 0)
		{
			// Schiff platzieren
			kollision = 1;
		}
		test_index_x++;
	}

	//Wenn das Schiff nicht in einem Sicherheitsbereich oder Schiff lieft, kann es platziert werden
	if (kollision == 0)
	{
		// Schiff platzieren
		for (int cur_index_x = start_x; cur_index_x < start_x + SCHIFF_GROESSE; cur_index_x++)
		{
			spielfeld_ptr[start_y][cur_index_x] = schiff_nummer;
		}
	}

	return kollision;
}

/**
 * @brief Funktion zur Eingabe der Spielregeln durch die Spieler
 * @param anz_zeilen_ptr Adresse für einen int-Wert für die eingelesene Zeilenanzahl
 * @param anz_spalten_ptr Adresse für einen int-Wert für die eingelesene Spaltenanzahl
 * @param anz_schiffe_ptr Adresse für einen int-Wert für die Anzahl der Schiffe der Spieler
*/
void eingabe_spielregeln(int* anz_zeilen_ptr, int* anz_spalten_ptr, int* anz_schiffe_ptr)
{
	int ret_val = 0;

	// Eingabe der Spaltenanzahl
	do
	{
		printf("Spaltenanzahl zwischen %d und %d eingeben: ", MIN_ANZ_SPALTEN, MAX_ANZ_SPALTEN);
		ret_val = get_int_input(anz_spalten_ptr);
		if (ret_val != 1)
		{
			printf("Fehler bei der Eingabe, der Spaltenanzahl.\n");
		}
	}
	while (ret_val != 1 || *anz_spalten_ptr < MIN_ANZ_SPALTEN || *anz_spalten_ptr > MAX_ANZ_SPALTEN);

	// Eingabe der Zeilenanzahl
	do
	{
		printf("Zeilenanzahl zwischen %d und %d eingeben: ", MIN_ANZ_ZEILEN, MAX_ANZ_ZEILEN);
		ret_val = get_int_input(anz_zeilen_ptr);
		if (ret_val != 1)
		{
			printf("Fehler bei der Eingabe der Zeilenanzahl.\n");
		}
	}
	while (ret_val != 1 || *anz_zeilen_ptr < MIN_ANZ_ZEILEN || *anz_zeilen_ptr > MAX_ANZ_ZEILEN);

	// Eingabe der Anzahl von Schiffen
	do
	{
		printf("Anzahl der Schiffe zwischen %d und %d eingeben: ", MIN_ANZ_SCHIFFE, MAX_ANZ_SCHIFFE);
		ret_val = get_int_input(anz_schiffe_ptr);
		if (ret_val != 1)
		{
			printf("Fehler bei der Eingabe der Zeilenanzahl.\n");
		}
	}
	while (ret_val != 1 || *anz_schiffe_ptr < MIN_ANZ_SCHIFFE || *anz_schiffe_ptr > MAX_ANZ_SCHIFFE);
}

/**
 * @brief Funktion für die Eingabe von Koordinaten durch den Spieler
 * @param x_koordinate_ptr Adresse für einen int-Wert für die eingegebene x-Koordinate
 * @param y_koordinate_ptr Adresse für einen int-Wert für die eingegebene y-Koordinate
 * @param anz_zeilen Anzahl der Zeilen des Spielfelds benötigt für die Gültigkeitsprüfung der Eingabe
 * @param anz_spalten Anzahl der Spalten des Spielfelds benötigt für die Gültigkeitsprüfung der Eingabe
*/
void get_valid_cords(int* x_koordinate_ptr, int* y_koordinate_ptr, const int anz_zeilen, const int anz_spalten)
{
	// Variablen für eingelesene Koordinaten
	int x_input = 0;
	int y_input = 0;

	int ret_val = 0; // für Überprüfung ob eine gültige Eingabe gemacht wurde

	// Eingabe eines gültigen Werts für X-Koordinate
	do
	{
		printf("X-Koordinate eingeben: ");
		ret_val = get_int_input(&x_input);
		if (ret_val != 1)
		{
			printf("Fehler bei der Eingabe, erneut eine X-Koordinate eingeben.\n");
		}
	}
	while (ret_val != 1 || x_input < 1 || x_input > anz_spalten);
	printf("\n");

	// Eingabe eines gültigen Werts für Y-Koordinate
	do
	{
		printf("Y-Koordinate eingeben: ");
		ret_val = get_int_input(&y_input);
		if (ret_val != 1)
		{
			printf("Fehler bei der Eingabe, erneut eine Y-Koordinate eingeben.\n");
		}
	}
	while (ret_val != 1 || y_input < 1 || y_input > anz_zeilen);

	// eingegebene Koordinaten haben einen Offset von 1 (Darstellung für den Spieler)
	*x_koordinate_ptr = x_input - 1;
	*y_koordinate_ptr = y_input - 1;
}

/**
 * @brief Zählt noch zu treffende Schiffsteile auf dem Spielfeld
 * @param spielfeld_ptr Adresse eines 2D-Felds als Spielfeld
 * @param anz_zeilen Anzahl der Zeilen des 2D-Felds
 * @param anz_spalten Anzahl der Spalten des 2D-Felds
 * @param anz_schiffe Anzahl der Schiffe auf dem Spielfeld
 * @return Anzahl der noch benötigten Treffer die für einen Sieg benötigt werden
*/
int count_not_hit_ship_parts(int** spielfeld_ptr, const int anz_zeilen, const int anz_spalten, const int anz_schiffe)
{
	int anz_nicht_getroffene_schiffsteile = 0;

	// Jeden Index auf einen positiven Wert überprüfen
	for (int index_zeilen = 0; index_zeilen < anz_zeilen; index_zeilen++)
	{
		for (int index_spalten = 0; index_spalten < anz_spalten; index_spalten++)
		{
			// Ein Schiffsteil ist nicht getroffen wenn der Wert positiv ist
			if (spielfeld_ptr[index_zeilen][index_spalten] > 0 && spielfeld_ptr[index_zeilen][index_spalten] <= anz_schiffe)
			{
				anz_nicht_getroffene_schiffsteile++;
			}
		}
	}
	return anz_nicht_getroffene_schiffsteile;
}

/**
 * @brief Überprüft und setzt ob an den übergebenen Koordinaten etwas getroffen wurde
 * @param spielfeld_ptr Adresse des Spielfelds auf das geschossen wurde
 * @param x_koordinate X-Koordinate des Tipps
 * @param y_koordinate Y-Koordinate des Tipps
 * @param anz_schiffe Anzahl der Schiffe auf dem Spielfeld
 * @return Bei einem Treffer 1, bei keinem Treffer 0 und bei wiederholtem Tipp -1
*/
int check_and_set_getroffen(int** spielfeld_ptr, const int y_koordinate, const int x_koordinate, const int anz_schiffe)
{
	int treffer = 0;

	// Ein Treffer liegt vor wenn an einem Index eine positive Zahl vorliegt
	if (spielfeld_ptr[y_koordinate][x_koordinate] <= anz_schiffe && spielfeld_ptr[y_koordinate][x_koordinate] > 0)
	{
		// Treffer im Spielfeld markieren
		treffer = 1;
		spielfeld_ptr[y_koordinate][x_koordinate] *= -1;
	}
	else
	{
		// Auf bereits ausgeführten Tipp überprüfen
		if (spielfeld_ptr[y_koordinate][x_koordinate] == 0)
		{
			// Verfehlt, Schuss ist im Wasser gelandet
			spielfeld_ptr[y_koordinate][x_koordinate] = -333;
		}
		else
		{
			// Bereits versuchter Tipp
			treffer = -1;
		}
	}

	return treffer;
}

/**
 * @brief Überprüft ob ein Schiff versenkt wurde, dies ist der Fall  wenn alle Teile des Schiffs eine negative Schiffsnummer aufweisen
 * @param spielfeld_ptr Adresse des Spielfelds
 * @param anz_zeilen Anzahl  der Zeilen des Spielfelds
 * @param anz_spalten Anzahl der Spalten des Spielfelds
 * @param anz_schiffe Anzahl der Schiffe auf dem Spielfeld
*/
void check_versenkt(int** spielfeld_ptr, const int anz_zeilen, const int anz_spalten, const int anz_schiffe)
{
	int anz_treffer = 0;
	// Iteration über alle Schiffe die auf dem Spielfeld liegen
	for (int schiff_nummer = 1; schiff_nummer <= anz_schiffe; schiff_nummer++)
	{
		// Jeder Index des Spielfelds wird auf einen negativen Wert für die Schiffsnummer überprüft
		for (int index_zeilen = 0; index_zeilen < anz_zeilen; index_zeilen++)
		{
			for (int index_spalten = 0; index_spalten < anz_spalten; index_spalten++)
			{
				// Wert des Spielfelds am Index auslesen und mit der negativen Schiffsnummer vergleichen
				const int value = spielfeld_ptr[index_zeilen][index_spalten];
				if ((schiff_nummer * -1) == value)
				{
					// Wenn ein Treffer vorhanden ist die Anzahl der Treffer für das Schiff erhöhen
					anz_treffer++;
				}
			}
		}

		// Wenn so viele Treffer für ein Schiff gezählt wurden wie die Schiffe groß sind, ist es versenkt worden
		if (anz_treffer == SCHIFF_GROESSE)
		{
			// Schiff auf versenkt setzen
			schiff_versenkt(spielfeld_ptr, anz_zeilen, anz_spalten, schiff_nummer);
		}

		// Zurücksetzen des Zählers für Treffer für das nächste Schiff
		anz_treffer = 0;
	}
}

/**
 * @brief Setzt alle Werte eines Schiffs auf den Status "versenkt" also -1000
 * @param spielfeld_ptr Adresse des Spielfelds
 * @param anz_zeilen Anzahl der Zeilen des Spielfelds
 * @param anz_spalten Anzahl der Spalten des Spielfelds
 * @param schiff_nummer Nummer des Schiffs, das als versenkt gesetzt werden soll.
*/
void schiff_versenkt(int** spielfeld_ptr, const int anz_zeilen, const int anz_spalten, const int schiff_nummer)
{
	//Jede koordinate wird auf die position des Schiffs geprüft
	for (int index_zeilen = 0; index_zeilen < anz_zeilen; index_zeilen++)
	{
		for (int index_spalten = 0; index_spalten < anz_spalten; index_spalten++)
		{
			//Wenn ein Index die Schiffsnummer als negative Zahl aufweist wird dieser Index auf -1000 gesetzt (versenkt)
			if (spielfeld_ptr[index_zeilen][index_spalten] == (schiff_nummer * (-1)))
			{
				spielfeld_ptr[index_zeilen][index_spalten] = -1000;
			}
		}
	}
}
