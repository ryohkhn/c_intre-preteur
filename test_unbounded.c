#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unbounded_int.h"

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

static void test_specifics_values(){
    unbounded_int a = string2unbounded_int("-9999999");
    unbounded_int b = string2unbounded_int("9999999");
    unbounded_int c = string2unbounded_int("0");
    unbounded_int d = string2unbounded_int("-0");

    printf("Test comparison unbounded_int / unbounded_int with basics values\n");
    printf("    -9999999 < 9999999  and function return : %d\n", unbounded_int_cmp_unbounded_int(a,b));
    printf("    9999999 > 0  and function return : %d\n", unbounded_int_cmp_unbounded_int(b,c));
    printf("    0 = -0 and function return :  %d\n", unbounded_int_cmp_unbounded_int(c,d));

    printf("Test comparison unbounded_int / long_long with basics values\n");
    printf("    -9999999 < 9999999  and function return : %d\n", unbounded_int_cmp_ll(a,9999999));
    printf("    9999999 > 0  and function return : %d\n", unbounded_int_cmp_ll(b,0));
    printf("    0 = -0 and function return :  %d\n", unbounded_int_cmp_ll(c,-0));

    printf("Test sum unbounded_int with basics values\n");
    printf( "   -9999999 + 9999999 = 0 and function return :\n");
    unbounded_int somme = unbounded_int_somme(a,b);
    print_unbounded_int(&somme);
    printf("   9999999 + 0 = 9999999 and function return :\n");
    somme = unbounded_int_somme(b,c);
    print_unbounded_int(&somme);
    printf( "   0 + -0 = 0 and function return :\n");
    somme = unbounded_int_somme(c,d);
    print_unbounded_int(&somme);

    printf("Test difference unbounded_int with basics values\n");
    printf( "   -9999999 - 9999999 = -19999998 and function return :\n");
    unbounded_int difference = unbounded_int_difference(a,b);
    print_unbounded_int(&difference);
    printf("   9999999 - 0 = 9999999 and function return :\n");
    difference = unbounded_int_difference(b,c);
    print_unbounded_int(&difference);
    printf( "   0 - -0 = 0 and function return :\n");
    difference = unbounded_int_difference(c,d);
    print_unbounded_int(&difference);

    printf("Test produit unbounded_int with basics values\n");
    printf( "   -9999999 * 9999999 = -99999980000001 and function return :\n");
    unbounded_int produit = unbounded_int_produit(a,b);
    print_unbounded_int(&produit);
    printf("   9999999 * 0 = 0 and function return :\n");
    produit = unbounded_int_produit(b,c);
    print_unbounded_int(&produit);
    printf( "   0 * -0 = 0 and function return :\n");
    produit = unbounded_int_produit(c,d);
    print_unbounded_int(&produit);
    printf("\n\n\n");
}

static long long randomll(){
    int length = (random() % 18) + 1;
    long long r = ((long long)random() << 25) + random();
    for(int i = 0; i < length; i++) r /= 10;
    int key = rand() % 2;
    if(key == 0) r *= -1;
    return r;
}

static char* randomString(){
    long long r = randomll();
    char * randomString = malloc(sizeof(char) * 20);
    if(r >= 0){
        sprintf(randomString, "%c%lld",'+', r);
    }else{
        sprintf(randomString, "%lld", r);
    }
    return randomString;
}

static void testS2UI2S(){
    int grade = 0;
    for(int j = 1; j <= 1000; j++){
        char * s = randomString();
        unbounded_int to_unbounded_int = string2unbounded_int(s);
        char * to_string = unbounded_int2string(to_unbounded_int);
        if(strcmp(s,to_string) == 0){
            grade += 1;
        }
    }

    printf("test String to unbounded_int to String : grade of %d / 1000 \n", grade);
}

static void testll2UI(){
    int grade = 0;
    for(int j = 1; j <= 1000; j++){
        long long ll = randomll();
        unbounded_int to_unbounded_int = ll2unbounded_int(ll);
        char * to_string = unbounded_int2string(to_unbounded_int);
        long long to_ll = atoll(to_string);
        if(ll == to_ll){
            grade += 1;
        }
    }
    printf("test long long to unbounded_int : grade of %d / 1000 \n", grade);
}

