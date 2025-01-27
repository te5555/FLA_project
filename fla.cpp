#include <iostream>
#include <string>
#include <vector>
#include "pda.h"
#include "tm.h"

using namespace std;

// Function to check if a string is a valid hexadecimal number
// Function to display usage/help information
void printUsage()
{
    std::cout << "usage: fla [-h|--help] <pda> <input>" << std::endl;
    std::cout << "       fla [-v|--verbose] [-h|--help] <tm> <input>" << std::endl;
}

void processPDA(const std::string inputFileName, bool verbose, string input)
{
    PDA pda;
    PDA_Parser parser(&pda);

    parser.process(inputFileName);
    if (pda.interpret(input, verbose))
    {
        cout << "true" << endl;
    }
    else
    {
        cout << "false" << endl;
    }
}

void processTM(std::string inputFileName, bool verbose, string input)
{
    TM tm;
    TM_Parser parser(&tm);

    parser.process(inputFileName, input);
    tm.interpret(input, verbose);
    cout << "Result: ";
    tm.printResult();
    cout<<endl;
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printUsage();
        return 1;
    }

    std::string firstArg = argv[1];
    if (firstArg == "-h" || firstArg == "--help")
    {
        printUsage();
        return 0;
    }

    // check verbose mode
    bool verbose = false;
    if (firstArg == "-v" || firstArg == "--verbose")
    {
        verbose = true;
        if (argc < 4)
        {
            printUsage();
            return 1;
        }
        firstArg = argv[2];
    }

    // 获取文件名和输入字符串
    std::string filename = firstArg;
    std::string input = argv[argc - 1];

    // 判断文件后缀
    size_t extPos = filename.find_last_of('.');
    if (extPos == std::string::npos)
    {
        std::cerr << "Invalid file format. Expected '.pda' or '.tm'" << std::endl;
        return 1;
    }
    std::string extension = filename.substr(extPos);

    if (extension == ".pda")
    {
        processPDA(filename, verbose, input);
    }
    else if (extension == ".tm")
    {
        // cout<< verbose<<endl;
        processTM(filename, verbose, input);
    }
    else
    {
        std::cerr << "Unsupported file format. Expected '.pda' or '.tm'" << std::endl;
        return 1;
    }

    return 0;
}