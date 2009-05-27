/* 
   The MIT License

   Copyright (c) 2009, by Aljoscha Krettek <aljoscha.krettek@googlemail.com>

   Permission is hereby granted, free of charge, to any person obtaining
   a copy of this software and associated documentation files (the
   "Software"), to deal in the Software without restriction, including
   without limitation the rights to use, copy, modify, merge, publish,
   distribute, sublicense, and/or sell copies of the Software, and to
   permit persons to whom the Software is furnished to do so, subject to
   the following conditions:

   The above copyright notice and this permission notice shall be
   included in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
   BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/

/*
   A simple program that does the same as cat but highlights
   the text given in the first parameter. The highlighted text
   is found by using the Knuth, Morris, Pratt text searching
   algorithm. This is just a simple hack to play around with
   Knuth, Morris, Pratt
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

int main(void)
{
    char T[MAX] = "aaaaaaaaabb";
    char W[MAX] = "aab";
    int F[MAX] = {-1};
    int N = strlen(T);
    int M = strlen(W);
    int I,J;
    int FOUND = 0;
    printf("Knuth, Morris, Pratt\n");
    printf("T: %s\n", T);
    printf("N: %d\n", N);
    printf("W: %s\n", W);
    printf("M: %d\n", M);

    printf("\ncreating fault vector...\n");

    printf("  set F[0] = -1\n");
    F[0] = -1;
    printf("  set I = -1\n");
    I = -1;
    for(J = 1; J < M; J++)
    {
        printf("-------------\n");
        printf("    J = %d\n", J);
        printf("    I = %d\n", I);
        while (I > -1 && W[I+1] != W[J])
        {
            printf("    set I = F[I]: F[I] = %d\n", F[I]);
            I = F[I];
        }
        if (W[I+1] == W[J])
        {
            printf("    adding one to I\n");
            I = I+1;
            printf("    F[%d] = %d\n",J,I);
            F[J] = I;
        }
        else
        {
            printf("    F[%d] = %d\n",J,-1);
            F[J] = -1;
        }
    }

    printf("\nF: [");
    for(int i = 0; i < M; i++)
    {
        if (i == M-1)
            printf("%d", F[i]);
        else
            printf("%d,", F[i]);
    }
    printf("]\n");

    printf("\nsearching...\n");
    printf("  set I = 0\n");
    I = 0;
    printf("  set J = -1\n");
    J = -1;
    while (I < N && !FOUND)
    {
        printf("-------------\n");
        printf("    I = %d\n", I);
        while (J > -1 && W[J+1] != T[I])
        {
            printf("        set J = F[J] : %d\n", F[J]);
            J = F[J];
        }
        printf("    J = %d\n", J);
        if (W[J+1] == T[I])
        {
            printf("    J++: %d\n", J+1);
            J++;
        }
        if ( J == M-1 )
        {
            printf("    found:");
            I = I - J;
            printf(" I = %d\n", I);
            FOUND = 1;
        }
        else
        {
            printf("    I++: %d\n", I+1);
            I++;
        }
    }

    return 0;
}
