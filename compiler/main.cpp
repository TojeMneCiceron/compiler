#include <iostream>
#include <string>
#include <memory>
#include <map>
#include <fstream>
using namespace std;

ifstream input("whileif.pas");

enum KeyWords {
    varSy,
    beginSy, endSy,
    andSy, orSy, notSy, evenSy, lessSy, moreSy, lessEvenSy, moreEvenSy, notEvenSy,
    assignSy, plusSy, minusSy, multSy, subSy, divSy, modSy,
    leftBrSy, rightBrSy,
    ifSy, thenSy, elseSy,
    whileSy, doSy,
    quotSy, colonSy, semiColonSy, dotSy, commaSy,
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
    {"=", evenSy},
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
    {"div", divSy},
    {"mod", modSy},
    {"(", leftBrSy},
    {")", rightBrSy},
    {"if", ifSy},
    {"then", thenSy},
    {"else", elseSy},
    {"while", whileSy},
    {"do", doSy},
    {"\"", quotSy},
    {":", colonSy},
    {";", semiColonSy},
    {".", dotSy},
    {",", commaSy},
    {"program", programSy}
};


#define DECL_PTR(ClassName) using ClassName ## Ptr = unique_ptr<ClassName>;

#define PURE = 0
enum TokenType {
    ttIdent ,
    ttKeyword,
    ttConst
};

class CToken {
private:
    TokenType tType;
public:
    CToken(TokenType _type) { tType = _type; }
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

class CCharConstToken : public CToken
{
private:
    char value;
public:
    CCharConstToken(char val) : CToken(ttConst), value(val) {}
    string ToString()
    {
        return "" + value;
    }
};
typedef CCharConstToken* CCharConstTokenPtr;
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
            //cout << buffer << endl;
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
        if (!cur_ch)
            tie(cur_ch, cur_tp) = io->nextch();

        if (!cur_ch)
            return nullptr;

        while (cur_ch == '\n' || cur_ch == ' ')
            tie(cur_ch, cur_tp) = io->nextch();
        
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
            if (cur_ch == '=' || cur_ch == '>')
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
        case '\'':
            //char
            tie(cur_ch, cur_tp) = io->nextch();
            next_t += cur_ch;
            // '
            tie(cur_ch, cur_tp) = io->nextch();
            next_t += cur_ch;
            tie(cur_ch, cur_tp) = io->nextch();
            return new CCharConstToken(next_t[1]);
       /* case ';':
            tie(cur_ch, cur_tp) = io->nextch();
            return next_t;*/

