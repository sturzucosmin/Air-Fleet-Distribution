# Air-Fleet-Distribution

Facultatea de Automatica si Calculatore UPB, specializarea Ingineria sistemelor
Student: Sturzu Cosmin
Grupa: 312AB
AN: I

////Scopul programului: automatizarea rutelor de zboruri intre insulele unui arhipelag  
pentru a asigura distribuția optimă a resurselor

////Pentru fisierele de intrare, respectiv cel de iesire am folosit argumente in linia de 
comanda pentru functia main.

////Exista 2 fisiere: date.in si out.out asociate lui argv[1],argv[2]
pe care le-am atribuit variabilelor de tip "char*" :date si out, urmand sa 
le folosesc in functia "fopen" pentru a deschide fisierele

////Tema este impartita in 3 etape: proiectare, analiza si gestiune; dintre care am rezolvat
numai primele 2 etape.

///////////////////////////////////////////////
/////////////////PROIECTAREA///////////////////
//////////////////////////////////////////////
Eapa de proiectare presupune citirea datelor si rezolvarea a 6 task-uri:


=======CITIRE DATE=======

-->pentru stocarea datelor despre arhipelag am ales sa folosesc o structura de tip GRAF , 
reprezentat prin MATRICE DE ADIACENTA, in structura caruia am adaugat un vector de tipul
[Island *insule], alocat dinamic in care sa stochez informatiile despre insulele 
arhipelagului

-->numarul de insule, care reprezinta numarul de noduri din graf, a fost citit initial in
variabila [int nr_insule], folosita pentru alocarea grafului/matricei de adiacenta
in functia [alocGraph(nr_insule)] (initial toate valorile din matrice sunt 0 ->nu exista 
zboruri)

-->urmeaza citrea informatiilor despre insule

-->urmeaza citirea conexiunilor initiale dintre insule si adaugarea lor in matricea de 
adiacenta 


======REZOLVARE TASK-uri PROIECTARE======
-->citirea cerintei se face intr-o bucla while pana la terminarea fisierului date.in
-->in interiorul buclei urmeaza a fi rezolvate task-urile in functie de numele cerintei

==[conexiune IslandX IslandY]==
-->se citestsc insulele intre care se doreste sa se faca noua conexiune
-->se extrag index-urile insulelor cu functia [find_index()] 
(ex: find_index(Island7, index1) va stoca in variabila int index1 valoare 7)
-->dupa ce stiu index-urile verific matricea de adiacenta si afisez mesajul corespunzator

==[legatura IslandX]==
--> pe acelasi principiu se calculeaza index-ul insulei, dupa care se verifica legaturile
in matricea de adiacenta si se afiseaza legaturile directe cu insulaX

==[adauga_zbor IslandX IslandY cost]==
-->se afla index-urile si se citeste costul, dupa care se fac modificari in matricea de adiacenta

==[anulare_zbor IslandX IslandY]==
-->se afla index-urile si se fac modificari in matricea de adiacenta (valorile aferente
zborului dintre cele 2 indule devin 0)

==[max_resurse]==
-->am creat functia max_resurse(arhipelag, file_r)
-->pentru aceasta cerinta am creat un BST-Arbore binar de cautare, in care am adaugat toate
resursele distincte de pe arhipelag in ordine lexicografica

-->pentru afisarea numarului de resurse am folosit fct. [count_resources(bst)], care returneaza
numarul de noduri din BST-ul in care sunt stocate resursele
-->pentru afisarea resurselor in ordine alfabetica am parcurs arborele in "inordine inversa"
cu functia [reverse_inorder(bst, file_r)]

		

///////////////////////////////////////////////
///////////////////ANALIZA/////////////////////
//////////////////////////////////////////////

//////Aceasta etapa presupune rezolvarea a 3 task-uri: [drum_zbor IslandX IslandY], 
[timp_zbor IslandX IslandY] si [min_zbor IslandX IslandY]
-->se citesc insulele si se afla index-urile
-->pentru toate cele 3 cerinte m-am folosit de algoritmul lui DIJKSTRA pentru calcularea
drumului de cost/timp minim dintre oricare 2 noduri/insule
-->algoritmul calculeaza drumul de cost minim de la indula IslandX la toate celelalte insule

-->ceea ce am particularizat sunt afisarile folosind 3 functii :
				-printSolution() pentru "drum_zbor"
				-print_time() pentru "timp_zbor"
				-print_time2() pentru "min_zbor"

-->aceste functii sunt apelate in functie de variabilele "int timp=0; timp_2=0;" care iau 
valoarea 1 in momentul in care se intra pe cerinta aferenta,
		-->variabila "timp" ia valoarea 1 daca se executa cerinta "timp_zbor"
		-->variabila "timp_2" ia valoarea 1 daca se executa cerinta "min_zbor"



