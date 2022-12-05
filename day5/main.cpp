#include <vector>
#include <fstream>
#include <iostream>
#include <stack>

using namespace std;

#define MOVE "move"

class Moves
{
public:
    Moves(uint32_t nr_moves, uint32_t from, uint32_t to)
    {
        this->nr_moves = nr_moves;
        this->from = from;
        this->to = to;
    };
    uint32_t nr_moves;
    uint32_t from;
    uint32_t to;
};

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

vector<stack<char>> parse(vector<string> &lines)
{
    uint32_t nr_stacks = (lines[0].length() + 1) / 4;
    vector<stack<char>> stacks;
    for (int i = 0; i < nr_stacks; ++i)
        stacks.push_back(stack<char>());

    for (auto &line : lines)
    {
        if ('1' <= line[1] && line[1] <= '9')
            break;

        for (int i = 0; i < line.length(); i += 4)
        {
            uint32_t stack = i / 4;
            uint32_t crate_str_index = i + 1;
            if ('A' <= line[crate_str_index] && line[crate_str_index] <= 'Z')
            {
                stacks[stack].push(line[crate_str_index]);
            }
        }
    }

    vector<stack<char>> reversed;
    for (auto &curr : stacks)
    {
        stack<char> tmp = stack<char>();
        while (!curr.empty())
        {
            tmp.push(curr.top());
            curr.pop();
        }
        reversed.push_back(tmp);
    }

    return reversed;
}

vector<Moves> parse_moves(vector<string> &lines)
{
    size_t move_len = sizeof(MOVE);
    vector<Moves> moves = vector<Moves>();
    for (auto &line : lines)
    {
        // skip first section
        if (line.substr(0, 4).compare(MOVE) != 0)
        {
            continue;
        }

        size_t pos = 0;

        string temp = line.substr(move_len);
        size_t next = temp.find(' ');
        uint32_t nr = stoi(temp.substr(0, next));
        temp = temp.substr(next + 1);
        next = temp.find(' ');
        temp = temp.substr(next + 1);
        next = temp.find(' ');
        uint32_t from = stoi(temp.substr(0, next));
        temp = temp.substr(next + 1);
        next = temp.find(' ');
        temp = temp.substr(next + 1);
        next = temp.find(' ');
        uint32_t to = stoi(temp.substr(0, next));
        cout << "nr:" << nr << " from:" << from << " to:" << to << endl;
        moves.push_back(Moves(nr, from, to));
    }

    return moves;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cerr << "usage: " << argv[0] << " <input.txt>" << endl;
        exit(1);
    }

    vector<string> lines = read_file(argv[1]);

    auto res = parse(lines);
    auto moves = parse_moves(lines);

    for (auto &move : moves)
    {
        auto &from = res[move.from - 1];
        auto &to = res[move.to - 1];
        stack<char> temp = stack<char>();
        for (int i = 0; i < move.nr_moves && !from.empty(); ++i)
        {
            cout << "move:" << from.top() << " to:" << move.to << endl;
            temp.push(from.top());
            from.pop();
        }

        while (!temp.empty())
        {
            to.push(temp.top());
            temp.pop();
        }
    }

    for (auto &stack : res)
    {
        cout << stack.top();
    }

    cout << endl;

    return 0;
}