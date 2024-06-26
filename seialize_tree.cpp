#include <variant>
#include <string>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

using DataType = std::variant<int, double, std::string>;

struct TreeNode {
    DataType data;
    std::vector<TreeNode> children;
};

void serialize(const TreeNode& node, std::string& serialize_str) {
    
    uint8_t typeIndex = 0;
    if (std::holds_alternative<int>(node.data)) {
        typeIndex = 0;
    } else if (std::holds_alternative<double>(node.data)) {
        typeIndex = 1;
    } else if (std::holds_alternative<std::string>(node.data)) {
        typeIndex = 2;
    }
    
    serialize_str.append(std::to_string(typeIndex));
    
    if (std::holds_alternative<int>(node.data)) {
        
        std::string value;
        value.append(15 - std::to_string(std::get<int>(node.data)).length(), '0');
        value.append(std::to_string(std::get<int>(node.data)));
        serialize_str.append(value);
    } else if (std::holds_alternative<double>(node.data)) {
        
        std::string value;
        value.append(15 - std::to_string(std::get<double>(node.data)).length(), '0');
        value.append(std::to_string(std::get<double>(node.data)));
        serialize_str.append(value);
    } else if (std::holds_alternative<std::string>(node.data)) {
        
        std::string value = std::get<std::string>(node.data);
        std::string length = "";
        length.append(15 - std::to_string(value.length()).length(), '0');
        length.append(std::to_string(value.length()));
        serialize_str.append(length);
        serialize_str.append(std::get<std::string>(node.data));
    }
    
    std::string child_num = "";
    child_num.append(15 - std::to_string(node.children.size()).length(), '0');
    child_num.append(std::to_string(node.children.size()));
    serialize_str.append(child_num);

    for (const auto& child : node.children) {
        serialize(child, serialize_str);
    }
}

TreeNode deserialize(std::string& serialize_tree) {
 
    TreeNode node;   
    char typeIndex = serialize_tree.front();
    serialize_tree.erase(0, 1);
    
    if (typeIndex == '0') {
        
        int value;
        value = std::stoi(serialize_tree.substr(0, 15));
        serialize_tree.erase(0, 15);
        node.data = value;
    } else if (typeIndex == '1') {
        
        double value;
        value = std::stod(serialize_tree.substr(0, 15));
        serialize_tree.erase(0, 15);
        node.data = value;
    } else if (typeIndex == '2') {
        
        uint32_t length= std::stoi(serialize_tree.substr(0, 15));
        serialize_tree.erase(0, 15);
        
        std::string value = serialize_tree.substr(0, length);
        serialize_tree.erase(0, length);

        node.data = value;
    }
    
    uint32_t numChildren;
    numChildren = std::stoi(serialize_tree.substr(0, 15));
    serialize_tree.erase(0, 15);
    
    node.children.resize(numChildren);
    for (auto& child : node.children) {
        child = deserialize(serialize_tree);
    }
    
    return node;
}

int main() {
    
    TreeNode root;
    root.data = 1;
    //root.children.push_back({2.5, {}});
    root.children.push_back({"hello", {}});
    //root.children[0].children.push_back({3, {}});
    //root.children[0].children.push_back({4.5, {}});
    
    std::string serialize_tree = "";
    serialize(root, serialize_tree);
    std::cout << serialize_tree << std::endl;
    
    
    TreeNode des_node = deserialize(serialize_tree);
    
    std::string serialize_tree2 = "";
    serialize(des_node, serialize_tree2);
    std::cout << serialize_tree2 << std::endl;

    
    return 0;   
}