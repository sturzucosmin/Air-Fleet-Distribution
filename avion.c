#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<limits.h>

typedef struct Resursa {
    char *nume;
    int cantitate;
} Resursa;

typedef struct Island {
    char *nume;
    int nrResurse;
    Resursa *inventarResurse;
    int nrAvioane;
    int *avioane;
    int tolAvioane;
}Island;

//STRUCTURA PENTRU GRAF IN CARE AM ADAUGAT UN VECTOR DE INSULE PENTRU A 
//STOCA DATELE DESPRE ELE
typedef struct graphArhipelag
{
    int nr_insule, nr_zboruri;
    int **matrice_zboruri;
    Island *insule;
}graphArhipelag;

//STRUCTURA FOLOSITA PENTRU CERINTA "max_cantitate"
typedef struct cantitate_maxima {
    int indice;
    int cantitate;
}cantitate_maxima;

graphArhipelag * alocGraph (int val){ // functie pentru alocat spatiu pentru un graf,
    int i,j ; // fara initializare arce
    graphArhipelag *g = malloc(sizeof(graphArhipelag));
    
    if (g==NULL) return NULL;
    
    (g->nr_insule) = val;
    
    g->matrice_zboruri = malloc(sizeof(int*)*g->nr_insule);
    
    for (i = 0; i < g->nr_insule; i++) //TOATE ZBORURILE LE INITIALIZEZ CU 0
        g->matrice_zboruri[i] = calloc(g->nr_insule, sizeof(int));

    return g;
}

//GASESTE INDEXUL UNEI INSULE ex: find_index(Island7, index) => index = 7
void find_index(char *insula, int *index) {
	int i = 0;
	char *end;
	while (!isdigit(insula[i])) {
		i++;
	}
	*index = strtol(insula + i, &end, 10);
}

struct N{//NOD ARBORE FOLOSIT PENTRU CERINTA "max_resurse"
    char* resource;
    struct N *left, *right;
};
typedef struct N NodeBST;

NodeBST* newNode(char* new_resource) { 
    NodeBST* node = (NodeBST*)malloc(sizeof(NodeBST));
    node->resource = new_resource;
    node->left = node->right = NULL;
    return node;
}

NodeBST* insert(NodeBST* node, char* new_resource) { //ADAUGA O RESURSA IN GRAF IN ORDINE LEXICOGRAFICA
    if (node == NULL) 
        return newNode(new_resource);

    int i1 = strcmp(new_resource, node->resource);//COMPAR RESURSA NOUA CU NODUL CURENT 

    if (i1 > 0) { //DACA ESTE MAI MARE DPDV LEXICOFRAFIC SE ADAUGA IN STANGA 
        node->left = insert(node->left, new_resource);
    }
    else if (i1 < 0){//DACA ESTE MAI MIC DPDV LEXICOGRAFIC SE ADAUGA IN DREAPTA
        node->right = insert(node->right, new_resource);
    }
        else if(i1 == 0){ //DACA NOUA RESURSA ESE DEJA IN ARBORE, VA RAMANE ADAUGATA O SINGURA DATA
            node->resource = new_resource;
        }
    return node;
}

//NUMARA NODURILE DIN ARBORE, ADICA NR. DE RESURSE DISTINCTE
int count_resources(NodeBST *root)
{
    int counter =  1; 
    if (root ==NULL)
        return 0;
    else
    {
        counter = counter + count_resources(root->left);
        counter = counter + count_resources(root->right);
        return counter;
    }
}

//AFISEAZA RESURSELE IN ORDINE ALFABETICA
void reverse_inorder(NodeBST *root, FILE* file_r) {
        if (root){                                 
        reverse_inorder(root->right, file_r);       
        fprintf(file_r,"%s ", root->resource);
        reverse_inorder(root->left, file_r);
    }
}

