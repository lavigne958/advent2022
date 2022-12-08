#include <vector>
#include <fstream>
#include <iostream>
#include <set>

using namespace std;

class Node
{
public:
    virtual string getName() = 0;
    virtual string getPath() = 0;
    virtual bool getType() = 0;
    virtual uint32_t getSize() = 0;
};

class MyFile : public Node
{
private:
    string name;
    uint32_t size;

public:
    MyFile(string name, uint32_t size) : name(name), size(size){};

    string getName()
    {
        return this->name;
    }

    string getPath()
    {
        return this->getName();
    }

    uint32_t getSize()
    {
        return this->size;
    }

    bool getType()
    {
        return false;
    }
};

class Directory : public Node
{
private:
    vector<Node *> nodes;
    string name;
    Directory *parent;

public:
    Directory(string name, Directory *parent) : name(name), parent(parent)
    {
        this->nodes = vector<Node *>();
    };

    void addNode(Node *n)
    {
        this->nodes.push_back(n);
        cout << "[";
        if (this->parent == nullptr)
        {
            cout << "/";
        }
        else
        {
            cout << this->getPath();
        }
        cout << "]";
        for (auto &n : this->nodes)
        {
            cout << " " << n->getName();
        }
        cout << endl;
    }

    vector<Node *> getNodes()
    {
        return this->nodes;
    }

    string getName()
    {
        return this->name;
    }

    string getPath()
    {
        if (this->parent == nullptr)
        {
            return "";
        }

        return this->parent->getPath() + "/" + this->getName();
    }

    Directory *getDirectory(string name)
    {
        for (auto node : this->nodes)
        {
            if (node->getName().compare(name) == 0)
            {
                Directory *dir = (Directory *)node;
                return dir;
            }
        }

        return nullptr;
    }

    Directory *getParent()
    {
        return this->parent;
    }

    bool getType()
    {
        return true;
    }

    uint32_t getSize()
    {
        uint32_t sum = 0;
        for (auto node : this->nodes)
        {
            sum += node->getSize();
        }

        return sum;
    }

    void findLargeFolder(uint32_t max, uint32_t &sum)
    {
        for (auto node : this->nodes)
        {
            if (node->getType())
            {
                Directory *dir = (Directory *)node;
                dir->findLargeFolder(max, sum);
                uint32_t size = dir->getSize();
                if (size <= max)
                {
                    sum += size;
                }
            }
        }
    }

    void findLargeEnough(uint32_t required, uint32_t &min_found)
    {
        for (auto node : this->nodes)
        {
            if (node->getType())
            {
                Directory *dir = (Directory *)node;
                dir->findLargeEnough(required, min_found);
                uint32_t dir_size = dir->getSize();
                if (dir_size >= required && dir_size < min_found)
                {
                    min_found = dir_size;
                }
            }
        }
    }
};

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

Directory *build_fs(vector<string> lines)
{
    Directory *root = new Directory("/", nullptr);
    Directory *current_directory = root;
    string current_dir;
    for (auto &line : lines)
    {
        if (line[0] == '$')
        {
            string command = line.substr(2, 2);
            if (command.compare("cd") == 0)
            {
                current_dir = line.substr(5);
                if (current_dir.compare("..") == 0)
                {
                    current_directory = current_directory->getParent();
                }
                else
                {
                    current_directory = current_directory->getDirectory(current_dir);
                }
            }
        }
        else
        {
            if (line.substr(0, 3).compare("dir") == 0)
            {
                current_directory->addNode(new Directory(line.substr(4), current_directory));
            }
            else
            {
                size_t split = line.find(' ');
                uint32_t size = stoul(line.substr(0, split));
                string name = line.substr(split + 1);
                Node *file = new MyFile(name, size);
                current_directory->addNode(file);
            }
        }
    }

    return root;
}

uint32_t part1(vector<string> lines)
{
    Directory *root = build_fs(lines);
    cout << "full size: " << root->getSize() << endl;

    uint32_t sum = 0;
    root->findLargeFolder(100000, sum);

    return sum;
}

uint32_t part2(vector<string> lines)
{
    Directory *root = build_fs(lines);
    uint32_t total_size = 70000000;
    uint32_t upgrade_space = 30000000;
    uint32_t current_free_space = total_size - root->getSize();
    uint32_t required_space = upgrade_space - current_free_space;
    cout << "current free space: " << current_free_space << endl;
    cout << "required to be freed: " << required_space << endl;
    uint32_t min_max = UINT32_MAX;
    root->findLargeEnough(required_space, min_max);

    return min_max;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cerr << "usage: " << argv[0] << " <input.txt>" << endl;
        exit(1);
    }

    vector<string> lines = read_file(argv[1]);
    lines.erase(lines.begin());
    cout << part2(lines) << endl;

    return 0;
}