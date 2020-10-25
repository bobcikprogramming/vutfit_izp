## Projekt 1: 5/5 bodů + 1/1 bonusových bodů</br>
## Projekt 2: 8/8 bodů</br>
## Projekt 3: 8,25/10 bodů</br>
      ====== Prehled hodnoceni ==========
1: dekompozicia
1.25: parametre a dlzka fcii
1.5: citatelny kod
-0.15: magicke konstanty - ASCII
0.25: identifikatory
1: algoritmus prechodu
0.5:ok: test validniho bludiste
0.5:ok: test spatneho bludiste
0.5:ok: test lpath na malem bludisti
0.5:ok: test lpath na vetsim bludisti
0.1:ok: test lpath na jedinem ohranicenem policku
0.5:ok: test rpath na malem bludisti
0.5:ok: test rpath na vetsim bludisti
0.1:ok: test rpath na jedinem ohranicenem policku
0.05:ok: reakce na napovedu
0.05:ok: reakce na stderr kvuli zadnym argumentum
0.05:ok: reakce na stderr na chybejici jmeno souboru (lpath)
0.05:ok: reakce na stderr na chybejici jmeno souboru (rpath)
0: segmentation fault
0:chyba: reakce na stderr na nepovedeny fopen
0:chyba: reakce na stderr na nepovedenou alokaci
0: pametove chyby

====== Log z prekladu =============
Ok

====== Analyza kodu ===============
Ok

====== Log z jednotlivych testu ===
0.5:ok: test validniho bludiste
Chyba při načítání obsahu bludiště.
Neplatná velikost matice - zadejte pouze čísla
Neplatná velikost bludiště. Prosím vložte velikost: řádky(mezera)sloupce
0.5:ok: test spatneho bludiste
0.5:ok: test lpath na malem bludisti
0.5:ok: test lpath na vetsim bludisti
0.1:ok: test lpath na jedinem ohranicenem policku
0.5:ok: test rpath na malem bludisti
0.5:ok: test rpath na vetsim bludisti
0.1:ok: test rpath na jedinem ohranicenem policku
0.05:ok: reakce na napovedu
 
NÁPOVĚDA PROJEKTU 3 - BLUDIŠTĚ

Soubor bludiště:
- Při každém zapnutí nahráváme soubor s bludištěm.
- O validní soubor se jedná pouze tehdy, obsahuje-li první řádek
počet řádků (mezera) počet sloupců.
- Následující řádky obsahují čísla, od 0-7, reprezentující stěny bludiště,
jenž jsou odděleny mezerou.
- Tyto čísla musí být obsažena ve stejném počtu řádků a sloupců,
jako jsme určili v prvním řádku.
- Řádky * sloupce musí odpovídat množství čísel tvořící bludiště.
- Bludiště mohou tvořit pouze čísla od 0 po 7.
 
Způsoby zapnutí:
- ./proj3 --help - otevření nápovědy 
- ./proj3 --test 'nazev_souboru' - ověření validity bludiště 
- ./proj3 --rpath 'cislo_radku' 'cislo_sloupce' 'nazev_souboru'
- hledání cesty podle pravidla pravé ruky z pozice R x C 
- ./proj3 --lpath 'cislo_radku' 'cislo_sloupce' 'nazev_souboru'
- hledání cesty podle pravidla levé ruky z pozice R x C 

0.05:ok: reakce na stderr kvuli zadnym argumentum
Nesprávný počet argumentů, pro více informací: ./proj3 --help
0.05:ok: reakce na stderr na chybejici jmeno souboru (lpath)
Nesprávný počet argumentů, pro více informací: ./proj3 --help
0.05:ok: reakce na stderr na chybejici jmeno souboru (rpath)
Nesprávný počet argumentů, pro více informací: ./proj3 --help
0: segmentation fault
0:chyba: reakce na stderr na nepovedeny fopen
Chyba při otvírání souboru.
timeout: the monitored command dumped core
../3-test-inc: line 58: 29225 Segmentation fault timeout $waitfor "$@" > stdout.tmp
0:chyba: reakce na stderr na nepovedenou alokaci
Neplatná velikost bludiště. Prosím vložte velikost: řádky(mezera)sloupce
free(): invalid pointer
timeout: the monitored command dumped core
../3-test-inc: line 58: 29243 Aborted timeout $waitfor "$@" > stdout.tmp
0: pametove chyby
testerr.valgrind:==29226== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
