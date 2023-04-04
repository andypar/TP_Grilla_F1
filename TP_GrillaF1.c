#include <stdio.h>
#define DIM 15
#define CXV 100

struct config{
    char Aplicacion[DIM];
    char Circuito[DIM];
    char Version[DIM];
};

struct corredor{
    int Numero;
    char Nombre[DIM];
    char Apellido[DIM];
    char Escuderia[DIM];
};

struct tiempos {
    int Vuelta;
    int Corredor;
    float Tiempo;
};

int Menu(struct config configuracion);
struct config CargaConfiguracion (FILE* archivo);
void InicializaCorredor(int vec[]);
void LeerCorredores (FILE* archivo, int vec[]);
struct corredor CargarCorredor (FILE* archivo, int vec[], int id);
void ImprimirCorredor(struct corredor UnCorredor);
void CargarVuelta (FILE* archivo, struct tiempos UnaVuelta[], int NVuelta);
void Intercambio (struct tiempos* a, struct tiempos* b);
void OrdenamientoBurbuja(struct tiempos vec[], int tam);
void InicializaTiempo(struct tiempos *vec);
int ObtenerTiemposValidos(struct tiempos vec[]);
void ImprimirVuelta (struct tiempos vec[], FILE* archivoC, int posicion[]);
void MostrarGrilla(FILE* archivo, FILE* archivoC, int posicion[]);
void MejorTiempoenVuelta (FILE* archivoT, FILE* archivoC, int posicion[]);
int ObtenerUbicacionCorredor (struct tiempos vec[], int NCorredor);
void CargarCarrera (FILE* archivo, struct tiempos UnaVuelta[]);
void PeleaPorPosicionVuelta (FILE* archivoT,FILE* archivoC, int posicion[]);
void MejorTiempoVueltaCarrera (FILE* archivoT, FILE* archivoC, int posicion[]);
void FinalCarrera (FILE* archivoT, FILE* archivoC, struct tiempos carrera[], int posicion[]);
struct tiempos ObtenerMejorTiempo (FILE* archivo);
void ImprimirPodio (FILE* archivoT, FILE* archivoC, struct tiempos carrera[], struct config configuracion, int posicion[]);

int main() {

    int opc;
    int ubicacion_corredores[CXV];
    FILE *configuracion;
    FILE *corredores;
    FILE *tiempos;
    struct tiempos tiempo[CXV];
    struct config configurac;
    configurac = CargaConfiguracion(configuracion);
    InicializaCorredor(ubicacion_corredores);
    LeerCorredores(corredores, ubicacion_corredores);
    opc=Menu(configurac);
    while (opc != 0) {
        switch (opc) {
            case 1:
                MostrarGrilla(tiempos, corredores, ubicacion_corredores);
                break;
            case 2:
                MejorTiempoenVuelta(tiempos, corredores, ubicacion_corredores);
                break;
            case 3:
                MejorTiempoVueltaCarrera(tiempos, corredores, ubicacion_corredores);
                break;
            case 4:
                PeleaPorPosicionVuelta(tiempos, corredores, ubicacion_corredores);
                break;
            case 5:
                FinalCarrera(tiempos, corredores, tiempo, ubicacion_corredores);
                break;
            case 6:
                ImprimirPodio(tiempos, corredores, tiempo, configurac, ubicacion_corredores);
                break;
            default:
                printf("Opcion Invalida, ingrese un numero del 1 al 6\n");
        }
        opc=Menu(configurac);
    }
    return 0;
}
int Menu(struct config configuracion){

    int opc;

    printf("%s\n",configuracion.Aplicacion);
    printf("1) Mostrar grilla\n");
    printf("2) Mejor tiempo de vuelta en vuelta X\n");
    printf("3) Mejor tiempo de vuelta en carrera\n");
    printf("4) Pelea por posición en vuelta X\n");
    printf("5) Final de carrera\n");
    printf("6) Imprimir podio\n\n");
    printf("Ingrese Opcion:\n");
    scanf("%d", &opc);
    fflush(stdin);

    return opc;
}

struct config CargaConfiguracion (FILE* archivo){

    struct config Configuracion;

    archivo=fopen("config.txt","r");

    if (archivo != NULL){
        fgets(Configuracion.Aplicacion,DIM,archivo);
        fgets(Configuracion.Circuito,DIM,archivo);
        fgets(Configuracion.Version,DIM,archivo);
        fclose(archivo);
    }
    else{
        printf("Error en la apertura\n");
    }

    return Configuracion;

}

void InicializaCorredor(int vec[]){

    for (int i = 0; i < CXV ; ++i) {
        vec[i]=-1;
    }
}

void LeerCorredores (FILE* archivo, int vec[]){

    int i=0;
    archivo=fopen("corredores.dat","rb");
    struct corredor UnCorredor;

    if(archivo != NULL){

        fread(&UnCorredor,sizeof(UnCorredor),1,archivo);
        while (!feof(archivo)){
            vec[i]=UnCorredor.Numero;
            fread(&UnCorredor,sizeof(UnCorredor),1,archivo);
            i=i+1;
        }
        fclose(archivo);
    } else {
        printf("Error en el archivo\n");
    }
}

