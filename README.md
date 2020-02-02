# Systempraktikum WS 16/17

## Client für den MNN Gameserver
_Version: 1.0_
_Berherschte Spiele: Bashni_

Dieser Client wurde im Wintersemeter 16/17 im Rahmen des System-Praktikums an der LMU von Gruppe 16 erstellt.

Autoren waren:
Timo Erdelt
Stephan Holzner
Elena Terzieva

## Nutzung
- Wie im Meilenstein gefordert: Kompiliert das Projekt und führt es direkt mit gesetzten Variablen `GAME_ID`, `PLAYER` und `CONFIG` als Kommandozeilenparameter aus. `PLAYER` und `CONFIG` sind hierbei optional. Bei nur `make play` wird die im Makefile gesetzte GAME_ID genommen.
```
make play GAME_ID=<game_id> PLAYER=<player_number> CONFIG=<config_filename>
```

- Erstellt ausführbare Datei `/bin/client`
```
make
```

- Führt das Programm aus. `game_id` muss angegeben werden, der Rest ist optional

```
./bin/client -g <game_id> -p<player_number> -f<config_filename>
```

- Fehlersuche mit valgrind. valgrind muss hierfür installiert sein.
```
valgrind --leak-check=full --trace-children=yes ./bin/client -g <game_id>
```

- Entfernt alle erstellten object und ausführbaren Dateien
```
make clean
```
