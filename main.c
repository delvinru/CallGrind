#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char* find_func_name(char* string, char* func_name)
{
    char *token;
    strtok(string, "(");
    char *name[128];
    for(int j = 0; j < 50; j++)
        name[j] = malloc((81) * sizeof(char));
    int i = 0;
    for(token = strtok(string, " "); token != NULL; token = strtok(NULL, " "))
        strcpy(name[i++], token);
    strcpy(func_name, *(name+i-1));
    for(int j = 0; j < 50; j++)
        free(name[j]);
    return func_name;
}

void build_file(FILE* file, FILE* source)
{
    char buffer[128] = {0}; // Текущий буфер
    char name_buf[128] = {0};
    char return_buf[128] = {0};
    char function_name[128]; // Имя функции
    short int is_function = 0; // Проверка на функцию
    short int left_brackets = 0; //  Количество {
    short int right_brackets = 0; // Количество }
    short int injected_sens = 0; //
    short int long_comment = 0;
    short int have_return = 0;
    //Сделать проверку на cs файлы
    //Разобраться как они подключаются и компилятся
    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    fprintf(file, "/*\n\tCreated by Tree ฅ^•ﻌ•^ฅ\n\t%s*/\n#include \"../headers/analyze.h\"\n", asctime(timeinfo));

    while(fgets(buffer, 127, source))
    {
        //Проверка на комментарий
        if(buffer[0] == '/' && buffer[1] == '/')
        {
            fputs(buffer, file);
            continue;
        }
        if(strstr(buffer,"/*"))
            long_comment = 1;
        if(strstr(buffer,"*/"))
            long_comment = 0;
        if(long_comment)
        {
            fputs(buffer, file);
            continue;
        }
        //Работа с внутренностями функции
        if(is_function)
        {
            if(strstr(buffer,"{"))
                ++left_brackets;
            if(strstr(buffer,"}"))
                ++right_brackets;
            if(!injected_sens)
            {
                if(strstr(name_buf, "{"))
                {
                    fprintf(file, "%s\tadd_sensor(\"%s()\", g++);\n%s", name_buf,function_name, buffer);
                } else
                    fprintf(file, "\tadd_sensor(\"%s()\", g++);\n%s%s", function_name, name_buf, buffer);
                memset(name_buf,0, sizeof(name_buf));
                injected_sens = 1;
                continue;
            }
            if(left_brackets != right_brackets && strstr(buffer, "return") == NULL)
            {
                fputs(buffer, file);
                continue;
            }
            if(strstr(buffer, "return"))
            {
                strcpy(return_buf, buffer);
                have_return = 1;
                continue;
            }
            if(left_brackets == right_brackets)
            {
                if(have_return)
                    fprintf(file,"\tg--;\n%s%s", return_buf, buffer);
                else
                    fprintf(file,"\tg--;\n%s", buffer);
                is_function = 0;
            }
            continue;
        }else
            fputs(buffer, file);

        if((strstr(buffer, "float") || strstr(buffer, "double") || strstr(buffer, "char")|| strstr(buffer, "void") || strstr(buffer, "int") || strstr(buffer, "::")) && strstr(buffer, "(") && !(strstr(buffer,"for") || strstr(buffer,"while")) && strstr(buffer, "printf") == NULL)
        {
            fgets(name_buf,127,source);
            if(strstr(buffer, "{") || strstr(name_buf,"{"))
            {
                find_func_name(buffer, function_name);
                printf("\e[1;32m[+]\e[0mFunction \e[1;37m%s\e[0m find!\n", function_name);
                is_function = 1;
                injected_sens = 0;
                have_return = 0;
                left_brackets = right_brackets = 0;
                ++left_brackets;
                continue;
            }
        }
    }
}

char* create_name(char* name)
{
    int len = strlen(name);
    if(strstr(name, ".cpp"))
    {
        name[len-1] = '\0';
        name[len-2] = '\0';
        name[len-3] = '\0';
        name[len-4] = '\0';
        strcat(name, "_backup.cpp");
    }
    else if(strstr(name, ".cs"))
    {
        name[len-1] = '\0';
        name[len-2] = '\0';
        name[len-3] = '\0';
        strcat(name, "_backup.cs");
    }
    else
    {
        name[len-1] = '\0';
        name[len-2] = '\0';
        strcat(name, "_backup.c");
    }
    return name;
}

int main(int argc, char* argv[])
{
    // Open and create backup
    FILE* user_file;
    FILE* edited_file;
    char* file_name;
    if(argv[1] != NULL)
    {
        user_file = fopen(argv[1],"r");
        file_name = create_name(argv[1]);
    } else{
        printf("\e[1;31mUsage: callgrind path/to/file\e[0m\n");
        exit(1);
    }
    //Created backup file
    printf("\e[1;32m[+]\e[0mCreated file: %s\n", file_name);
    edited_file = fopen(file_name, "w");
    //Build edited file
    printf("\e[1;32m[+]\e[0mStart the analysis program\n");
    build_file(edited_file, user_file);
    //End of analysis
    printf("\e[1;32m[+]\e[0mAnalysis completed!\n");
    fclose(user_file);
    fclose(edited_file);

    char buffer[128] = "";
    if(strstr(file_name, ".cpp"))
    {
        strcat(buffer, "g++ ");
        strcat(buffer, file_name);
        strcat(buffer," headers/analyze.c -o output.out; ./output.out; rm output.out;");
    }
    else if(strstr(file_name, ".cs"))
    {
        printf("\e[1;31m[!]\e[0mSorry but now, I can only add_sensor to function, but not run this programm\n");
        exit(0);
    }
    else if(strstr(file_name,".c"))
    {
        strcat(buffer, "gcc ");
        strcat(buffer, file_name);
        strcat(buffer," headers/analyze.c -o output.out; ./output.out; rm output.out;");
    }
    else{
        printf("%s\n", file_name);
        printf("\e[1;31m[!]\e[0mSorry work only with \e[1;37mC/C++/C#\e[0m files\n");
        exit(1);
    }

    printf("\e[1;32m[+]\e[0mExecutable command:%s\n", buffer);
    printf("\e[1;32m[+]\e[0mRun user program...\n");
    printf("\e[1;33m#################################\e[0m\n");
    system(buffer);
    printf("\e[1;33m#################################\e[0m\n");
    printf("\e[1;32m[+]\e[0mChecking result...:\n");
    system("cat output.txt");

    return 0;
}