struct corredor CargarCorredor (FILE* archivo, int vec[], int id){

    archivo=fopen("corredores.dat","rb");
    struct corredor UnCorredor;

    if(archivo != NULL){

        fread(&UnCorredor,sizeof(UnCorredor),1,archivo);
        for (int i = 0; i <CXV ; ++i) {
            if(vec[i]==id){
                fseek(archivo,sizeof(UnCorredor)*i,SEEK_SET);
                fread(&UnCorredor,sizeof(UnCorredor),1,archivo);
                return UnCorredor;
            }
        }
        fclose(archivo);
    } else {
        printf("Error en el archivo\n");
    }
}

void ImprimirCorredor(struct corredor UnCorredor){

    printf("Nombre: %s, Apellido: %s, Escuderia: %s\n", UnCorredor.Nombre, UnCorredor.Apellido, UnCorredor.Escuderia);
}


void CargarVuelta (FILE* archivo, struct tiempos UnaVuelta[], int NVuelta){

    int i=0;
    archivo=fopen("tiempos.dat","rb");
    struct tiempos VueltaAux;

    if(archivo != NULL){

        fread(&VueltaAux,sizeof(VueltaAux),1,archivo);
        while (!feof(archivo)){
            if (VueltaAux.Vuelta==NVuelta){
                UnaVuelta[i]=VueltaAux;
                i=i+1;
            }
            fread(&VueltaAux,sizeof(VueltaAux),1,archivo);
        }
        fclose(archivo);
    } else {
        printf("Error en el archivo\n");
    }
}


void Intercambio (struct tiempos* a, struct tiempos* b){
    struct tiempos c;

    c=*a;
    *a=*b;
    *b=c;
}

void OrdenamientoBurbuja(struct tiempos vec[], int tam){
    int i, j;
    for (i = 0; i < tam-1; i++) {
        for (j = 0; j < tam-i-1; j++){
            if (vec[j].Tiempo > vec[j+1].Tiempo){
                Intercambio(&vec[j], & vec[j+1]);
            }
        }
    }
}

void InicializaTiempo(struct tiempos *vec){

    for (int i = 0; i < CXV ; ++i) {
        vec[i].Tiempo=-1;
        vec[i].Corredor=-1;
        vec[i].Vuelta=-1;
    }
}

int ObtenerTiemposValidos(struct tiempos vec[]){

    int cant=0;

    for (int i = 0; i < CXV ; ++i) {
        if (vec[i].Tiempo>0){
            cant=cant+1;
        }
    }

    return cant;
}

void ImprimirVuelta (struct tiempos vec[], FILE* archivoC, int posicion[]){

    struct corredor UnCorredor;
    int tam=ObtenerTiemposValidos(vec);

    for (int i = 0; i <tam ; ++i) {
        UnCorredor=CargarCorredor (archivoC, posicion, vec[i].Corredor);
        printf("Vuelta: %d, Num Corredor: %d, Nombre: %s, Apellido: %s, Tiempo: %.3f\n",vec[i].Vuelta, vec[i].Corredor, UnCorredor.Nombre, UnCorredor.Apellido, vec[i].Tiempo);
    }

}

void MostrarGrilla(FILE* archivo, FILE* archivoC, int posicion[]){

    int tam;
    struct tiempos tiempo[CXV];

    InicializaTiempo(tiempo);
    CargarVuelta(archivo, tiempo, 0);
    tam=ObtenerTiemposValidos(tiempo);
    OrdenamientoBurbuja(tiempo,tam);
    ImprimirVuelta(tiempo, archivoC, posicion);
}

void MejorTiempoenVuelta (FILE* archivoT, FILE* archivoC, int posicion[]){

    int Nvuelta;
    int tam;
    struct tiempos tiempo[CXV];

    printf("Ingrese el numero de vuelta que desea ver\n");
    scanf("%d", &Nvuelta);

    InicializaTiempo(tiempo);
    CargarVuelta(archivoT, tiempo, Nvuelta);
    tam=ObtenerTiemposValidos(tiempo);
    OrdenamientoBurbuja(tiempo,tam);
    printf("Mejor tiempo en vuelta %d\n", Nvuelta);
    printf("Tiempo: %.3f, ", tiempo[0].Tiempo);
    ImprimirCorredor(CargarCorredor (archivoC, posicion, tiempo[0].Corredor));
}

int ObtenerUbicacionCorredor (struct tiempos vec[], int NCorredor){

    int tam;
    tam=ObtenerTiemposValidos(vec);

    for (int i = 0; i <tam ; ++i) {
        if (vec[i].Corredor==NCorredor){
            return i;
        }
    }
    return -1;
}