static void testUIcmpUIandUIcmpll(){
    int gradeTrue = 0;
    int gradeFalse = 0;
    int gradeSign = 0;

    int grade_ui_llTrue = 0;
    int grade_ui_llFalse = 0;
    int grade_ui_llSign = 0;
    for(int i = 1; i <= 100; i++){
        long long base = randomll();
        long long base2 = -1 * base;
        long long base3 = randomll();

        if(base3 == base){
            base3 += 1;
        }
        unbounded_int ui1 = ll2unbounded_int(base);
        unbounded_int ui2 = ll2unbounded_int(base);
        unbounded_int ui3 = ll2unbounded_int(base2);
        unbounded_int ui4 = ll2unbounded_int(base3);

        //test equals unbounded int
        if(unbounded_int_cmp_unbounded_int(ui1,ui2) == 0) {
            gradeTrue += 1;
        }

        // test equals unbounded int and long long
        int result_unbounded_int_cmp_ll= unbounded_int_cmp_ll(ui1,base);
        if(result_unbounded_int_cmp_ll == 0){
            grade_ui_llTrue += 1;
        }

        // test sign unbounded int
        int result_cmp_sign = unbounded_int_cmp_unbounded_int(ui1,ui3);
        if(result_cmp_sign != 0){
            if(base > base2 && result_cmp_sign == 1){
                gradeSign += 1;
            }else if(base < base2 && result_cmp_sign == -1){
                gradeSign += 1;
            }
        }else{
            if(base == 0) gradeSign += 1;
        }


        // test sign long long
        int result_cmp_sign_ui_ll = unbounded_int_cmp_ll(ui1,base2);
        if(result_cmp_sign_ui_ll != 0){
            if(base > base2 && result_cmp_sign_ui_ll == 1){
                grade_ui_llSign += 1;
            }else if(base < base2 && result_cmp_sign_ui_ll == -1){
                grade_ui_llSign += 1;
            }
        }else{
            if(base == 0) grade_ui_llSign += 1;
        }




        // test non equals unbounded int
        int result_cmp_ui_ui = unbounded_int_cmp_unbounded_int(ui1,ui4);
        if(result_cmp_ui_ui != 0){
            if(base > base3 && result_cmp_ui_ui == 1){
                gradeFalse += 1;
            }
            else if(base < base3 && result_cmp_ui_ui == -1){
                gradeFalse += 1;
            }
        }else{
            if(base == base3) gradeFalse += 1;
        }

        // test non equals unbounded int and long long
        int result_cmp_ui_ll = unbounded_int_cmp_ll(ui1,base3);
        if(result_cmp_ui_ll != 0){
            if(base > base3 && result_cmp_ui_ll == 1){
                grade_ui_llFalse += 1;
            }else if(base < base3 && result_cmp_ui_ll == -1){
                grade_ui_llFalse += 1;
            }
        }else{
            if(base == base3) grade_ui_llFalse += 1;
        }
    }



    printf("test comparison unbounded_int and unbounded_int : \n    "
           "- comparison between two identical unbounded_int  %d/100\n    "
           "- comparison with only the sign varying %d/100\n   "
           "- comparison with two different unbounded_int %d/100\n"
           ,gradeTrue,gradeSign,gradeFalse);
    printf("test comparison unbounded_int and long long : \n    "
           "- comparison between two identical numbers  %d/100\n    "
           "- comparison with only the sign varying %d/100\n   "
           "- comparison with two different numbers %d/100\n"
            ,grade_ui_llTrue,grade_ui_llSign,grade_ui_llFalse);
}

static void testSomme(){
    int grade = 0;
    for(int i = 0; i < 1000; i++){
        //initialization
        long long l1 = randomll();
        long long l2 = randomll();
        unbounded_int u1 = ll2unbounded_int(l1);
        unbounded_int u2 = ll2unbounded_int(l2);

        //operation
        long long l3 = l1 + l2;
        unbounded_int u3 = unbounded_int_somme(u1,u2);
        long long toll = atoll(unbounded_int2string(u3));

        //comparison
        if(l3 == toll){
            grade += 1;
        }else{
            printf("%lld ll | %lld ui          with somme = %lld + %lld\n\n",l3, toll,l1,l2);
        }
    }
    printf("test somme : %d/1000\n",grade);

}

static void testDifference(){
    int grade = 0;
    for(int i = 0; i < 1000; i++){
        //initialization
        long long l1 = randomll();
        long long l2 = randomll();
        unbounded_int u1 = ll2unbounded_int(l1);
        unbounded_int u2 = ll2unbounded_int(l2);

        //operation
        long long l3 = l1 - l2;
        unbounded_int u3 = unbounded_int_difference(u1,u2);
        long long toll = atoll(unbounded_int2string(u3));
        //comparison
        if(l3 == toll){
            grade += 1;
        }else{
            printf("%lld ll | %lld ui          with difference = %lld - %lld\n\n",l3, toll,l1,l2);
        }
    }
    printf("test difference : %d/1000\n",grade);

}

static void testProduit(){
    int grade = 0;
    for(int i = 0; i < 1000; i++){
        //initialization
        long long l1 = random();
        long long l2 = random();
        unbounded_int u1 = ll2unbounded_int(l1);
        unbounded_int u2 = ll2unbounded_int(l2);

        //operation
        long long l3 = l1 * l2;
        unbounded_int u3 = unbounded_int_produit(u1,u2);
        long long toll = atoll(unbounded_int2string(u3));

        //comparison
        if(l3 == toll){
            grade += 1;
        }else{
            printf("%lld ll | %lld ui          with produit = %lld * %lld\n\n",l3, toll,l1,l2);
        }
    }
    printf("test produit : %d/1000\n",grade);

}

int main(void) {
    test_specifics_values();
    testS2UI2S();
    testll2UI();
    testUIcmpUIandUIcmpll();
    testSomme();
    testDifference();
    testProduit();
}
