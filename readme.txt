Konsolenaufrufe:

----
make
----
Erstellt ausführbare Datei "/bin/client".

-----------------------------
./bin/client -g 1234567890123
-----------------------------
Führt das Programm aus, mit Game-Id als Kommandozeilenparameter

---------
make play
---------
Wie im Meilenstein gefordert: Kompiliert das Projekt und führt es mit der Standard Game ID und Player-Nr. aus.

------------------------------------------------------------
valgrind ./bin/client --leak-check=full --trace-children=yes
------------------------------------------------------------
Fehlersuche mit valgrind (valgrind muss installiert sein!)
