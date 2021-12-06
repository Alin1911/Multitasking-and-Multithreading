bug1:
se folosea metoda run care executa secventiala, am inlocuit-o cu start
care face executia in paralel 

bug2:
am creat o sectiune critica in care avem inca o sectiune critica, ma gandesc ca
facem asta deoarece run reprezinta o executie seriala.

bug3:
a si b trebuie sa fie diferite din cauza lui synchronized care ar putea bloca ambele
for-uri, verificarea din synchronized 

bug4:
am schimbat tipul variabilei keepRunning in volatile deoarece valoarea lui keepRunning
ramanea pe true si nu se iesea din while, acum valoarea lui keepRunning se va schimba
in false si totul va fi ok.

bug5:
am modificat in MyThreadA din lockA in synchronized (Main.lockB) deoarece ar trebui
codul sa porneasca la fel pe ambele threduri, deoarece daca intra un thred in lookA
si unul in lookB se asteptau amandoi se termine astfel nici unul nu ar fi trecut mai
departe

bug6