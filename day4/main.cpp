#include <vector>
#include <fstream>
#include <iostream>

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

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cerr << "usage: " << argv[0] << " <input.txt>" << endl;
        exit(1);
    }

    vector<string> lines = read_file(argv[1]);
    uint32_t sum = 0;

    for (auto &line : lines)
    {
        uint32_t ranges[4];
        size_t comma = line.find(',');
        string first = line.substr(0, comma);
        string second = line.substr(comma + 1);
        size_t first_sep = first.find('-');
        size_t second_sep = second.find('-');

        ranges[0] = stoi(first.substr(0, first_sep));
        ranges[1] = stoi(first.substr(first_sep + 1));
        ranges[2] = stoi(second.substr(0, second_sep));
        ranges[3] = stoi(second.substr(second_sep + 1));

        if (ranges[0] >= ranges[2] && ranges[1] <= ranges[3])
        {
            ++sum;
        }
        else if (ranges[2] >= ranges[0] && ranges[3] <= ranges[1])
        {
            ++sum;
        }
        else if (ranges[2] <= ranges[1] && ranges[1] <= ranges[3])
        {
            ++sum;
        }
        else if (ranges[2] <= ranges[0] && ranges[0] <= ranges[3])
        {
            ++sum;
        }
    }

    cout << "ovalping ranges: " << sum << endl;

    return 0;
}
