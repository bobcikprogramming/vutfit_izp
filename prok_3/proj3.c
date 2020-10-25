/**
 * IZP Třetí projekt
 * Autor: Pavel Bobčík, xbobci03@stud.fit.vutbr.cz
 * Datum: 09.12.2019
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define SPACE 32 //ASCII kód čísla
#define COORDINATION_R_C 2 //Povolený počet čísel určujícíh rozměr (X / Y)
#define BINARY_ARRAY_SIZE 3 //Velikost pole pro uložení binární hodnoty
#define LEFT_BORDER 2 //Identifikátor levé strany
#define RIGHT_BORDER 1 //Identifikátor pravé strany
#define VERT_BORDER 0 //Identifikátor vertikální strany
#define LEFT_HAND 0 //Identifikátor levé ruky
#define RIGHT_HAND 1 //Identifikátor pravé ruky
#define ARG_TEST "--test" //Identifikátor vstupního argumentu pro --test
#define ARG_RIGHT "--rpath" //Identifikátor vstupního argumentu pro --rpath
#define ARG_LEFT "--lpath" //Identifikátor vstupního argumentu pro --lpath
#define ARG_HELP "--help" //Identifikátor vstupního argumentu pro --help
#define ARG_SHORTEST "--shortest" //Identifikátor vstupního argumentu pro --shortest
#define GET_ROW 0 //Identifikátor, aby byla vzata hodnota pro row
#define GET_COL 1 //Identifikátor, aby byla vzata hodnota pro col

typedef struct{
    int rows;
    int cols;
    unsigned char *cells;
} Map;

int load_size(FILE *maze, Map *map);
int load_and_test_maze(FILE *maze, Map *map);
int alloc_cells(Map *map);
int get_content_of_maze(Map *map);
void dec_to_bin(int decNum, int *binary);
bool isborder(Map *map, int r, int c, int border);
int get_r_c(Map *map, int pos, int getCoor);
int compare_borders(Map *map, int rowOper, int colOper, int mainBrd, int neigBrd, int mainC, int mainR, int pos);
int check_border(Map *map);
int start_border(Map *map, int r, int c, int leftright);
int get_start_border(Map *map, int r, int c);
int find_start_border_for_corner(Map *map, int r, int c, int border);
int find_start_border_for_center(Map *map, int r, int c, int border);
int even_row(Map *map, int r, int c, int border);
int odd_row(Map *map, int r, int c, int border);
int right_hand_rule(int r, int c, int border);
int left_hand_rule(int r, int c, int border);
void right_hand_move(int *r, int *c, int *border);
void left_hand_move(int *r, int *c, int *border);
int find_way(Map *map, int startR, int startC, int leftright);
int compare_size(Map *map, int entryR, int entryC);
void close_file_and_malloc(Map *map, FILE *file);
int vaid_run(Map *map, FILE *file, int r, int c);
int run(Map *map, int r, int c, char *handRule);
int check_arguments(int argc, char *argv[]);
void show_help();
int try_load_file_and_allocate_memory(Map *map, FILE *file);

/**
 * @parametr argc - počet argumentů na vstupu
 * @parametr argv[] - řetězec znaků reprezentující argumenty ze vstupu
 * Funkce kontroluje argumenty na vstupu.
 * Dále otvírá soubor s bludištěm a kontroluje jeho otevření.
 * Poté zavolá funkci na načtení souboru a alokování paměti a jejich kontrolu.
 * Dále volá soubor na kontrolu hranic.
 * Poté podle vstupního argumentu provede test bludiště, nebo jeho spuštění.
 * Nebudou-li argumenty u spuštění odpovídat, tak vypíše chybu.
 */
