#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <time.h>

#if (defined (_WIN32) || defined (_WIN64))
    #define WIN 1
#else
    #define WIN 0
#endif

char* find_func_name(char*, char*);
void  build_file(FILE*, FILE*);
char* create_name(char*);
void  create_command(char*, char*, char* argv[], int);
void  create_files(int , char**, int);

int main(int argc, char** argv)
{
    if(WIN)
    {
        printf("\e[1;31[!]\e[0mWork only on Linux\n");
        exit(0);
    }
    
    if(argv[1] != NULL)
    {
        if(strstr(argv[1], "projects/all"))
        {
            printf("\e[1;32m### Choosen mode to edit ALL files in directory ###\e[0m\n");
            create_files(1, NULL, 0);
        } else
        {
            printf("\e[1;32m### Choosen mode to edit ONE file ###\e[0m\n");
            create_files(0, argv, argc);
        }
    } else{
        printf("\e[1;32mUsage: callgrind projects/file args_for_you_program\nOR: callgrind projects/all to choose all files from directory\n\e[0m");
        exit(0);
    }
    return 0;
}

void create_files(int mode, char** argv, int argc)
{
    if (mode)
    {
        struct dirent *file;
        DIR *dir;
        dir = opendir("projects");
        if(dir == NULL)
        {
            printf("Cannot open directory projects\n");
            exit(1);
        }
        while((file = readdir(dir)) != NULL)
        {
            if(strcmp(file->d_name, ".") == 0)
                continue;
            if(strcmp(file->d_name, "..") == 0)
                continue;

            //Begin edit file
            printf("\n\e[1;36mFILE: %s\e[0m\n", file->d_name);

            FILE *edited_file;
            FILE *user_file;
            // Basic var for work with filename
            char input_name[128] = {0};
            char file_name[128] = {0};
            char user_file_name[128] = {0};

            // Base setup for begin
            strcat(input_name, file->d_name);
            strcat(user_file_name, "projects/");
            strcat(user_file_name, file->d_name);

            // Open user file
            user_file = fopen(user_file_name, "r");

            // Create dir path
            strcat(file_name, "projects_edited/");
            strcat(file_name, create_name(input_name));

            // Command for compile
            char buffer[128] = "";
            create_command(buffer, file_name, NULL, 0);
            printf("\e[1;35mCommand to compile: %s\e[0m\n", buffer);

            // Create a backup file
            printf("\e[1;32m[+]\e[0m Created file: %s\n", file_name);
            edited_file = fopen(file_name, "w"); 

            // Build edited file
            printf("\e[1;32m[+]\e[0m Start the analysis program\n");
            build_file(edited_file, user_file);

            // End of analysis
            printf("\e[1;32m[+]\e[0m Analysis completed!\n");
            fclose(user_file);
            fclose(edited_file);
        }
        closedir(dir);
    }else
    {
        FILE* user_file;
        FILE* edited_file;
        char* file_name;

        //Open user file
        char input_name[128] = {0};
        strcat(input_name, argv[1]);
        user_file = fopen(input_name, "r");

        //Create edited file
        memset(input_name, 0, sizeof(input_name));
        strcat(input_name, "projects_edited/");
        strtok(argv[1], "/");
        strcat(input_name, strtok(NULL, "/"));
        file_name = create_name(input_name);

        printf("\n\e[1;36mFILE: %s\e[0m\n", file_name);

        //Create command
        char buffer[128] = "";
        create_command(buffer, file_name, argv, argc);

        //Created backup file
        printf("\e[1;32m[+]\e[0m Created file: %s\n", file_name);
        edited_file = fopen(file_name, "w");

        //Build edited file
        printf("\e[1;32m[+]\e[0m Start the analysis program\n");
        build_file(edited_file, user_file);

        //End of analysis
        printf("\e[1;32m[+]\e[0m Analysis completed!\n");
        fclose(user_file);
        fclose(edited_file);

        printf("\e[1;32m[+]\e[0m Executable command:%s\n", buffer);
        printf("\e[1;31m[!]\e[1;35m If your programm required include some libraries copy this command and insert them\n\e[0m");
        printf("\e[1;32m[+]\e[0m Run user program...\n");
        printf("\e[1;33m#################################\e[0m\n");
        system(buffer);
        printf("\e[1;33m#################################\e[0m\n");
        printf("\e[1;32m[+]\e[0m Checking result...:\n");
        system("cat output.txt");
    }
}

