#include <cstdio>
#include <iostream>
#include <map>
#include <vector>
#include <set>
#include "pda.h"
#include <iomanip>
using namespace std;
void PDA::printStack()
{
    stack<int> tempStack = st;
    vector<int> elements;
    while (!tempStack.empty())
    {
        elements.push_back(tempStack.top());
        tempStack.pop();
    }
    for (int i = 0; i < elements.size(); ++i)
    {
        cout << stackSymbols_[elements[i]] << " ";
    }
    cout << endl;
}
void PDA::trim(std::string &str) // remove whitespace
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
void PDA_Parser ::process(string inputFileName)
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
        // cerr<<"DEBUG: line ?"<< cnt << " is read"<<endl;
        if (line.empty() || line[0] == ';')
        {
            continue;
        }
        if (line[0] == '#')
        {
            if (line.rfind("#Q = ", 0) == 0)
            {
                pda->readStates(line);
                pda->resize_transFunction();
            }
            else if (line.rfind("#S = ", 0) == 0)
            {
                pda->readInputSymbols(line);
            }
            else if (line.rfind("#G = ", 0) == 0)
            {
                pda->readStackSymbols(line);
            }
            else if (line.rfind("#q0 = ", 0) == 0)
            {
                pda->readInitialState(line);
            }
            else if (line.rfind("#z0 = ", 0) == 0)
            {
                pda->readInitialStack(line);
            }
            else if (line.rfind("#F = ", 0) == 0)
            {
                pda->readFinalStates(line);
            }
            else
            {
                std::cerr << "syntax error" << std::endl;
                exit(1);
            }
        }
        else
        {
            pda->readTransitionFunction(line);
        }
    }
}

void PDA ::readStates(std::string &str)
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
void PDA ::readFinalStates(std::string &str)
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
void PDA::validateSymbol(const std::string &symbol)
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
void PDA::readInputSymbols(std::string &str)
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
void PDA::readStackSymbols(std::string &str)
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
        stackSymbolMap[symbol] = stackSymbolCounter++;
        stackSymbols_.push_back(symbol);
    }

    // add empty symbol
    stackSymbolMap["_"] = stackSymbolCounter++;
    stackSymbols_.push_back("_");
}
void PDA::readInitialState(std::string &str)
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
void PDA::readInitialStack(std::string &str)
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
    auto it = stackSymbolMap.find(symbol);
    if (it != stackSymbolMap.end())
    {
        z0 = it->second;
    }
    else
    {
        cerr << "syntax error" << endl;
        exit(1);
    }
}
std::string PDA::extractToken(std::string &line, size_t &pos)
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
void PDA::readTransitionFunction(std::string &line)
{

    size_t pos = 0;

    std::string oldState = extractToken(line, pos);
    auto it = stateMap.find(oldState);
    if (it == stateMap.end())
    {
        std::cerr << "syntax error" << std::endl;
        exit(1);
    }

    std::string inputSymbolStr = extractToken(line, pos);
    it = inputSymbolMap.find(inputSymbolStr);
    if (it == inputSymbolMap.end())
    {
        std::cerr << "syntax error" << std::endl;
        exit(1);
    }

    std::string oldStackTopStr = extractToken(line, pos);
    it = stackSymbolMap.find(oldStackTopStr);
    if (it == stackSymbolMap.end())
    {
        std::cerr << "syntax error" << std::endl;
        exit(1);
    }

    std::string newState = extractToken(line, pos);
    it = stateMap.find(newState);
    if (it == stateMap.end())
    {
        std::cerr << "syntax error" << std::endl;
        exit(1);
    }
    std::string stackTopSeq = line.substr(pos);
    trim(stackTopSeq);
    int len = stackTopSeq.length();
    for (int i = 0; i < len; i++)
    {
        auto it = stackSymbolMap.find(stackTopSeq.substr(i, 1));
        if (it == stackSymbolMap.end())
        {
            std::cerr << "syntax error" << std::endl;
            exit(1);
        }
    }
    // cerr << oldState << " " << stateMap[oldState] << endl;
    transFunction[stateMap[oldState]].push_back(std::make_tuple(oldState, inputSymbolStr, oldStackTopStr, newState, stackTopSeq));
}
void PDA::printPDAInfo()
{
    std::cout << "PDA Information:" << std::endl;

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
    std::cout << "Stack Symbols (Γ):" << std::endl;
    for (const auto &symbol : stackSymbols_)
    {
        std::cout << "  " << symbol << " (ID: " << stackSymbolMap.at(symbol) << ")" << std::endl;
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
    std::cout << "Initial Stack Symbol (z0):" << std::endl;
    for (const auto &pair : stackSymbolMap)
    {
        if (pair.second == z0)
        {
            std::cout << "  " << pair.first << " (ID: " << z0 << ")" << std::endl;
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

    // 输出转移函数
    std::cout << "Transition Functions (Δ):" << std::endl;
    for (int i = 0; i < stateCounter; i++)
    {
        if (!transFunction[i].empty())
        {
            for (const auto &transition : transFunction[i])
            {
                std::cout << "  ("
                          << std::get<0>(transition) << ", "
                          << std::get<1>(transition) << ", "
                          << std::get<2>(transition) << ") -> ("
                          << std::get<3>(transition) << ", "
                          << std::get<4>(transition) << ")" << std::endl;
            }
        }
    }
}
void PDA::printError(string errorSymbol, string input, size_t position)
{
    std::cout << "==================== ERR ====================" << std::endl;
    std::cout << "error: '" << errorSymbol << "' was not declared in the set of input symbols" << std::endl;
    std::cout << "Input: " << input << std::endl;

    std::cout << std::string(position+7, ' ') << "^" << std::endl;

    std::cout << "==================== END ====================" << std::endl;
}
bool PDA::checkInput(string &input, bool verbose)
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
        }
    }
    return true;
}
void PDA::printStep(int stepCnt, int curState, int inputIterator, const string &input)
{
    cout << setw(8) << "Step" << " : " << stepCnt << endl;
    cout << setw(8) << "State" << " : " << states_[curState] << endl;

    // Print the input tape with the current input position marked
    std::cout << setw(8) << "Input" << " : " << input << std::endl;

    std::cout << std::string(inputIterator + 11, ' ') << "^" << std::endl;

    std::cout << setw(8) << "Stack" << " : ";
    printStack();
    // Print the stack content

    std::cout << "---------------------------------------------" << endl;
}