int main(int argc, char *argv[]){
    if(check_arguments(argc, argv) != 0){
        return 1;
    }else if(strcmp(argv[1], ARG_HELP) == 0){
        show_help();
        return 0;
    }
    
    Map map;
    FILE *maze;
    
    if(strcmp(argv[1], ARG_TEST) == 0){
        maze = fopen(argv[2], "r");
    }else{
        maze = fopen(argv[4], "r");
    }
    
    if(maze == NULL){
        fprintf(stderr, "Chyba při otvírání souboru.\n");
        fclose(maze);
        return 1;
    }
    
    int loadAndAlloc = try_load_file_and_allocate_memory(&map, maze);
    int valid = check_border(&map);
    
    if(strcmp(argv[1], ARG_TEST) == 0){
        if(valid != 0 || loadAndAlloc != 0){
            fprintf(stdout, "Invalid\n");
        }else{
            fprintf(stdout, "Valid\n");
        }
        close_file_and_malloc(&map, maze);
        return valid;
    }
    
    if((strcmp(argv[1], ARG_RIGHT) == 0 || strcmp(argv[1], ARG_LEFT) == 0) && (valid == 0 &&
       loadAndAlloc == 0)){
        int r = atoi(argv[2]);
        int c = atoi(argv[3]);
        if(compare_size(&map, r, c) != 0){
            fprintf(stderr, "Hodnoty jsou mimo pole bludiště.\n");
            close_file_and_malloc(&map, maze);
            return 1;
        }
        
        int found = run(&map, r, c, argv[1]);
        close_file_and_malloc(&map, maze);
        return found;
    }else{
        close_file_and_malloc(&map, maze);
        return 1;
    }
}

int load_size(FILE *maze, Map *map){
    char size;
    int sizeInt = 0;
    int posOfCoord = 1;
    
    while((size = fgetc(maze)) != '\n'){
        if(size != SPACE){
            
            if(!(size >= 48 && size <= 57)){
                fprintf(stderr, "Neplatná velikost matice - zadejte pouze čísla\n");
                return 1;
            }
            sizeInt = sizeInt *10 + (size - '0');
        }else{
            posOfCoord++;
            sizeInt = 0;
        }
        if(posOfCoord == 1) map->rows = sizeInt;
        if(posOfCoord == 2) map->cols = sizeInt;
    }
    
    return posOfCoord;
}

/**
 * @parametr *map - přebírá strukturu
 * Funkce alokuje paměť pro načtení hodnot mapy bludiště.
 */
int alloc_cells(Map *map){
    int content = get_content_of_maze(map);
    map->cells = malloc(sizeof(int)*content+1);
    
    if(map->cells == NULL){
        fprintf(stderr, "Chyba při alokování paměti\n");
        return 1;
    }
    
    return 0;
}

/**
 * @parametr *map - přebírá strukturu
 * Funkce vrací počet prvků v bludišti dle rows a cols.
 */
int get_content_of_maze(Map *map){
    return map->rows * map->cols;
}

/**
 * @parametr *map - přebírá strukturu
 * @parametr *file - přebírá složku s bludištěm, nad kterou má test provádět
 * @parametr
 * Funkce načte bludiště do map->cells a otestuje.
 * Bere pouze hodnoty 0-7.
 */
int load_and_test_maze(FILE *maze, Map *map){
    char value;
    int numOfRows = 0;
    int numOfCols = 0;
    int pos = 0;
    
    while((value = fgetc(maze)) != EOF){
        if(value != SPACE && ((value >= 48 && value <= 55) || value == '\n')){
            if(value != '\n'){
                map->cells[pos] = value;
                pos++;
                numOfCols ++;
            }else{
                if(numOfCols != map->cols){
                    return 1;
                }
                numOfCols = 0;
                numOfRows ++;
            }
        }else if(value != SPACE){
            fprintf(stderr, "Neplatná hodnota bludiště - zadejte pouze čísla\n");
            return 1;
        }
    }
    
    if(numOfRows != map->rows){
        return 1;
    }
    
    return 0;
}

/**
 * @parametr decNum - obdrží desetiné číslo na převod
 * @parametr *binary - pole pro uložení binární hodnoty
 * Funkce převádí desetiné číslo na binární.
 */
void dec_to_bin(int decNum, int *binary){
    int decimal = decNum;
    
    int i = BINARY_ARRAY_SIZE;
    while(i > 0){
        i--;
        binary[i] = decimal % 2;
        decimal = decimal / 2;
    }
}

/**
 * @parametr *map - přebírá strukturu
 * @parametr r - obdrží na vstupu hodnotu řádku
 * @parametr c - obdrží na vstupu hodnotu sloupce
 * @parametr border - hodnota, jakou hranici má zkontrolovat, uložena v konstantě
 * Metoda kontroluje, zda-li se na dané pozici (border) nachází díra nebo mezera.
 */
bool isborder(Map *map, int r, int c, int border){
    int pos = ((r-1)*map->cols + (c-1));
    int value = map->cells[pos] - '0';
    bool wall = false;
    int *binary = malloc(sizeof(int)*BINARY_ARRAY_SIZE+1);
    dec_to_bin(value, binary);
    if(binary[border] == 1){
        wall = true;
    }
    
    free(binary);
    return wall;
}


