#ifndef COMPILER_H
#define COMPILER_H

#include "Expression.h"
#include "PrimitiveType.h"
#include "Scope.h"
#include <fstream>
#include <map>
#include <string>
#include <vector>
using std::map;
using std::ofstream;
using std::string;
using std::vector;

class SourceFile
{
public:
    wstring path;
    ExpressionPtr program;
    SourceFile();
    SourceFile(wstring path, ExpressionPtr program);
};

class Compiler
{
public:
    string workingDirectory = "/home/jasonhe/Documents/MyCode/CPP/Qt/"
                              "build-Cygni-Desktop-Debug/TestCases/";
    string extension = ".txt";
    vector<SourceFile> files;
    string mainFile;
    map<wstring, SourceFile&> table;

    Compiler(string mainFile);

    void ParseAll();
    void Parse(string path);
    vector<SourceFile*> GetReversedSequence();
    void ParseModules(ExpressionPtr program);
    void CheckType();
    void Compile(std::string output);
    void WriteByteCode(ofstream& outFile, vector<byte>& code);
};
#endif // COMPILER_H
