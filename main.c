/*TIMOFTE Andrei-Ioan - 315CC*/

#include <string.h>
#include "arbore.h"

// functie pentru rezolvarea primei cerinte
void cerinta1(Arb arbore, char *nume, int dimensiune)
{
    Arb aux;
    Lista lst, final;
    FILE *fisier = fopen(nume, "w");

    // se initializeaza cei 2 pointeri ai cozii
    init_coada(&lst, &final);

    enqueue(&lst, &final, arbore);
    // valorile dorite pentru prima cerinta
    int niveluri = 0, nrPatrat = 0;
    int valMin = dimensiune;
    // se parcurge pana coada este vida
    while (final->next != NULL)
    {
        dequeue(&lst, &final, &aux);

        // se afla nr de nivele
        if (aux->level > niveluri)
            niveluri = aux->level;
        // daca este nod frunza
        if (aux->fiu[0] == NULL)
        {
            nrPatrat++;
            // se afla blocul de pe cel mai mic nivel
            if (aux->level < valMin)
                valMin = aux->level;
        }
        
        // se adauga in coada daca nu este frunza
        if (aux->fiu[0] != NULL)
            enqueue(&lst, &final, aux->fiu[0]);
        if (aux->fiu[1] != NULL)
            enqueue(&lst, &final, aux->fiu[1]);
        if (aux->fiu[2] != NULL)
            enqueue(&lst, &final, aux->fiu[2]);
        if (aux->fiu[3] != NULL)
            enqueue(&lst, &final, aux->fiu[3]);
    }
    valMin--;
    valMin = 1 << valMin;
    valMin = dimensiune / valMin;

    // se afiseaza informatiile cerute
    fprintf(fisier, "%d\n%d\n%d\n", niveluri, nrPatrat, valMin);

    // se elibereaza memoria alocata
    free(lst);
    free(final);

    // se inchide fisierul
    fclose(fisier);
}

// functie pentru rezolvarea cerintei 2
void cerinta2(Arb arbore, char *nume, int dimensiune)
{
    // se deschide fisierul
    FILE *fisier = fopen(nume, "wb");
    // o se se parcurga pe linii arborele 
    Arb aux;
    Lista lst, final;
    unsigned int dimImag = (unsigned int )dimensiune;
    // se afiseaza dimensiunea imaginii
    fwrite(&dimImag, sizeof(unsigned int), 1, fisier);

    // se initializeaza cei 2 pointeri ai cozii
    init_coada(&lst, &final);

    enqueue(&lst, &final, arbore);

    // se parcurge pana coada este vida
    while (final->next != NULL)
    {
        dequeue(&lst, &final, &aux);

        // se verifica daca este nod frunza
        if (aux->fiu[0] == NULL)
        {
            unsigned char auxnum = 1;
            fwrite(&auxnum, sizeof(unsigned char), 1, fisier);
            fwrite(&(aux->elem), sizeof(Culoare), 1, fisier);
        }
        else
        {
            unsigned char auxnum = 0;
            fwrite(&auxnum, sizeof(unsigned char), 1, fisier);
        }
            

        // se adauga in coada daca nu este frunza
        if (aux->fiu[0] != NULL)
            enqueue(&lst, &final, aux->fiu[0]);
        if (aux->fiu[1] != NULL)
            enqueue(&lst, &final, aux->fiu[1]);
        if (aux->fiu[2] != NULL)
            enqueue(&lst, &final, aux->fiu[2]);
        if (aux->fiu[3] != NULL)
            enqueue(&lst, &final, aux->fiu[3]);
    }
    
    // se elibereaza memoria alocata
    free(lst);
    free(final);
    
    // se inchide fisierul
    fclose(fisier);
}

// functie pentru rezolvarea cerintei 3
void cerinta3(Arb arbore, char *numeInp, char *numeOutp)
{
    // se cheschide fisierul de input
    FILE *fisierOut = fopen(numeOutp, "wt");
    FILE *fisierIn = fopen(numeInp, "rb");

    // se defineste matricea
    Culoare **imag = NULL;
    arbore = NULL;
    NodExtins *vect = NULL;
    int dimensiuneVect, dimensiuneImag, i, j;
    fread(&dimensiuneImag, sizeof(unsigned int), 1, fisierIn);

    citireFisier(&vect, &dimensiuneVect, fisierIn);
    // se aloca memorie pentru imagine
    imag = malloc(sizeof(Culoare *) * dimensiuneImag);
    for (i = 0; i < dimensiuneImag; i++)
    {
        imag[i] = malloc(sizeof(Culoare) * dimensiuneImag);
    }

    // se construieste arborele
    arbore = reconstArb(arbore, vect, dimensiuneVect);
    
    makeImag(imag, arbore, 0, 0, dimensiuneImag);

    fprintf(fisierOut, "P6\n%d %d\n255\n", dimensiuneImag, dimensiuneImag);

    // se afiseaza matricea in fisier
    for (i = 0; i < dimensiuneImag; i++)
    {
        for (j = 0; j < dimensiuneImag; j++)
        {
            fwrite(&imag[i][j], sizeof(Culoare), 1, fisierOut);
        }
    }
    // se elibereaa memoria matricei
    clearImag(imag, dimensiuneImag);

    // se elibereaza memoria
    free(vect);
    clearArb(&arbore);
    // se inchide fisierul
    fclose(fisierIn);
    fclose(fisierOut);
}
int main(int argc, char **argv)
{
    Culoare **imag;
    int dimensiune;
    Arb arbore = NULL;

    // se verifica daca este cerinta 3
    if (strcmp(argv[1], "-d") == 0)
    {
        // se citeste arborele si se reconstituie imaginea
        cerinta3(arbore, argv[argc - 2], argv[argc - 1]);
       
    }
    // se verifica daca este cerinta 1 sau 2
    else
    {
        // se citeste imaginea si se construieste matricea
        citireImag(&imag, &dimensiune, argv[argc - 2]);
        int factor = atoi(argv[argc - 3]);
        
        arbore =makeArboare(imag, arbore, 0, 0, dimensiune, factor, dimensiune);

        
        // se afiseaa in functie de cerinta
        if (strcmp(argv[1], "-c1") == 0)
        {
            // se afiseaza rezultatele cerute in fisier
            cerinta1(arbore, argv[argc - 1], dimensiune);
        }
        // se verifica daca este a doua cerinta
        else
        {
            // se afiseaza in fisier, imaginea comprimata
            cerinta2(arbore, argv[argc - 1], dimensiune);
            
        }

        // se elibereaza memorie alocata matricei
        clearImag(imag, dimensiune);

        // se elibereaza memoria alocata arborelui
        clearArb(&arbore);
    }
    
    return 0;
}