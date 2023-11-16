/*TIMOFTE Andrei-Ioan - 315CC*/
#include <stdio.h>
#include "culori.h"

// structura pentru nodurile arborelui
typedef struct nod
{
    Culoare elem;
    int level;
    struct nod *fiu[4];
}Nod, *Arb;

// structura pentru simularea unei cozi folosind o lista
typedef struct nodQueue
{
    Arb elem;
    struct nodQueue *next;
}NodQueue, *Lista;

// initializare coada - lista simplu inlantuita cu doua capete
void init_coada(Lista *lst, Lista *final)
{
    *lst = (Lista)malloc(sizeof(NodQueue));
    *final = (Lista)malloc(sizeof(NodQueue));

    // seteaza la NULL
    (*lst)->next = NULL;
    (*lst)->next = NULL;
}

// adauga la finalul listei
void enqueue(Lista *lst, Lista *final, Arb value)
{
    // se aloca memorie
    Lista nou = (Lista)malloc(sizeof(NodQueue));

    // se copiaza valorile
    nou->elem = value;

    nou->next = NULL;
    
    // daca este primul element din coada
    if ((*lst)->next == NULL)
    {
        (*lst)->next = nou;
        (*final)->next = nou;
        return ;
    }

    // se fac legaturile
    (*final)->next->next = nou;
    (*final)->next = nou;
}

// elimina de la inceputul listei
void dequeue(Lista *lst, Lista *final, Arb *pozitie)
{
    Lista aux = (*lst)->next;

    // se copiaza valorile
    *pozitie = aux->elem;

    // cazul cu un singur element in coada
    if ((*lst)->next == (*final)->next)
    {
        (*final)->next = NULL;
        (*lst)->next = NULL;
        free(aux);
        return ;
    }

    (*lst)->next = aux->next;
    aux->next = NULL;
    
    // se sterge nodul
    free(aux);
}

// functie pentru creare unui nod
Arb nodNou(Culoare val, int nivel)
{
    // se aloca memorie
    Arb nou = malloc(sizeof(Nod));

    // se copiaza valorile
    nou->elem.red = val.red;
    nou->elem.blue = val.blue;
    nou->elem.green = val.green;
    nou->level = nivel;

    // se pun pe NULL pointerii intrucat se adauga ca frunza
    nou->fiu[0] = NULL;
    nou->fiu[1] = NULL;
    nou->fiu[2] = NULL;
    nou->fiu[3] = NULL;

    return nou;
}

// functie care modifica valoarea dintr un nod
Arb modifNod(Arb arbore, Culoare val)
{
    arbore->elem.red = val.red;
    arbore->elem.blue = val.blue;
    arbore->elem.green = val.green;
    return arbore;
}

// functie auxiliara pentru calculul nivelului folosind logaritumul 
int log2Level(int aux1, int aux2)
{
    // se foloseste pentru operatia log2(aux1/aux2)
    int contor = 0;
    // se afla log2 din rezultat
    aux1 = aux1 / aux2;
    while (aux1)
    {
        aux1 = aux1 / 2;
        contor++;
    }

    return contor;
}

// functie pentru construirea arborelui pe baza percurgerii matricei
Arb makeArboare(Culoare **imag, Arb arbore, int x, int y, int dim, int limita, int dimConst)
{   
    // se verifica daca a ajuns la dimensiunea 1 si se opreste
    if (dim == 1)
    {
        // se afla nivelul
        int nivel = log2Level(dimConst, 1);
        arbore = nodNou(imag[x][y], nivel);
        
        return arbore;
    }
    else
    {
        Culoare valGeneric;
        // se adauga valoarea generica pentru nodurile interioare
        valGeneric.red = 0;
        valGeneric.blue = 0;
        valGeneric.green = 0;

        int nivel = log2Level(dimConst, dim);
        
        unsigned long long indice;
        Culoare medie;
        
        // se calculeaza indicele
        indice = calcFact(imag, x, y, dim, &medie);
        
        // conditia de oprire cu factorul
        if (indice <= limita)
        {

            // se adauga nodul in arbore
            arbore = nodNou(medie, nivel);
            return arbore;
        }
        else
        {
            // se calculeaza nivelul pentru nodul redundant
            int nivel = log2Level(dimConst, dim);

            // se micsoreaza dimensiunea
            dim = dim - dim / 2;
            
            // se introduce nodul parinte
            arbore = nodNou(valGeneric, nivel);
            
            // se merge la urmatoarea iteratie
            arbore->fiu[0] = makeArboare(imag, arbore->fiu[0], x, y, dim, limita, dimConst);
            
            arbore->fiu[1] = makeArboare(imag, arbore->fiu[1], x, y + dim, dim, limita, dimConst);
            
            arbore->fiu[2] = makeArboare(imag, arbore->fiu[2], x + dim, y + dim, dim, limita, dimConst);
            
            arbore->fiu[3] = makeArboare(imag, arbore->fiu[3], x + dim, y, dim, limita, dimConst);

        }

    }
    return arbore;
}

// functie pentru eliberarea arborelui
void clearArb(Arb *arbore)
{
    Arb aux = *arbore;

    // conditie de oprire
    if (!aux)
        return ;

    // se parcurge recursiv arborele
    clearArb(&((*arbore)->fiu[0]));
    clearArb(&((*arbore)->fiu[1]));
    clearArb(&((*arbore)->fiu[2]));
    clearArb(&((*arbore)->fiu[3]));

    // se elibereaza memoria
    free(aux); 
}

