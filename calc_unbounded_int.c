#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unbounded_int.c"

/*
 * https://developpement-informatique.com/article/215/lire-et-ecrire-dans-un-fichier-en-langage-c
 * Pour des renseignements sur comment lire et écrire dans des fichiers.
 */

typedef struct variable{
    char* nom;
    unbounded_int valeur;
}variable;

char * printValue(char * s, variable* liste, FILE * output){
    while(*s == ' '){
        s++;
    }
    if(*s >= '0' && *s <= '9'){
        int isSpace = 0;
        while(*s != '\0'){
            if(*s == ' '){
                isSpace = 1;
            }else{
                if(isSpace != 0){
                    printf("Erreur dans le fichier. Il y a une tentative d'affichage d'un nombre coupé par un espace.\n");
                    exit(EXIT_FAILURE);
                }
                else if(*s <= '0' || *s >= '9'){
                    printf("Erreur dans le fichier. Le nombre a afficher est coupé par un caractere qui n'est pas un chiffre.\n");
                    exit(EXIT_FAILURE);
                }
            }
            s++;
        }
        return s;
    }
    else if ((*s >= 'a' && *s <= 'z') || (*s >= 'A' && *s <= 'Z')){
        //fputs(getVariableFromTemp(s, temp), output);
    }
}

void interpreterLineByLine(FILE* source,FILE* sortie,variable* listeVar,char* ligne){
    int startVariable = 0;
    char * isPrint = malloc(sizeof(char) * 5);
    int printAlreadyVerified = 0;
    int compteur=0;
    char c=*ligne;
    while (c != EOF && c != '\n' ){ // on itere lettre par lettre jusqu'a reconnaitre une variable, un print ou une erreur.
        if(c!=' '){
            if (startVariable == 0) { // cas de la premiere lettre
                if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')){
                    startVariable += 1;
                }
                else{
                    printf("Erreur: La ligne ne commence pas par une lettre.\n");
                    exit(EXIT_FAILURE);
                }
            }
            else if(startVariable < 5) {
                if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) {
                    *isPrint++ = c;
                    startVariable += 1;
                }
            }
            else if(startVariable == 5 && printAlreadyVerified == 0){ // cas print
                printAlreadyVerified = 1;
                if(strcmp(isPrint, "print") == 0){
                    c = fgetc(source);
                    while(c == ' '){
                        c = fgetc(source);
                    }
                    //printValue(fgets(source),liste,source);
                }
            }
        }
        compteur++;
        c=*(ligne+compteur);
    }
}

void interpreter(FILE* source, FILE* sortie){
    variable* listeVar=malloc(sizeof(variable));
    int taille_max=1024;
    char* ligne=malloc(sizeof(char)*taille_max);
    while(fgets(ligne,taille_max,source)!=0){
        interpreterLineByLine(source,sortie,listeVar,ligne);
    }
}

int main(int argc,char* argv[]){
    if(argc>5){
        printf("Erreur: nombre d'arguments invalides");
        return EXIT_FAILURE;
    }

    FILE* fichierEntree;
    FILE* fichierSortie;
    if(argc>1 && strcmp(argv[1],"-i")==0){
        fichierEntree=fopen(argv[2],"r");
        if(fichierEntree==NULL){
            printf("Erreur \"%s\": chemin d'accès invalide",argv[2]);
            return EXIT_FAILURE;
        }
        if(argc>3 && strcmp(argv[3],"-o")==0){
            fichierSortie=fopen(argv[4],"w");
            interpreter(fichierEntree,fichierSortie);
        }
        else{
            interpreter(fichierEntree,stdout);
        }
    }
    else if(argc>1 && strcmp(argv[1],"-o")==0){
        fichierSortie=fopen(argv[2],"w");
        if(fichierSortie==NULL){
            printf("Erreur \"%s\": chemin d'accès invalide",argv[2]);
            return EXIT_FAILURE;
        }
        interpreter(stdin,fichierSortie);
    }
    else{
        interpreter(stdin,stdout);
    }
}