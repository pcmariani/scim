#include <sys/types.h>
#include <string.h>
#include <stdlib.h>      // for atoi
#include <curses.h>
#include <ctype.h>
#include <unistd.h>

#include "sc.h"
#include "macros.h"
#include "utils/dictionary.h"
#include "utils/string.h"
#include "color.h"

static struct ucolor ucolors[N_INIT_PAIRS];
static struct dictionary * d_colors_param = NULL;

struct dictionary * get_d_colors_param() {
    return d_colors_param;
}

// Funcion que genera los initcolor de colores DEFAULT
void start_default_ucolors() {

    // Blanqueo los atributos de todos los colores
    int i;
    for (i=0; i< N_INIT_PAIRS; i++) {
        ucolors[ i ].bold      = 0;
        ucolors[ i ].dim       = 0;
        ucolors[ i ].reverse   = 0;
        ucolors[ i ].standout  = 0;
        ucolors[ i ].underline = 0;
        ucolors[ i ].blink     = 0;
    }

    ucolors[ HEADINGS        ].fg = WHITE;
    ucolors[ HEADINGS        ].bg = BLUE;
    ucolors[ WELCOME         ].fg = BLUE;
    ucolors[ WELCOME         ].bg = BLACK;
    ucolors[ CELL_SELECTION  ].fg = BLUE;         // cell selection in headings
    ucolors[ CELL_SELECTION  ].bg = WHITE;

    ucolors[ CELL_SELECTION_SC ].fg = BLACK;      // cell selection in spreadsheet
    ucolors[ CELL_SELECTION_SC ].bg = WHITE;

    ucolors[ NUMB            ].fg = CYAN;
    ucolors[ NUMB            ].bg = BLACK;

    ucolors[ STRG            ].fg = RED;
    ucolors[ STRG            ].bg = BLACK;
    ucolors[ STRG            ].bold = 1;

    ucolors[ DATEF           ].fg = YELLOW;
    ucolors[ DATEF           ].bg = BLACK;

    ucolors[ EXPRESSION      ].fg = YELLOW;
    ucolors[ EXPRESSION      ].bg = BLACK;

    ucolors[ INFO_MSG        ].fg = CYAN;
    ucolors[ INFO_MSG        ].bg = BLACK;
    ucolors[ INFO_MSG        ].bold = 1;
    ucolors[ ERROR_MSG       ].fg = RED;
    ucolors[ ERROR_MSG       ].bg = WHITE;
    ucolors[ ERROR_MSG       ].reverse = 1;
    ucolors[ ERROR_MSG       ].bold = 1;

    ucolors[ MODE            ].fg = WHITE;
    ucolors[ MODE            ].bg = BLACK;
    ucolors[ MODE            ].bold = 1;

    ucolors[ CELL_ID         ].fg = BLUE;
    ucolors[ CELL_ID         ].bg = BLACK;
    ucolors[ CELL_ID         ].bold = 1;
    ucolors[ CELL_FORMAT     ].fg = GREEN;
    ucolors[ CELL_FORMAT     ].bg = BLACK;
    ucolors[ CELL_CONTENT    ].fg = CYAN;
    ucolors[ CELL_CONTENT    ].bg = BLACK;
    ucolors[ CELL_CONTENT    ].bold = 1;

    ucolors[ INPUT           ].fg = WHITE;
    ucolors[ INPUT           ].bg = BLACK;

    ucolors[ NORMAL          ].fg = WHITE;
    ucolors[ NORMAL          ].bg = BLACK;

    ucolors[ CELL_ERROR      ].fg = RED;
    ucolors[ CELL_ERROR      ].bg = BLACK;
    ucolors[ CELL_ERROR      ].bold = 1;

    ucolors[ CELL_NEGATIVE   ].fg = GREEN;
    ucolors[ CELL_NEGATIVE   ].bg = BLACK;

    init_pair(HEADINGS,          ucolors[HEADINGS].fg,            ucolors[HEADINGS].bg);
    init_pair(MODE,              ucolors[MODE].fg,                ucolors[MODE].bg);
    init_pair(NUMB,              ucolors[NUMB].fg,                ucolors[NUMB].bg);
    init_pair(STRG,              ucolors[STRG].fg,                ucolors[STRG].bg);
    init_pair(DATEF,             ucolors[DATEF].fg,               ucolors[DATEF].bg);
    init_pair(EXPRESSION,        ucolors[EXPRESSION].fg,          ucolors[EXPRESSION].bg);
    init_pair(CELL_ERROR,        ucolors[CELL_ERROR].fg,          ucolors[CELL_ERROR].bg);
    init_pair(CELL_NEGATIVE,     ucolors[CELL_NEGATIVE].fg,       ucolors[CELL_NEGATIVE].bg);
    init_pair(CELL_SELECTION,    ucolors[CELL_SELECTION].fg,      ucolors[CELL_SELECTION].bg);
    init_pair(CELL_SELECTION_SC, ucolors[CELL_SELECTION_SC].fg,   ucolors[CELL_SELECTION_SC].bg);
    init_pair(INFO_MSG,          ucolors[INFO_MSG].fg,            ucolors[INFO_MSG].bg);
    init_pair(ERROR_MSG,         ucolors[ERROR_MSG].fg,           ucolors[ERROR_MSG].bg);
    init_pair(CELL_ID,           ucolors[CELL_ID].fg,             ucolors[CELL_ID].bg);
    init_pair(CELL_FORMAT,       ucolors[CELL_FORMAT].fg,         ucolors[CELL_FORMAT].bg);
    init_pair(CELL_CONTENT,      ucolors[CELL_CONTENT].fg,        ucolors[CELL_CONTENT].bg);
    init_pair(WELCOME,           ucolors[WELCOME].fg,             ucolors[WELCOME].bg);
    init_pair(NORMAL,            ucolors[NORMAL].fg,              ucolors[NORMAL].bg);
    init_pair(INPUT,             ucolors[INPUT].fg,               ucolors[INPUT].bg);

}

