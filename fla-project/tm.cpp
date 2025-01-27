#include <cstdio>
#include <iostream>
#include <map>
#include <vector>
#include <set>
#include "tm.h"
#include <iomanip>
void TM_Parser::process(std::string inputFileName, string input)
{
    int cnt = 0; // debug output
    ifstream inputFile(inputFileName);
    if (!inputFile.is_open())
    {
        std::cerr << "Failed to open input file" << std::endl;
        exit(1);
    }
    string line;
    while (std::getline(inputFile, line))
    {
        cnt++;
        // cerr<<"DEBUG: line "<< cnt << " is read"<<endl;
        int commentPos = line.find(';');
        if (line.empty() || line[0] == ';')
        {
            continue;
        }
        if (commentPos != string::npos)
        {
            line = line.substr(0, commentPos);
        }
        // cerr<<"DEBUG: line "<< cnt << " is read"<<endl;

        if (line[0] == '#')
        {
            if (line.rfind("#Q = ", 0) == 0)
            {
                tm->readStates(line);
            }
            else if (line.rfind("#S = ", 0) == 0)
            {
                tm->readInputSymbols(line);
            }
            else if (line.rfind("#G = ", 0) == 0)
            {
                tm->readTapeSymbols(line);
            }
            else if (line.rfind("#q0 = ", 0) == 0)
            {
                tm->readInitialState(line);
            }
            else if (line.rfind("#B = ", 0) == 0)
            {
                tm->readBlankSymbol(line);
            }
            else if (line.rfind("#F = ", 0) == 0)
            {
                tm->readFinalStates(line);
            }
            else if (line.rfind("#N = ", 0) == 0)
            {
                tm->readTapeNumber(line);
                tm->init(input);
            }
            else
            {
                std::cerr << "syntax error" << std::endl;
                exit(1);
            }
        }
        else
        {
            tm->readTransitionFunction(line);
        }
    }
}
void TM::readStates(std::string &str)
{
    size_t equalPos = str.find('=');
    if (equalPos == std::string::npos)
    {
        std::cerr << "syntax error" << std::endl;
        exit(1);
    }

    size_t braceStart = str.find('{', equalPos);
    size_t braceEnd = str.find('}', braceStart);

    if (braceStart == std::string::npos || braceEnd == std::string::npos)
    {
        std::cerr << "syntax error" << std::endl;
        exit(1);
    }

    std::string states = str.substr(braceStart + 1, braceEnd - braceStart - 1);

    std::vector<std::string> stateList;
    size_t pos = 0, commaPos;
    while ((commaPos = states.find(',', pos)) != std::string::npos)
    {
        std::string state = states.substr(pos, commaPos - pos);
        trim(state);
        if (!state.empty())
        {
            stateList.push_back(state);
        }
        pos = commaPos + 1;
    }

    std::string lastState = states.substr(pos);
    trim(lastState);
    if (!lastState.empty())
    {
        stateList.push_back(lastState);
    }

    for (const string &state : stateList)
    {
        stateMap[state] = stateCounter++;
        states_.push_back(state);
    }
    // cerr<<stateMap.size()<<endl;
}
void TM::trim(std::string &str) // remove whitespace
{
    size_t start = 0;
    while (start < str.size() && std::isspace(static_cast<unsigned char>(str[start])))
    {
        ++start;
    }
    size_t end = str.size();
    while (end > start && std::isspace(static_cast<unsigned char>(str[end - 1])))
    {
        --end;
    }
    str = str.substr(start, end - start);
}
void TM::readInputSymbols(std::string &str)
{
    size_t equalPos = str.find('=');
    if (equalPos == std::string::npos)
    {
        std::cerr << "syntax error" << std::endl;
        exit(1);
    }

    size_t braceStart = str.find('{', equalPos);
    size_t braceEnd = str.find('}', braceStart);

    if (braceStart == std::string::npos || braceEnd == std::string::npos)
    {
        std::cerr << "syntax error" << std::endl;
        exit(1);
    }

    std::string symbols = str.substr(braceStart + 1, braceEnd - braceStart - 1);

    std::vector<std::string> symbolList;
    size_t pos = 0, commaPos;
    while ((commaPos = symbols.find(',', pos)) != std::string::npos)
    {
        std::string symbol = symbols.substr(pos, commaPos - pos);
        trim(symbol);
        if (!symbol.empty())
        {
            validateSymbol(symbol);
            symbolList.push_back(symbol);
        }
        pos = commaPos + 1;
    }

    std::string lastSymbol = symbols.substr(pos);
    trim(lastSymbol);
    if (!lastSymbol.empty())
    {
        validateSymbol(lastSymbol);
        symbolList.push_back(lastSymbol);
    }
    for (const auto &symbol : symbolList)
    {
        if (symbol == "_" || symbol.length() != 1)
        {
            cerr << "syntax error" << std::endl;
            exit(1);
        }
        inputSymbolMap[symbol] = inputSymbolCounter++;
        inputSymbols_.push_back(symbol);
    }
    inputSymbolMap["_"] = inputSymbolCounter++;
    inputSymbols_.push_back("_");
}
void TM::validateSymbol(const std::string &symbol)
{
    if (symbol.size() != 1)
    {
        std::cerr << "syntax error" << std::endl;
        exit(1);
    }

    char ch = symbol[0];
    if (!std::isprint(static_cast<unsigned char>(ch)) ||
        ch == ' ' || ch == ',' || ch == ';' || ch == '{' || ch == '}' || ch == '*')
    {
        std::cerr << "syntax error" << std::endl;
        exit(1);
    }
}
void TM::readTapeSymbols(std::string &str)
{
    size_t equalPos = str.find('=');
    if (equalPos == std::string::npos)
    {
        std::cerr << "syntax error" << std::endl;
        exit(1);
    }

    size_t braceStart = str.find('{', equalPos);
    size_t braceEnd = str.find('}', braceStart);

    if (braceStart == std::string::npos || braceEnd == std::string::npos)
    {
        std::cerr << "syntax error" << std::endl;
        exit(1);
    }

    std::string symbols = str.substr(braceStart + 1, braceEnd - braceStart - 1);

    std::vector<std::string> symbolList;
    size_t pos = 0, commaPos;
    while ((commaPos = symbols.find(',', pos)) != std::string::npos)
    {
        std::string symbol = symbols.substr(pos, commaPos - pos);
        trim(symbol);
        if (!symbol.empty())
        {
            validateSymbol(symbol);
            symbolList.push_back(symbol);
        }
        pos = commaPos + 1;
    }

    std::string lastSymbol = symbols.substr(pos);
    trim(lastSymbol);
    if (!lastSymbol.empty())
    {
        validateSymbol(lastSymbol);
        symbolList.push_back(lastSymbol);
    }
    for (const auto &symbol : symbolList)
    {
        tapeSymbolMap[symbol] = tapeSymbolCounter++;
        tapeSymbols_.push_back(symbol);
    }
}
void TM::readInitialState(std::string &str)
{
    size_t equalPos = str.find('=');
    if (equalPos == std::string::npos)
    {
        std::cerr << "syntax error" << std::endl;
        exit(1);
    }
    int len = str.length();
    std::string symbol = str.substr(equalPos + 2, len - equalPos - 1);
    auto it = stateMap.find(symbol);
    if (it != stateMap.end())
    {
        q0 = it->second;
    }
    else
    {
        cerr << "syntax error" << endl;
        exit(1);
    }
}
void TM::readBlankSymbol(std::string &str)
{
    size_t equalPos = str.find('=');
    if (equalPos == std::string::npos)
    {
        std::cerr << "syntax error" << std::endl;
        exit(1);
    }
    int len = str.length();
    std::string symbol = str.substr(equalPos + 2, len - equalPos - 1);
    // cerr << symbol << " "<< std::endl; //debug
    auto it = tapeSymbolMap.find(symbol);
    if (it != tapeSymbolMap.end())
    {
        blankSymbol = it->second;
    }
    else
    {
        cerr << "syntax error" << endl;
        exit(1);
    }
    if (symbol != "_")
    {
        cerr << "invalid blank symbol" << endl;
        exit(1);
    }
}
void TM ::readFinalStates(std::string &str)
{
    size_t equalPos = str.find('=');
    if (equalPos == std::string::npos)
    {
        std::cerr << "syntax error" << std::endl;
        exit(1);
    }

    size_t braceStart = str.find('{', equalPos);
    size_t braceEnd = str.find('}', braceStart);

    if (braceStart == std::string::npos || braceEnd == std::string::npos)
    {
        std::cerr << "syntax error" << std::endl;
        exit(1);
    }

    std::string states = str.substr(braceStart + 1, braceEnd - braceStart - 1);

    std::vector<std::string> stateList;
    size_t pos = 0, commaPos;
    while ((commaPos = states.find(',', pos)) != std::string::npos)
    {
        std::string state = states.substr(pos, commaPos - pos);
        trim(state);
        if (!state.empty())
        {
            stateList.push_back(state);
        }
        pos = commaPos + 1;
    }

    std::string lastState = states.substr(pos);
    trim(lastState);
    if (!lastState.empty())
    {
        stateList.push_back(lastState);
    }

    for (const string &state : stateList)
    {
        auto it = stateMap.find(state);
        if (it != stateMap.end())
        {
            finalStates.insert(it->second);
        }
        else
        {
            cerr << "syntax error" << endl;
            exit(1);
        }
    }
}
void TM::readTapeNumber(std::string &str)
{
    size_t equals_pos = str.find('=');
    if (equals_pos != std::string::npos)
    {
        std::string number_str = str.substr(equals_pos + 1);
        try
        {
            int N = std::stoi(number_str);
            tapeNumber = N;
        }
        catch (const std::invalid_argument &e)
        {
            cerr << "syntax error" << endl;
        }
        catch (const std::out_of_range &e)
        {
            cerr << "syntax error" << endl;
        }
    }
    else
    {
        cerr << "syntax error" << endl;
    }
}
void TM::readTransitionFunction(string &line)
{
    size_t pos = 0;

    std::string oldState = extractToken(line, pos);
    // debug
    //  cerr << oldState << " " << stateMap[oldState] << endl; // debug
    auto it = stateMap.find(oldState);

    if (it == stateMap.end())
    {
        std::cerr << "syntax error" << std::endl;
        exit(1);
    }

    std::string oldSymbols = extractToken(line, pos);
    if (oldSymbols.length() != tapeNumber)
    {
        cerr << "syntax error" << endl;
    }
    // cerr << oldSymbols << endl; // debug
    checkSymbols(oldSymbols);
    std::string newSymbols = extractToken(line, pos);
    if (newSymbols.length() != tapeNumber)
    {
        cerr << "syntax error" << endl;
    }
    checkSymbols(newSymbols);
    std::string directions = extractToken(line, pos);
    if (directions.length() != tapeNumber)
    {
        cerr << "syntax error" << endl;
    }
    checkDirections(directions);
    std::string newState = extractToken(line, pos);
    trim(newState);
    it = stateMap.find(newState);
    if (it == stateMap.end())
    {
        std::cerr << "syntax error" << std::endl;
        exit(1);
    }
    // cerr << oldState << " " << stateMap[oldState] << endl;
    transFunction[stateMap[oldState]].push_back(std::make_tuple(oldState, oldSymbols, newSymbols, directions, newState));
}
std::string TM::extractToken(std::string &line, size_t &pos)
{
    size_t start = line.find_first_not_of(' ', pos);
    if (start == std::string::npos)
    {
        std::cerr << "syntax error" << std::endl;
        exit(1);
    }

    size_t end = line.find(' ', start);
    pos = (end == std::string::npos) ? line.size() : end;
    return line.substr(start, end - start);
}
void TM::checkSymbols(std::string &str)
{
    int len = str.length();
    for (int i = 0; i < len; i++)
    {
        string symbol = str.substr(i, 1);
        if (symbol == "*")
            continue;
        auto it = tapeSymbolMap.find(symbol);
        if (it == tapeSymbolMap.end())
        {
            cerr << "syntax error" << endl;
            exit(1);
        }
    }
}
void TM::checkDirections(std::string &str)
{
    int len = str.length();
    for (int i = 0; i < len; i++)
    {
        if (str[i] != 'l' && str[i] != 'r' && str[i] != '*')
        {
            cerr << "syntax error" << endl;
            exit(1);
        }
    }
}
void TM::printTMInfo()
{
    std::cout << "TM Information:" << std::endl;
    std::cout << "Number of Tape Symbols (N): " << tapeNumber << std::endl;
    // 输出状态集合
    std::cout << "States (Q):" << std::endl;
    for (const auto &state : states_)
    {
        std::cout << "  " << state << " (ID: " << stateMap.at(state) << ")" << std::endl;
    }

    // 输出输入符号集合
    std::cout << "Input Symbols (Σ):" << std::endl;
    for (const auto &symbol : inputSymbols_)
    {
        std::cout << "  " << symbol << " (ID: " << inputSymbolMap.at(symbol) << ")" << std::endl;
    }

    // 输出栈符号集合
    std::cout << "Tape Symbols (Γ):" << std::endl;
    for (const auto &symbol : tapeSymbols_)
    {
        std::cout << "  " << symbol << " (ID: " << tapeSymbolMap.at(symbol) << ")" << std::endl;
    }

    // 输出初始状态
    std::cout << "Initial State (q0):" << std::endl;
    for (const auto &pair : stateMap)
    {
        if (pair.second == q0)
        {
            std::cout << "  " << pair.first << " (ID: " << q0 << ")" << std::endl;
            break;
        }
    }

    // 输出初始栈顶符号
    std::cout << "Blank Tape Symbol (B):" << std::endl;
    for (const auto &pair : tapeSymbolMap)
    {
        if (pair.second == blankSymbol)
        {
            std::cout << "  " << pair.first << " (ID: " << blankSymbol << ")" << std::endl;
            break;
        }
    }

    // 输出最终状态集合
    std::cout << "Final States (F):" << std::endl;
    for (int finalStateID : finalStates)
    {
        for (const auto &pair : stateMap)
        {
            if (pair.second == finalStateID)
            {
                std::cout << "  " << pair.first << " (ID: " << finalStateID << ")" << std::endl;
                break;
            }
        }
    }

    std::cout << "Transition Functions (Δ):" << std::endl;
    for (int i = 0; i < stateCounter; i++)
    {
        if (!transFunction[i].empty())
        {
            for (const auto &transition : transFunction[i])
            {
                std::cout << std::get<0>(transition) << " "
                          << std::get<1>(transition) << " "
                          << std::get<2>(transition) << " "
                          << std::get<3>(transition) << " "
                          << std::get<4>(transition) << " " << std::endl;
            }
        }
    }
}
void TM::interpret(std::string &input, bool verbose)
{
    int stepCnt = 0;
    // int inputIterator = 0;
    bool accept = false;
    int curState = q0;
    if (!checkInput(input, verbose))
    {
        return;
    }
    if (verbose)
    {
        cout << "==================== RUN ====================" << endl;
        printStep(0, curState, input);
    }
    while (!accept)
    {
        stepCnt++;
        bool canTransition = false;
        for (int i = 0; i < transFunction[curState].size(); i++)
        {
            string oldSymbols = std::get<1>(transFunction[curState][i]);
            string newSymbols = std::get<2>(transFunction[curState][i]);
            string directions = std::get<3>(transFunction[curState][i]);
            string newState = std::get<4>(transFunction[curState][i]);
            if (matchTransition(oldSymbols))
            {
                canTransition = true;
                curState = stateMap[newState];
                for (int j = 0; j < tapeNumber; j++)
                {
                    tape[j].write(newSymbols.substr(j, 1));
                }
                for (int j = 0; j < tapeNumber; j++)
                {
                    tape[j].move(directions.substr(j, 1));
                }
                auto it = finalStates.find(curState);
                // if (it != finalStates.end())
                // {
                //     accept = true;
                // }
                if (verbose)
                    printStep(stepCnt, curState, input);
                break;
            }
        }
        if (!canTransition)
        {
            break;
        }
    }
}
bool TM::matchTransition(string oldSymbols)
{
    for (int i = 0; i < tapeNumber; i++)
    {
        if (oldSymbols.substr(i, 1) == "*" && tape[i].getHead() != "_")
        {
            continue;
        }
        if (oldSymbols.substr(i, 1) != tape[i].getHead())
        {
            return false;
        }
    }
    return true;
}
bool TM::checkInput(string &input, bool verbose)
{
    int len = input.length();
    for (int i = 0; i < len; i++)
    {
        string str = input.substr(i, 1);
        auto it = inputSymbolMap.find(str);
        if (it == inputSymbolMap.end())
        {
            if (verbose)
            {
                printError(str, input, i);
                return false;
            }
            else {
                cerr << "illegal input" << endl;
                exit(1);
            }
        }
    }
    return true;
}
void TM::printError(string errorSymbol, string input, size_t position)
{
    std::cout << "==================== ERR ====================" << std::endl;
    std::cout << "error: '" << errorSymbol << "' was not declared in the set of input symbols" << std::endl;
    std::cout << "Input: " << input << std::endl;

    std::cout << std::string(position + 7, ' ') << "^" << std::endl;

    std::cout << "==================== END ====================" << std::endl;
    exit(1);
}
void TM::printStep(int stepCnt, int curState, const string &input)
{
    cout << left<< setw(8) << "Step" << " : " << stepCnt << endl;
    cout << left<< setw(8) << "State" << " : " << states_[curState] << endl;

    for (int i = 0; i < tapeNumber; i++)
    {
        tape[i].printTape(i);
    }

    std::cout << "---------------------------------------------" << endl;
}
void TM::printResult()
{
    tape[0].printTapeWithoutDetails();
}