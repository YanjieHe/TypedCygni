#include "ReadBinaryFile.h"
#include "Exception.h"
#include <fstream>
using std::ifstream;
using std::wstring;
vector<byte> ReadBinaryFile(string path)
{
    vector<byte> stream;
    ifstream input(path, ifstream::binary);
    if (input)
    {
        byte item;
        while (input.peek() != EOF)
        {
            input.read(reinterpret_cast<char*>(&item), sizeof(item));
            stream.push_back(item);
        }
        input.close();
        return stream;
    }
    else
    {
        throw FileNotFoundException(wstring(path.begin(), path.end()));
    }
}
