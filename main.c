#include <stdio.h>
#include <stdbool.h>
#include <windows.h>
#include <string.h>
#include <time.h>

struct Equipo
{
    int nro;
    char nombre[15];
};

struct Equipo equipos[32];

struct Grupo
{
    int nro_equipo;
    int nro_grupo;
    char nombre_equipo[15];
};

struct Grupo grupos[8][4];

struct Partidos
{
    int nro_partido;
    char equipo1[15];
    int goles1;
    char equipo2[15];
    int goles2;
};

struct Partidos partidos[8][6];


struct Posiciones
{
    int puntos;
    int dif_gol;
    char equipo[15];
};

struct Posiciones posiciones[8][4];
struct Posiciones aux[1];

//---PROTOTIPOS DE FUNCIONES---
void mostrarMenu();
void leerEquipos();
void mostrarEquipos();
void sortearGrupos();
int sacarBolillas();
void mostrarGrupos();
void guardarGrupos();
void confeccionarPartidos();
void mostrarPartidos();
void cargarResultados();
void cargarPosiciones();
void mostrarPosiciones();
bool ficheroVacio();

//---INICIO PROGRAMA---
int main()
{
    int opcionMenu; //Variable que lee la opcion ingresada por el usuario respecto al men�

    leerEquipos(); //Cargo los equipos. Debe estar el vector "equipos" lleno(con los 32 equipos del archivo) antes de entrar al programa

    do
    {
        mostrarMenu();

        scanf("%d", &opcionMenu);
        fflush(stdin);

        switch (opcionMenu)
        {
        case 1:
            system("cls");
            mostrarEquipos();
            break;
        case 2:
            system("cls");
            sortearGrupos();
            guardarGrupos();
            confeccionarPartidos();
            break;
        case 3:
            system("cls");
            mostrarGrupos();
            break;
        case 4:
            system("cls");
            mostrarPartidos();
            break;
        case 5:
            system("cls");
            cargarResultados();
            cargarPosiciones();
            break;
        case 6:
            system("cls");
            mostrarPosiciones();
            break;
        case 7:
            exit(1);
        }
    }
    while (opcionMenu != 7);


    return 0;
}

//Menu de opciones al que se llama siempre que terminamos alguna tarea.
void mostrarMenu()
{
    system("cls");
    printf("  ==========================MENU PRINCIPAL==========================\n");
    printf(" | 1- Mostrar equipos participantes.                                |\n");
    printf(" | 2- Realizar sorteo.                                              |\n");
    printf(" | 3- Mostrar grupos.                                               |\n");
    printf(" | 4- Mostrar partidos fase de grupos.                              |\n");
    printf(" | 5- Cargar resultados fase de grupos.                             |\n");
    printf(" | 6- Mostrar posiciones primera fase.                              |\n");
    printf(" | 7- Salir.                                                        |\n");
    printf("  ==================================================================\n");
    printf("\n\nOpcion: ");
}

void leerEquipos() // Funcion que lee el fichero equipo y almacena los datos en el vector de estructuras equipos
{

    int i=0;
    FILE *registroEquipos;
    registroEquipos = fopen("equipos.txt", "r");

    if(registroEquipos==NULL)
    {
        printf("No se ha podido abrir correctamente el archivo.\n");
        exit(1);
    }

    if(!(ficheroVacio("equipos.txt"))) // validacion que pregunta si el archivo esta vacio
    {
        while(!feof(registroEquipos))
        {

            fscanf(registroEquipos, "%d ", &equipos[i].nro);
            fflush(stdin);
            fgets(equipos[i].nombre,15,registroEquipos);
            strtok(equipos[i].nombre,"\n");
            i++;
        }
    }
    else
    {
        printf("El archivo de equipos está vacío");
        system("pause");
        exit(1);
    }

    fclose(registroEquipos);

}


void mostrarEquipos() // Funcion que imprime los equipos que participan del mundial
{
    printf("    Selecciones mundial Qatar 2022\n\n");

    for(int i=0; i<32; i++)
    {
        printf("    Nro: %*d   -   Nombre: %*s\n", -2, equipos[i].nro, -11, equipos[i].nombre);
    }

    printf("\n");
    system("pause");
}


