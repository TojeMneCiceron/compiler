#pragma once
#include <string>
#include <map>
using namespace std;

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
    ttIdent,
    ttKeyword,
    ttConst
};

class CToken {
private:
    TokenType tType;
public:
    CToken(TokenType _type);
    TokenType getType();
    virtual string ToString() PURE;
};

typedef CToken* CTokenPtr;
//DECL_PTR(CToken);

class CIdentToken : public CToken
{
private:
    string ident;
public:
    CIdentToken(string _ident) : CToken(ttIdent), ident(_ident) {}
    string ToString();
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
    KeyWords getCode();
    string ToString();
};
typedef CKeywordToken* CKeywordTokenPtr;
//DECL_PTR(CKeywordToken);

class CIntConstToken : public CToken
{
private:
    int value;
public:
    CIntConstToken(int val) : CToken(ttConst), value(val) {}
    string ToString();
};
typedef CIntConstToken* CIntConstTokenPtr;
//DECL_PTR(CIntConstToken);

class CRealConstToken : public CToken
{
private:
    double value;
public:
    CRealConstToken(double val) : CToken(ttConst), value(val) {}
    string ToString();
};
typedef CRealConstToken* CRealConstTokenPtr;
//DECL_PTR(CRealConstToken);

class CCharConstToken : public CToken
{
private:
    char value;
public:
    CCharConstToken(char val) : CToken(ttConst), value(val) {}
    string ToString();
};
typedef CCharConstToken* CCharConstTokenPtr;
//DECL_PTR(CStringConstToken);

class CBoolConstToken : public CToken
{
private:
    bool value;
public:
    CBoolConstToken(bool val) : CToken(ttConst), value(val) {}
    string ToString();
};
typedef CBoolConstToken* CBoolConstTokenPtr;
//DECL_PTR(CBoolConstToken);

class CTextPosition {
private:
    int line_number;
    int ch_number;
public:
    CTextPosition(int _line_number, int _ch_number);
    int getLineNumber();
    int getChNumber();
};
typedef CTextPosition* CTextPositionPtr;

class CError {
private:
    string error_code;
    CTextPositionPtr text_position;
public:
    CError(string code, CTextPositionPtr tp);
    string getErrorCode();
    CTextPositionPtr getTextPosition();
};
typedef CError* CErrorPtr;

class CioModule {
private:
    string buffer;
    int cur_ch;
    int cur_line = 0;
public:
    tuple<char, CTextPositionPtr> nextch();
    //void WriteError(CError error)
    void WriteError(string error);
    void WriteError(CError* error);
};
//DECL_PTR(CioModule);
//auto io = new CioModule();

class CLexer {
private:
    char cur_ch;
    CTextPositionPtr cur_tp;
public:
    CTokenPtr getNextToken();
};
//auto lexer = new CLexer();

class CSyntax {
private:
    CTokenPtr curToken;
public:

    void accept(KeyWords keyword);
    void accept(TokenType tt);
    void vardeclaration();
    void varpart();
    bool is_add_op(CTokenPtr token);
    void add_op();
    bool is_mult_op(CTokenPtr token);
    void mult_op();
    void factor();
    void term();
    void simple_expr();
    bool is_relation_op(CTokenPtr token);
    void relation_op();
    void expr();
    void assignstatement();
    void statement();
    void compositestatement();
    void preccycle();
    void tail();
    void condition();
    void statementpart();
    void block();
    void program();
};