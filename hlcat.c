/*
    hlcat License

    hlcat is distributed under the following BSD-style license:  

    Copyright (c) 2009, Aljoscha Krettek <aljoscha.krettek@googlemail.com>
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
        * Redistributions of source code must retain the above copyright
          notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright
          notice, this list of conditions and the following disclaimer in the
          documentation and/or other materials provided with the distribution.
        * The name of the author may not be used to endorse or promote products
          derived from this software without specific prior written permission. 

    THIS SOFTWARE IS PROVIDED BY Aljoscha Krettek ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL Aljoscha Krettek BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

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
#include <stdarg.h>

//#define DEBUG 1
inline void dprintf(char *text, ...)
{
#ifdef DEBUG
    va_list list;
    va_start(list,text);
    vprintf(text, list);
    va_end(list);
#endif
}

#define MAX_LINE 200
#define MAX_WORD 20

int main(int argc, char **argv)
{
    char line_buffer[MAX_LINE];         // for reading one line from the input file
    char print_buffer[MAX_LINE];        // for preparing the printout, and printing it
                                        // we use strncpy to copy part of the current line there
                                        // because we want to print the search string in a different
                                        // color
    FILE *file;
    int fvector[MAX_WORD] = {-1};       // for storing the fault vector
    char *sword;                        // the search string, will be set to argv[1] later
    int word_length;
    int line_length;
    int i,j;                            // loop vars
    int current_scan_start;             // the start of the string we are currently scanning
                                        // this is used because we continue scanning
                                        // after we found the search string once in a line

    if (argc != 3)
    {
        printf("hlcat Copyright (c) 2009, Aljoscha Krettek <aljoscha.krettek@googlemail.com>\n");
        printf("usage: hlcat <search string> <filename>\n");
        exit(1);
    }

    sword = argv[1];
    word_length = strlen(sword);
    
    if ((file = fopen(argv[2], "r")) == NULL)
    {
        printf("error: %s\n", strerror(errno));
        exit(1);
    }
        
    dprintf("search word: %s\n", sword);
    dprintf("search word length: %d\n", word_length);
    dprintf("\ncreating fault vector...\n");
    dprintf("  set fvector[0] = -1\n");
    dprintf("  set i = -1\n");
    fvector[0] = -1;
    i = -1;

    for(j = 1; j < word_length; j++)
    {
        dprintf("-------------\n");
        dprintf("    j = %d\n", j);
        dprintf("    i = %d\n", i);
        while (i > -1 && sword[i+1] != sword[j])
        {
            dprintf("    set i = fvector[i]: fvector[i] = %d\n", fvector[i]);
            i = fvector[i];
        }
        if (sword[i+1] == sword[j])
        {
            dprintf("    adding one to i\n");
            dprintf("    fvector[%d] = %d\n",j,i);
            i = i+1;
            fvector[j] = i;
        }
        else
        {
            dprintf("    fvector[%d] = %d\n",j,-1);
            fvector[j] = -1;
        }
    }

    printf("\033[1;34;38mfvector: [");
    for(i = 0; i < word_length; i++)
    {
        if (i == word_length-1)
            printf("%d", fvector[i]);
        else
            printf("%d,", fvector[i]);
    }
    printf("]\033[0;;m\n");

    while (!feof(file))
    {
        if (fgets(line_buffer, MAX_LINE, file) == NULL) // might get EOF here
            break;
        //printf("%s", line_buffer);
        line_length = strlen(line_buffer);
        current_scan_start = 0;
        i = 0;
        j = -1;
        while (i < line_length)
        {
            while (j > -1 && sword[j+1] != line_buffer[i])
            {
                j = fvector[j];
            }
            if (sword[j+1] == line_buffer[i])
            {
                j++;
            }
            if ( j == word_length-1 )
            {
                //printf("\nfound i: %d, j: %d, css: %d\n", i, j, current_scan_start);
                strncpy(print_buffer, &line_buffer[current_scan_start], i-j-current_scan_start);
                print_buffer[i-j-current_scan_start] = '\0';
                printf("%s", print_buffer);
                strncpy(print_buffer, &line_buffer[i-j], j+1);
                print_buffer[j+1] = '\0';
                printf("\033[1;37;41m%s\033[0;;m", print_buffer);
                i++;
                j = -1;
                current_scan_start = i; 
            }
            else
            {
                i++;
            }
        }
        if (current_scan_start <= line_length) // maybe there is something left at the end of the string
        {
            strncpy(print_buffer, &line_buffer[current_scan_start], line_length-current_scan_start);
            print_buffer[line_length-current_scan_start] = '\0';
            printf("%s", print_buffer);
        }
    }
    fclose(file);
    return 0;
}