void sortearGrupos() // Funcion que sortea los equipos de los 8 grupos y comprueba que no sean repetidos
{
    //UPDATE POST-ENVIO ----> Por si alguien sorteaba nuevamente luego de completar los partidos, deja vacio el fichero para activar la validacion, ya que antes mostraba las posiciones anteriores
    FILE *posiciones;
    posiciones = fopen("posiciones.txt", "w");
    while(feof(posiciones))
        fprintf(posiciones, "");
    fclose(posiciones);

    int arreglo_no_repetidos[32]; // Arreglo de banderas
    memset(arreglo_no_repetidos, 0, sizeof(int) *32); // Setea todos los elementos del arreglo con el valor de 0

    srand(time(NULL));

    for(int i=0; i<8; i++)
    {
        for(int j=0; j<4; j++)
        {
            int bandera = 0;
            while(bandera != 1) // Repite el bucle hasta entrar en la condicion y marcar la bandera == 1
            {
                int bolilla = sacarBolillas(j); // Almacena el num retornado por la funcion
                if(arreglo_no_repetidos[bolilla]==0) // Comprueba si ese numero no paso por esa posicion anteriormente (1)
                {
                    grupos[i][j].nro_equipo = equipos[bolilla].nro; // Almacena los datos en la matriz estructura grupos
                    grupos[i][j].nro_grupo = i+1;
                    strcpy(grupos[i][j].nombre_equipo, equipos[bolilla].nombre);
                    arreglo_no_repetidos[bolilla] = 1; // marca en el arreglo que esa posicion/bolilla fue utilizada
                    bandera = 1; // Corta el while
                }
            }
        }
    }

    printf("\nSorteo realizado con exito!\n\n");


    system("pause");
}

int sacarBolillas(int bombo)  //Funcion que retorna un numero aleatorio de acuerdo al bombo a sortear
{
    switch(bombo)
    {
    case 0:
        return rand()%8;
        break;
    case 1:
        return rand()%8 + 8;
        break;
    case 2:
        return rand()%8 + 16;
        break;
    case 3:
        return rand()%8 + 24;
        break;
    }
}

//Funcion que escribe en el archivo de grupos, el resultado del sorteo
void guardarGrupos()
{
    FILE *crearGrupos;
    crearGrupos = fopen("grupos.txt", "w");

    for(int i=0; i<8; i++)
        for(int j=0; j<4; j++)
        {
            fprintf(crearGrupos, "%d ", grupos[i][j].nro_grupo);
            fprintf(crearGrupos, "%d ", grupos[i][j].nro_equipo);
            fprintf(crearGrupos, "%s",grupos[i][j].nombre_equipo);
            fprintf(crearGrupos,"\n");
        }
    fclose(crearGrupos);
}


void mostrarGrupos() // Funcion que muestra todos los grupos por pantalla
{
    FILE *traerGrupos;
    traerGrupos = fopen("grupos.txt", "r");

    if(traerGrupos==NULL)
    {
        printf("No se ha podido abrir correctamente el archivo.\n");
        exit(1);
    }

    //ERROR --> FeOf hace que se duplique las ordenes
    if(!(ficheroVacio("equipos.txt")))
    {

        printf("\n  ************************\n");
        for(int i=0; i<8; i++)
        {
            printf("       -   GRUPO %d   -\n", i+1);
            for(int j=0; j<4; j++)
            {
                fscanf(traerGrupos, "%d ", &grupos[i][j].nro_grupo);
                fflush(stdin);
                fscanf(traerGrupos, "%d ", &grupos[i][j].nro_equipo);
                fgets(grupos[i][j].nombre_equipo,15,traerGrupos);
                strtok(grupos[i][j].nombre_equipo, "\n");
                printf("          %s\n",grupos[i][j].nombre_equipo);
                strcpy(posiciones[i][j].equipo, grupos[i][j].nombre_equipo); // UPDATE --> Lo asigno aca para no repetir lectura en confeccionPartidos. Copia el nombre en posiciones para usarlo en la opc 6. Tiene un error
                // ya que si no entra en la funcion luego el nombre no se imprime en el fichero posiciones.
            }
            printf("  ************************\n");
        }
    }
    else
    {
        printf("El archivo de grupos está vacío");
        system("pause");
        exit(1);
    }
    fclose(traerGrupos);
    printf("\n");
    system("pause");
}


