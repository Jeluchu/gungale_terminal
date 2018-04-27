#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <locale.h>
#include <ncurses.h>
#include <time.h>

#define DELTA_T 0.01

#define ROJO "\x1B[1;31m"
#define NORMAL "\x1B[0m"
#define AMARILLO "\x1B[1;33m"
#define VERDE "\x1B[1;32m"
#define AZUL "\x1B[1;34m"
#define NEGRITA "\x1B[1m"
#define AZULETE "\x1B[1;36m"

typedef struct {
    double x;
    double y;
} Coord;

typedef struct {
    Coord pos;
    Coord dir;
    Coord vel;
    Coord acc;
} Movil;

typedef struct {
    Movil cuerpo;
    int vida;
    int ammo; //MUNICIÓN
} Player;

void op_game(){

    time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);

    char output[128];
    strftime(output, 128, "%d/%m/%y a las %H:%M:%S", tlocal);

    /*
    system("clear");
    for (int i=0; i<10; i++)
        printf("\n");
    printf("\t");
    */
    printf("\n");
    system("clear");
    system("toilet -fpagga GUN GALE TERMINAL");
    sleep(2);
    printf( "|\tTe damos la bienvenida ");
    printf("%s", getenv("USER"));
    printf("\n" );
    sleep(1);
    printf( "|\tGUN GALE TERMINAL GAME v0.1  ~ Versión 'ESTABLE'\n" );
    sleep(1);
    printf( "|\tIniciado el %s\n", output);
    sleep(2);

}

void pintar(Player jugador[2]){
    mvprintw(LINES - jugador[0].cuerpo.pos.y,
             jugador[0].cuerpo.pos.x, "▞█████▚");
    mvprintw(LINES - jugador[1].cuerpo.pos.y,
             jugador[1].cuerpo.pos.x, "▞█████▚");
}

Coord preguntar_disparo(int turno){
    Coord res;
    double vel, alpha;
    init_pair(3, COLOR_WHITE, COLOR_WHITE);

    /* INICIANDO COLORES EN NCURSES */
    if (has_colors())
        start_color();

    mvprintw( 1, 2, "┏━━━━━━━━━━━┓");
    mvprintw( 2, 2, "┃ Jugador %i ┃", turno % 2 + 1 );
    mvprintw( 3, 2, "┗━━━━━━━━━━━┛ ");
    mvprintw( 4, 2, "                                   " );
    mvprintw( 5, 2, "                                     " );
    mvprintw( 4, 2, "┃ Velocidad: " );
    scanw(" %lf", &vel);
    mvprintw( 5, 2, "┃ Angulo:" );
    scanw(" %lf", &alpha);

    res.x = vel * cos(alpha / 360. * 2 * M_PI);
    res.y = vel * sin(alpha / 360. * 2 * M_PI);

    return res;
}

void disparar(Movil bala){
    Coord ini;
    do{
        ini = bala.pos;
        bala.vel.x += bala.acc.x * DELTA_T;
        bala.vel.y += bala.acc.y * DELTA_T;
        bala.pos.x += bala.vel.x * DELTA_T;
        bala.pos.y += bala.vel.y * DELTA_T;

        mvprintw(LINES - ini.y, ini.x, " ");
        mvprintw(LINES - bala.pos.y, bala.pos.x, "*");
        refresh();
        usleep(1000000 * DELTA_T);
    }while(bala.pos.y > 0);
}

int main(){
    int turno = 0;
    Movil bala = {{0., 0.}, {0., 0.}, {0., 0.}, {0., -9.8}};

    Player jugador[] = {
        {{ { 5., 1.}, { 1, 0}, {0., 0.}, {0., 0.}  }, 100, 20},
        {{ {65., 1.}, {-1, 0}, {0., 0.}, {0., 0.}  }, 100, 20}
    };

    setlocale(LC_ALL, ""); //DISPONER DE CARACTERES UNICODE

    op_game();

    initscr(); //INICIAMOS NCURSES
    curs_set(0); //OCULTAMOS EL CURSOR

    while (1) {
        pintar(jugador);
        refresh();
        bala.pos = jugador[turno % 2].cuerpo.pos;
        bala.vel = preguntar_disparo(turno);
        disparar(bala);
        turno++;
    }

    curs_set(1);
    endwin();
    return EXIT_SUCCESS;
}
