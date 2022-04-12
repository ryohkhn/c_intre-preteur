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
        if(*(e+i)<48 || *(e+i)>57){
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

    if((a.signe=='+' && b<0) || (a.len>bSize)) return 1;
    if((a.signe=='-' && b>0) || (a.len<bSize)) return -1;

    long long tab[bSize];
    while(bSize--){
        tab[b]=b%10;
        b/=10;
    }

    chiffre* nextChiffre=a.premier;
    int compt=0;
    while(nextChiffre!=NULL){
        if(*(tab+compt)>nextChiffre->c){
            return -1;
        }
        else if(*(tab+compt)<nextChiffre->c){
            return 1;
        }
        nextChiffre=nextChiffre->suivant;
        compt++;
    }
    return 0;
}