void confeccionarPartidos() // Funcion que crea el fichero con todos los partidos a disputarse
{
    //Elimine la lectura de datos pq ya se hacía con la funcion anterior

    int nroPartido = 1;
    FILE *confeccionarPartidos;
    confeccionarPartidos = fopen("partidos.txt", "w");


    for(int i=0; i<8; i++)
        for(int j=0; j<3; j++)
            for(int k=3; k>j; k--) //Matchea los primeros tres partidos del primero, dos del segundo y uno con los dos ultimos equipos, que figuran en el grupo
            {
                partidos[i][j].goles1 = 0;
                partidos[i][j].goles2 = 0;
                fprintf(confeccionarPartidos, "%d ",nroPartido);
                nroPartido++;
                fprintf(confeccionarPartidos, "%d ",partidos[i][j].goles1);
                fprintf(confeccionarPartidos, "%s\n",grupos[i][j].nombre_equipo); //Equipo que figura primero/izquierda
                fprintf(confeccionarPartidos, "%d ",partidos[i][j].goles2);
                fprintf(confeccionarPartidos, "%s\n\n",grupos[i][k].nombre_equipo); // Equipo que figura segundo/derecha
            }

    fclose(confeccionarPartidos);
}


void mostrarPartidos() // Funcion que lee el fichero partidos y los muestra por pantalla
{
    FILE *leerPartidos;
    leerPartidos = fopen("partidos.txt", "r");

    if(leerPartidos==NULL)
    {
        printf("No se ha podido abrir correctamente el archivo.\n");
        exit(1);
    }

    if(!(ficheroVacio("partidos.txt")))
    {
        for(int i=0; i<8; i++)
            for(int j=0; j<6; j++)
            {
                fscanf(leerPartidos, "%d ", &partidos[i][j].nro_partido);
                fflush(stdin);
            //Asigna los datos en una misma estructura diferenciando atributo goles y equipos.

                fscanf(leerPartidos, "%d ", &partidos[i][j].goles1);
                fflush(stdin);
                fgets(partidos[i][j].equipo1, 15, leerPartidos);
                strtok(partidos[i][j].equipo1, "\n");

                fscanf(leerPartidos, "%d ", &partidos[i][j].goles2);
                fflush(stdin);
                fgets(partidos[i][j].equipo2, 15, leerPartidos);
                strtok(partidos[i][j].equipo2, "\n");
            }
    }
    else
    {
        printf("El archivo de partidos está vacío");
        system("pause");
        exit(1);
    }

    fclose(leerPartidos);

    printf("           GRILLA DE PARTIDOS MUNDIAL QATAR 2022\n");
    printf("  ___________________________________________________________\n\n");
    for(int i=0; i<8; i++)
        for(int j=0; j<6; j++)
        {
            printf(" |   Partido %*d: ", -2,partidos[i][j].nro_partido);
            printf("%*s %*d", -11, partidos[i][j].equipo1, 3, partidos[i][j].goles1); // Quita 11 espacios hacia atras hasta que la cadena mas larga se empareje con el resto
            printf("    VS    ");
            printf("%*s %*d    |", -11, partidos[i][j].equipo2, 3,  partidos[i][j].goles2);
            printf("\n");
        }
    printf("  ___________________________________________________________\n");
    getchar();
}


