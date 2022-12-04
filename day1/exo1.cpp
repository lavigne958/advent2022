#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

vector<string> read_file(const string filename)
{
    ifstream in;
    in.open(filename);
    if (!in.is_open())
    {
        cerr << "failed to open file: " << filename << endl;
        exit(1);
    }

    vector<string> lines;
    string buff;
    while (getline(in, buff))
    {
        lines.push_back(buff);
    }

    return lines;
}

vector<int> to_int(vector<string> values)
{
    vector<int> output;
    for (auto &line : values)
    {
        output.push_back(stoi(line));
    }

    return output;
}

void biggest(int current, int max[3])
{
    if (current > max[0])
    {
        max[2] = max[1];
        max[1] = max[0];
        max[0] = current;
    }
    else if (current > max[1])
    {
        max[2] = max[1];
        max[1] = current;
    }
    else if (current > max[2])
        max[2] = current;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cerr << "usage: " << argv[0] << " <filename>" << endl;
        exit(1);
    }

    vector<string> lines = read_file(string(argv[1]));

    int max[3] = {0, 0, 0};
    int current = 0;
    for (auto &line : lines)
    {
        if (line.empty())
        {
            biggest(current, max);
            current = 0;
        }
        else
        {
            current += stoi(line);
        }
    }

    biggest(current, max);

    cout << "1: " << max[0] << " 2: " << max[1] << " 3: " << max[2] << endl;
    cout << "max energy: " << (max[0] + max[1] + max[2]) << endl;
    return 0;
}