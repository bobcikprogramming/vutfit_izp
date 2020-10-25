/**
 * IZP První projekt
 * Autor: Pavel Bobčík, xbobci03@stud.fit.vutbr.cz
 * Datum: 23.10.2019
 */

#include<stdio.h>
#include<string.h>
#include<stdbool.h>

void numToChar(char *chars[], char number);
void getContact(int argc, char *argv[]);
bool findContactByName(char name[], char argv[]);
bool findContactByNumber(char number[], char argv[]);
int getPosOfChr(char arrOfChrs[], char srch);
bool checkCharsOfArgv(char argv[]);

int main(int argc, char *argv[]){
	
    /**
     * Podmínka pro kontrolu množství argumentů na vstupu a pro kontrolu znaků.
     * V případě většího množství než 2 (spuštěč, hledané číslo) vyhodí program chybovou hlášku.
     * Pokud máme jiné znaky než čísla nebo +, vyhodí program chybovou hlášku.
     */
    if(argc > 2){
        fprintf(stderr, "Příliš mnoho argumentů!\n");
        return 1;
    }else if(argc > 1 && !checkCharsOfArgv(argv[1])){
        fprintf(stderr, "Nesprávný argument! Zadejte pouze čísla, prosím.\n");
        return 1;
    }else{
        getContact(argc, argv);
    }
	
	return 0;
}

/**
 * @parametr argc - počet argumentů
 * @parametr argv[] - řetězec znaků jenž dostane z argumentu
 * Pomocí fgets v cyklu načteme řádky ze vstupu. Zjistíme pozici konce řádku, kterou nahradíme znakem konce textu,
 * abychom mohli umístnin druhý řádek za první.
 * Pro druhý řádek máme pomocí podmínky stanoveno, nepodaří-li se jej načíst, že se nevypíše poslední kontakt (jméno bez čísla).
 * Pokud bude podmínka splněna, zkontroluje se shoda čísel, nebude-li nalezena, zavolá se funkce pro kontrolu znaků.
 * V případě schody vypíšeme daný kontakt.
 * Poslední podmínka zkontroluje, zda-li byl nalezen nějaký kontakt. Pokud ne, vypíše Not found.
 */
void getContact(int argc, char *argv[]){
    char name[102];
    char number[102];
    bool cntFndByNum = false; //TRUE - pokud byl kontakt nalezen podle čísla
    bool cntFndByChr = false; //TRUE - pokud byl kontakt nalezan podle znaků
    
    while(fgets(name, 102, stdin) != NULL){
        name[getPosOfChr(name, '\n')] = '\0';
        
        if(fgets(number, 102, stdin) != NULL){
            if(argc == 1){
                fprintf(stdout, "%s, %s", name, number);
            }else if(findContactByNumber(number, argv[1]) == true){
                fprintf(stdout, "%s, %s", name, number);
                cntFndByNum = true;
            }else if(findContactByName(name, argv[1]) == true){
                fprintf(stdout, "%s, %s", name, number);
                cntFndByChr = true;
            }
        }
    }
    
    if(!cntFndByNum && !cntFndByChr && argc == 2){
        fprintf(stdout, "Not found\n");
    }
}

/**
 * @parametr arrOfChars[] - pole, ve kterém budeme hledat
 * @parametr srch - hledaný znak
 * Funkce vrací pozici hledaného znaku, který obdrží na vstupu společně s polem znaků.
 */
int getPosOfChr(char arrOfChrs[], char srch){
    int posOfLastChr = 0;
    while(arrOfChrs[posOfLastChr] != srch){
        posOfLastChr++;
    }
    return posOfLastChr;
}

/**
 * @parametr argv[] - pole znaků jenž obsahuje argument ze vstupu
 * Funkce kontroluje argument hledaného výrazu, jestli obsahuje pouze čísla od 0 po 9 nebo "+".
 */