//functie pentru cerinta "max_resurse"
void max_resurse(graphArhipelag *arhipelag, FILE *file_r) {
    int counter;
    NodeBST *bst_resource = NULL;
    int i, j;

    for(i = 0; i < arhipelag->nr_insule; i++) {//adaug resursele in arbore cu functia "insert()"
        for(j = 0; j < arhipelag->insule[i].nrResurse; j++) {
            bst_resource = insert(bst_resource, arhipelag->insule[i].inventarResurse[j].nume);
        }
    }

    counter = count_resources(bst_resource);//calculez nr. de resurse/noduri din arbore
    fprintf(file_r, "%d ", counter);       //si il afisez 
    reverse_inorder(bst_resource, file_r);  //afisez resursele alfabetic 
    fprintf(file_r, "\n");

    free(bst_resource);
}

int minDistance(int dist[], int sptSet[], int nr_insule) 
{ 
    int min = INT_MAX, min_index; 
  
    for (int v = 0; v < nr_insule; v++) 
        if (sptSet[v] == 0 && dist[v] <= min) 
            min = dist[v], min_index = v; 
  
    return min_index; 
} 

//Afiseaza ruta pentru costul minim
void printPath(int parent[], int j, FILE* file_r) {
    if (parent[j] == - 1) 
        {   
            return;
        } 
    printPath(parent, parent[j], file_r); 
    fprintf(file_r, "Island%d ", j+1); 
} 

//numar insulele prin care a aterizat un avion pentru a calcula 
//timpul la cerinta "min_zbor"
void count_landings(int parent[], int j, int *landings) {
    if (parent[j] == - 1) 
        {   
            return;
        } 
    count_landings(parent, parent[j], landings); 
    (*landings)++;
} 

//functie pentru afisarea solutiei la cerinta "drum_zbor"
void printSolution(int dist[], int parent[], int nr_insule, int index2, FILE* file_r) 
{
        for (int i = 0; i < nr_insule; i++) {
            if(index2 == i)
            {
                if(dist[i] == INT_MAX){
                   fprintf(file_r,"NO\n");
                   goto here;
                }
                printPath(parent, i, file_r);
            }
    }
    fprintf(file_r,"\n");
    here: ;
}

//functie pentru afisarea timpului la cerinta "timp_zbor"
void print_time(int dist[], int parent[], int nr_insule, int index2, FILE* file_r) 
{ 
        for (int i = 0; i < nr_insule; i++) {
            if(index2 == i)
            {
                if(dist[i] == INT_MAX){
                   fprintf(file_r,"INF\n");
                   break;
                }
                fprintf(file_r, "%d\n", dist[i]); 
            }
    }
} 

//functie pentru afisarea timpului la cerinta "min_zbor"
void print_time2(int dist[], int **zboruri,int parent[], int nr_insule,int index1, int index2, FILE* file_r) 
{ 
        int landings = 0;
        for (int i = 0; i < nr_insule; i++) {
            if(index2 == i)
            {
                if(dist[i] == INT_MAX){
                   fprintf(file_r,"INF\n");
                   break;
                }
                count_landings(parent, i, &landings); //aflu numarul de aterizari pentru a calcula 
                                                        //timpul necesar 
                //daca exista o ruta directa intre insula care este mai rapida decat drumul de minim (+15 min/aterizare)
                //afisez timpul pentru ruta directa
                if(dist[i]+(15*(landings-1)) > zboruri[index1][index2] && zboruri[index1][index2] != 0) {
                    fprintf(file_r, "%d\n", zboruri[index1][index2]);
                } else fprintf(file_r, "%d\n", dist[i]+(15*(landings-1)));
            }
    }
}

