#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#define leng "c.txt"
#define boca "bocajuniors.txt"
#define idDocArch "idDoc.bin"

/// ESTRUCTURAS

typedef struct
{
    char palabra[20];
    int idDOC;
    int pos; //incrementa palabra por palabra, y no letra por letra
} termino;

/// PROTOTIPADOS
// Primera parte
void inicIdDoc();
int cantDatosArch();
void agregaCaracterAPalabra(char*, char);
void leerArchivo(termino*, int*, char*);
// Segunda parte
void escrituraDiccionario(termino*, int);
void leer();

int main()
{
   // inicIdDoc();

    termino *arreglo;
    int validos = 0;
    arreglo = (termino*)malloc(sizeof(termino)*cantDatosArch());

    leerArchivo(arreglo, &validos, boca);

    printf("%s\n", arreglo[0].palabra);     /// MUESTRA MAL LA PRIMERA PALABRA

    for (int i = 0; i < validos; i++)
    {
        printf("____________________________\n");
        printf("Palabra: %s \n", arreglo[i].palabra);
        printf("Id: %i \n", arreglo[i].idDOC);
        printf("Posicion: %i \n", arreglo[i].pos);
        printf("____________________________\n");
    }

    printf("Validos: %i\n", validos);
    printf("Cant: %i\n", cantDatosArch());

    printf("----------- ARCHIVO DICCIONARIO -----------\n");

    escrituraDiccionario(arreglo, validos);
    leer();



    return 0;
}

int cantDatosArch()
{
    FILE* puntArch = fopen(boca, "rb");

    int total = 0;

    if(puntArch != NULL)
    {
        fseek(puntArch, 0, SEEK_END);
        total = ftell(puntArch)/sizeof(char);
        fclose(puntArch);
    }
    return total;
}

/// ------------- Diccionario -------------
/// Se tienen N documentos, donde se leerán todas las palabras de cada uno de ellos.
/// Los documentos deben ser archivos de texto leídos como un binario (char)), sobre un vector que contendrá cada palabra,
/// id de documento al que pertenece, y posición en dicho documento.

void agregaCaracterAPalabra(char* palabras, char caracter)
{
    int i = 0;

    while (palabras[i] != '\0')
    {
        i++;
    }

    palabras[i] = caracter;
    palabras[i+1] = '\0';
}

void inicIdDoc ()
{
    FILE* fp = fopen (idDocArch, "wb");

    int idDoc = 0;

    if (fp)
    {
        fwrite(&idDoc, sizeof(int), 1, fp);

        fclose(fp);
    }
    else
        printf("ERROR.\n");
}

void leerArchivo(termino* terminos, int* validos, char* nomArch)
{
    FILE* fpIdDoc = fopen(idDocArch, "r+b");
    FILE* puntArch = fopen(nomArch, "rb");

    int idDoc;

    int flag = 0, pos = 0;   // variable para saber si se leyó un caracter raro

    char palabra[20], caracter;
    memset(palabra, 0, 20*sizeof(palabra[0]));

    if(fpIdDoc != NULL && puntArch != NULL)
    {
        fread(&idDoc, sizeof(int), 1, fpIdDoc);
        idDoc++;
        fseek(fpIdDoc, -1*sizeof(int), SEEK_CUR);
        fwrite(&idDoc, sizeof(int), 1, fpIdDoc);

        while(fread(&caracter, sizeof(char), 1, puntArch) > 0)
        {
//                    (65 a 90 MAYUSCULAS)                   (97 a 122 MINUSCULAS)
            if((caracter >= 65 && caracter <= 90) || (caracter >= 97 && caracter && 122) || (caracter >= 160 && caracter <= 165))   // AGREGAR ACENTOS
            {
                flag = 0;
                agregaCaracterAPalabra(palabra, caracter);
            }
            else
            {
                if(flag == 0)
                {
                    flag = 1;

                    strcpy(terminos[*validos].palabra, palabra);
                    terminos[*validos].idDOC = idDoc;
                    terminos[*validos].pos = pos;
                    pos++;

                    (*validos)++;

                    //     palabra[0] = '\0';      // VER OTRA FORMA PARA ESTO
                    memset(palabra, 0, 20*sizeof(palabra[0]));  // Inicializa todas las celdas en 0
                }
            }
        }

        fclose(fpIdDoc);
        fclose(puntArch);
    }
    else
    {
        printf("Error de archivo\n");
    }
}

/// Una vez finalizada la lectura de los documentos, se procede a generar el diccionario.
/// El diccionario es un archivo binario de registros de tipo término

void escrituraDiccionario(termino* terminos, int validos)
{
    FILE* puntArch = fopen("diccionario.bin", "ab");

    int i = 0;

    if(puntArch != NULL)
    {
        while(i < validos)
        {
            fwrite(&terminos[i], sizeof(termino), 1, puntArch);
            i++;
        }
        fclose(puntArch);
    }
}

void leer()
{
    FILE* puntArch = fopen("diccionario.bin", "rb");

    termino palabra;

    if(puntArch != NULL)
    {
        while(fread(&palabra, sizeof(termino), 1, puntArch) > 0)
        {
            printf("____________________________\n");
            printf("Palabra: %s \n", palabra.palabra);
            printf("Id: %i \n", palabra.idDOC);
            printf("Posicion: %i \n", palabra.pos);
            printf("____________________________\n");
        }
        fclose(puntArch);
    }
}
