#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "unbounded_int.h"

unbounded_int string2unbounded_int(const char* e){
    unsigned long stringLen=strlen(e);
    chiffre* premier=malloc(sizeof(chiffre));
    chiffre* dernier=malloc(sizeof(chiffre));
    if(premier==NULL || dernier==NULL || stringLen==0){
        unbounded_int empty={.signe='*'};
        return empty;
    }
    chiffre* tmp;
    unbounded_int res={.premier=premier,.dernier=dernier};
    int i=0;
    int signePresent=0;
    if(*(e)=='-' || *(e)=='+'){
        res.signe=*(e);
        res.len=stringLen-1;
        i++;
        signePresent=1;
    }
    else{
        res.signe='+';
        res.len=stringLen;
    }
    while(i<stringLen){
        if((*(e+i)-'0')<0 || (*(e+i)-'0')>9){
            unbounded_int empty={.signe='*'};
            return empty;
        }
        if((signePresent && i==1) || (!signePresent && i==0)){
            premier->c=*(e+i);
            premier->precedent=NULL;
            tmp=premier;
        }
        else{
            chiffre* nouveau=malloc(sizeof(chiffre));
            nouveau->c=*(e+i);
            nouveau->precedent=tmp;
            tmp->suivant=nouveau;
            tmp=nouveau;
        }
        i++;
    }
    return res;
}


char* unbounded_int2string(unbounded_int i){
    char* res=malloc(sizeof(char)*(i.len+2));
    *res=i.signe;
    int compt=1;
    chiffre* tmp=i.premier;
    while(tmp!=NULL){
        *(res+compt)=tmp->c;
        compt++;
        tmp=tmp->suivant;
    }
    *(res+compt)='\0';
    return res;
}


int unbounded_int_cmp_ll(unbounded_int a, long long b){
    long long bCopy=b;
    unsigned int bSize=1;
    while(bCopy/=10) bSize++;

    // test de signe et de longueur
    if((a.signe=='+' && b<0) || (a.len>bSize)) return 1;
    if((a.signe=='-' && b>0) || (a.len<bSize)) return -1;

    // transformation du long long en tableau de long long unité par unité
    long long tab[bSize];
    while(bSize--){
        tab[bSize]=b%10;
        b/=10;
    }

    chiffre* nextChiffre=a.premier;
    int compt=0;
    while(nextChiffre!=NULL){
        if(*(tab+compt)>(nextChiffre->c)-'0'){
            return -1;
        }
        else if(*(tab+compt)<(nextChiffre->c)-'0'){
            return 1;
        }
        nextChiffre=nextChiffre->suivant;
        compt++;
    }
    return 0;
}

// addition de deux unbounded_int positifs
static unbounded_int unbounded_int_somme_a_b_positifis(unbounded_int a, unbounded_int b){

}

// soustraction a-b pour a et b positifs tels que a>=b
static unbounded_int unbounded_int_difference_a_b_positifs(unbounded_int a, unbounded_int b){
    chiffre* premier=malloc(sizeof(chiffre));
    chiffre* dernier=malloc(sizeof(chiffre));
    if(premier==NULL || dernier==NULL){
        unbounded_int empty={.signe='*'};
        return empty;
    }
    unbounded_int res={.signe='+',.premier=premier,.dernier=dernier};
    int retenue=0;
    int count=0;
    chiffre* nextChiffre=res.dernier;
    chiffre* nextChiffreA=a.dernier;
    chiffre* nextChiffreB=b.dernier;
    while(nextChiffreB!=NULL){
        if((nextChiffreA->c-'0')-(nextChiffreB->c-'0')+retenue>=0){
            nextChiffre->c=(char)(((a.dernier->c-'0')-(b.dernier->c-'0')+retenue)+'0');
            retenue=0;
        }
        else{
            nextChiffre->c=(char)(((a.dernier->c-'0')-(b.dernier->c-'0')+retenue+10)+'0');
            retenue=-1;
        }
        if(count<b.len){
            chiffre* next=malloc(sizeof(chiffre));
            next->suivant=nextChiffre;
            nextChiffre->precedent=next;
        }
        nextChiffre=nextChiffre->precedent;
        nextChiffreB=nextChiffreB->precedent;
        nextChiffreA=nextChiffreA->precedent;
        count++;
    }
    count=0;
    while(nextChiffreA!=NULL){
        nextChiffre->c=nextChiffreA->c;
        if(count<a.len){
            chiffre* next=malloc(sizeof(chiffre));
            next->suivant=nextChiffre;
            nextChiffre->precedent=next;
        }
        nextChiffre=nextChiffre->precedent;
        nextChiffreA=nextChiffreA->precedent;
        count++;
    }
    res.premier=nextChiffre;
    return res;
}

unbounded_int unbounded_int_difference(unbounded_int a, unbounded_int b){

}