#include <vector>
#include <fstream>
#include <iostream>
#include <set>

using namespace std;

vector<string> read_file(string filename)
{
    ifstream input(filename);
    if (!input.is_open())
    {
        cerr << "Failed to open file " << filename << endl;
        exit(1);
    }

    vector<string> lines;
    string buff;
    while (getline(input, buff))
    {
        lines.push_back(buff);
    }

    return lines;
}

uint32_t part1(string line)
{
    size_t lengh = line.length() - 4;
    uint32_t sum = 0;
    for (uint32_t i = 0; i < lengh; ++i)
    {
        string patern = line.substr(i, 4);
        set<char> marker = set<char>(patern.begin(), patern.end());
        if (marker.size() == 4)
        {
            return i + 4;
        }
    }

    return 0;
}

uint32_t part2(string line)
{
    size_t lengh = line.length() - 14;
    uint32_t sum = 0;
    for (uint32_t i = 0; i < lengh; ++i)
    {
        string patern = line.substr(i, 14);
        set<char> marker = set<char>(patern.begin(), patern.end());
        if (marker.size() == 14)
        {
            return i + 14;
        }
    }

    return 0;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cerr << "usage: " << argv[0] << " <input.txt>" << endl;
        exit(1);
    }

    vector<string> lines = read_file(argv[1]);
    for (auto &line : lines)
    {
        uint32_t marker_pos = part2(line);
        cout << "message starts at: " << marker_pos << endl;
    }

    return 0;
}