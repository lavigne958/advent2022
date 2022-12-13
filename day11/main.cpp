#include <vector>
#include <fstream>
#include <iostream>
#include <utility>
#include <set>
#include <string>
#include <sstream>
#include <unordered_map>

#define STARTING "Starting items"
#define OPERATION "Operation"
#define TEST "Test"
#define IF_TRUE "If true"
#define IF_FALSE ""

using namespace std;

enum op_operator
{
    ADD = 0,
    SUB,
    DIV,
    MUL,
};

class Operation
{
public:
    uint64_t left;
    uint64_t right;
    enum op_operator op;
};

ostream &operator<<(ostream &res, Operation &op)
{
    if (op.left == 0)
    {
        res << "old";
    }
    else
    {
        res << op.left;
    }

    res << " ";

    switch (op.op)
    {
    case ADD:
        res << "+";
        break;
    case SUB:
        res << "-";
        break;
    case DIV:
        res << "/";
        break;
    case MUL:
        res << "*";
        break;

    default:
        break;
    }

    res << " ";

    if (op.right == 0)
    {
        res << "old";
    }
    else
    {
        res << op.right;
    }

    return res;
}

class Monkey
{
public:
    uint32_t id;
    vector<uint64_t> items;
    Operation operation;
    Operation test;
    uint32_t if_true;
    uint32_t if_false;
    uint32_t nr_inspected_items;
};

ostream &operator<<(ostream &res, Monkey &m)
{
    res << "[" << m.id << "] inspections: " << m.nr_inspected_items << " items:";
    for (auto &it : m.items)
    {
        res << " " << it;
    }
    // res << " op: " << m.operation << " test: " << m.test;
    // res << " true: " << m.if_true << " false: " << m.if_false;

    return res;
}

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

Monkey parse_monkey(string &line)
{
    Monkey monkey;
    auto comma_index = line.find(':');

    // start of a monkey
    monkey.id = stoul(line.substr(comma_index - 2, comma_index));

    return monkey;
}

void parse_items(string &line, Monkey &monkey)
{
    auto comma_index = line.find(':');
    auto items_str = line.substr(comma_index + 1);
    size_t from = 0;
    size_t to = 0;
    while (to != string::npos)
    {
        to = items_str.find(',', to + 1);
        auto item = stoull(items_str.substr(from, to));
        monkey.items.push_back(item);
        cout << "item: " << item << endl;
        from = to + 1;
    }
}

Operation parse_operation(enum op_operator op, string line, size_t sign_pos)
{
    Operation operation;
    uint64_t left, right;
    auto equal_index = line.find('=');
    auto left_str = line.substr(equal_index + 2, sign_pos - 2);
    if (!left_str.compare("old"))
    {
        left = 0;
    }
    else
    {
        left = stoull(left_str);
    }

    auto right_str = line.substr(sign_pos + 2);
    if (!right_str.compare("old"))
    {
        right = 0;
    }
    else
    {
        right = stoull(right_str);
    }

    operation.left = left;
    operation.right = right;
    operation.op = op;

    return operation;
}

void parse_operations(string line, Monkey &monkey)
{
    auto equal_index = line.find('=');
    auto operation_str = line.substr(equal_index + 1);

    if (operation_str.find('+') != string::npos)
    {
        // add operator
        auto plus_pos = operation_str.find('+');
        monkey.operation = parse_operation(ADD, operation_str, plus_pos);
    }

    if (operation_str.find('-') != string::npos)
    {
        // sub operator
        auto sub_pos = operation_str.find('-');
        monkey.operation = parse_operation(SUB, operation_str, sub_pos);
    }

    if (operation_str.find('*') != string::npos)
    {
        // mul operator
        auto mul_pos = operation_str.find('*');
        monkey.operation = parse_operation(MUL, operation_str, mul_pos);
    }

    if (operation_str.find('/') != string::npos)
    {
        // div operator
        auto div_pos = operation_str.find('/');
        monkey.operation = parse_operation(DIV, operation_str, div_pos);
    }

    cout << "operation: " << monkey.operation << endl;
}

uint32_t line_last_digiets(string &line)
{
    auto last_space_index = line.find_last_of(' ');
    auto last_digits = line.substr(last_space_index + 1);

    return stoul(last_digits);
}

void parse_test(string &line, Monkey &monkey)
{
    monkey.test.left = 0;
    monkey.test.op = DIV;
    monkey.test.right = line_last_digiets(line);

    cout << "test: " << monkey.test << endl;
}

void parse_true(string &line, Monkey &monkey)
{
    monkey.if_true = line_last_digiets(line);
}

void parse_false(string &line, Monkey &monkey)
{
    monkey.if_false = line_last_digiets(line);
}

void round(unordered_map<uint32_t, Monkey> &monkeys, Monkey &monkey)
{
    while (!monkey.items.empty())
    {
        auto item = monkey.items.front();
        monkey.items.erase(monkey.items.begin());
        monkey.nr_inspected_items += 1;

        // if right is 0 it means old -> item
        auto right = (!monkey.operation.right) ? item : monkey.operation.right;

        switch (monkey.operation.op)
        {
        case MUL:
            item = item * right;
            break;
        case ADD:
            item = item + right;
            break;
        case SUB:
            item = item - right;
            break;
        case DIV:
            item = item / right;
            break;

        default:
            break;
        }

        item /= 3;

        if (item % monkey.test.right == 0)
        {
            monkeys[monkey.if_true].items.push_back(item);
        }
        else
        {
            monkeys[monkey.if_false].items.push_back(item);
        }
    }
}

uint64_t part1(vector<string> &lines)
{
    cout << "parse monkeys" << endl;
    unordered_map<uint32_t, Monkey> monkeys;
    for (int i = 0; i < lines.size(); ++i)
    {
        // start of monkey
        auto monkey = parse_monkey(lines[i]);

        // start of items
        i += 1;
        parse_items(lines[i], monkey);

        // start of operation
        i += 1;
        parse_operations(lines[i], monkey);

        // parse test
        i += 1;
        parse_test(lines[i], monkey);

        // parse if true
        i += 1;
        parse_true(lines[i], monkey);

        // parse if false
        i += 1;
        parse_false(lines[i], monkey);

        cout << monkey << endl;
        i += 1;

        monkeys[monkey.id] = monkey;
    }

    cout << "=== start rounds ===" << endl;

    for (int i = 0; i < 20; ++i)
    {
        cout << "=== round " << i << " ===" << endl;
        for (int i = 0; i < monkeys.size(); ++i)
        {
            round(monkeys, monkeys[i]);
        }

        for (auto &m : monkeys)
        {
            cout << m.second << endl;
        }
    }

    uint32_t max[2] = {0};

    for (auto &m : monkeys)
    {
        if (m.second.nr_inspected_items > max[0])
        {
            max[1] = max[0];
            max[0] = m.second.nr_inspected_items;
        }
        else if (m.second.nr_inspected_items > max[1])
        {
            max[1] = m.second.nr_inspected_items;
        }
    }

    cout << "biggest: " << max[0] << "*" << max[1] << endl;

    return max[0] * max[1];
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cerr << "usage: " << argv[0] << " <input.txt>" << endl;
        exit(1);
    }

    auto lines = read_file(argv[1]);
    auto res = part1(lines);
    cout << "score: " << res << endl;

    return 0;
}