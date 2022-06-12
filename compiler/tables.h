#pragma once
#include <map>
#include <string>
using namespace std;

enum KeyWords {
    assignSy = 51,
    beginSy = 17,
    endSy = 13,
    evenSy = 16,
    leftBrSy = 9,
    rightBrSy = 4,
    ifSy = 56,
    thenSy = 52,
    doSy = 54,
    colonSy = 5, semiColonSy = 14, commaSy = 20,
    programSy = 3, dotSy = 61,
    varSy, andSy, orSy, notSy, lessSy, moreSy, lessEvenSy, moreEvenSy, notEvenSy,
    plusSy, minusSy, multSy, subSy, divSy, modSy,
    elseSy,
    whileSy,
    quotSy,
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

enum VariantType {
    vtInt, vtReal, vtChar, vtBoolean, vtUndefined
};
map<string, VariantType> types
{
    {"integer", vtInt},
    {"real", vtReal},
    {"char", vtChar},
    {"Boolean", vtBoolean}
};