// Funcion que setea un color
void set_ucolor(WINDOW * w, int uc) {
    long attr = A_NORMAL;
    if (ucolors[uc].bold)      attr |= A_BOLD;
    if (ucolors[uc].dim)       attr |= A_DIM;
    if (ucolors[uc].reverse)   attr |= A_REVERSE;
    if (ucolors[uc].standout)  attr |= A_STANDOUT;
    if (ucolors[uc].blink)     attr |= A_BLINK;
    if (ucolors[uc].underline) attr |= A_UNDERLINE;
    wattrset (w, attr | COLOR_PAIR(uc) );
}

// Funcion que crea un diccionario y guarda en el 
// la equivalencia entre las macros y los valores
// de las claves (enteros) que se definen en
// los archivos .sc o a través del comando color.
void set_colors_param_dict() {
    d_colors_param = create_dictionary();

    char str[3];
    str[0]='\0';

    sprintf(str, "%d", WHITE);
    put(d_colors_param, "WHITE", str);
    sprintf(str, "%d", BLACK);
    put(d_colors_param, "BLACK", str);
    sprintf(str, "%d", RED);
    put(d_colors_param, "RED", str);
    sprintf(str, "%d", GREEN);
    put(d_colors_param, "GREEN", str);
    sprintf(str, "%d", YELLOW);
    put(d_colors_param, "YELLOW", str);
    sprintf(str, "%d", BLUE);
    put(d_colors_param, "BLUE", str);
    sprintf(str, "%d", MAGENTA);
    put(d_colors_param, "MAGENTA", str);
    sprintf(str, "%d", CYAN);
    put(d_colors_param, "CYAN", str);

    sprintf(str, "%d", HEADINGS);
    put(d_colors_param, "HEADINGS", str);
    sprintf(str, "%d", WELCOME);
    put(d_colors_param, "WELCOME", str);
    sprintf(str, "%d", CELL_SELECTION);
    put(d_colors_param, "CELL_SELECTION", str);
    sprintf(str, "%d", CELL_SELECTION_SC);
    put(d_colors_param, "CELL_SELECTION_SC", str);
    sprintf(str, "%d", NUMB);
    put(d_colors_param, "NUMB", str);
    sprintf(str, "%d", STRG);
    put(d_colors_param, "STRG", str);
    sprintf(str, "%d", DATEF);
    put(d_colors_param, "DATEF", str);
    sprintf(str, "%d", EXPRESSION);
    put(d_colors_param, "EXPRESSION", str);
    sprintf(str, "%d", INFO_MSG);
    put(d_colors_param, "INFO_MSG", str);
    sprintf(str, "%d", ERROR_MSG);
    put(d_colors_param, "ERROR_MSG", str);
    sprintf(str, "%d", MODE);
    put(d_colors_param, "MODE", str);
    sprintf(str, "%d", CELL_ID);
    put(d_colors_param, "CELL_ID", str);
    sprintf(str, "%d", CELL_FORMAT);
    put(d_colors_param, "CELL_FORMAT", str);
    sprintf(str, "%d", CELL_CONTENT);
    put(d_colors_param, "CELL_CONTENT", str);
    sprintf(str, "%d", INPUT);
    put(d_colors_param, "INPUT", str);
    sprintf(str, "%d", NORMAL);
    put(d_colors_param, "NORMAL", str);
    sprintf(str, "%d", CELL_ERROR);
    put(d_colors_param, "CELL_ERROR", str);
    sprintf(str, "%d", CELL_NEGATIVE);
    put(d_colors_param, "CELL_NEGATIVE", str);
}

