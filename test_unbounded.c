#include <stdio.h>
#include "unbounded_int.c"

static void print_unbounded_int(const unbounded_int* s){
	printf("%c",s->signe);
	int i=0;
	chiffre* tmp=s->premier;
	while(i<s->len){
        printf("%c",tmp->c);
        if(tmp->suivant!=NULL){
			tmp=tmp->suivant;
		}
		i++;
	}
    printf("\n");
}

static void testPrintLong(long long a){
	long long copy=a;
	unsigned int ret=1;
	while (copy/=10) ret++;
	long long tab[ret];
	while(ret--){
		tab[ret]=a%10;
		a/=10;
	}
	int j=0;
	while(j< sizeof(tab)/ sizeof(tab[0])){
		printf("%lld",*(tab+j));
		j++;
	}
    printf("\n");
}

int main(void) {
    //char* s="-4543676543298";
    char* s="12345";
    unbounded_int test1=string2unbounded_int(s);
	char* s2="123";
	unbounded_int test2=string2unbounded_int(s);
    //print_unbounded_int(&test1);
    //printf("%s",unbounded_int2string(string2unbounded_int("-4543676543298")));
    //printf("%d\n",unbounded_int_cmp_ll(test1,1111));
	unbounded_int res=unbounded_int_difference_a_b_positifs(test1,test2);
	print_unbounded_int(&res);
}