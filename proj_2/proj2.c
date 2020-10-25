/**
 * IZP Druhý projekt
 * Autor: Pavel Bobčík, xbobci03@stud.fit.vutbr.cz
 * Datum: 23.11.2019
 */
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define I0 1e-12 //Závěrečný saturační proud
#define UT 25.8563e-3 //Tepelné napětí
#define UP_FIRST 0.0 //Počáteční hodnota min intervalu
#define EPS_MIN 1e-15 //Minimální možná hodnota epsilon

int checkArg(int argc, char *argv[]);
double calculateIP(double uP);
double diode(double u0, double r, double eps);

/**
 * @parametr arcg - počet argumentů na vstupu
 * @parametr *argv[] - řetězec znaků reprezentující argumenty ze vstupu
 * Volá funkci na kontrolu argumentů.
 * Pokud byl vstup správný, tak se inicializují proměnné a zavolají funkce.
 * V posledním kroku vypíšeme obdržené hodnoty.
 */
int main(int argc, char *argv[]){
    if(checkArg(argc, argv) == 1) return 1;
    
    char *txt; //Řetězec znaků pro uložení textové části u strtod.
    double iP;
    double uP;
    double u0 = strtold(argv[1], &txt);
    double r = strtold(argv[2], &txt);
    double eps = strtold(argv[3], &txt);
    
    if(eps<EPS_MIN) eps = EPS_MIN;
    
    uP = diode(u0, r, eps);
    iP = calculateIP(uP);
    
    fprintf(stdout, "Up= %g V\nIp= %g A\n", uP, iP);
    
    return 0;
}

/**
 * @parametr arcg - počet argumentů na vstupu
 * @parametr *argv[] - řetězec znaků reprezentující argumenty ze vstupu
 * Podle podmínek kontroluje argumenty na vstupu.
 */
int checkArg(int argc, char *argv[]){
    char *txt; //Řetězec znaků pro uložení textové části u strtod.
    if(argc != 4){
        fprintf(stderr, "Chybový vstup - špatný počet argumentů\n");
        return 1;
    }else if(strtod(argv[1], &txt) < 0 || txt == argv[1]){
        fprintf(stderr, "Chybový vstup - špatná hodnota U0\n");
        return 1;
    }else if(strtod(argv[2], &txt) <= 0){
        fprintf(stderr, "Chybový vstup - špatná hodnota R\n");
        return 1;
    }else if(strtod(argv[3], &txt) <= 0 || isinf(strtod(argv[3], &txt)) == 1){
        fprintf(stderr, "Chybový vstup - špatná hodnota EPS\n");
        return 1;
    }else{
        return 0;
    }
}

/**
 * @parametr uP - hodnota napětí uP, jenž získáme půletím intervalu
 * Funkce pro výpočet Ip. Hodnoty přebírá z parametru funkce a konstant.
 * Návratová hodnota double vrací výsledek.
 * Funkce byla vytvořena kvůli opakování vzorce, případná změna se projeví na všech místech,
 * kde ji voláme.
 */
double calculateIP(double uP){
    return I0*(exp(uP/UT)-1);
}

/**
 * @parametr u0 - počáteční hodnota napětí, obdržená na vstupu
 * @parametr r - odpor rezistoru, obdržen na vstupu
 * @parametr eps - epsilon přesnosti výsledku, obdržen na vstupu
 * Funkce diode provádí vyhledávání hodnoty uP pomocí půlení intervalu.
 * Pracuje s proměnnými min a max, které jsou na začátku reprezentovány 0 a u0.
 * Hodnoty min a max se v jednotlivých cyklech mění podle podmínky.
 * Cyklus se opakuje do doby, dokud hodnota rozdílu max a min nedosáhne velikosti menší,
 * než je zvolený epsilon.
 * Funkce vrací hodnotu uP.
 */
double diode(double u0, double r, double eps){
    double uP = UP_FIRST;
    double min = uP;
    double max = u0;
    double iP;
    double iR;
    do{
        uP = (min + max)/2;
        iP = calculateIP(uP);
        iR = (u0-uP)/r;
        if((iP - iR) > 0){
            max = uP;
        }else{
            min = uP;
        }
    }while(fabs(min - max) > eps);
    
    return uP;
}
