#include <vector>
#include <fstream>
#include <iostream>
#include <utility>
#include <set>

#define str_head "  head"
#define str_tail "  tail"

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

void print_pos(pair<int32_t, int32_t> head, string name)
{
    cout << name << " pos: " << head.second << ":" << head.first << endl;
}

void compute_same_axis(pair<int32_t, int32_t> &head, pair<int32_t, int32_t> &tail)
{
    // check rows
    int32_t delta = head.first - tail.first;
    // tail is too far away, move it
    if (abs(delta) >= 2)
    {
        if (delta > 0)
        {
            tail.first += 1;
        }
        else if (delta < 0)
        {
            tail.first -= 1;
        }
        else
        {
            cerr << "0 delta can't be right " << delta << " abs(delta): " << abs(delta) << endl;
        }
    }

    // check cols
    delta = head.second - tail.second;
    // tail is too far away, move it
    if (abs(delta) >= 2)
    {
        if (delta > 0)
        {
            tail.second += 1;
        }
        else if (delta < 0)
        {
            tail.second -= 1;
        }
        else
        {
            cerr << "0 delta can't be right " << delta << " abs(delta): " << abs(delta) << endl;
        }
    }
}

void compute_diagonale(pair<int32_t, int32_t> &head, pair<int32_t, int32_t> &tail)
{
    int32_t delta_row = head.first - tail.first;
    int32_t delta_ol = head.second - tail.second;

    // head is above tail
    if (delta_row > 0)
    {
        // head is right to tail
        if (delta_ol > 0)
        {
            tail.first += 1;
            tail.second += 1;
        }
        else if (delta_ol < 0)
        {
            // head is left to tail
            tail.first += 1;
            tail.second -= 1;
        }
    }
    else if (delta_row < 0)
    {
        // head is below tail
        if (delta_ol > 0)
        {
            // head is right to tail
            tail.first -= 1;
            tail.second += 1;
        }
        else if (delta_ol < 0)
        {
            tail.first -= 1;
            tail.second -= 1;
        }
    }
}

bool head_is_diagonal(pair<int32_t, int32_t> &head, pair<int32_t, int32_t> &tail)
{
    int32_t row_diff = head.first - tail.first;
    int32_t col_diff = head.second - tail.second;
    return (abs(row_diff) == 2 && abs(col_diff) >= 1) || (abs(row_diff) >= 1 && abs(col_diff) == 2);
}

void compute_tail_pos(pair<int32_t, int32_t> &head, pair<int32_t, int32_t> &tail)
{
    // first case: they are on the col / row
    if (head.first == tail.first || head.second == tail.second)
    {
        cout << "same axis" << endl;
        compute_same_axis(head, tail);
    }
    else if (head_is_diagonal(head, tail))
    {
        cout << "diagonal" << endl;
        compute_diagonale(head, tail);
    }
}

uint32_t part1(vector<string> &lines)
{
    uint32_t sum = 0;

    // row,col
    pair<int32_t, int32_t> head(0, 0);
    pair<int32_t, int32_t> tail(0, 0);
    set<pair<int32_t, int32_t>> tail_history;

    print_pos(head, str_head);
    tail_history.insert(tail);

    for (auto &line : lines)
    {
        char str_dir = line[0];
        uint32_t nr = stoul(line.substr(line.find(' ') + 1));

        for (int i = 0; i < nr; ++i)
        {
            switch (str_dir)
            {
            case 'L':
                head.second -= 1;
                break;
            case 'U':
                head.first += 1;
                break;
            case 'R':
                head.second += 1;
                break;
            case 'D':
                head.first -= 1;
                break;

            default:
                break;
            }
            print_pos(head, str_head);
            compute_tail_pos(head, tail);
            pair<int32_t, int32_t> copy(tail);
            tail_history.insert(copy);
            print_pos(tail, str_tail);
        }
    }

    sum = tail_history.size();

    return sum;
}

uint32_t part2(vector<string> lines)
{
    uint32_t sum = 0;

    // row,col
    vector<pair<int32_t, int32_t>> rope;
    for (int i = 0; i < 10; i++)
    {
        rope.push_back(pair<int32_t, int32_t>(0, 0));
    }
    set<pair<int32_t, int32_t>> tail_history;

    print_pos(rope[0], str_head);
    tail_history.insert(rope[9]);

    for (auto &line : lines)
    {
        char str_dir = line[0];
        uint32_t nr = stoul(line.substr(line.find(' ') + 1));

        for (int i = 0; i < nr; ++i)
        {
            switch (str_dir)
            {
            case 'L':
                rope[0].second -= 1;
                break;
            case 'U':
                rope[0].first += 1;
                break;
            case 'R':
                rope[0].second += 1;
                break;
            case 'D':
                rope[0].first -= 1;
                break;

            default:
                break;
            }
            print_pos(rope[0], str_head);

            for (auto it = rope.begin() + 1; it != rope.end(); ++it)
            {
                compute_tail_pos(*(it - 1), *it);
                print_pos(*it, "node");
            }
            pair<uint32_t, uint32_t> copy(rope[9]);
            tail_history.insert(copy);
            print_pos(rope[9], str_tail);
        }
    }

    sum = tail_history.size();

    return sum;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cerr << "usage: " << argv[0] << " <input.txt>" << endl;
        exit(1);
    }

    vector<string> lines = read_file(argv[1]);
    auto res = part2(lines);
    cout << "result: " << res << endl;
    return 0;
}
