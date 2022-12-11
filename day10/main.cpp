#include <vector>
#include <fstream>
#include <iostream>
#include <utility>
#include <set>

#define noop "noop"
#define addx "addx"

using namespace std;

vector<string>
read_file(string filename)
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

bool is_cycle_strength(uint32_t cycle)
{
    return cycle == 20 || (cycle > 20 && (cycle - 20) % 40 == 0);
}

uint32_t part1(vector<string> &lines)
{
    vector<int32_t> sums;
    int32_t x = 1;
    int32_t x_value;
    uint32_t strength = 0;
    uint32_t cycle = 0;

    for (auto &line : lines)
    {
        string op = line.substr(0, 4);

        if (!op.compare(noop))
        {
            cycle += 1;
            cout << "noop" << endl;
        }
        else if (!op.compare(addx))
        {
            cycle += 1;
            x_value = stoi(line.substr(line.find(' ')));

            if (is_cycle_strength(cycle))
            {
                int32_t sum = x * cycle;
                sums.push_back(sum);
                cout << "[" << cycle << "] x=" << x << " sum=" << sum << " - midle cycle -" << endl;
            }

            cycle += 1;
            x += x_value;
            cout << "[" << cycle << "] add " << x_value << endl;
        }

        if (is_cycle_strength(cycle))
        {
            int32_t sum = (x * cycle);
            sums.push_back(sum);
            cout << "[" << cycle << "] x=" << x << " sum=" << sum << endl;
        }
    }

    uint32_t sum = 0;
    for (auto it = sums.begin(); it != sums.end(); ++it)
    {
        sum += *it;
    }

    return sum;
}

bool is_sprint_on_pixel(int32_t x, uint32_t pixel)
{
    return x - 1 <= (pixel % 40) && (pixel % 40) <= x + 1;
}

void draw(int32_t x, uint32_t cycle, fstream &output)
{
    if (cycle % 40 == 0)
    {
        cout << "cycle: " << cycle << endl;
        output << "\n";
    }

    if (is_sprint_on_pixel(x, cycle))
    {
        output << "#";
    }
    else
    {
        output << ".";
    }

}

void part2(vector<string> lines)
{
    int32_t x = 1;
    int32_t x_value;
    uint32_t strength = 0;
    uint32_t cycle = 0;

    fstream output("output.txt", ios_base::out);

    if (!output.is_open())
    {
        cerr << "failed to open file output.txt" << endl;
        exit(1);
    }

    for (auto &line : lines)
    {
        string op = line.substr(0, 4);

        draw(x, cycle, output);

        if (!op.compare(noop))
        {
            cycle += 1;
        }
        else if (!op.compare(addx))
        {
            cycle += 1;
            x_value = stoi(line.substr(line.find(' ')));

            draw(x, cycle, output);

            cycle += 1;
            x += x_value;
        }
    }

    output.close();
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cerr << "usage: " << argv[0] << " <input.txt>" << endl;
        exit(1);
    }

    auto lines = read_file(argv[1]);

    part2(lines);

    return 0;
}