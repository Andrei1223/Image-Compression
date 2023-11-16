# HW 2 Image Compression using Quaternary Trees

## INTRODUCERE
Pentru rezolvarea cerintei a trebuit sa implementez un arbore care are
4 fii, iar pentru a face asta am ales ca fiii sa fie reprezentati printr un
vector de pointeri, acesta fiind indexat de la 0.

## IMPLEMENTARE

--> Cerinta 1 - Pentru rezolvarea cerintei s a folosit o matrice de
structuri alocata dinamic pentru a se citi imaginea. Apoi se parcurge
fiecare submatrice din matrice pentru a se construi arborele.

Acestea sunt parcurse pana se ajunge la submatrice care au un singur element
sau factorul de compresie asociat submatricei nu este potrivit.

Aceste operatii sunt folosite atat pentru cerinta 1 cat si pentru 
cerinta 2, fiind diferita doar parcurgerea bsf aflata in functiile
"cerinta1" si "cerinta2".

Dupa construirea arborelui de compresie, se parcurge acesta folosind o
parcurgere pe nivel pentru a determina informatiile cerute

--> Cerinta 2 - Difera fata de cerinta 1 doar la parcurgerea de final,
unde se afiseaza intr un fisier nodurile arborelui parcurs pe nivel.

--> Cerinta 3 - In aceasta cerinta se refac pasii de la cerinta 2 si, astfel,
marea majoritate a functiilor folosite in decomprimarea imaginii sunt, functii
inverse pentru cele foloste la cerinta 2.

Initial se verifica in "main" daca este prezent argumentul "-d" in apelarea
executabilului, in caz afirmativ este apelata functia "cerinta3" in care se
afla integral rezolvarea cerintei prin apelarea altor functii ajutatoare.
Ordinea pasilor de apelare a functiilor este urmatorul:

citireFisier --->reconstArb--->makeImag--->clearImag--->clearArb;

Se citeste din fisierul de input, intr un vector, nodurile arborelui parcurs
linie cu linie, pentru a fi mai usor de parcurs cand este construit arborele
pe baza nodurilor date. Apoi, este apelat subprogramul de construire a 
arborelui, care parcurge vectorul de noduri in functie de numarul de noduri
terminale.

Daca tipul nodului este 0 inseamna ca este un nod interior si prin urmare,
acesta are automat 4 fii. Astfel, la fiecare iteratie se numara numarul de 
noduri interioare, se inmulteste cu 4 si asa se determina numarul de elemente
care terbuiesc citite din vector la urmatoarea iteratie(inturcat atatea noduri
se afla pe urmatoarul nivel).

Este important de precizat ca elementele vectorului sunt indexate de la 1

Spre exemplu daca in vector se afla valorile : 0, 1, 1, 1, 0, 1, 1, 1, 1;

numarul de noduri de pe primul nivel este automat setat pe 4, iar cel pt
urmatoarea iteratie pe 0. (intrucat daca se intampla ca imaginea sa aiba numai
pixeli de aceeasi culoare este verificat inainte de bucla)

iteratia 1 : se parcurge vectorul de la 2 la 5 si se observa ca este doar un 0,
de unde reiese ca la urmatoarea iteratie o sa fie 1 * 4 elemente de citit.

iteratia 2 : se parcurge vectorul de la 6 la 9 si se observa ca nu mai este
niciun 0, astfel dupa nu se mai citeste nimic.

Aceasta metoda de parcurgere a vectorului imi permite sa parcurg BFS vectorul 
de noduri in acelasi timp cu arborele, si astfel sa introduc corect fiecare 
valoare in arbore. Totodata, pentru nodurile interioare s a volosit valoarea
generica pentru valoarea culorii (0 ,0 ,0).


## PRECIZARI
1) Cerinta 1, fiind atat de asemanatoare cu cerinta 2 (in ceea ce priveste
prelucrarea informatiilor primite din fisier) au la baza aceeasi metoda de 
parcurgere a arborelui (BFS) doar cu niste mici modificari. Astfel, functiile
"cerinta1" si "cerinta2" sunt aproape identice cu exceptia elementelor afisate
in fisier. 

2) Pentru usurinta in ceea ce priveste rezolvarea cerintelor, am decis ca
structura "nod" care defineste arborele, sa aiba inca un element in plus care
tine cont de nivelul pe care se afla fiecare nod, raportat la nodul radacina.
Astfel,  

3) Pentru implementarea algoritmului BFS care este prezent in toate 
parcurgerile arborelui, am ales pe post de coada folosirea unei liste
simplu inplantuite cu 2 pointeri (unul spre inceputul listei si
celalalt spre finalul listei).


## FISIERE

Pentru rezolvarea cerintei am facut 2 headere separat care contin functiile de
adaugare si eliminare specifice structurii lor. Totodata, mai exista un fisier
care contine doar implementarea comenzilor date in enunt.

1. culori.h

--> citireImag - functie care realizeaza citirea imaginii din fisierul de tip
                 ppm, matricea se aloca dinamic; aceasta functie este folosita
                 in rezolvarea primelor 2 cerinte

--> clearImag - functie care are rolul de a eliberea memoria alocata din
                matricea care contine pixelii din imagine

--> calcFact -  functie care are rolul de a calcula pentru o submatrice 
                valoarea medie si factorul mean, valorile medii sunt transmise
                prin intermediul pointerilor, iar factorul este returnat de
                functie      

2. arbore.h

--> init_coada - functie care este folosita pentru initializarea cozii,
                 utilizata pentru BFS

--> enqueue - functie care este folosita pentru adaugarea unui nou element in
              coada se adauga la finalul listei simplu inlantuite noul element

--> dequeue - functie care este folosita pentru stergerea primului element din 
              coada

--> nodNou - functie care are rolul de a adauga un nou element in arborele de
             compresie, alocandu se memorie pentru un nod nou si initializand
             toti pointerii cu null

--> modifNod - functie care are rolul de a modifica valoarea dintr un nod al
               arborelui

--> log2Level - functie care calculeaza logaritmul in baza 2 pentru raportul
                a doua numere, cu scopul de a determina livelul pe care se
                afla un nod din arbore

--> makeArboare - functie care construieste arborele pornind de la matricea
                  care contine pixelii imaginii. Aceasta functie este folosita
                  in a doua cerinta. Se parcurge recursiv matricea, mergand pe
                  fiecare submatrice din aceasta, pana se ajunge ori la
                  submatricea cu dimensiune 1 ori factorul mean nu este mai
                  mare decat cel dat

--> clearArb - functie folosita pentru parcurgerea recursiva a arborelui si 
               eliberarea memoriei din acesta

--> citireFisier - functie care are rolul de a citi intr un vector alocat 
                   dinamic toate elementele aflate in fisierul comprimat.
                   Vectorul este folosit in cadrul functiei "reconstArb" 
                   pentru a usura construirea arborelui de compresie

--> reconstArb - functie care reface arborele de compresie pornind de la 
                 informatiile date in fisierul de input pentru cerinta 3. 
                 Astfel se parcurge bfs arborele si in acelasi timp si vectorul
                 tinandu se cont de tipul nodului 0 sau 1. In arbore se adauga
                 initial doar noduri cu valori generice (0,0,0) urmand sa fie 
                 modificata valoarea daca se constata ca este nod frunza
                
--> makeImag - functie care reface imaginea pornind de la arborele cuaternal
