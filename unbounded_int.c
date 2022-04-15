#include <stdlib.h>
#include <string.h>

#include "unbounded_int.h"

unbounded_int string2unbounded_int(const char* e){
    unsigned long stringLen=strlen(e);
    chiffre* premier=malloc(sizeof(chiffre));
    if(premier==NULL || stringLen==0){
        unbounded_int empty={.signe='*'};
        return empty;
    }
    chiffre* tmp;
    unbounded_int res={.premier=premier};
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
    res.dernier=tmp;
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

int unbounded_int_cmp_unbounded_int(unbounded_int a, unbounded_int b){

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
    chiffre* dernier=malloc(sizeof(chiffre));
    if(dernier==NULL){
        unbounded_int empty={.signe='*'};
        return empty;
    }
    // création des pointeurs de chiffre pour avancer dans la structure
    unbounded_int res={.signe='+',.dernier=dernier};
    int retenue=0;
    int count=0;
    chiffre* nextChiffre=res.dernier;
    chiffre* nextChiffreA=a.dernier;
    chiffre* nextChiffreB=b.dernier;
    // boucle qui compare le chiffre A au B, sur toute la longueur de B
    while(nextChiffreB!=NULL){
        if((nextChiffreA->c-'0')-(nextChiffreB->c-'0')+retenue>=0){
            nextChiffre->c=(char)(((nextChiffreA->c-'0')-(nextChiffreB->c-'0')+retenue)+'0');
            retenue=0;
        }
        else{
            nextChiffre->c=(char)(((nextChiffreA->c-'0')-(nextChiffreB->c-'0')+retenue+10)+'0');
            retenue=-1;
        }
        // on se déplace au chiffre précédent seulement si on n'est pas au dernier chiffre de la valeur
        if(count<b.len-1){
            chiffre* next=malloc(sizeof(chiffre));
            next->suivant=nextChiffre;
            nextChiffre->precedent=next;
            nextChiffre=nextChiffre->precedent;
        }
        // on va sur le chiffre précédent de A et de B
        nextChiffreB=nextChiffreB->precedent;
        nextChiffreA=nextChiffreA->precedent;
        res.len+=1;
        count++;
    }
    // boucle pour compléter le résultat, si A est plus grand que B
    while(nextChiffreA!=NULL){
        chiffre* next=malloc(sizeof(chiffre));
        if((nextChiffreA->c-'0')+retenue>=0){
            next->c=(char)(((nextChiffreA->c-'0')+retenue)+'0');
            retenue=0;
        }
        else{
            next->c=(char)(((nextChiffreA->c-'0')+retenue+10)+'0');
            retenue=-1;
        }
        next->suivant=nextChiffre;
        nextChiffre->precedent=next;
        nextChiffre=nextChiffre->precedent;
        nextChiffreA=nextChiffreA->precedent;
        res.len+=1;
        count++;
    }
    res.premier=nextChiffre;
    return res;
}

unbounded_int unbounded_int_difference(unbounded_int a, unbounded_int b){
    if(a.signe=='+' && b.signe=='+'){
        return unbounded_int_difference_a_b_positifs(a,b);
    }
    if(a.signe=='-' && b.signe=='-'){
        if(unbounded_int_cmp_unbounded_int(a,b)==-1){
            a.signe='+';
            b.signe='+';
            unbounded_int res=unbounded_int_difference_a_b_positifs(a,b);
            res.signe='-';
            return res;
        }
        else{
            a.signe='+';
            b.signe='+';
            return unbounded_int_difference_a_b_positifs(b,a);
        }
    }
    if(a.signe=='+' && b.signe=='-'){
        b.signe='-';
        // appel de la somme
    }
    if(a.signe=='-' && b.signe=='+'){
        a.signe='+';
        // appel de la somme
    }
}