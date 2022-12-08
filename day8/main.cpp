#include <vector>
#include <fstream>
#include <iostream>
#include <set>

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

vector<vector<uint32_t>> convert_to_int(vector<string> &lines)
{
    vector<vector<uint32_t>> matrix;
    for (auto &line : lines)
    {
        vector<uint32_t> values;

        for (int i = 0; i < line.size(); ++i)
        {
            values.push_back(stoul(line.substr(i, 1)));
        }

        matrix.push_back(values);
    }

    return matrix;
}

bool is_visible(vector<vector<uint32_t>> matrix, int row, int col)
{
    // from left
    bool visible = true;
    uint32_t current = matrix[row][col];
    for (auto i = 0; i < col && visible; ++i)
    {
        if (matrix[row][i] >= current)
        {
            visible = false;
        }
    }

    if (visible)
        return true;

    // from top
    visible = true;
    for (auto i = 0; i < row && visible; ++i)
    {
        if (matrix[i][col] >= current)
        {
            visible = false;
        }
    }

    if (visible)
        return true;

    // from right
    visible = true;
    for (auto i = matrix[row].size() - 1; i > col; --i)
    {
        if (matrix[row][i] >= current)
        {
            visible = false;
        }
    }

    if (visible)
        return true;

    visible = true;
    for (auto i = matrix[row].size() - 1; i > row; --i)
    {
        if (matrix[i][col] >= current)
        {
            visible = false;
        }
    }

    return visible;
}

uint32_t part1(vector<string> &lines)
{
    uint32_t sum = 0;

    auto matrix = convert_to_int(lines);

    // add top and bottom row
    sum += matrix[0].size() * 2;
    for (auto row = 1; row < matrix.size() - 1; ++row)
    {
        // add first and last values
        sum += 2;
        for (auto col = 1; col < matrix[row].size() - 1; ++col)
        {
            if (is_visible(matrix, row, col))
            {
                sum += 1;
            }
        }
    }

    return sum;
}

#define LEFT 0
#define TOP 1
#define RIGHT 2
#define BOTTOM 3

uint32_t compute_view(vector<vector<uint32_t>> matrix, int row, int col)
{
    uint32_t distances[4] = {0};
    uint32_t current = matrix[row][col];

    // chek left side
    for (auto i = col - 1; i >= 0; --i)
    {
        distances[LEFT] += 1;

        if (matrix[row][i] >= current)
        {
            break;
        }
    }

    // check top side
    for (auto i = row - 1; i >= 0; --i)
    {
        distances[TOP] += 1;
        if (matrix[i][col] >= current)
        {
            break;
        }
    }

    // check right side
    for (auto i = col + 1; i < matrix[row].size(); ++i)
    {
        distances[RIGHT] += 1;

        if (matrix[row][i] >= current)
        {
            break;
        }
    }

    // check bottom side
    for (auto i = row + 1; i < matrix.size(); ++i)
    {
        distances[BOTTOM] += 1;

        if (matrix[i][col] >= current)
        {
            break;
        }
    }

    return distances[LEFT] * distances[TOP] * distances[RIGHT] * distances[BOTTOM];
}

uint32_t part2(vector<string> lines)
{
    auto matrix = convert_to_int(lines);
    uint32_t highest = 0;

    for (auto row = 0; row < matrix.size(); ++row)
    {
        for (auto col = 0; col < matrix[row].size(); ++col)
        {
            uint32_t score = compute_view(matrix, row, col);
            if (score > highest)
            {
                highest = score;
            }
        }
    }

    return highest;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cerr << "usage: " << argv[0] << " <input.txt>" << endl;
        exit(1);
    }

    vector<string> lines = read_file(argv[1]);

    auto sum = part1(lines);
    cout << "visible trees: " << sum << endl;

    auto highest = part2(lines);
    cout << "highest scenic view " << highest << endl;

    return 0;
}