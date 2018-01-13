#include "Compiler.h"
#include "ByteCodeGenerator.h"
#include "DefinitionCollector.h"
#include "Exception.h"
#include "Expression.h"
#include "Lexer.h"
#include "Parser.h"
#include "PrimitiveType.h"
#include "TypeChecker.h"
#include <fstream>
#include <iostream>
#include <memory>
using namespace std;
SourceFile::SourceFile()
{
}

SourceFile::SourceFile(wstring path, DebugInfo debugInfo, ExpressionPtr program,
                       LocationRecord record)
    : path{path}, debugInfo{debugInfo}, program{program}, record{record}
{
}

Compiler::Compiler(std::string mainFile) : mainFile{mainFile}
{
}

void Compiler::ParseAll()
{
    Parse(mainFile);
}

void Compiler::Parse(std::string path)
{
    wcout << L"parsing " << wstring(path.begin(), path.end()) << endl;
    Lexer lexer(path);
    vector<Token> tokens = lexer.ReadAll();
    for (Token& item : tokens)
    {
        item.Display();
    }
    DebugInfo debugInfo;
    Parser parser(tokens, debugInfo);
    ExpressionPtr program = parser.Program();
    SourceFile file(wstring(path.begin(), path.end()), debugInfo, program,
                    LocationRecord());
    files.push_back(file);
    table.insert(std::pair<wstring, SourceFile&>(file.path, file));

    TreeViewer viewer;
    file.program->Accept(&viewer);
    ParseModules(file.program);
}

vector<SourceFile*> Compiler::GetReversedSequence()
{
    vector<SourceFile*> sequence;
    for (i32 i = static_cast<i32>(files.size()) - 1; i >= 0; i--)
    {
        sequence.push_back(&(files.at(static_cast<u32>(i))));
    }
    return sequence;
}

void Compiler::ParseModules(ExpressionPtr program)
{
    for (ExpressionPtr expression :
         static_pointer_cast<BlockExpression>(program)->expressions)
    {
        if (expression->kind == ExpressionKind::Import)
        {
            ImportExpressionPtr import =
                static_pointer_cast<ImportExpression>(expression);
            if (table.find(import->name) == table.end())
            {
                wstring name = import->name;
                Parse(workingDirectory + string(name.begin(), name.end()) +
                      extension);
            }
        }
    }
}

void Compiler::CheckType()
{
    vector<SourceFile*> sequence = GetReversedSequence();
    DefinitionCollector collector;
    for (SourceFile* file : sequence)
    {
        file->program->Accept(&collector);
    }
    for (SourceFile* file : sequence)
    {
        TypeChecker checker(file->debugInfo, file->record, collector.scope,
                            collector.fenv);

        file->program->Accept(&checker);
        TreeViewer viewer;
        file->program->Accept(&viewer);
    }
}

void Compiler::Compile(string output)
{
    vector<SourceFile*> sequence = GetReversedSequence();

    ofstream outFile(output, ofstream::binary);
    if (!outFile)
    {
        throw FileNotFoundException(wstring(output.begin(), output.end()));
    }
    else
    {
        vector<ByteCode> codeList;
        ByteCode topCode;
        for (SourceFile* file : sequence)
        {
            ByteCodeGenerator generator(file->debugInfo, file->record);
            file->program->Accept(&generator);
            for (Function& f : generator.functions)
            {
                codeList.push_back(generator.GetFunctionCode(f));
            }
            for (byte item : generator.GetTopCode())
            {
                topCode.push_back(item);
            }
        }
        for (ByteCode& item : codeList)
        {
            WriteByteCode(outFile, item);
        }
        WriteByteCode(outFile, topCode);
    }
}

void Compiler::WriteByteCode(ofstream& outFile, vector<byte>& code)
{
    for (byte item : code)
    {
        wcout << item << endl;
        outFile.write(reinterpret_cast<char*>(&item), sizeof(byte));
    }
}
