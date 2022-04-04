#include <iostream>
#include <string>
#include <memory>
#include <map>
#include <fstream>
using namespace std;

ifstream input("program.txt");

enum KeyWords {
    varSy,
    beginSy, endSy,
    andSy, orSy, notSy, lessSy, moreSy, lessEvenSy, moreEvenSy, notEvenSy,
    assignSy, plusSy, minusSy, multSy, subSy,
    leftBrSy, rightBrSy,
    ifSy, elseSy,
    whileSy, doSy,
    quotSy, colonSy, semiColonSy, dotSy,
};

map<string, int> keywords
{
    {"var", varSy},
    {"begin", beginSy},
    {"end", endSy},
    {"and", andSy},
    {"or", orSy},
    {"not", notSy},
    {"<", lessSy},
    {">", moreSy},
    {"<=", lessEvenSy},
    {">=", moreEvenSy},
    {"<>", notEvenSy},
    {":=", assignSy},
    {"+", plusSy},
    {"-", minusSy},
    {"*", multSy},
    {"/", subSy},
    {"(", leftBrSy},
    {")", rightBrSy},
    {"if", ifSy},
    {"else", elseSy},
    {"while", whileSy},
    {"do", doSy},
    {"\"", quotSy},
    {":", colonSy},
    {";", semiColonSy},
    {".", dotSy},
};


class CTextPosition {
private:
    int line_number;
    int ch_number;
public:
    int getLineNumber() { return line_number; };
    int getChNumber() { return ch_number; };
};

class CError {
private:
    int error_code;
    CTextPosition text_position;
public:
    int getErrorCode() { return error_code; };
    CTextPosition getTextPosition() { return text_position; };
};

class CioModule {
private:
    string buffer;
    int cur_ch;
public:
    char nextch()
    {
        if (cur_ch == buffer.size() && input.eof())
            return NULL;
        if (cur_ch == buffer.size() && !input.eof())
        {
            getline(input, buffer);
            cur_ch = 0;
        }
        cur_ch++;
        return buffer[cur_ch - 1];
    }
    void WriteError(CError error);
};
//DECL_PTR(CioModule);

class CLexer {
    //...
public:
    //CTokenPtr getNextToken(CioModule* io);
    char getNextToken(CioModule* io)
    {
        return io->nextch();
    };
};

int main() {
    auto io = new CioModule();
    //CTokenPtr token = nullptr;
    auto lexer = new CLexer();

    char ch;
    while (ch = lexer->getNextToken(io))
    {
        //cout << token->ToString();
        cout << ch << endl;
    }

    cout << "d o n e";

    delete io;
    delete lexer;
    input.close();
}