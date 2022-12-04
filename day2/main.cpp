#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

#define ROCK_OP 'A'
#define PAPER_OP 'B'
#define SICOR_OP 'C'
#define ROCK_ME 'X'
#define PAPER_ME 'Y'
#define SICOR_ME 'Z'
#define LOSE_ME 'X'
#define DRAW_ME 'Y'
#define WIN_ME 'Z'

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

int action_to_points(char action)
{
    int point = 0;
    switch (action)
    {
    case ROCK_OP:
    case ROCK_ME:
        point = 1;
        break;
    case PAPER_OP:
    case PAPER_ME:
        point = 2;
        break;
    case SICOR_OP:
    case SICOR_ME:
        point = 3;
        break;
    default:
        break;
    }

    return point;
}

int round_points(char op, char me)
{
    switch (op)
    {
    case ROCK_OP:
        return (me == ROCK_ME) ? 3 : (me == PAPER_ME) ? 6
                                                      : 0;
    case PAPER_OP:
        return (me == ROCK_ME) ? 0 : (me == PAPER_ME) ? 3
                                                      : 6;
    case SICOR_OP:
        return (me == ROCK_ME) ? 6 : (me == PAPER_ME) ? 0
                                                      : 3;
    }
    return 0;
}

char my_move(char op, char result)
{
    switch (op)
    {
    case ROCK_OP:
        return (result == LOSE_ME) ? SICOR_ME : (result == DRAW_ME) ? ROCK_ME
                                                                    : PAPER_ME;
    case PAPER_OP:
        return (result == LOSE_ME) ? ROCK_ME : (result == DRAW_ME) ? PAPER_ME
                                                                   : SICOR_ME;
    case SICOR_OP:
        return (result == LOSE_ME) ? PAPER_ME : (result == DRAW_ME) ? SICOR_ME
                                                                    : ROCK_ME;
    }

    return 0;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cerr << "usage: " << argv[0] << " <filename>" << endl;
        exit(1);
    }

    vector<string> lines = read_file(string(argv[1]));

    uint32_t points = 0;
    for (auto &line : lines)
    {
        char op = line[0];
        char result = line[2];

        char me = my_move(op, result);

        points += round_points(op, me) + action_to_points(me);
    }

    cout << "my points: " << points << endl;

    return 0;
}