void create_command(char* buffer, char* file_name, char* argv[], int argc)
{
    if(strstr(file_name, ".cpp"))
    {
        strcat(buffer, "g++ ");
        strcat(buffer, file_name);
        strcat(buffer," headers/analyze.c -o output.out; ./output.out ");
        for(int i = 2; i < argc; i++, strcat(buffer, " "))
        {
            strcat(buffer, "'");
            strcat(buffer, argv[i]);
            strcat(buffer, "'");
        }
        strcat(buffer, "; rm output.out");
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
        strcat(buffer," headers/analyze.c -o output.out; ./output.out ");
        for(int i = 2; i < argc; i++, strcat(buffer, " "))
        {
            strcat(buffer, "'");
            strcat(buffer, argv[i]);
            strcat(buffer, "'");
        }
        strcat(buffer, "; rm output.out");
    }
    else{
        printf("%s\n", file_name);
        printf("\e[1;31m[!]\e[0mSorry work only with \e[1;37mC/C++/C#\e[0m files\n");
        exit(1);
    }
}

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
    char function_name[128]; // Имя функции
    short int is_function = 0; // Проверка на функцию
    short int left_brackets = 0; //  Количество {
    short int right_brackets = 0; // Количество }
    short int injected_sens = 0; 
    short int long_comment = 0;
    short int have_return = 0;
    short int only_return = 0;

    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    //Little Debug info and include headers
    fprintf(file, "/*\n\tCreated by Callgrind ฅ^•ﻌ•^ฅ\n\t%s*/\n#include \"../headers/analyze.h\"\n", asctime(timeinfo));
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
                    if(strstr(buffer, "return"))
                    {
                        fprintf(file, "%s\tadd_sensor(\"%s()\", callgrind_checker++);\n\tcallgrind_checker--;\n%s}\n", name_buf, function_name, buffer);
                        only_return = 1;
                    }
                    else
                        fprintf(file, "%s\tadd_sensor(\"%s()\", callgrind_checker++);\n%s", name_buf,function_name, buffer);
                } else
                {
                    if(strstr(name_buf, "return"))
                    {
                        fprintf(file, "\tadd_sensor(\"%s()\", callgrind_checker++);\n\tcallgrind_checker--;\n%s%s", function_name, name_buf, buffer);
                        only_return = 1;
                    }
                    else
                        fprintf(file, "\tadd_sensor(\"%s()\", callgrind_checker++);\n%s%s", function_name, name_buf, buffer);
                }
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
                have_return = 1;
                fprintf(file, "\t{callgrind_checker--;\n%s\t}\n", buffer); //{ заворачивание в блок кода
                continue;
            }

            if(left_brackets == right_brackets)
            {
                if(!only_return)
                {
                    if(have_return)
                        fprintf(file, "%s", buffer);
                    else
                        fprintf(file,"\tcallgrind_checker--;\n%s", buffer);
                }
                is_function = 0;
            }
            continue;
        }else
            fputs(buffer, file);

        if((strstr(buffer, "float ") || strstr(buffer, "double ") || strstr(buffer, "char ")|| strstr(buffer, "void ") || strstr(buffer, "int ") || strstr(buffer, "::")) && strchr(buffer, '(') && strchr(buffer,';') == NULL && !(strstr(buffer,"for") || strstr(buffer,"while")) && strstr(buffer, "%") == NULL)
        {
            fgets(name_buf,127,source);
            if(strstr(buffer, "{") || strstr(name_buf,"{"))
            {
                find_func_name(buffer, function_name);
                printf("\e[1;32m[+]\e[0m Function \e[1;33m%s\e[0m find!\n", function_name);
                is_function = 1;
                injected_sens = 0;
                have_return = 0;
                only_return = 0;
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
        strcat(name, "_edited.cpp");
    }
    else if(strstr(name, ".cs"))
    {
        name[len-1] = '\0';
        name[len-2] = '\0';
        name[len-3] = '\0';
        strcat(name, "_edited.cs");
    }
    else
    {
        name[len-1] = '\0';
        name[len-2] = '\0';
        strcat(name, "_edited.c");
    }
    return name;
}