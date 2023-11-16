/*TIMOFTE Andrei-Ioan - 315CC*/
#include <stdlib.h>

// se defineste structura care reprezinta un pixel
typedef struct culoare
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} Culoare;


// se defineste structura pentru citirea din fisierul cu imag comprimat
typedef struct nodExtins
{
    int tipNod;
    Culoare valoare;
}NodExtins;

// functie pentru citirea matricei din fisierul de tip PPM
void citireImag(Culoare ***imag, int *dim, char *nume)
{
    // se deschide fisierul
    FILE *fisier = fopen(nume, "r");

    // se citeste formatul si dimensiunea
    char aux[4];
    fscanf(fisier, "%s", aux);

    // se citeste de 2 ori fiind aceeasi valoare
    fscanf(fisier, "%d %d", dim, dim);

    // se citeste valoarea maxima
    int valMax;
    fscanf(fisier, "%d", &valMax);
    fgetc(fisier);

    // se aloca memorie pentru imagine
    int i;
    *imag = (Culoare **)malloc(sizeof(Culoare *) * (*dim));
    for (i = 0; i < *dim; i++)
        (*imag)[i] = malloc(sizeof(Culoare) * (*dim));

    // se citeste din fisier
    for (i = 0; i < *dim; i++)
    {
        fread((*imag)[i], sizeof(Culoare), *dim, fisier);
    }

    // se inchide fisierul
    fclose(fisier);
}

// functie pentru eliberarea memoriei din matrice
void clearImag(Culoare **imag, int dim)
{
    // se parcurge pe linii
    int i;
    for (i = 0; i < dim; i++)
    {
        // eliberare linii din matrice
        free(imag[i]);
    }

    // se elibereaza pointerul
    free(imag);
}

// adauga in matrice culorile primite ca parametru
void addInMatrix(Culoare **imag, int x,int y, int dim, Culoare valoare)
{
    int i, j;
    for (i = x; i < x + dim; i++)
    {
        for (j = y; j < y + dim; j++)
        {
            imag[i][j].blue = valoare.blue;
            imag[i][j].green = valoare.green;
            imag[i][j].red = valoare.red;
        }
    }
}

// returneaza gredul de similaritate dintr o submatrice
unsigned long long calcFact(Culoare **imag, int x, int y, int lung, Culoare *medie)
{
    // se initializeaza auxiliare intrucat suma ar depasi 1 octet
    unsigned long long medieRed, medieBlue, medieGreen;
    medieRed = 0;
    medieGreen = 0;
    medieBlue = 0;

    int i, j;
    unsigned long long indice = 0;

    // se calculeaza media
    for ( i = x; i < x + lung; i++)
    {
        for (j = y; j < y + lung; j++)
        {
            medieRed += imag[i][j].red;
            medieBlue += imag[i][j].blue;
            medieGreen += imag[i][j].green;
        }    
    }
    
    unsigned long long aux = lung * lung;
    // se imparte la lungime
    medieRed = medieRed / aux;    
    medieGreen = medieGreen / aux;
    medieBlue = medieBlue / aux;
   
    // se calculeaza indicele
    for ( i = x; i < x + lung; i++)
    {
        for (j = y; j < y + lung; j++)
        {
            // se calculeaza conform formulei prezentate in cerinta
            indice += (medieRed - imag[i][j].red)*(medieRed - imag[i][j].red);
            indice += (medieGreen - imag[i][j].green)*(medieGreen - imag[i][j].green);
            indice += (medieBlue - imag[i][j].blue)*(medieBlue - imag[i][j].blue);
        }    
    }
    indice = indice / (3 * lung * lung);

    // se copiaza in polinter valoarea mediei
    medie->blue = medieBlue;
    medie->red = medieRed;
    medie->green = medieGreen;
    // se returneaza indicele dorit
    return indice;
}