/**
 * @parametr *map - přebírá strukturu
 * @parametr pos - přebírá pozici v poli
 * @parametr getCoor - parametr, kterým určujeme, jakou souřadnici má funkce vrátit.
 * Funkce v obdržené hodnoty pomocí výpočtu získá pozici políčka. Souřadnice R x C.
 */
int get_r_c(Map *map, int pos, int getCoor){
    int r = 1;
    int c = pos;
    while((pos - (map->cols * r)) >= 0){
        r++;
    }
    c = (c+1)-((r-1)*map->cols);
    
    if(getCoor == GET_ROW){
        return r;
    }else{
        return c;
    }
}


/**
 * @parametr *map - přebírá strukturu
 * @parametr rowOper - operace na sloupci (plus/mínus - vpravo/vlevo)
 * @parametr colOper - operace na řádku (plus/mínus - dolů/nahoru)
 * @parametr mainBrd - obdrží původní hranici, vůči které budeme provádět kontrolu
 * @parametr neigBrd - obdrží sousedící hranici, kterou chce porovnávat
 * @parametr mainR - obdrží původní pozici řádku
 * @parametr maintC - obdrží původní pozici sloupce
 * @parametr pos - obdrží hodnotu políčka bludiště
 * Funkce získá souřadnice vedlejšího políčka a poté pomocí funkce isborder zkontroluje,
 * zda-li mají mezi sebou sousedící buňky stejnou hodnotu hranice.
 */
int compare_borders(Map *map, int rowOper, int colOper, int mainBrd, int neigBrd, int mainR, int mainC, int pos){
    int neigR = get_r_c(map, pos + rowOper*map->cols, GET_ROW); /*mainR + rowOper*/;
    int neigC = get_r_c(map, pos + colOper, GET_COL); /*mainC + colOper*/;
    
    if(isborder(map, neigR, neigC, neigBrd) != isborder(map, mainR, mainC, mainBrd)){
        return 1;
    }
    return 0;
}

/**
 * @parametr *map - přebírá strukturu
 * Funkce vrací, zda-li je bludiště validní, a to podle toho, jestli mají
 * sousedící políčka stejnou hranici.
 * Funkce zjistí souřadnice aktuálního políčka. A podle podmínek bude kontrolovat
 * levou, pravou a spodní sousedící buňku u každé sudé buňky.
 */
int check_border(Map *map){
    int valid = 0;
    
    int contant = get_content_of_maze(map);
    for(int pos = 0; pos < contant; pos++){
        int mainR = get_r_c(map, pos, GET_ROW);
        int mainC = get_r_c(map, pos, GET_COL);
        
        //printf("%d, %d\n",mainR, mainC);
        
        
        if((mainR + mainC) % 2 != 0){
            if(mainC > 1){
                valid = compare_borders(map, 0, -1, LEFT_BORDER, RIGHT_BORDER, mainR, mainC, pos);
                if(valid == 1) break;
            }
            
            if(mainC < map->cols){
                valid = compare_borders(map, 0, 1, RIGHT_BORDER, LEFT_BORDER, mainR, mainC, pos);
                if(valid == 1) break;
            }
            
            if(mainR < map->rows){
                valid = compare_borders(map, 1, 0, VERT_BORDER, VERT_BORDER, mainR, mainC, pos);
                if(valid == 1) break;
            }
        }
    }
    
    return valid;
}

/**
 * @parametr *map - přebírá strukturu
 * @parametr r - řádek zadán na vstupu
 * @parametr c - sloupec zadán na vstupu
 * @parametr leftright - předává informaci, zda-li jít pravidlem pravé či levé ruky
 * Funkce hledá na které hranici má pokračovat po vstoupení do bludiště,
 */
int start_border(Map *map, int r, int c, int leftright){
    int border = get_start_border(map, r, c);
    if(border < 0){
        return -1;
    }
    
    while(1){
        if(leftright == RIGHT_HAND){
            border = right_hand_rule(r, c, border);
        }else{
            border = left_hand_rule(r, c, border);
        }
        if(!isborder(map, r, c, border)){
            break;
        }
    }
    return border;
}

/**
 * @parametr *map - přebírá strukturu
 * @parametr r - řádek zadán na vstupu
 * @parametr c - sloupec zadán na vstupu
 * Funkce dle podmínek zjistí, jestli je jedná o krají políčka, pokud ne, vrací chybu.
 */