void free_colors_param_dict() {
    destroy_dictionary(d_colors_param);
    return;
}

// Funcion que cambia la definicion de un color
// por una definida por el usuario.
// str = es la definicion que se lee del archivo .sc
// o bien ingresada en tiempo de ejecucion a través
// del comando :color str
void chg_color(char * str) {
    
    // creo un diccionario para guardar las claves y valores que figuran en el string
    struct dictionary * d = create_dictionary();

    // elimino commilas de str
    if (str[0]=='"') del_char(str, 0);
    if (str[strlen(str)]=='"') del_char(str, strlen(str));

    parse_str(d, str);

    // valido que existan las minimas claves necesarias para cambiar un color
    // TODO validar tambien que los valores que tengan esas claves sean correctos
    if (get(d, "fg") == '\0' || get(d, "bg") == '\0' || get(d, "type") == '\0') {
        error("Color definition incomplete");
        destroy_dictionary(d);
        return;
    }

    // cambio el color
    int type = atoi(get(d_colors_param, get(d, "type")));
    ucolors[ type ].fg = atoi(get(d_colors_param, get(d, "fg")));
    ucolors[ type ].bg = atoi(get(d_colors_param, get(d, "bg")));
    if (get(d, "bold")      != '\0')     ucolors[ type ].bold      = atoi(get(d, "bold"));
    if (get(d, "dim")       != '\0')     ucolors[ type ].dim       = atoi(get(d, "dim"));
    if (get(d, "reverse")   != '\0')     ucolors[ type ].reverse   = atoi(get(d, "reverse"));
    if (get(d, "standout")  != '\0')     ucolors[ type ].standout  = atoi(get(d, "standout"));
    if (get(d, "blink")     != '\0')     ucolors[ type ].blink     = atoi(get(d, "blink"));
    if (get(d, "underline") != '\0')     ucolors[ type ].underline = atoi(get(d, "underline"));

    init_pair(type, ucolors[type].fg, ucolors[type].bg);

    destroy_dictionary(d);

    return;
}