bool checkCharsOfArgv(char argv[]){
    int sizeOfArgv = getPosOfChr(argv, '\0');
    bool onlyNum = true;
    
    for(int i = 0; i < sizeOfArgv; i++){
        if((argv[i] != 43 && argv[i] < 48) || argv[i] > 57){
            onlyNum = false;
        }
    }
    
    return onlyNum;
}


/**
 * @parametr *chars[] - ukazatel na pole znaků, kam chceme zapsat znaky reprezentované čísly
 * @parametr number - znak (číslo/+), jenž chceme nahradit reprezentovanými znaky
 * Funkce dostane odkaz na pole a hledaný znak. Podle podmínky přiřadí znaky do odkazovaného řetězce.
 */
void numToChar(char *chars[], char number){
    switch (number) {
        case '2':
            *chars = "abcABC";
            break;
        case '3':
            *chars = "defDEF";
            break;
        case '4':
            *chars = "ghiGHI";
            break;
        case '5':
            *chars = "jklJKL";
            break;
        case '6':
            *chars = "mnoMNO";
            break;
        case '7':
            *chars = "pqrsPQRS";
            break;
        case '8':
            *chars = "tuvTUV";
            break;
        case '9':
            *chars = "wxyzWXYZ";
            break;
        case '0':
            *chars = "+";
            break;
        default:
            *chars = &number;
            break;
    }
}

/**
 * @parametr name[] - pole znaků, které předává načtené jméno
 * @parametr argv[] - pole znaků pro hledaný argument
 * Funkce s logickou návratovou hodnotou, jenž vrací výsledek, zda-li byl kontakt nalezen podle znaků.
 * První forcyklus prochází jednotlivé znaky hledenáho argumentu.
 * Druhý forcyklus prochází znaky daného jména s tím, že při dalším cyklu začíná od místa, kde minulý cyklus skončil.
 * Podmínka v druhém cyklu porovnává právě hledaný znak jména s polem znaků, reprezentující dané číslo.
 * Při shodě se zvýší hodnota posCmp.
 * Poslední podmínka zkontroluje, zdali se posCmp (počet pozitivních shod) rovná počtu znaků argumentu.
 */
bool findContactByName(char name[], char argv[]){
    int posCmp = 0; //pozitivní shoda
    int size = 9;
    int namePosOfChar = 0; //Pozice posledního porovnávaného znaku jména.
    for(int i = 0; argv[i] != '\0'; i++){
        char cur_num = argv[i];
        char *chars[size];
        numToChar(chars, cur_num); //Zapíše znaky reprezentované číslem do chars.
        
        for(int y = namePosOfChar; name[y] != '\0'; y++){
            namePosOfChar++;
            if(strchr(*chars, name[y])){
                posCmp++;
                break;
            }
        }
        /**
         * Podmínka ukončí cyklus, pokud jsme nenašli hledaný znak a jsme již na konci jména.
         */
        if(name[namePosOfChar] == '\0'){
            break;
        }
    }
    
    /**
     * Pokud se bude rovnat počet shod velkosti hledaného argumentu, kontakt byl nalezen a vrátím true.
     */
    if(posCmp == getPosOfChr(argv, '\0')){
        return true;
    }else{
        return false;
    }
}

/**
 * @parametr number[] - pole znaků, které předává načtené číslo
 * @parametr argv[] - pole znaků pro hledaný argument
 * Funkce funguje na obdobném principu jako předešlá.
 * Rozdíl tvoří fakt, že neprocházíme jméno, ale telefoní číslo,
 * nemusíme tedy volat funkci pro převedení číslic na znaky.
 */
bool findContactByNumber(char number[], char argv[]){
    int posCmp = 0;
    
    int posInNum = 0;
    for(int i = 0; argv[i] != '\0'; i++){
        char cur_num = argv[i];
        
        for(int y = posInNum; number[y] != '\n'; y++){
            posInNum++;
            if(strchr(&cur_num, number[y])){
                posCmp++;
                break;
            }
        }
        
        if(number[posInNum] == '\0'){
            break;
        }
    }
    
    if(posCmp == getPosOfChr(argv, '\0')){
        return true;
    }else{
        return false;
    }
}
