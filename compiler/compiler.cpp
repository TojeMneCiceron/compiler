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
    programSy,
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
    {"program", programSy}
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
    KeyWords code;
public:
    CKeywordToken(string _keyword) : CToken(ttKeyword), keyword(_keyword), code(keywords[_keyword]) {}
    KeyWords getCode()
    {
        return code;
    }
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
    CTextPosition(int _line_number, int _ch_number) {}
    int getLineNumber() { return line_number; };
    int getChNumber() { return ch_number; };
};
typedef CTextPosition* CTextPositionPtr;

class CError {
private:
    string error_code;
    CTextPositionPtr text_position;
public:
    CError(string code, CTextPositionPtr tp) {}
    string getErrorCode() { return error_code; };
    CTextPositionPtr getTextPosition() { return text_position; };
};
typedef CError* CErrorPtr;

class CioModule {
private:
    string buffer;
    int cur_ch;
    int cur_line = 0;
public:
    tuple<char, CTextPositionPtr> nextch()
    {
        if (cur_ch == buffer.size() && input.eof())
            return make_tuple(NULL, new CTextPosition(cur_line, cur_ch));
        if (cur_ch == buffer.size() && !input.eof())
        {
            getline(input, buffer);
            cur_ch = 0;
            cur_line++;
            return make_tuple('\n', new CTextPosition(cur_line, cur_ch));
        }
        cur_ch++;
        return make_tuple(buffer[cur_ch - 1], new CTextPosition(cur_line, cur_ch));
    }
    //void WriteError(CError error)
    void WriteError(string error)
    {
        cout << error << endl;
    }
    void WriteError(CError* error)
    {
        cout << "=== error...\n";
        /*cout << "\n=== line: " << error->getTextPosition()->getLineNumber() << "char: " << error->getTextPosition()->getChNumber();
        cout << "\n=== error code: " << error->getErrorCode();*/
    }
};
//DECL_PTR(CioModule);
auto io = new CioModule();

class CLexer {
private:
    char cur_ch;
    CTextPositionPtr cur_tp;
public:
    CTokenPtr getNextToken()
    {
        if (!cur_ch || cur_ch == '\n' || cur_ch == ' ')
            tie(cur_ch, cur_tp) = io->nextch();

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
                tie(cur_ch, cur_tp) = io->nextch();
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
                tie(cur_ch, cur_tp) = io->nextch();
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
            tie(cur_ch, cur_tp) = io->nextch();
            if (cur_ch == '=')
            {
                next_t += cur_ch;
                tie(cur_ch, cur_tp) = io->nextch();
            }
            return new CKeywordToken(next_t);
        case '<':
            tie(cur_ch, cur_tp) = io->nextch();
            if (cur_ch == '=')
            {
                next_t += cur_ch;
                tie(cur_ch, cur_tp) = io->nextch();
            }
            return new CKeywordToken(next_t);
        case '>':
            tie(cur_ch, cur_tp) = io->nextch();
            if (cur_ch == '=')
            {
                next_t += cur_ch;
                tie(cur_ch, cur_tp) = io->nextch();
            }
            return new CKeywordToken(next_t);
        case '\"':
            tie(cur_ch, cur_tp) = io->nextch();
            while (cur_ch != '\"')
            {
                next_t += cur_ch;
                tie(cur_ch, cur_tp) = io->nextch();
                if (cur_ch == '\n')
                {
                    io->WriteError(new CError("лекс ошибка???", cur_tp));
                    break;
                }
            }
            if (cur_ch != '\n')
                next_t += cur_ch;
            tie(cur_ch, cur_tp) = io->nextch();
            return new CStringConstToken(next_t);
       /* case ';':
            tie(cur_ch, cur_tp) = io->nextch();
            return next_t;*/

        default:
            tie(cur_ch, cur_tp) = io->nextch();
            return new CKeywordToken(next_t);
        }

        //return next_t;
    };
};
auto lexer = new CLexer();

class CSyntax {
private:
    CTokenPtr curToken;
public:

    void accept(KeyWords keyword)
    {
        if (curToken->getType() == ttKeyword)
        {
            auto kw = dynamic_cast<CKeywordTokenPtr>(curToken);
            if (kw->getCode() != keyword)
                io->WriteError("expected keyword: " + keyword);
            curToken = lexer->getNextToken();
        }
        else
            io->WriteError("expected keyword: " + keyword);
    }

    void accept(TokenType tt)
    {
        if (curToken->getType() != tt)
            io->WriteError("expected token type: " + tt);
        curToken = lexer->getNextToken();
    }

    void typepart() 
    {

    }
    void varpart()
    {
        accept(varSy);


    }
    void statementpart()
    {
        accept(beginSy);


        accept(endSy);
    }

    void block()
    {
        typepart();
        varpart();
        statementpart();
    }

    void program()
    {
        curToken = lexer->getNextToken();
        accept(programSy);
        accept(ttIdent);
        accept(semiColonSy);
        block();
        accept(dotSy);
    }
};

int main() {
    CTokenPtr token = nullptr;
    

    /*while (token = lexer->getNextToken())
    {
        cout << token->ToString() << endl;
    }*/

    cout << "d o n e";

    delete io;
    delete lexer;
    input.close();
}