        default:
            tie(cur_ch, cur_tp) = io->nextch();
            //if (keywords.find(next_t) != keywords.end())
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
                io->WriteError("\t>expected keyword: " + to_string((int)keyword));
            curToken = lexer->getNextToken();
            if (curToken != nullptr)
                cout << curToken->ToString() << endl;
        }
        else
            io->WriteError("\t>expected keyword: " + to_string((int)keyword));
    }

    void accept(TokenType tt)
    {
        if (curToken->getType() != tt)
            io->WriteError("\t>expected token type: " + to_string((int)tt));
        curToken = lexer->getNextToken();
        if (curToken != nullptr)
            cout << curToken->ToString() << endl;
    }

    void typepart() 
    {

    }
    void vardeclaration()
    {
        accept(ttIdent);
        if (curToken->getType() == ttKeyword)
        {
            auto kw = dynamic_cast<CKeywordTokenPtr>(curToken);        
            while (kw->getCode() == commaSy)
            {
                accept(commaSy);
                accept(ttIdent);
                if (curToken->getType() == ttKeyword)
                    kw = dynamic_cast<CKeywordTokenPtr>(curToken);
                else
                    break;
            }
        }
        accept(colonSy);
        accept(ttIdent);
    }
    void varpart()
    {
        accept(varSy);
        while (curToken->getType() == ttIdent)
        {
            vardeclaration();
            accept(semiColonSy);
        }
    }
    bool is_add_op(CTokenPtr token)
    {
        auto kw = dynamic_cast<CKeywordTokenPtr>(curToken);
        return kw->getCode() == plusSy || kw->getCode() == minusSy || kw->getCode() == orSy;
    }
    void add_op()
    {
        if (curToken->getType() == ttKeyword)
        {
            auto kw = dynamic_cast<CKeywordTokenPtr>(curToken);
            switch (kw->getCode())
            {
            case plusSy:
                accept(plusSy);
                return;
            case minusSy:
                accept(minusSy);
                return;
            }
        }
        accept(orSy);
    }
    bool is_mult_op(CTokenPtr token)
    {
        auto kw = dynamic_cast<CKeywordTokenPtr>(curToken);
        return kw->getCode() == multSy || kw->getCode() == subSy 
            || kw->getCode() == divSy || kw->getCode() == modSy || kw->getCode() == andSy;
    }
    void mult_op()
    {
        if (curToken->getType() == ttKeyword)
        {
            auto kw = dynamic_cast<CKeywordTokenPtr>(curToken);
            switch (kw->getCode())
            {
            case multSy:
                accept(multSy);
                return;
            case subSy:
                accept(subSy);
                return;
            case divSy:
                accept(divSy);
                return;
            case modSy:
                accept(modSy);
                return;
            }
        }
        accept(andSy);
    }
    void factor()
    {
        switch (curToken->getType())
        {
        case ttConst:
            accept(ttConst);
            break;
        case ttIdent:
            accept(ttIdent);
            break;
        case ttKeyword:
            auto kw = dynamic_cast<CKeywordTokenPtr>(curToken);
            if (kw->getCode() == leftBrSy)
            {
                accept(leftBrSy);
                expr();
                accept(rightBrSy);
            }
            break;
        }
    }
    void term()
    {
        factor();
        if (curToken->getType() == ttKeyword && is_mult_op(curToken))
        {
            while (curToken->getType() == ttKeyword && is_mult_op(curToken))
            {
                mult_op();
                factor();
            }
        }
    }
    void simple_expr()
    {
        term();
        if (curToken->getType() == ttKeyword && is_add_op(curToken))
        {
            while (curToken->getType() == ttKeyword && is_add_op(curToken))
            {
                add_op();
                term();
            }
        }
    }
    bool is_relation_op(CTokenPtr token)
    {
        auto kw = dynamic_cast<CKeywordTokenPtr>(curToken);
        return kw->getCode() == evenSy || kw->getCode() == lessSy
            || kw->getCode() == lessEvenSy || kw->getCode() == moreSy 
            || kw->getCode() == moreEvenSy || kw->getCode() == notEvenSy;
    }
    void relation_op()
    {
        if (curToken->getType() == ttKeyword)
        {
            auto kw = dynamic_cast<CKeywordTokenPtr>(curToken);
            switch (kw->getCode())
            {
                case evenSy:
                    accept(evenSy);
                    return;
                case lessSy:
                    accept(lessSy);
                    return;
                case lessEvenSy:
                    accept(lessEvenSy);
                    return;
                case moreSy:
                    accept(moreSy);
                    return;
                case moreEvenSy:
                    accept(moreEvenSy);
                    return;
            }
        }
        accept(notEvenSy);
    }
    void expr()
    {
        simple_expr();
        if (curToken->getType() == ttKeyword && is_relation_op(curToken))
        {
            relation_op();
            simple_expr();
        }
    }
    void assignstatement()
    {
        accept(ttIdent);
        accept(assignSy);
        expr();
    }
    void statement()
    {
        switch (curToken->getType())
        {
        case ttIdent:
            assignstatement();
            break;
        case ttKeyword:
            auto kw = dynamic_cast<CKeywordTokenPtr>(curToken);
            switch (kw->getCode())
            {
                case beginSy:
                    compositestatement();
                    break;
                case ifSy:
                    condition();
                    break;
                case whileSy:
                    preccycle();
                    break;
            }
            break;
        }
    }
    void compositestatement()
    {
        accept(beginSy);
        statement();
        if (curToken->getType() == ttKeyword)
        {
            auto kw = dynamic_cast<CKeywordTokenPtr>(curToken);
            while (kw->getCode() == semiColonSy)
            {
                accept(semiColonSy);
                statement();
                
                if (curToken->getType() == ttKeyword)
                    kw = dynamic_cast<CKeywordTokenPtr>(curToken);
            }
        }
        accept(endSy);
    }
    void preccycle()
    {
        accept(whileSy);
        expr();
        accept(doSy);
        statement();
    }
    void tail()
    {
        if (curToken->getType() == ttKeyword)
        {
            auto kw = dynamic_cast<CKeywordTokenPtr>(curToken);
            if (kw->getCode() == elseSy)
            {
                accept(elseSy);
                statement();
            }
        }
    }
    void condition()
    {
        accept(ifSy);
        expr();
        accept(thenSy);
        statement();
        tail();
    }
    void statementpart()
    {
        compositestatement();
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
    /*CTokenPtr token = nullptr;
    while (token = lexer->getNextToken())
    {
        cout << token->ToString() << "\t";
        switch (token->getType())
        {
        case ttConst:
            cout << "c" << endl;
            break;
        case ttIdent:
            cout << "i" << endl;
            break;
        case ttKeyword:
            cout << "kw" << endl;
            break;
        }
    }*/

    auto syntax = new CSyntax();
    syntax->program();

    cout << "d o n e";

    delete io;
    delete lexer;
    delete syntax;
    input.close();
}