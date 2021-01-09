Schiffe versenken:

	Der Spielfeldrand hat je nachdem welcher Spieler am Zug ist eine andere Farbe:
		1. Spieler hat die Farbe: grün
		2. Spieler hat die Farbe: rot

	1. Nach dem Starten des Spiels werden die Spielregeln abgefragt
		- Spaltenanzahl von 10 bis 26
		- Zeilenanzahl von 10 bis 20
		- Anzahl der Schiffe von 1 bis 9
		- Schiffe haben immer eine Länge von 5
	2. Jeder der beiden Spieler setzt die eigenen Schiffe zuerst Spieler 2 dann Spieler 1
		- platzierte Schiffe sind durch eine Nummerierung gekennzeichnet
		- mit 'z' kann ein Schiff zufällig auf dem Spielfeld platziert werden
		- mit den Pfeiltasten kann die Startposition eines Schiffs angesteuert werden
			- mit Enter kann das Platzieren begonnen werden
			- mit den Pfeiltasten kann dann die Ausrichtung des Schiffs angegeben werden
			- ungültige Platzierungen sorgen für eine erneute Aufforderung ein Schiff zu platzieren
	3. Abwechselnd werden jetzt Tipps darüber abgegeben wo sich die Schiffe des Gegners befinden Spieler 1 beginnt
		- mit den Pfeiltasten wird das Spielfeld navigiert
		- mit Enter wird für die derzeitig ausgewählte Position der Tipp abgegeben
		- Wasser ist durch '~'  dargestellt, ein getroffenes Schiffsteil 'T', ein versenktes Schiff 'V'
		- Durch drücken von 'c' kann geschummelt werden, dabei werden die Schiffe sichtbar gemacht
	4. Wenn alle Schiffe eines Gegners versenkt wurden endet das Spiel
		- der Gewinner wird auf der Konsole mit der benötigten Anzahl von Tipps ausgegeben
	
Installation (WPU):

	Projekt herunterladen
	Projekt entpacken z.B. mit "unzip 2020-01-09_C-Mini-Projekt-main.zip"
	
	Falls gewünscht kann das Projekt mit dem Script "install_and_run_project.sh" automatisch installiert werden
	dabei werden die benötigen Pakete installiert, das Projekt erstellt und ausgeführt
		1. Mit einem Terminal in das Projektverzeichnis wechseln
		2. Das Script mit "sudo chmod +x install_and_run_project.sh" ausführbar machen
		3. Starten des Scripts durch ausführen von "sudo ./install_and_run_project.sh"
		4. Das Script sollte alle benötigten Packages installieren, das Projekt erstellen und dann ausführen
	
	Für beide Möglichkeiten sind die folgenen Pakete erforderlich
	Benötigte Packages installieren
		1. "sudo apt-get install make"
		2. "sudo apt-get install cmake"
		3. "sudo apt-get install libncurses-dev"


	Entweder mit der Bash a) oder mit Atom b) Compilieren und Ausführen
	a) Compilieren und Ausführen in der Bash:
		1. In das Projektverzeichnis wechseln "cd C-Mini-Projekt-main"
		Im Projektverzeichnis die Befehle
			1. "cmake CMakeLists.txt"
			2. "make"
		ausführen.

		Falls nötig den Befehl "chmod +x schiffe_versenken" ausführen um die die Datei ausführbar zu machen.

		Das Spiel mit "./schiffe_versenken" starten

	b) Compilieren und Ausführen mit Atom:
		1. gpp-compiler installieren "https://atom.io/packages/gpp-compiler"
		2. build installieren "https://github.com/noseglid/atom-build" mit dependency busy-signal "https://atom.io/packages/busy-signal"
		3. build-cmake installieren "https://atom.io/packages/build-cmake"
		4. Atom neu starten
		5. Projekt Ordner mit Atom öffnen
		6. Mit F7 die Liste der Targets aufrufen falls vorhanden Debug:generate auswählen
		7. Mit F7 jetzt Debug:all auswählen um alle Datein zu erstellen
		8. Wenn mit F7 nichts angezeigt wird ggf. Atom neu starten und die Schritte 6 und 7 wiederholen
		9. Das Spiel kann dann über ein Terminal gestartet werden durch
			1. Mit dem Terminal in den Projektordner wechseln z.B. "cd C-Mini-Projekt-main"
			2. In den Ordner mit der auszuführenden Datei wechseln "cd build/Debug/"
			3. Falls nötig "chmod +x schiffe_versenken" ausführen um die Datei ausfürbar zu machen
			4. Das Spiel durch ausführen von "./schiffe_versenken" starten
			
Projektstruktur: 
	main.c: Beeinhaltet die main-Funktion und ist der Einstiegspunkt
	schiffe_versenken.h/schiffe_versenken.c : Ist für die Eingabe der Spielregeln und dem Spielablauf zuständig
	oberflaeche.h/oberflaeche.c : Stellt Funktionen zur interaktiven Darstellung des Spielfelds mit ncurses bereit
	spieler.h : Struktur um Informationen über einen SPieler zu speichern
	felder_2d.h/felder_2d.c : Sammlung Funktionen für 2D-Felder für z.B. Speicher reservieren, alle Werte setzen, Speicher freigeben
	input_int.h/input_int.c : Sammlung von Funktionen für die Eingabe von int-Werten auf der Konsole z.B. für Gültigkeitsprüfung einer Eingabe



