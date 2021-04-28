#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <windows.h>

#define HELP 0x1
#define HEADER 0x2
#define DEBUG 0x4

int g_Flags = 0;
int g_Index = 0;
std::string g_Filename;
char* g_Delimitter = ",";
bool ParseArgs(int argc, char *argv[]);
std::string ParseLine(const std::string* line, char* delimiter, int index);

int main(int argc, char *argv[])
{
    
    size_t max_index = 0;
    int result = 0;
    std::string line;
    std::ifstream csvfile("Test.csv");

    if(argc < 2)
    {
        std::cout << "Not enough arguments. Use --help for more details." << std::endl;
        result = 1;
        goto Exit;
    }

    if(!ParseArgs(argc, argv))
    {
        std::cout << " --help for full options" << std::endl;
        result = 1;
        goto Exit;
    }

    if((g_Flags & DEBUG) == DEBUG)
    {

        std::cout << "g_Flags: " << g_Flags << std::endl;
        std::cout << "g_Index: " << g_Index << std::endl;
        std::cout << "g_Delimitter: " << g_Delimitter << std::endl;
        std::cout << "g_Filename: " << g_Filename << std::endl;

    }

    if((g_Flags & HELP) == HELP)
    {
        std::cout << "pcsv.exe [-t][-i:#] CSVFile" << std::endl;
        std::cout << "\nDESCRIPTION:" << std::endl;
        std::cout << "\tParses CSV files for specific columns and prints the output to the console.NOTE: File must end in .csv" << std::endl;
        std::cout << "\nOPTIONS:" << std::endl;
        std::cout << "\t-t\tPrint the CSV headers" << std::endl;
        std::cout << "\t-i:\tThe index of the columns to be print" << std::endl;
        goto Exit;
    }

    if(csvfile.is_open())
    {

        std::getline(csvfile, line);
        max_index = std::count_if(line.begin(), line.end(), [](char c) {
                if(c == ',') 
                { return true;}
                return false;});

        if(g_Index > max_index)
        {
            std::cout << "Invalid arguments. Index of " << g_Index << " is greater than column count of " << max_index << std::endl;
            result = 2;
            csvfile.close();
            goto Exit;
        }

        if(g_Flags & HEADER)
        {
            std::cout << g_Filename << " header: " << std::endl;
            std::cout << line << std::endl;
        }
        else
        {
            while(std::getline(csvfile, line))
            {
                std::cout << ParseLine(&line, ",", g_Index) << std::endl;;
            }
        }
        csvfile.close();
    }

Exit:

    return result;
}

std::string ParseLine(const std::string* line, char* delimiter, int index)
{

    size_t pos = line->find(delimiter);
    size_t initialPos = 0;
    int counter = 0;
    std::string result;

    while(pos != std::string::npos)
    {
        result = line->substr(initialPos, pos - initialPos);
        if(counter == index)
            break;
        initialPos = pos + 1;
        pos = line->find(delimiter, initialPos);
        counter++;
    }

    if(counter != index)
        return NULL;
    else
        return result;
}


bool ParseArgs(int argc, char* argv[])
{
    bool ret = true;
    DWORD dwFileInfo = 0;
    for(int i = 1; i < argc; i++)
    {
        std::string arg(argv[i]);

        if(arg.find("-i:") != std::string::npos)
        {
            size_t pos = arg.find(":");
            try 
            {
                g_Index = std::stoi(arg.substr(pos + 1), nullptr, 10); 
            }
            catch (...)
            {
                std::cout << "Invalid index"; 
                ret = false;
                goto Exit;
            }
        }

        else if(arg == "-t" || arg == "--top")
            g_Flags = HEADER; 
        else if(arg == "--help")
        {
            g_Flags = HELP;
            goto Exit;
        }
        else if(arg == "--debug")
            g_Flags |= DEBUG;

        arg.clear();
    }

    g_Filename = argv[argc - 1];
    dwFileInfo = GetFileAttributes(g_Filename.c_str()); 
    if(g_Filename.length() < 4)
    {
        std::cout << "Invalid file";
        ret = false;
        goto Exit;
    }
    if(dwFileInfo == INVALID_FILE_ATTRIBUTES || (dwFileInfo & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY || g_Filename.substr(g_Filename.length() - 4, 4) != ".csv")
    {
        std::cout << "Invalid file"; 
        ret = false;
    }

Exit:

    return ret;
}