//folosita pentru cerintele: "drum_zbor", "timp_zbor" si "min_zbor"
void dijkstra(int **graph, int index1, int nr_insule, int index2, int timp, FILE *file_r, int timp_2) 
{
    int dist[nr_insule]; // vector pentru distante/zboruri minime
    int parent[nr_insule];

    int sptSet[nr_insule];

    for (int i = 0; i < nr_insule; i++){ 
        dist[i] = INT_MAX, 
        sptSet[i] = 0; 
        parent[i] = -1; 
    }

    dist[index1] = 0; 

    for (int count = 0; count < nr_insule - 1; count++) { 

        int u = minDistance(dist, sptSet, nr_insule); 

        sptSet[u] = 1; 

        for (int v = 0; v < nr_insule; v++) 
            if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX 
                && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v]; 
                parent[v] = u;
                }
    }

    //conditii pentru afisarea particularizata in functie de cerinta 
    if(!timp && !timp_2) 
        printSolution(dist, parent, nr_insule, index2, file_r); 

    if(timp)
        print_time(dist, parent, nr_insule, index2, file_r);

    if(timp_2)
        print_time2(dist, graph,parent, nr_insule,index1, index2, file_r);

} 


int main(int argc, char *argv[])
{
    char *date, *out;
    date = argv[1];
    out = argv[2];

    FILE *file_d, *file_r;
    
    int nr_insule, cost;


    file_d = fopen(date, "rt");
    fscanf(file_d, "%d", &nr_insule); //citesc initial in variabila aceasta pentru a o folosi in 
                                      //functia alocGraph

    graphArhipelag *arhipelag = alocGraph(nr_insule);//aloc memorie pentru graf
    arhipelag->insule = (Island*)malloc(sizeof(Island)*nr_insule);

    int i, j;
    char lenth1[20], lenth2[20], insula1[20], insula2[20];

//////CITIRE DATE DESPRE INSULE///////
    for(i = 0; i < nr_insule; i++) {
        fscanf(file_d, "%s", lenth1);
        arhipelag->insule[i].nume = (char*)malloc((sizeof(char)*strlen(lenth1))+1);
        strcpy(arhipelag->insule[i].nume, lenth1);

        fscanf(file_d, "%d", &arhipelag->insule[i].nrResurse);
        arhipelag->insule[i].inventarResurse = (Resursa*)malloc(sizeof(Resursa)*arhipelag->insule[i].nrResurse);

        for(j = 0; j < arhipelag->insule[i].nrResurse; j++) {
            fscanf(file_d, "%s %d", lenth2, &arhipelag->insule[i].inventarResurse[j].cantitate);
            arhipelag->insule[i].inventarResurse[j].nume = (char*)malloc((sizeof(char)*strlen(lenth1))+1);
            strcpy(arhipelag->insule[i].inventarResurse[j].nume, lenth2);
        }
    }

    fscanf(file_d, "%d", &arhipelag->nr_zboruri);
    //FORMEZ MATRICEA DE ADIACENTA-ZBORURILE//
    int index1, index2;
    for(i = 0; i < arhipelag->nr_zboruri; i++) {
        fscanf(file_d, "%s - %s %d", insula1, insula2, &cost);

        find_index(insula1, &index1);
        find_index(insula2, &index2);

        arhipelag->matrice_zboruri[index1-1][index2-1] = cost;
        arhipelag->matrice_zboruri[index2-1][index1-1] = cost;
    }


    file_r = fopen(out, "wt");

    char task[20];

                ////PROIECTARE////

    //citirea cerintei se face in while pana la terminarea fisierului "date.in"
    while(fscanf(file_d,"%s", task)==1){ 

        ////CERINTA [conexiune IslandX IslandY]////
        if(!strcmp(task, "conexiune")) {
            fscanf(file_d, "%s %s", insula1, insula2);

            find_index(insula1, &index1);
            find_index(insula2, &index2);

            if(arhipelag->matrice_zboruri[index1-1][index2-1]){
                fprintf(file_r,"OK\n");
            }else fprintf(file_r, "NO\n");

        }

        ////CERINTA [legatura IslandX]////
        if(!strcmp(task, "legatura")) {

            fscanf(file_d, "%s", insula1);
            find_index(insula1, &index1);

            for(j = 0; j < nr_insule; j++) {
                if(arhipelag->matrice_zboruri[index1-1][j]){
                    fprintf(file_r,"%s ", arhipelag->insule[j].nume);
                }
            }
            fprintf(file_r,"\n");
        }

        ////CERINTA [adauga_zbor IslandX IslandY cost]////
        if(!strcmp(task, "adauga_zbor")) {
            fscanf(file_d, "%s %s %d", insula1, insula2, &cost);

            find_index(insula1, &index1);
            find_index(insula2, &index2);

            arhipelag->matrice_zboruri[index1-1][index2-1] = cost;
            arhipelag->matrice_zboruri[index2-1][index1-1] = cost;
        }

        ////CERINTA [anulare_zbor IslandX IslandY]////
        if(!strcmp(task, "anulare_zbor")) {
            fscanf(file_d, "%s %s", insula1, insula2);
                        
            find_index(insula1, &index1);
            find_index(insula2, &index2);

            arhipelag->matrice_zboruri[index1-1][index2-1] = 0;
            arhipelag->matrice_zboruri[index2-1][index1-1] = 0;
        }

        ////CERINTA [max_resurse]////
        if(!strcmp(task, "max_resurse")) {
            max_resurse(arhipelag,file_r);
        }

        ////CERINTA [max_cantitate resursa]////
        if(!strcmp(task, "max_cantitate")) {
            char resursa[20];
            cantitate_maxima *max = malloc(sizeof(cantitate_maxima)*nr_insule);
            fscanf(file_d, "%s", resursa);

            for(i = 0; i < nr_insule; i++){
                for(j = 0; j < arhipelag->insule[i].nrResurse; j++){
                    if(strcmp(arhipelag->insule[i].inventarResurse[j].nume, resursa) == 0) {
                        max[i].indice = i+1;
                        max[i].cantitate = arhipelag->insule[i].inventarResurse[j].cantitate;
                    }
                    
                }
            }

            int aux_max = 0;
            for(i = 0; i < nr_insule; i++){
                if(max[i].cantitate > aux_max){
                    aux_max = max[i].cantitate;
                }
            }

            for(i = 0; i < nr_insule; i++) {
                if(aux_max == max[i].cantitate) {
                    fprintf(file_r, "Island%d ", max[i].indice);
                }
            }
            fprintf(file_r, "\n");
        }

        int timp = 0;
        int timp_2 = 0;

                ////ANALIZA////

        ////CERINTA [drum_zbor IslandX IslandY]////
        if(!strcmp(task, "drum_zbor")) {            
            fscanf(file_d, "%s %s", insula1, insula2);

            find_index(insula1, &index1);
            find_index(insula2, &index2);

            dijkstra(arhipelag->matrice_zboruri, index1-1, nr_insule, index2-1, timp, file_r, timp_2);
        }

        ////CERINTA [timp_zbor IslandX IslandY]////
        if(!strcmp(task, "timp_zbor")) {
            timp = 1;
            
            fscanf(file_d, "%s %s", insula1, insula2);

            find_index(insula1, &index1);
            find_index(insula2, &index2);

            dijkstra(arhipelag->matrice_zboruri, index1-1, nr_insule, index2-1, timp, file_r, timp_2);
        }

        ////CERINTA [min_zbor IslandX IslandY]////
        if(!strcmp(task, "min_zbor")) {
            timp_2 = 1;

            fscanf(file_d, "%s %s", insula1, insula2);

            find_index(insula1, &index1);
            find_index(insula2, &index2);

            dijkstra(arhipelag->matrice_zboruri, index1-1, nr_insule, index2-1, timp, file_r, timp_2);
        }
    }

    for(i = 0; i < nr_insule; i++) {
        free(arhipelag->insule[i].nume);
        free(arhipelag->insule[i].inventarResurse);
    }
    free(arhipelag->insule);
    free(arhipelag->matrice_zboruri);
    free(arhipelag);

    fclose(file_d);
    fclose(file_r);

    return 0;
}