int get_start_border(Map *map, int r, int c){
    if(c == 1){
        if(r == 1 || r == map->rows){
            return find_start_border_for_corner(map, r, c, LEFT_BORDER);
        }else{
            return find_start_border_for_center(map, r, c, LEFT_BORDER);
        }
    }else if(c == map->cols){
        if(r == 1 || r == map->rows){
            return find_start_border_for_corner(map, r, c, RIGHT_BORDER);
        }else{
            return find_start_border_for_center(map, r, c, RIGHT_BORDER);
        }
    }else{
        if(r == 1){
            return odd_row(map, r, c, VERT_BORDER);
        }else if((r == map->rows) % 2 == 0){
            return even_row(map, r, c, VERT_BORDER);
        }else if((r == map->rows) % 2 != 0){
            return odd_row(map, r, c, VERT_BORDER);
        }
    }
    
    return -1;
}

/**
 * @parametr *map - přebírá strukturu
 * @parametr r - hodnota řádku na vstupu
 * @parametr c - hodnota sloupce na vstupu
 * @parametr border - předává hodnotu hranice
 * Funkce kontroluje, zdali se jedná o vstup (na rohových polích).
 */
int find_start_border_for_corner(Map *map, int r, int c, int border){
    if(find_start_border_for_center(map, r, c, border) == border){
        return border;
    }else if(!isborder(map, r, c, VERT_BORDER)){
        return VERT_BORDER;
    }else{
        return -1;
    }
}

/**
 * @parametr *map - přebírá strukturu
 * @parametr r - hodnota řádku na vstupu
 * @parametr c - hodnota sloupce na vstupu
 * @parametr border - předává hodnotu hranice
 * Funkce kontroluje, zdali se jedná o vstup (na krajních polích).
 */
int find_start_border_for_center(Map *map, int r, int c, int border){
    if(!isborder(map, r, c, border)){
        return border;
    }else{
        return -1;
    }
}

/**
 * @parametr *map - přebírá strukturu
 * @parametr r - hodnota řádku na vstupu
 * @parametr c - hodnota sloupce na vstupu
 * @parametr border - předává hodnotu hranice
 * Funkce kontroluje, zdali je na liché pozici správně orientovaná buňka.
 */
int odd_row(Map *map, int r, int c, int border){
    if(c % 2 != 0){
        return find_start_border_for_center(map, r, c, border);
    }
    return -1;
}

/**
 * @parametr *map - přebírá strukturu
 * @parametr r - hodnota řádku na vstupu
 * @parametr c - hodnota sloupce na vstupu
 * @parametr border - předává hodnotu hranice
 * Funkce kontroluje, zdali je na sudé pozici správně orientovaná buňka.
 */
int even_row(Map *map, int r, int c, int border){
    if(c % 2 == 0){
        return find_start_border_for_center(map, r, c, border);
    }
    return -1;
}

/**
 * @parametr r - hodnota řádku na vstupu
 * @parametr c - hodnota sloupce na vstupu
 * @parametr border - předává hodnotu hranice
 * Funkce udává, jak se má cesta bludištěm chovat, při pravidlu pravé ruky.
 */
int right_hand_rule(int r, int c, int border){
    if((r+c) % 2 == 0){ // \/
        if(border == LEFT_BORDER){
            return RIGHT_BORDER;
        }else if(border == VERT_BORDER){
            return LEFT_BORDER;
        }else{
            return VERT_BORDER;
        }
    }else{
        if(border == LEFT_BORDER){
            return VERT_BORDER;
        }else if(border == VERT_BORDER){
            return RIGHT_BORDER;
        }else{
            return LEFT_BORDER;
        }
    }
}

/**
 * @parametr r - hodnota řádku na vstupu
 * @parametr c - hodnota sloupce na vstupu
 * @parametr border - předává hodnotu hranice
 * Funkce udává, jak se má cesta bludištěm chovat, při pravidlu levé ruky.
 */
int left_hand_rule(int r, int c, int border){
    if((r+c) % 2 == 0){ // \/
        if(border == LEFT_BORDER){
            return VERT_BORDER;
        }else if(border == VERT_BORDER){
            return RIGHT_BORDER;
        }else{
            return LEFT_BORDER;
        }
    }else{
        if(border == LEFT_BORDER){
            return RIGHT_BORDER;
        }else if(border == VERT_BORDER){
            return LEFT_BORDER;
        }else{
            return VERT_BORDER;
        }
    }
}

