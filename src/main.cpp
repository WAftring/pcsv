#include <iostream>
#include <fstream>
#include <string>

#define HELP 0x1
#define HEADER 0x2
#define DEBUG 0x4

int g_Flags = 0;
int g_Index = 0;
std::string g_Filename;
char* g_Delimitter = ",";
bool ParseArgs(int argc, char *argv[]);
std::string ParseLine(const std::string* line, char* delimiter, int index);

/* TODO
 * - Finish the arg parsing
 */
int main(int argc, char *argv[])
{

    std::string line;
    std::ifstream csvfile("Test.csv");

    if(!ParseArgs(argc, argv))
    {
        std::cout << "Invalid parameters. --help for full options" << std::endl;
        return 1;
    }
    if((g_Flags & DEBUG) == DEBUG)
    {

        std::cout << "g_Flags: " << g_Flags << std::endl;
        std::cout << "g_Index: " << g_Index << std::endl;
        std::cout << "g_Delimitter: " << g_Delimitter << std::endl;
        std::cout << "g_Filename: " << g_Filename << std::endl;

    }
    if(csvfile.is_open())
    {
        while(std::getline(csvfile, line))
        {
//            std::cout << ParseLine(&line, ",", 0) << std::endl;;
        }
        csvfile.close();
    }
    return 0;
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

    for(int i = 1; i < argc; i++)
    {
        std::string arg(argv[i]);
        if(arg.find("-i:") != std::string::npos)
        {
            size_t pos = arg.find(":");
            g_Index = std::stoi(arg.substr(pos + 1), nullptr, 10); 
        }
        else if(arg == "-t")
            g_Flags = HEADER; 
        else if(arg == "--help")
            g_Flags = HELP;
        else if(arg == "--debug")
            g_Flags |= DEBUG;
        else
            ret = false;

        arg.clear();
    }

    return ret;
}
