Konsolenaufrufe:

----
make
----
Erstellt ausführbare Datei "/bin/client".

-----------------------------
./bin/client -g 123gameid1234
-----------------------------
Führt das Programm aus, mit Game-Id als Kommandozeilenparameter

----------------------------------------------------------
make play GAMEID=123gameid1234 PLAYER=0
make play GAMEID=123gameid1234 PLAYER=0 CONFIG=client.conf
----------------------------------------------------------
Wie im Meilenstein gefordert:
Kompiliert das Projekt, setzt die Variablen GAMEID, PLAYER (und optional CONFIG)
und führt es mit diesen als Kommandozeilenparameter aus.

-----------------------------------------------------------------------------
valgrind --leak-check=full --trace-children=yes ./bin/client -g 123gameid1234
-----------------------------------------------------------------------------
Fehlersuche mit valgrind (valgrind muss installiert sein!)
