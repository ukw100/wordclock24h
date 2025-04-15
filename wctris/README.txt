wctris - Speieleconsole für WordClock24
=======================================

wctris läuft unter Linux, Unix und Windows (Eingabeaufforderung).

Aufruf:

  wctris IP [wctris|snake]

Beispiele:

  Tetris spielen:

    wctris 192.168.10.35 wctris

  Snake spielen:

    wctris 192.168.10.35 snake

Wird der Name des Spiels weggelassen, wird Tetris gestartet.

Für Windows ist eine EXE in der ZIP-Datei enthalten. AVIRA kann aber aufgrund
von Cloud-Heuristiken fälschlicherweise Malware erkennen.

Abhilfe: Entweder Cloud-Heuristik in den Einstellungen abschalten oder eine
Ausnahme für wctris.exe oder für den Ordner, in dem sich das Executable befindet,
einstellen.

Das Programm kann man auch selbst übersetzen:

Linux und Unix: cc -O wctris.exe -o wctris
Windos MINGW:   cc -O wctris.exe -o wctris

Für Visual Studio muss man ein eigenes Konsoleprogramm-Projekt erstellen und lediglich
wctris.c dem Projekt hinzufügen. In den Eigenschaften des Projektes muss dann noch
das Verwenden von vorkompilierten Header-Dateien abschalten. stdafx.h kann aus dem
Projekt entfernt werden.

Viel Spaß!