void CargarCarrera (FILE* archivo, struct tiempos UnaVuelta[]){

    archivo=fopen("tiempos.dat","rb");
    struct tiempos VueltaAux;
    int i;
    InicializaTiempo(UnaVuelta);
    CargarVuelta(archivo, UnaVuelta, 1);

    if(archivo != NULL){

        fread(&VueltaAux,sizeof(VueltaAux),1,archivo);
        while (!feof(archivo)){
            if (VueltaAux.Vuelta>1){
                i=ObtenerUbicacionCorredor(UnaVuelta, VueltaAux.Corredor);
                UnaVuelta[i].Tiempo=UnaVuelta[i].Tiempo+VueltaAux.Tiempo;
            }
            fread(&VueltaAux,sizeof(VueltaAux),1,archivo);
        }
        fclose(archivo);
    } else {
        printf("Error en el archivo\n");
    }
}

void PeleaPorPosicionVuelta (FILE* archivoT,FILE* archivoC, int posicion[]){

    int Nvuelta;
    int tam;
    struct tiempos tiempo[CXV];
    struct corredor corredor1;
    struct corredor corredor2;

    printf("Ingrese el numero de vuelta que desea ver:\n");
    scanf("%d", &Nvuelta);

    InicializaTiempo(tiempo);
    CargarVuelta(archivoT, tiempo, Nvuelta);
    tam=ObtenerTiemposValidos(tiempo);
    OrdenamientoBurbuja(tiempo,tam);

    for (int i = 0; i <tam-1 ; ++i) {
        if (tiempo[i+1].Tiempo - tiempo[i].Tiempo < 0.01){
            corredor1=CargarCorredor (archivoC, posicion, tiempo[i].Corredor);
            corredor2=CargarCorredor (archivoC, posicion, tiempo[i+1].Corredor);
            printf("%s y %s pelean por la posicion %d\n", corredor1.Apellido, corredor2.Apellido, i+1);
        }
    }
}

void FinalCarrera (FILE* archivoT, FILE* archivoC, struct tiempos carrera[], int posicion[]){

    int tam;
    CargarCarrera(archivoT,carrera);
    tam=ObtenerTiemposValidos(carrera);
    OrdenamientoBurbuja(carrera,tam);

    for (int i = 0; i <tam ; ++i) {
        printf("Tiempo final %.3f, ", carrera[i].Tiempo);
        ImprimirCorredor(CargarCorredor (archivoC, posicion, carrera[i].Corredor));
    }

}

struct tiempos ObtenerMejorTiempo (FILE* archivo){

    archivo=fopen("tiempos.dat","rb");
    struct tiempos VueltaAux;
    struct tiempos corredor;

    if(archivo != NULL){

        fread(&VueltaAux,sizeof(VueltaAux),1,archivo);
        float mintiempo=99999;
        while (!feof(archivo)){
            if (VueltaAux.Vuelta>0){
                if(VueltaAux.Tiempo <mintiempo){
                    mintiempo=VueltaAux.Tiempo;
                    corredor=VueltaAux;
                }
            }
            fread(&VueltaAux,sizeof(VueltaAux),1,archivo);
        }
        fclose(archivo);
    } else {
        printf("Error en el archivo\n");
    }
    return corredor;
}

void MejorTiempoVueltaCarrera (FILE* archivoT, FILE* archivoC, int posicion[]){

    struct tiempos TiempoMasRapido;

    TiempoMasRapido=ObtenerMejorTiempo(archivoT);

    printf("Mejor tiempo en carrera %.3f\n", TiempoMasRapido.Tiempo);
    ImprimirCorredor(CargarCorredor (archivoC, posicion, TiempoMasRapido.Corredor));

}

void ImprimirPodio (FILE* archivoT, FILE* archivoC, struct tiempos carrera[], struct config configuracion, int posicion[]){


    FILE* archivo;
    archivo=fopen("podio.txt","w");
    int tam;
    struct tiempos TiempoMasRapido;
    struct corredor CorredorMasRapido;
    struct corredor UnCorredor;

    CargarCarrera(archivoT,carrera);
    tam=ObtenerTiemposValidos(carrera);
    OrdenamientoBurbuja(carrera,tam);

    TiempoMasRapido=ObtenerMejorTiempo(archivoT);
    CorredorMasRapido=CargarCorredor (archivoC, posicion, TiempoMasRapido.Corredor);

    if (archivo != NULL) {
        fprintf(archivo, "Podio de %s\n", configuracion.Circuito);
        for (int i = 0; i < 3; ++i) {
            fprintf(archivo, "%d° ", i + 1);
            fprintf(archivo, "Tiempo final %.3f, ", carrera[i].Tiempo);
            UnCorredor = CargarCorredor(archivoC, posicion, carrera[i].Corredor);
            fprintf(archivo, "Nombre: %s, Apellido: %s, Escuderia: %s\n", UnCorredor.Nombre, UnCorredor.Apellido,
                    UnCorredor.Escuderia);
        }
        fprintf(archivo, "Corredor mas rapido: ");
        fprintf(archivo, "Tiempo: %.3f, Vuelta: %d, Nombre: %s, Apellido: %s, Escuderia: %s\n", TiempoMasRapido.Tiempo,
                TiempoMasRapido.Vuelta, CorredorMasRapido.Nombre, CorredorMasRapido.Apellido,
                CorredorMasRapido.Escuderia);
        fclose(archivo);
    } else {
        printf("Error en el archivo\n");
    }
}