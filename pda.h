#include <cstdio>
#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <fstream>
#include <stack>
#include <iomanip>

using namespace std;

class PDA
{
    // P = (Q,\Sigma, \Gamma, \delta, q_0, z_0, F)
public:
    using Transition = std::tuple<std::string, std::string, std::string, std::string, std::string>; // oldstate, input symbol, old stack top, new state, new stack top
    string extractToken(std::string &line, size_t &pos);                                            // remove whitespace and exact token
    void printPDAInfo();

    bool interpret(string &input, bool verbose);

    bool checkInput(string &input, bool verbose);
    void readStates(string &str);
    void readStackSymbols(string &str);
    void readTransitionFunction(string &str);
    void readInputSymbols(string &str);
    void readInitialState(string &str);
    void readFinalStates(string &str);
    void readInitialStack(string &str);
    void trim(std::string &str);
    void validateSymbol(const std::string &symbol);
    void resize_transFunction()
    {
        transFunction.clear();
        transFunction.resize(stateCounter);
    }
    int getStateNumber() { return stateCounter; }
    void printStep(int stepCnt, int curState, int inputIterator, const string &input);
    void checkInput(string &input);
    void printError(string errorSymbol, string input, size_t position);

private:

    map<string, int> stateMap;
    map<string, int> inputSymbolMap;
    map<string, int> stackSymbolMap;
    vector<string> states_;
    vector<string> stackSymbols_;
    vector<string> inputSymbols_;
    int stateCounter = 0;
    int inputSymbolCounter = 0;
    int stackSymbolCounter = 0;
    int q0;
    int z0;
    stack<int> st;

    set<int> finalStates;
    vector<vector<Transition>> transFunction;
    void printStack();
};
class PDA_Parser
{
public:
    PDA_Parser() : pda(nullptr) {} // 初始化 pda 为 nullptr
    explicit PDA_Parser(PDA *pda) : pda(pda) {}
    void process(const string inputFileName);

private:
    PDA *pda;
};