// citeste din fisier intr un vector alocat dinamic toate nodurile
void citireFisier(NodExtins **vect, int *dimVect, FILE *fisier)
{
    // se initializeaza dimensiunea vectorului
    *dimVect = 0;

    // indicele pentru tipul nodului
    unsigned char indice;
    *vect = malloc(sizeof(NodExtins));

    // se parcurge fisierul pana nu mai este nimic in el
    while (fread(&indice, sizeof(unsigned char), 1, fisier) == 1)
    {
        // se incrementeaza dimensiunea
        (*dimVect)++;

        // se realoca dimensiunea
        *vect = realloc(*vect, sizeof(NodExtins) * (*dimVect + 1));
        (*vect)[*dimVect].tipNod = indice;

        // daca este nod frunza in arbore
        if (indice == 1)
        {
            fread(&((*vect)[*dimVect].valoare), sizeof(Culoare), 1, fisier);
        }
        // se verifica daca este nod interior
        else if (indice == 0)
        {
            // se adauga valori generice in vector
            (*vect)[*dimVect].valoare.red = 0;
            (*vect)[*dimVect].valoare.green = 0;
            (*vect)[*dimVect].valoare.blue = 0;

        }
    }

}

// subprogram pentru reconstruirea arborelui
Arb reconstArb(Arb arbore, NodExtins *vect, int dimVect)
{
    Arb aux = NULL;
    Lista lst, final;
    int i = 2, nivel = 1, j, nrelemNiv = 4, nrelemNivUrm = 0, k = 0;
    
    // daca trebuie bagat doar un nod in arbore
    if (vect[1].tipNod == 1)
    {
        arbore = nodNou(vect[1].valoare, 1);
        printf("%hhu", arbore->elem.green);
        return arbore;
    }                      

    Culoare valGeneric;
    // se initializeaza cei 2 pointeri ai cozii
    init_coada(&lst, &final);

    // se adauga valoarea generica pentru nodurile goale
    valGeneric.red = 0;
    valGeneric.blue = 0;
    valGeneric.green = 0;
    
    // se verifica daca vectorul nu are doar un element
    if (vect[1].tipNod == 0)
    {
        arbore = nodNou(valGeneric, 0);
        //enqueue(&lst, &final, arbore);
        arbore->fiu[0] = nodNou(valGeneric, nivel);
        arbore->fiu[1] = nodNou(valGeneric, nivel);
        arbore->fiu[2] = nodNou(valGeneric, nivel);
        arbore->fiu[3] = nodNou(valGeneric, nivel);

        // se baga in coada toti fiii
        enqueue(&lst, &final, arbore->fiu[0]);
        enqueue(&lst, &final, arbore->fiu[1]);
        enqueue(&lst, &final, arbore->fiu[2]);
        enqueue(&lst, &final, arbore->fiu[3]);
    }
    
    // se parcurge pana coada este vida
    while (i <= dimVect)
    {
        nrelemNivUrm = 0;
        // se afla cate noduri sunt pe acest nivel
        for (j = i; j < nrelemNiv + i; j++)
        {
            if (vect[j].tipNod == 0)
                nrelemNivUrm++;
            
        }
        // se calculeaza cate noduri frunza o sa fie pe urmatorul nivel
        nrelemNivUrm = nrelemNivUrm * 4;
        nivel++;
        j = i;
        for (k = j; k < nrelemNiv + j; k++)
        {
            // se scoate din coada
            dequeue(&lst, &final, &aux);
            if (vect[k].tipNod == 0)
            {
                // se adauga noduri noi cu valoarea generica
                aux->fiu[0] = nodNou(valGeneric, nivel);
                aux->fiu[1] = nodNou(valGeneric, nivel);
                aux->fiu[2] = nodNou(valGeneric, nivel);
                aux->fiu[3] = nodNou(valGeneric, nivel);
                
                // se baga in coada
                enqueue(&lst, &final, aux->fiu[0]);
                enqueue(&lst, &final, aux->fiu[1]);
                enqueue(&lst, &final, aux->fiu[2]);
                enqueue(&lst, &final, aux->fiu[3]);
            }
            // daca nu este nod interior se modifica valoarea din nod
            else
            {
                aux = modifNod(aux, vect[k].valoare);                
            }
            
            i++;
        }
        
        nrelemNiv = nrelemNivUrm;
    }
    
    // se elibereaza memoria alocata
    free(lst);
    free(final);
    return arbore;
}

// functie pentru construirea imaginii pe baza arborelui cuaternal
void makeImag(Culoare **imag, Arb arbore, int x, int y, int dim)
{
    // se verifica daca s a ajuns la un nod terminal
    if (arbore == NULL)
    {   
        return;
    }
    else
    {    
        // conditie de oprire daca a ajuns la nod frunza
        if (!arbore->fiu[0])
        {
            // se adauga patratele in matrice in arbore
            addInMatrix(imag, x, y, dim, arbore->elem);
            return ;
        }
        else
        {
            // se injumatateste dimensiunea submatricei
            dim = dim - dim / 2;
            
            // se merge la urmatoarea submatrice
            makeImag(imag, arbore->fiu[0], x, y, dim);
            
            makeImag(imag, arbore->fiu[1], x, y + dim, dim);
            
            makeImag(imag, arbore->fiu[2], x + dim, y + dim, dim);
            
            makeImag(imag, arbore->fiu[3], x + dim, y, dim);
        }
        
    }
}