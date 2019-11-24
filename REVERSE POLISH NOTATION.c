#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
#define SIZE 11
int error =0;


typedef struct pile
{
        int valeur;
        struct pile* next;
}pile;
 

pile* nextpile(int valeur, pile* previous)
{
    pile* next = (pile*)malloc(sizeof(pile));
    if ( next == NULL )
    {
        fprintf(stderr, "Mémoire saturé \n");
        exit(1);
    }
    next->valeur = valeur;
    next->next = previous;
    return next;
}

pile* ajouterpile(pile* b, pile* level)
{
    if ( b == NULL )
    {
        level->next = NULL;
        return level;
    }
    level->next = b;
    return level;
}

void supprimerpile(pile* level) 
{
    if ( level != NULL ) 
    {
        supprimerpile(level->next);
        free(level);
    }
}

void afficherpile(pile* level, int point)
{
    if ( level != NULL) 
    {
        afficherpile(level->next, point + 1);
        printf("%d", level->valeur);
        if ( point != 0){printf(" ");}
    }
}

int verif (char instruction[SIZE])
{
    int i;
    for ( i = 0; i < SIZE && instruction[i] != '\0'; i++ )
        if ( instruction[0] != '-' && !isdigit(instruction[i]) ) 
            return 0;
    return 1;
}

int transfint (char instruction[SIZE])
{
    char *ptr;
    return strtol(instruction, &ptr, 10);
}

pile* POP( pile* level )
{
    pile* previous = NULL;
    if ( level != NULL ) 
    {
        previous = level->next;
        free(level);
    }
    return previous;
}

pile* DIV( pile* level )
{
    if ( level == NULL)
        return NULL;
    if ( level->next == NULL ) 
    {
        error = 1;
        level = POP(level);
        return level;
    }
    int left = level->next->valeur, right = level->valeur;
    if ( right == 0 )
    {
        error = 1;
        level = POP(level);
        level = POP(level);
        return level;
    }
    pile* next = level->next->next;
    free(level);
    free(level->next);
    return ajouterpile(next, nextpile(left/right, next));
}

pile* MUL( pile* level )
{
    if ( level == NULL)
        return NULL;
    if ( level->next == NULL ) 
    {
        error = 1;
        level = POP(level);
        return level;
    }
    int left = level->next->valeur, right = level->valeur;
    pile* next = level->next->next;
    free(level);
    free(level->next);
    return ajouterpile(next, nextpile(left*right, next));
}

pile* MOD( pile* level )
{
    if ( level == NULL)
        return NULL;
    if ( level->next == NULL ) 
    {
        error = 1;
        level = POP(level);
        return level;
    }
    int left = level->next->valeur, right = level->valeur;
    pile* next = level->next->next;
    free(level);
    free(level->next);
    return ajouterpile(next, nextpile(left%right, next));
}


pile* ROL( pile* prec, pile* level, int o)
{
    if ( prec == NULL || level == NULL ) 
    {
        error = 1;
        return 0;
    }
    if ( o == 1 )
    {
        pile* swap = level->next;
        int num = level->valeur;
        free(level); 
        prec->next = swap;
        return num;
    }
    else
         return ROL ( level, level->next, o-1 );
}

pile* SWP( pile* level )
{
    if ( level == NULL)
        return NULL;
    if ( level->next != NULL )
    {
        int swap = level->valeur;
        level->valeur = level->next->valeur;
        level->next->valeur = swap;
    }
    return level;
}

pile* SUB( pile* level ) /* soustraction */
{
    if ( level == NULL)
        return NULL;
    if ( level->next == NULL )
    {
        error = 1;
        level = POP(level);
        return level;
    }
    int left = level->next->valeur, right = level->valeur;
    pile* next = level->next->next;
    free(level);
    free(level->next);
    return ajouterpile(next, nextpile(left-right, next));
}

pile* ADD( pile* level ) /* addition */
{
    if ( level == NULL)
        return NULL;
    if ( level->next == NULL )
    {
        error = 1;
        level = POP(level);
        return level;
    }
    int left = level->next->valeur, right = level->valeur;
    pile* next = level->next->next;
    free(level);
    free(level->next);
    return ajouterpile(next, nextpile(left+right, next));
}

pile* DUP(pile* level)
{
   if ( level == NULL )
        return NULL;
    return ajouterpile(level, nextpile(level->valeur, level));
}


pile* calcul ( pile* a, char instruction[SIZE] )
{
    if ( !strcmp("ROL", instruction) )
    {
        int n = a->valeur;
        a = POP(a);
        n = ROL(a, a->next, n-1);
        a = ajouterpile(a, nextpile(n, a));
    }
    else if ( !strcmp("SWP", instruction))
        a = SWP(a);
    else if ( !strcmp("DUP", instruction))
        a = DUP(a);
    else if ( !strcmp("DUP", instruction))
        a = DUP(a);
    else if ( !strcmp("POP", instruction))
        a = POP(a);
    else if ( !strcmp("MOD", instruction))
        a = MOD(a);
    else if ( !strcmp("DIV", instruction))
        a = DIV(a);
    else if ( !strcmp("MUL", instruction))
        a = MUL(a);
    else if ( !strcmp("SUB", instruction))
        a = SUB(a);
    else if ( !strcmp("ADD", instruction))
        a = ADD(a);
    else
        error = 1;
    return a;
}

int main()
{
    pile* a = NULL;
    int N;
    scanf("%d", &N);
    for (int i = 0; i < N && error != 1; i++) 
    {
        char instruction[SIZE];
        scanf("%s", instruction);
        
        if (verif(instruction))
        {
            a = ajouterpile(a, nextpile(transfint(instruction), a));
        }
        else
        {
            a = calcul(a, instruction);
        }
        
    }
    afficherpile(a,0);
    if ( error == 1)
    {
        if (a != NULL )
        {
            printf(" ");
        }
        printf("ERROR");
    }
    
    printf("\n");
    supprimerpile(a);

    return 0;
}