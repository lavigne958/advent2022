#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <bits/stdc++.h>

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

uint32_t priority(char current)
{
    if ('a' <= current && current <= 'z')
    {
        return current - 'a' + 1;
    }
    else
    {
        return 26 + (current - 'A') + 1;
    }
}

uint32_t brute_force(string line)
{
    char current;
    bool found = false;
    uint32_t res = 0;
    uint32_t half = line.length() / 2;
    for (uint32_t i = 0; !found; ++i)
    {
        current = line[i];
        for (uint32_t j = line.length() - 1; j >= half && !found; --j)
        {
            if (current == line[j])
            {
                res = priority(current);
                found = true;

                if (i > half)
                {
                    cerr << "Found: " << current << " over the first half" << endl;
                }

                if (res > 52)
                {
                    cerr << "priority cannot exceed 52: " << res << endl;
                }
                cout << "[";
                cout.fill(' ');
                cout.width(2);
                cout << i;
                cout.fill();
                cout.width();
                cout << "/" << line.length() << "]: " << current;
                cout << "-> " << res << endl;
            }
        }
    }

    return res;
}

uint32_t using_set(string line)
{
    string first = line.substr(0, line.length() / 2);
    string second = line.substr(line.length() / 2, line.length() - 1);
    set<char> first_set(first.begin(), first.end());
    set<char> second_set(second.begin(), second.end());

    vector<char> res(1);
    set_intersection(first_set.begin(), first_set.end(), second_set.begin(), second_set.end(), res.begin());

    return priority(res[0]);
}

uint32_t using_set_3(string line1, string line2, string line3)
{
    set<char> first_set(line1.begin(), line1.end());
    set<char> second_set(line2.begin(), line2.end());
    set<char> third_set(line3.begin(), line3.end());

    vector<char> res(1024);
    vector<char> final(1024);
    vector<char>::iterator first_end = set_intersection(first_set.begin(), first_set.end(), second_set.begin(), second_set.end(), res.begin());
    vector<char>::iterator second_end = set_intersection(res.begin(), first_end, third_set.begin(), third_set.end(), final.begin());
    for (first_end = final.begin(); first_end != second_end; first_end++)
    {
        cout << "found: " << *first_end << endl;
    }

    return priority(final[0]);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cerr << "usage: " << argv[0] << " <filename>" << endl;
        exit(1);
    }

    vector<string> lines = read_file(string(argv[1]));

    uint64_t sum = 0;
    for (int i = 0; i < lines.size(); i += 3)
    {
        sum += using_set_3(lines[i], lines[i + 1], lines[i + 2]);
    }

    cout << "sum: " << sum << endl;

    return 0;
}