bool PDA::interpret(string &input, bool verbose)
{
    if (!checkInput(input, verbose))
    {
        return false;
    }
    int stepCnt = 0;
    int inputIterator = 0;
    bool accept = false;
    int curState = q0;
    st.push(z0);
    if (verbose)
    {
        cout << "==================== RUN ====================" << endl;
        printStep(0, curState, inputIterator, input);
    }

    while (inputIterator < input.length())
    {
        // find feasible transition
        bool canTransition = 0;
        for (int i = 0; i < transFunction[curState].size(); i++)
        {
            string targetSymbol = std::get<1>(transFunction[curState][i]);
            string oldTop = std::get<2>(transFunction[curState][i]);
            string newState = std::get<3>(transFunction[curState][i]);
            string newTopSeq = std::get<4>(transFunction[curState][i]);
            if (st.empty())
                return false;
            if (input.substr(inputIterator, 1) == targetSymbol && st.top() == stackSymbolMap[oldTop])
            {
                st.pop();
                if (newTopSeq != "_")
                {
                    int newlen = newTopSeq.length();
                    for (int j = newlen - 1; j >= 0; j--)
                    {
                        st.push(stackSymbolMap[newTopSeq.substr(j, 1)]);
                    }
                }
                curState = stateMap[newState];
                inputIterator++;
                if (verbose)
                    printStep(++stepCnt, curState, inputIterator, input);
                canTransition = 1;
                break;
            }
        }
        if (!canTransition)
            return false;
    }
    if (finalStates.find(curState) != finalStates.end())
    {
        return true;
    }
    int FinalstepCnt = 0;
    while (FinalstepCnt <= stateCounter)
    {
        FinalstepCnt++;
        for (int i = 0; i < transFunction[curState].size(); i++)
        {
            string targetSymbol = std::get<1>(transFunction[curState][i]);
            string oldTop = std::get<2>(transFunction[curState][i]);
            string newState = std::get<3>(transFunction[curState][i]);
            string newTopSeq = std::get<4>(transFunction[curState][i]);
            if (st.empty())
                return false;
            if (st.top() == stackSymbolMap[oldTop])
            {
                if (newTopSeq != "_")
                {
                    int newlen = newTopSeq.length();
                    for (int j = newlen; j >= 0; j--)
                    {
                        st.push(stackSymbolMap[newTopSeq.substr(j, 1)]);
                    }
                }
                curState = stateMap[newState];
                if (verbose)
                    printStep(stepCnt + FinalstepCnt, curState, inputIterator, input);
                if (finalStates.find(curState) != finalStates.end())
                {
                    return true;
                }
            }
        }
    }
    return false;
}
