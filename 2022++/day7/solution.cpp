#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <numeric>
#include <functional>
#include <vector>
#include <unordered_map>

struct file_t{
    std::string name;
    int size;
    file_t* parent = nullptr;
    std::unordered_map<std::string, file_t*> nodes;
};

std::vector<std::string> parse_input(const std::string& file_name){
    std::vector<std::string> ret;
    
    std::ifstream fs(file_name);
    std::string line;
    while(std::getline(fs, line)){
        ret.push_back(line);
    }
    return ret;
}

bool is_command(std::string line){
    return line[0] == '$';
}

std::string get_file_name(const std::string line){
    auto div = line.find_first_of(' ');
    
    return line.substr(div + 1);
}

int get_file_size(const std::string line){
    auto div = line.find_first_of(' ');
    
    return stoi(line.substr(0, div));
}

bool is_dir(const std::string& line){
    return line[0] == 'd';
}

bool is_cd_command(const std::string line){
    return line.find("cd") != std::string::npos;
}

bool contains_file(const file_t & file, const std::string & file_name, file_t & child){
    auto it = file.nodes.find(file_name);
    
    if (it != file.nodes.end()){
        child = *(it -> second);

        return true;
    }

    return false;
}

void add_file(file_t & file, file_t & child){
    file.nodes[child.name] = &child;
}

file_t read_filesystem(std::vector<std::string> lines){
    file_t root {.name = "/"};

    auto current = root;
    std::string file_name;
    int file_size;

    for (auto l : lines){
        std::cout << current.name + " "<< current.nodes.size() << std::endl;
        for(auto c : current.nodes)
            std::cout<< c.first << std::endl;
        std::cout << "----" << std::endl;

        if (is_command(l)){
            if (! is_cd_command(l))
                continue;

            file_name = l.substr(l.find("cd") + 3);

            if(file_name == ".."){
                current = *current.parent;
                continue;
            }

            if(file_name == "/"){
                current = root;
                continue;
            }

            file_t next_dir { .name =  file_name, .parent = &current };
            if ( ! contains_file(current, file_name, next_dir))
                add_file(current, next_dir);

            current = next_dir;

            continue;
        }
        
        if (is_dir(l))
            continue;

        file_name = get_file_name(l);
        file_size = get_file_size(l);
        file_t file { .name = file_name, .size = file_size };
        
        if ( ! contains_file(current, file_name, file))
            add_file(current, file);
    }

    return root;
}


void print_filesystem(const file_t & node, int depth){
    std::string prelim = "";
    for (;depth; depth--)
        prelim += '\t';

    std::cout << prelim << node.name << std::endl;

    for (auto pair : node.nodes)
        print_filesystem(*(pair.second), depth + 1);
}

int part1(std::vector<std::string> lines){
    return 0;
}

int part2(std::vector<std::string> lines){
    return 0;
}

int main(){

    //auto lines = parse_input("input.txt");
    auto lines = parse_input("test.txt");

    auto root = read_filesystem(lines);
    print_filesystem(root, 0);

    auto r1 = part1(lines);
    std::cout << "Part 1: "<< r1 << std::endl;

    auto r2 = part2(lines);
    std::cout << "Part 2: "<< r2 << std::endl;
    return 0;
}