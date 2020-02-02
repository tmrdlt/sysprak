System-Praktikum WS 16/17
-------------------------
Client für den MNN Gameserver
-----------------------------

_Version: 1.0_
_Berherschte Spiele: Bashni_

Dieser Client wurde im Wintersemeter 16/17 im Rahmen des System-Praktikums an der LMU von Gruppe 16 erstellt.
Autoren waren:
Elena Terzieva, Stephan Holzner und Timo Erdelt


Nutzung
-------

    make play GAME_ID=<game_id> PLAYER=<player_number> CONFIG=<config_filename>
          -> Wie im Meilenstein gefordert:
             Kompiliert das Projekt und führt es direkt mit gesetzten Variablen GAME_ID, PLAYER und CONFIG aus.
             und führt es mit diesen als Kommandozeilenparameter aus.
             Bei nur make play wird die im Makefile gesetzte GAME_ID genommen.
             >Note: PLAYER und CONFIG sind hierbei optional.

    make
          -> Erstellt ausführbare Datei "/bin/client"

    ./bin/client -g <game_id> -p<player_number> -f<config_filename>
          -> Führt das Programm aus.
             >Note: game_id muss angegeben werden, der Rest ist optional

    valgrind --leak-check=full --trace-children=yes ./bin/client -g <game_id>
          -> Fehlersuche mit valgrind
             >Note: valgrind muss hierfür installiert sein

    make clean
          -> Entfernt alle erstellten object und ausführbaren Dateien