/**
 * @parametr r - hodnota řádku na vstupu
 * @parametr c - hodnota sloupce na vstupu
 * @parametr border - předává hodnotu hranice
 * Funkce vrací pozici, na kterou má podle daného políčka pokračovat.
 */
void right_hand_move(int *r, int *c, int *border){
    if((*c+*r) % 2 != 0){ // '/\'
        fprintf(stdout, "%d,%d\n", *r, *c);
        if(*border == RIGHT_BORDER){
            *c = *c + 1;
            *border = RIGHT_BORDER;
        }else if(*border == LEFT_BORDER){
            *c = *c - 1;
            *border = VERT_BORDER;
        }else{
            *r = *r + 1;
            *border = LEFT_BORDER;
        }
    }else{ // '\/'
        fprintf(stdout, "%d,%d\n", *r, *c);
        if(*border == RIGHT_HAND){
            *c = *c + 1;
            *border = VERT_BORDER;
        }else if(*border == LEFT_BORDER){
            *c = *c - 1;
            *border = LEFT_BORDER;
        }else{
            *r = *r - 1;
            *border = RIGHT_BORDER;
        }
    }
}

/**
 * @parametr r - hodnota řádku na vstupu
 * @parametr c - hodnota sloupce na vstupu
 * @parametr border - předává hodnotu hranice
 * Funkce vrací pozici, na kterou má podle daného políčka pokračovat.
 */
void left_hand_move(int *r, int *c, int *border){
    if((*c+*r) % 2 != 0){ // '/\'
        fprintf(stdout, "%d,%d\n", *r, *c);
        if(*border == RIGHT_BORDER){
            *c = *c + 1;
            *border = VERT_BORDER;
        }else if(*border == LEFT_BORDER){
            *c = *c - 1;
            *border = LEFT_BORDER;
        }else{
            *r = *r + 1;
            *border = RIGHT_BORDER;
        }
    }else{ // '\/'
        fprintf(stdout, "%d,%d\n", *r, *c);
        if(*border == RIGHT_BORDER){
            *c = *c + 1;
            *border = RIGHT_BORDER;
        }else if(*border == LEFT_BORDER){
            *c = *c - 1;
            *border = VERT_BORDER;
        }else{
            *r = *r - 1;
            *border = LEFT_BORDER;
        }
    }
}

/**
 * @parametr *map - přebírá strukturu
 * @parametr startR - hodnota řádku
 * @parametr startC - hodnota sloupce
 * @parametr leftright - předává údaje, jaké pravidlo máme následovat
 * Funkce hledá cestu bludištěm.
 */
int find_way(Map *map, int startR, int startC, int leftright){
    int r = startR;
    int c = startC;
    
    int border = start_border(map, r, c, leftright);
    if(border < 0){
        return 1;
    }
    
    while(1){
        if(leftright == RIGHT_HAND){
            if(isborder(map, r, c, border)){
                border = right_hand_rule(r, c, border);
            }else{
                right_hand_move(&r, &c, &border);
            }
        }else{
            if(isborder(map, r, c, border)){
                border = left_hand_rule(r, c, border);
            }else{
                left_hand_move(&r, &c, &border);
            }
        }
        if((c < 1 || c > map->cols)||(r < 1 || r > map->rows)){
            break;
        }
    }
    
    return 0;
}

/**
 * @parametr *map - přebírá strukturu
 * @parametr entryR - hodnota řádku na vstupu
 * @parametr entryC - hodnota sloupce na vstupu
 * Funkce kontroluje, že se zadaná vstupní hodnota nachází v bludišti.
 *
 */
int compare_size(Map *map, int entryR, int entryC){
    if(map->rows < entryR || entryR < 1) return 1;
    if(map->cols < entryC || entryC < 1) return 1;
    return 0;
}

/**
 * @parametr *map - přebírá strukturu
 * @parametr *file - soubor s polem
 * Funkce uvolní pamět a uzavře soubor.
 */
void close_file_and_malloc(Map *map, FILE *file){
    fclose(file);
    free(map->cells);
}

/**
 * @parametr *map - přebírá strukturu
 * @parametr r - hodnota řádku na vstupu
 * @parametr c - hodnota sloupce na vstupu
 * @parametr *handRule - předává, jaké pravidlo máme dodržet
 * Funkce započne cestu bludištěm, bude-li vstup mimo okrajové hodnoty,
 * vypíše chybovou hlášku.
 */
