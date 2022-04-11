#include <iostream>
#include <string>
#include <memory>
#include <map>
#include <fstream>
using namespace std;

ifstream input("program.pas");

enum KeyWords {
    varSy,
    beginSy, endSy,
    andSy, orSy, notSy, lessSy, moreSy, lessEvenSy, moreEvenSy, notEvenSy,
    assignSy, plusSy, minusSy, multSy, subSy,
    leftBrSy, rightBrSy,
    ifSy, elseSy,
    whileSy, doSy,
    quotSy, colonSy, semiColonSy, dotSy,
    intSy, realSy, strSy, boolSy
};

map<string, KeyWords> keywords
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
    {"integer", intSy},
    {"real", realSy},
    {"string", strSy},
    {"Boolean", boolSy},
};

#define DECL_PTR(ClassName) using ClassName ## Ptr = unique_ptr<ClassName>;

#define PURE = 0
enum TokenType {
    ttIdent,
    ttKeyword,
    ttConst
};

class CToken {
private:
    TokenType tType;
public:
    CToken(TokenType _type) {}
    TokenType getType() { return tType; };
    virtual string ToString() PURE;
};

typedef CToken* CTokenPtr;
//DECL_PTR(CToken);

class CIdentToken : public CToken
{
private:
    string ident;
public:
    CIdentToken(string _ident): CToken(ttIdent), ident(_ident) {}
    string ToString() 
    {
        return ident;
    }
};
typedef CIdentToken* CIdentTokenPtr;
//DECL_PTR(CIdentToken);

class CKeywordToken : public CToken
{
private:
    string keyword;
public:
    CKeywordToken(string _keyword) : CToken(ttKeyword), keyword(_keyword) {}
    string ToString()
    {
        return keyword;
    }
};
typedef CKeywordToken* CKeywordTokenPtr;
//DECL_PTR(CKeywordToken);

class CIntConstToken : public CToken
{
private:
    int value;
public:
    CIntConstToken(int val) : CToken(ttConst), value(val) {}
    string ToString()
    {
        return to_string(value);
    }
};
typedef CIntConstToken* CIntConstTokenPtr;
//DECL_PTR(CIntConstToken);

class CRealConstToken : public CToken
{
private:
    double value;
public:
    CRealConstToken(double val) : CToken(ttConst), value(val) {}
    string ToString()
    {
        return to_string(value);
    }
};
typedef CRealConstToken* CRealConstTokenPtr;
//DECL_PTR(CRealConstToken);

class CStringConstToken : public CToken
{
private:
    string value;
public:
    CStringConstToken(string val) : CToken(ttConst), value(val) {}
    string ToString()
    {
        return value;
    }
};
typedef CStringConstToken* CStringConstTokenPtr;
//DECL_PTR(CStringConstToken);

class CBoolConstToken : public CToken
{
private:
    bool value;
public:
    CBoolConstToken(bool val) : CToken(ttConst), value(val) {}
    string ToString()
    {
        return value ? "true" : "false";
    }
};
typedef CBoolConstToken* CBoolConstTokenPtr;
//DECL_PTR(CBoolConstToken);

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
            return '\n';
        }
        cur_ch++;
        return buffer[cur_ch - 1];
    }
    void WriteError(CError error);
};
//DECL_PTR(CioModule);

class CLexer {
private:
    char cur_ch;
public:
    CTokenPtr getNextToken(CioModule* io)
    {
        if (!cur_ch || cur_ch == '\n' || cur_ch == ' ')
            cur_ch = io->nextch();

        if (!cur_ch)
            return nullptr;
        
        string next_t = "";

        if (cur_ch >= 'a' && cur_ch <= 'z' ||
            cur_ch >= 'A' && cur_ch <= 'Z')
        {
            while (cur_ch >= 'a' && cur_ch <= 'z' ||
                cur_ch >= 'A' && cur_ch <= 'Z' ||
                cur_ch >= '0' && cur_ch <= '9')
            {
                next_t += cur_ch;
                cur_ch = io->nextch();
            }

            if (next_t == "true")
                return new CBoolConstToken(true);
            if (next_t == "false")
                return new CBoolConstToken(false);

            if (keywords.count(next_t) != 0)
                return new CKeywordToken(next_t);

            return new CIdentToken(next_t);
        }
        if (cur_ch >= '0' && cur_ch <= '9')
        {
            while (cur_ch >= '0' && cur_ch <= '9' || cur_ch == '.')
            {
                next_t += cur_ch;
                cur_ch = io->nextch();
            }

            if (next_t.find('.') == string::npos)
                return new CIntConstToken(stoi(next_t));
            else
                return new CRealConstToken(stod(next_t));
        }

        next_t += cur_ch;
        //cur_ch = io->nextch();

        switch (cur_ch)
        {
        case ':':
            cur_ch = io->nextch();
            if (cur_ch == '=')
            {
                next_t += cur_ch;
                cur_ch = io->nextch();
            }
            return new CKeywordToken(next_t);
        case '<':
            cur_ch = io->nextch();
            if (cur_ch == '=')
            {
                next_t += cur_ch;
                cur_ch = io->nextch();
            }
            return new CKeywordToken(next_t);
        case '>':
            cur_ch = io->nextch();
            if (cur_ch == '=')
            {
                next_t += cur_ch;
                cur_ch = io->nextch();
            }
            return new CKeywordToken(next_t);
        case '\"':
            cur_ch = io->nextch();
            while (cur_ch != '\"')
            {
                next_t += cur_ch;
                cur_ch = io->nextch();
            }
            next_t += cur_ch;
            cur_ch = io->nextch();
            return new CStringConstToken(next_t);
       /* case ';':
            cur_ch = io->nextch();
            return next_t;*/

        default:
            cur_ch = io->nextch();
            return new CKeywordToken(next_t);
        }

        //return next_t;
    };
};

int main() {
    auto io = new CioModule();
    CTokenPtr token = nullptr;
    auto lexer = new CLexer();
    //string ch;
    //ch = lexer->getNextToken(io);

    //while (ch != "")
    //{
    //    //cout << token->ToString();
    //    cout << ch << endl;
    //    ch = lexer->getNextToken(io);
    //}

    while (token = lexer->getNextToken(io))
    {
        cout << token->ToString() << endl;
    }

    cout << "d o n e";

    delete io;
    delete lexer;
    input.close();
}