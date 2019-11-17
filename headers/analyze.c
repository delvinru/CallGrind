//
// Created by delvin on 16.11.2019.
//
#include "analyze.h"
#include <stdio.h>
unsigned int g = 0;

void add_sensor(char* func_name, unsigned int g)
{
    static unsigned int id = 0;
    if(g==0)
        output = fopen("output.txt", "w");
    for(int i = 0; i < g; i++)
        fprintf(output,"%s", "#");
    fprintf(output, "id%d: %s\n", id++, func_name);
}