int run(Map *map, int r, int c, char *handRule){
    int leftright = LEFT_HAND;
    if(strcmp(handRule, ARG_RIGHT) == 0) leftright = RIGHT_HAND;
    
    if(find_way(map, r, c, leftright) != 0){
        fprintf(stderr, "Zde není vstup.\n");
        return 1;
    }

    return 0;
}

/**
 * @parametr argc - počet argumentů na vstupu
 * @parametr *argv[] - hodnota argumentů na vstupu
 * Funkce kontroluje správnost argumentů.
 */
int check_arguments(int argc, char  *argv[]){
    if(argc < 2 || (argc > 3 && argc != 5)){
        fprintf(stderr, "Nesprávný počet argumentů, pro více informací: ./proj3 --help\n");
        return 1;
    }else if(argc == 2){
        if(strcmp(argv[1], ARG_HELP) != 0){
            fprintf(stderr, "Špatný argument, pro více informací: ./proj3 --help\n");
            return 1;
        }
    }else if(argc == 3){
        if(strcmp(argv[1], ARG_TEST) != 0){
            fprintf(stderr, "Špatné argumenty, pro více informací: ./proj3 --help\n");
            return 1;
        }
    }else if(argc == 5){
        if(strcmp(argv[1], ARG_RIGHT) == 0|| strcmp(argv[1], ARG_LEFT) == 0){
            if(atoi(argv[2]) == 0 || atoi(argv[3]) == 0){
                fprintf(stderr, "Špatné hodnoty R a C.\n");
                return 1;
            }
        }else if(strcmp(argv[1], ARG_SHORTEST) == 0){
            fprintf(stderr, "Tuto cestu neznám.\n");
            return 1;
        }else{
            fprintf(stderr, "Špatné argumenty, pro více informací: ./proj3 --help\n");
            return 1;
        }
    }
    return 0;
}

/**
 * Funkce vypíše nápovědu.
 */
void show_help(){
    fprintf(stdout,
            "\nNÁPOVĚDA PROJEKTU 3 - BLUDIŠTĚ\n\n"
            "Soubor bludiště:\n"
            "   - Při každém zapnutí nahráváme soubor s bludištěm.\n"
            "   - O validní soubor se jedná pouze tehdy, obsahuje-li první řádek\n"
            "     počet řádků (mezera) počet sloupců.\n"
            "   - Následující řádky obsahují čísla, od 0-7, reprezentující stěny bludiště,\n"
            "     jenž jsou odděleny mezerou.\n"
            "   - Tyto čísla musí být obsažena ve stejném počtu řádků a sloupců,\n"
            "     jako jsme určili v prvním řádku.\n"
            "   - Řádky * sloupce musí odpovídat množství čísel tvořící bludiště.\n"
            "   - Bludiště mohou tvořit pouze čísla od 0 po 7.\n\n"
            "Způsoby zapnutí:\n"
            "   - ./proj3 --help - otevření nápovědy \n"
            "   - ./proj3 --test 'nazev_souboru' - ověření validity bludiště \n"
            "   - ./proj3 --rpath 'cislo_radku' 'cislo_sloupce' 'nazev_souboru'\n"
            "       - hledání cesty podle pravidla pravé ruky z pozice R x C \n"
            "   - ./proj3 --lpath 'cislo_radku' 'cislo_sloupce' 'nazev_souboru'\n"
            "       - hledání cesty podle pravidla levé ruky z pozice R x C \n\n"
            );
}

/**
 * @parametr *map - přebírá strukturu
 * @parametr *file - soubor s bludištěm
 * Funkce kontroluje, zdali se správně načetla velikost bludiště
 * alokovala paměť a načetly zbylé hodnoty ze souboru.
 */
int try_load_file_and_allocate_memory(Map *map, FILE *file){
    if(load_size(file, map) != COORDINATION_R_C){
        fprintf(stderr, "Neplatná velikost bludiště. Prosím vložte velikost: řádky(mezera)sloupce\n");
        return 1;
    }
    
    if(alloc_cells(map) == 1){
        fprintf(stderr, "Chyba při alokování paměti.\n");
        return 1;
    }
    
    if(load_and_test_maze(file, map) == 1){
        fprintf(stderr, "Chyba při načítání obsahu bludiště.\n");
        return 1;
    }
    
    return 0;
}
