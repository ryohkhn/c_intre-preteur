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

// todo je sais pas si on appelle ll2unbounded_int et ensuite la fonction qui compare deux unbounded_int entre eux (c'est un peu de l'arnaque)
int unbounded_int_cmp_ll(unbounded_int a, long long b){
    if(a.signe=='+' && b<0) return 1;
    if(a.signe=='-' && b>0) return -1;

}
