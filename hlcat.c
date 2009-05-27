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
#include <errno.h>

//#define DEBUG 1

#define MAX_LINE 200
#define MAX_WORD 20

int main(int argc, char **argv)
{
    char line_buffer[MAX_LINE];         // for reading one line from the input file
    FILE *file;
    int fvector[MAX_WORD] = {-1};       // for storing the fault vector
    char *sword;                         // the search string, will be set to argv[1] later
    int word_length;
    int line_length;
    int i,j;                            // loop vars

    if (argc != 3)
    {
        printf("hlcat\nCopyright (c) 2009, by Aljoscha Krettek <aljoscha.krettek@googlemail.com>\n");
        printf("usage: hlcat <word> <filename>\n");
        exit(1);
    }

    sword = argv[1];
    word_length = strlen(sword);
    
    if ((file = fopen(argv[2], "r")) == NULL)
    {
        printf("error: %s\n", strerror(errno));
        exit(1);
    }
        
    int FOUND = 0;

#ifdef DEBUG
    printf("search word: %s\n", sword);
    printf("search word length: %d\n", word_length);
    printf("\ncreating fault vector...\n");
    printf("  set fvector[0] = -1\n");
    printf("  set i = -1\n");
#endif
    fvector[0] = -1;
    i = -1;

    for(j = 1; j < word_length; j++)
    {
#ifdef DEBUG
        printf("-------------\n");
        printf("    j = %d\n", j);
        printf("    i = %d\n", i);
#endif
        while (i > -1 && sword[i+1] != sword[j])
        {
#ifdef DEBUG
            printf("    set i = fvector[i]: fvector[i] = %d\n", fvector[i]);
#endif
            i = fvector[i];
        }
        if (sword[i+1] == sword[j])
        {
#ifdef DEBUG
            printf("    adding one to i\n");
            printf("    fvector[%d] = %d\n",j,i);
#endif
            i = i+1;
            fvector[j] = i;
        }
        else
        {
#ifdef DEBUG
            printf("    fvector[%d] = %d\n",j,-1);
#endif
            fvector[j] = -1;
        }
    }

    printf("fvector: [");
    for(i = 0; i < word_length; i++)
    {
        if (i == word_length-1)
            printf("%d", fvector[i]);
        else
            printf("%d,", fvector[i]);
    }
    printf("]\n");
/*
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
*/
    return 0;
}