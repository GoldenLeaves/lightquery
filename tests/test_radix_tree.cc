//
// Created by tramboo on 2020/5/31.
//

#include <lightquery/util/radix_tree_index.hh>
#include <string>
#include <iostream>

int main() {
    lightquery::radix_tree_index<std::string*> tree;
    int key = 8;
    std::string value = "My_Name";
    tree.insert(reinterpret_cast<unsigned char*>(&key), &value, 4);
    auto ptr = tree.get(reinterpret_cast<unsigned char*>(&key), 4);
    std::cout << *ptr << std::endl;

    return 0;
}