void cargarResultados() //Funcion que lee el fichero partidos y pide al usuario que ingrese los goles anotados por cada equipo, para cargarlos/sobre-escribir el fichero de partidos
{
    for(int i=0; i<8; i++) // Bucle que reincia los puntos y goles en caso de re-escribir los resultados de los partidos. Evita que se sumen con los anteriores.
        for(int j=0; j<4; j++)
        {
            posiciones[i][j].puntos=0;
            posiciones[i][j].dif_gol=0;
        }

    FILE *leerPartidos;
    leerPartidos = fopen("partidos.txt", "r");

    if(leerPartidos==NULL)
    {
        printf("No se ha podido abrir correctamente el archivo.\n");
        exit(1);
    }

    if(!(ficheroVacio("partidos.txt")))
    {
        for(int i=0; i<8; i++)
            for(int j=0; j<6; j++)
            {
                fscanf(leerPartidos, "%d ", &partidos[i][j].nro_partido);
                fflush(stdin);

                fscanf(leerPartidos, "%d ", &partidos[i][j].goles1);
                fflush(stdin);
                fgets(partidos[i][j].equipo1, 15, leerPartidos);
                strtok(partidos[i][j].equipo1, "\n");

                fscanf(leerPartidos, "%d ", &partidos[i][j].goles2);
                fflush(stdin);
                fgets(partidos[i][j].equipo2, 15, leerPartidos);
                strtok(partidos[i][j].equipo2, "\n");
            }
    }
    else
    {
        printf("El archivo de partidos está vacío");
        system("pause");
        exit(1);
    }

    fclose(leerPartidos);


    for(int i=0; i<8; i++)
        for(int j=0; j<6; j++)
        {
            printf("%d) %s     vs     %s\n",partidos[i][j].nro_partido, partidos[i][j].equipo1, partidos[i][j].equipo2);

            do // Valida que los goles sean mayores o iguales que 0
            {
                printf("Goles %*s: ", -11, partidos[i][j].equipo1);
                scanf("%d", &partidos[i][j].goles1);
                fflush(stdin);

                printf("Goles %*s: ", -11, partidos[i][j].equipo2);
                scanf("%d", &partidos[i][j].goles2);
                fflush(stdin);

                if(partidos[i][j].goles1 < 0 || partidos[i][j].goles2 < 0)
                    printf("Datos Incorrectos. Intente nuevamente.\n");

            }
            while(partidos[i][j].goles1<0 || partidos[i][j].goles2<0);

            printf("\n");
        }

    FILE *cargarPartidos;
    cargarPartidos = fopen("partidos.txt", "w");

    for(int i=0; i<8; i++)
        for(int j=0; j<6; j++)
        {
            fprintf(cargarPartidos, "%d ", partidos[i][j].nro_partido);
            fflush(stdin);
            fprintf(cargarPartidos, "%d ", partidos[i][j].goles1);
            fflush(stdin);
            fprintf(cargarPartidos, "%s\n", partidos[i][j].equipo1);
            fprintf(cargarPartidos, "%d ", partidos[i][j].goles2);
            fflush(stdin);
            fprintf(cargarPartidos, "%s\n", partidos[i][j].equipo2);
            fprintf(cargarPartidos, "\n");
        }

    fclose(cargarPartidos);
    system("pause");
}


