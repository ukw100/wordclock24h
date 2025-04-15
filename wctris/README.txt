wctris - Speieleconsole f�r WordClock24
=======================================

wctris l�uft unter Linux, Unix und Windows (Eingabeaufforderung).

Aufruf:

  wctris IP [wctris|snake]

Beispiele:

  Tetris spielen:

    wctris 192.168.10.35 wctris

  Snake spielen:

    wctris 192.168.10.35 snake

Wird der Name des Spiels weggelassen, wird Tetris gestartet.

F�r Windows ist eine EXE in der ZIP-Datei enthalten. AVIRA kann aber aufgrund
von Cloud-Heuristiken f�lschlicherweise Malware erkennen.

Abhilfe: Entweder Cloud-Heuristik in den Einstellungen abschalten oder eine
Ausnahme f�r wctris.exe oder f�r den Ordner, in dem sich das Executable befindet,
einstellen.

Das Programm kann man auch selbst �bersetzen:

Linux und Unix: cc -O wctris.exe -o wctris
Windos MINGW:   cc -O wctris.exe -o wctris

F�r Visual Studio muss man ein eigenes Konsoleprogramm-Projekt erstellen und lediglich
wctris.c dem Projekt hinzuf�gen. In den Eigenschaften des Projektes muss dann noch
das Verwenden von vorkompilierten Header-Dateien abschalten. stdafx.h kann aus dem
Projekt entfernt werden.

Viel Spa�!
