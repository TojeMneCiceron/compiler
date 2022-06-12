#pragma once
#include <string>
#include <map>
#include <vector>
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

#define PURE = 0
enum TokenType {
    ttIdent,
    ttKeyword,
    ttConst
};
//
//class CToken {
//private:
//    TokenType tType;
//    CTextPositionPtr text_position;
//public:
//    CToken(TokenType _type, CTextPositionPtr tp);
//    TokenType getType();
//    CTextPositionPtr getTextPosition();
//    virtual string ToString() PURE;
//};
//typedef CToken* CTokenPtr;
//
//class CIdentToken : public CToken
//{
//private:
//    string ident;
//public:
//    CIdentToken(string _ident, CTextPositionPtr tp);
//    string ToString();
//};
//typedef CIdentToken* CIdentTokenPtr;
//
//class CKeywordToken : public CToken
//{
//private:
//    string keyword;
//    KeyWords code;
//public:
//    CKeywordToken(string _keyword, CTextPositionPtr tp);
//    KeyWords getCode();
//    string ToString();
//};
//typedef CKeywordToken* CKeywordTokenPtr;
//
//class CIntConstToken : public CToken
//{
//private:
//    int value;
//public:
//    CIntConstToken(int val, CTextPositionPtr tp);
//    string ToString();
//};
//typedef CIntConstToken* CIntConstTokenPtr;
//
//class CRealConstToken : public CToken
//{
//private:
//    double value;
//public:
//    CRealConstToken(double val, CTextPositionPtr tp);
//    string ToString();
//};
//typedef CRealConstToken* CRealConstTokenPtr;
//
//class CCharConstToken : public CToken
//{
//private:
//    char value;
//public:
//    CCharConstToken(char val, CTextPositionPtr tp);
//    string ToString();
//};
//typedef CCharConstToken* CCharConstTokenPtr;
//
//class CBoolConstToken : public CToken
//{
//private:
//    bool value;
//public:
//    CBoolConstToken(bool val, CTextPositionPtr tp);
//    string ToString();
//};
//typedef CBoolConstToken* CBoolConstTokenPtr;
//
//class CTextPosition {
//private:
//    int line_number;
//    int ch_number;
//public:
//    CTextPosition(int _line_number, int _ch_number);
//    int getLineNumber();
//    int getChNumber();
//};
//typedef CTextPosition* CTextPositionPtr;
//
//class CioModule {
//private:
//    ifstream input;
//    string buffer;
//    int cur_ch;
//    int cur_line = 0;
//public:
//    CioModule(string filename);
//    tuple<char, CTextPositionPtr> nextch();
//    void WriteError(int error, CTextPositionPtr tp);
//    ~CioModule();
//};
//
//class CLexer {
//private:
//    CioModule* io;
//    char cur_ch;
//    CTextPositionPtr cur_tp;
//public:
//    CTokenPtr getNextToken();
//};
//
//class CSyntax {
//private:
//    CioModule* io;
//    CLexer* lexer;
//    CTokenPtr curToken;
//public:
//    int symbol();
//    bool belong(int symbol, vector<int> starters);
//    void skip_to(vector<int> followers);
//    void skip_to(vector<int> starters, vector<int> followers);
//    void skip_to_followers(vector<int> followers);
//    void accept(KeyWords keyword);
//    void accept(TokenType tt);
//    void vardeclaration();
//    void varpart();
//    bool is_add_op(CTokenPtr token);
//    bool is_mult_op(CTokenPtr token);
//    void factor();
//    void term();
//    void simple_expr();
//    bool is_relation_op(CTokenPtr token);
//    void expr();
//    void assignstatement();
//    void statement();
//    void compositestatement();
//    void preccycle();
//    void condition();
//    void statementpart();
//    void block();
//    void program();
//};
//
//class CCompiler
//{
//private:
//    //ifstream input;
//    CioModule io;
//    CLexer lexer;
//    CSyntax syntax;
//public:
//    CCompiler(string filename);
//    void compile();
//    ~CCompiler();
//};