void cargarPosiciones() // Funcion que lee el fichero partido con los goles, los asigna en la matriz estructura posiciones ordenando los equipos en base a los puntos obtenidos. Luego los imprime en un fichero posiciones
{
    FILE *leerPartidos;
    leerPartidos = fopen("partidos.txt", "r");

    if(leerPartidos==NULL)
    {
        printf("No se ha podido abrir correctamente el archivo.\n");
        exit(1);
    }

    if(!(ficheroVacio("partidos.txt")))
    {
        while(!feof(leerPartidos))
            for(int i=0; i<8; i++)
                for(int j=0; j<6; j++)
                {
                    fscanf(leerPartidos, "%d ", &partidos[i][j].nro_partido);
                    fflush(stdin);
                    fscanf(leerPartidos, "%d ", &partidos[i][j].goles1);
                    fflush(stdin);
                    fgets(partidos[i][j].equipo1, 15, leerPartidos);
                    strtok(partidos[i][j].equipo1, "\n");

                    fscanf(leerPartidos, "%d ", &partidos[i][j].goles2);
                    fflush(stdin);
                    fgets(partidos[i][j].equipo2, 15, leerPartidos);
                    strtok(partidos[i][j].equipo2, "\n");
                }
    }

    fclose(leerPartidos);

    for(int grupo=0; grupo<8; grupo++)
    {
        int i=0;
        while(i < 6)// controla el primer indice de la estructura partidos
            for(int j=0; j<3; j++) // i controla el numero de partido del grupo y j el indic del equipo del lado izquierdo
                for(int k=3; k>j; k--) // k es el subindice del equipo del lado derecho
                {
                    if(partidos[grupo][i].goles1 > partidos[grupo][i].goles2) //
                    {
                        posiciones[grupo][j].puntos = posiciones[grupo][j].puntos + 3;
                        posiciones[grupo][j].dif_gol += (partidos[grupo][i].goles1 - partidos[grupo][i].goles2) ;
                        posiciones[grupo][k].dif_gol -= (partidos[grupo][i].goles1 - partidos[grupo][i].goles2) ;
                    }
                    else if(partidos[grupo][i].goles2 > partidos[grupo][i].goles1)
                    {
                        posiciones[grupo][k].puntos = posiciones[grupo][k].puntos + 3;
                        posiciones[grupo][k].dif_gol += (partidos[grupo][i].goles2 - partidos[grupo][i].goles1);
                        posiciones[grupo][j].dif_gol -= (partidos[grupo][i].goles2 - partidos[grupo][i].goles1);
                    }
                    else
                    {
                        posiciones[grupo][j].puntos += 1;
                        posiciones[grupo][k].puntos += 1;
                    }
                    i++;
                }
    }

    for(int i=0; i<8; i++)
        for(int k=0; k<4; k++)
            for(int j=0; j<4-1; j++)
                if(posiciones[i][j].puntos < posiciones[i][j+1].puntos)
                {
                    aux[1] = posiciones[i][j];
                    posiciones[i][j] =  posiciones[i][j+1];
                    posiciones[i][j+1] = aux[1];
                }


    FILE *posicionesGrupo;
    posicionesGrupo = fopen("posiciones.txt", "w");

    for(int i=0; i<8; i++)
        for(int j=0; j<4; j++)
        {
            fprintf(posicionesGrupo, "%d %d %s\n",posiciones[i][j].puntos, posiciones[i][j].dif_gol, posiciones[i][j].equipo);
        }

    fclose(posicionesGrupo);
}

void mostrarPosiciones() // Funcion que lee lee el fichero posiciones y las muestra por pantalla
{
   //UPDATE --> SE BORRA ASIGNACION EN 0 PQ NO SE USABA
    FILE *leerPosiciones;
    leerPosiciones = fopen("posiciones.txt", "r");


    if(leerPosiciones==NULL)
    {
        printf("No se ha podido abrir correctamente el archivo.\n");
        exit(1);
    }

    if(!(ficheroVacio("posiciones.txt")))
    {
        while(!feof(leerPosiciones))
        {
            for(int i=0; i<8; i++)
                for(int j=0; j<4; j++)
                {
                    fscanf(leerPosiciones, "%d ", &posiciones[i][j].puntos);
                    fscanf(leerPosiciones, "%d ", &posiciones[i][j].dif_gol);
                    fflush(stdin);
                    fgets(posiciones[i][j].equipo, 15, leerPosiciones);
                    strtok(posiciones[i][j].equipo, "\n");
                }
        }
    }
    else
    {
        printf("El archivo de posiciones está vacío\n");
        system("pause");
        exit(1);
    }

    fclose(leerPosiciones);

    char letra = 65;

    for(int i=0; i<8; i++)
    {
        int nro = 1;
        printf("\n       Posiciones GRUPO %c\n", letra);
        printf("    Equipo         Pts   DG\n");
        for(int j=0; j<4; j++)
        {
            printf("  %d- %*s  %*d  %*d\n", nro, -11, posiciones[i][j].equipo, 3, posiciones[i][j].puntos,4, posiciones[i][j].dif_gol);
            nro++;
        }
        letra++;
        printf("\n");
    }

    system("pause");
}

bool ficheroVacio(char path[])
{
    //Genero la lectura del fichero, de acuerdo a la ruta pasada
    FILE *file;
    file = fopen( path, "r" );
    //Voy a la ultima posicion del archivo
    fseek( file, 0, SEEK_END );
    //Consulto la posicion del archivo. Si la ultima es 0, está vacío.
    if (ftell( file ) == 0 )
        return true;
    else
        return false;
}
