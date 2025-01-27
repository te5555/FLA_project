#include <cstdio>
#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <fstream>
#include <deque>
#include <stack>
#include <iomanip>
using namespace std;
class TuringTape
{
private:
    deque<string> tape;
    int head = 0; // head can be negative, DequeIndex = RealIndex - head
    int mabs(int x) { return x < 0 ? -x : x; }
    int leftmost = 0;
    int rightmost = 0;

public:
    void initialize(string initialTape)
    {
        int len = initialTape.length();
        for (int i = 0; i < len; i++)
        {
            tape.push_back(initialTape.substr(i, 1));
        }
        rightmost = len - 1;
    }
    string getHead()
    {
        return tape[head - leftmost];
    }
    void write(string symbol)
    {
        if (symbol != "*")
            tape[head - leftmost] = symbol;
    }
    void move(string direction)
    {
        if (direction == "l")
        {
            if (head == leftmost)
            {
                tape.push_front("_");
                leftmost--;
            }
            head--;
        }
        else if (direction == "r")
        {
            if (head == rightmost)
            {
                tape.push_back("_");
                rightmost++;
            }
            head++;
        }
    }
    void printTape(int tapeNumber)
    {
        int firstNonEmpty = -114514, lastNonEmpty = 114514;
        for (int i = leftmost; i <= rightmost; i++)
        {
            if (tape[i - leftmost] != "_")
            {
                firstNonEmpty = i;
                break;
            }
        }
        for (int i = rightmost; i >= leftmost; i--)
        {
            if (tape[i - leftmost] != "_")
            {
                lastNonEmpty = i;
                break;
            }
        }
        if (firstNonEmpty == -114514) //
        {
            cout << left << setw(8) << "Index" + (to_string(tapeNumber)) << " : " << setw(3) << mabs(head) << endl;
            cout << left << setw(8) << "Tape" + (to_string(tapeNumber)) << " : " << setw(3) << "_" << endl;
            cout << left << setw(8) << "Head" + (to_string(tapeNumber)) << " : " << setw(3) << "^" << endl;
        }
        else
        {
            firstNonEmpty = min(firstNonEmpty, head);
            lastNonEmpty = max(lastNonEmpty, head);
            vector<int> indexWidth;
            int headIndex = 0;
            for (int i = firstNonEmpty; i <= lastNonEmpty; i++)
            {
                indexWidth.push_back((int)to_string(mabs(i)).length()+1);
                if (i < head){
                    headIndex += to_string(mabs(i)).length()+1;
                }
            }
            cout << left << setw(8) << "Index" + (to_string(tapeNumber)) << " : ";
            for (int i = firstNonEmpty; i <= lastNonEmpty; i++)
            {
                cout << setw(indexWidth[i-firstNonEmpty]) << mabs(i);
            }
            cout << endl;
            cout << left << setw(8) << "Tape" + (to_string(tapeNumber)) << " : ";
            for (int i = firstNonEmpty; i <= lastNonEmpty; i++)
            {
                cout << setw(indexWidth[i-firstNonEmpty]) << tape[i - leftmost];
            }
            cout << endl;
            cout << left << setw(8) << "Head" + (to_string(tapeNumber)) << " : ";
            cout << string(headIndex, ' ') << "^" << endl;
        }
    }
    void printTapeWithoutDetails()
    {
        int firstNonEmpty = -1, lastNonEmpty = -1;
        for (int i = 0; i < tape.size(); i++)
        {
            if (tape[i] != "_")
            {
                firstNonEmpty = i;
                break;
            }
        }
        for (int i = tape.size() - 1; i >= 0; i--)
        {
            if (tape[i] != "_")
            {
                lastNonEmpty = i;
                break;
            }
        }
        if (firstNonEmpty == -1) //
        {
            return;
        }
        else
        {
            for (int i = firstNonEmpty; i <= lastNonEmpty; i++)
            {
                cout << tape[i];
            }
        }
    }
};
class TM
{
public:
    void printError(string errorSymbol, string input, size_t position);
    void printStep(int stepCnt, int curState, const string &input);

    void interpret(string &input, bool verbose);
    void readStates(string &str);
    void trim(std::string &str);
    void readInputSymbols(string &str);
    void validateSymbol(const std::string &symbol);
    void readTapeSymbols(string &str);
    void readInitialState(string &str);
    void readBlankSymbol(string &str);
    void readFinalStates(string &str);
    void readTapeNumber(string &str);
    void readTransitionFunction(string &str);
    void checkSymbols(string &str);
    void checkDirections(string &str);
    string extractToken(std::string &line, size_t &pos); // remove whitespace and exact token
    void init(string input)
    {
        transFunction.clear();
        transFunction.resize(stateCounter);
        tapeLength.resize(tapeNumber);
        tape.resize(tapeNumber);
        tape[0].initialize(input);
        for (int i = 1; i < tapeNumber; i++)
        {
            tape[i].initialize("_");
        }
    }
    bool checkInput(string &input, bool verbose);
    void printTMInfo();
    bool matchTransition(string inputSymbols);
    void printResult();

private:
    using Transition = std::tuple<std::string, std::string, std::string, std::string, std::string>; // oldstate, old symbols, new symbols, direction, new state
    vector<int> tapeLength;
    vector<TuringTape> tape;
    map<string, int> stateMap;
    map<string, int> inputSymbolMap;
    map<string, int> tapeSymbolMap;
    vector<string> states_;
    vector<string> tapeSymbols_;
    vector<string> inputSymbols_;
    int stateCounter = 0;
    int inputSymbolCounter = 0;
    int tapeSymbolCounter = 0;
    int tapeNumber = 0;
    int q0;
    int blankSymbol;
    stack<int> st;
    vector<vector<Transition>> transFunction;

    set<int> finalStates;
};
class TM_Parser
{
public:
    TM_Parser() : tm(nullptr) {} // 初始化 pda 为 nullptr
    explicit TM_Parser(TM *tm) : tm(tm) {}
    void process(string inputFileName, string input);

private:
    TM *tm;
};
