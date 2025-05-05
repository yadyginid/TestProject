#include "List.h"

#include <iostream>
#include <random>
#include <unordered_map>
#include <unordered_set>
#include <vector>

bool List::TryWriteNode(FILE* file, const ListNode* node, int rand_index) {
    const int data_length = static_cast<int>(node->data.size());

    if (!fwrite(&data_length, sizeof(data_length), 1, file)) {
        perror("Node data_length can't write");
        return false;
    }

    if (!fwrite(node->data.data(), sizeof(char), data_length, file)) {
        perror("Node data can't write");
        return false;
    }

    if (!fwrite(&rand_index, sizeof(rand_index), 1, file)) {
        perror("Node rand_index can't write");
        return false;
    }

    return true;
}

bool List::TryReadNode(void* buffer, size_t element_size, size_t count, FILE* file, const char* error_message) {
    if (!file || !buffer) return false;
    if (fread(buffer, element_size, count, file) != count) {
        perror(error_message);
        return false;
    }
    return true;
}

void List::Serialize(FILE* file)
{
    if (!file) return;
    
    std::unordered_map<ListNode*, int> node_map;
    int node_index = 0;
    for (ListNode* current_node = head; current_node != nullptr; current_node = current_node->next) {
        node_map[current_node] = node_index;
        ++node_index;
    }
    
    if(!fwrite(&count, sizeof(count), 1, file))
    {
        perror("Node count can't write");
        return;
    }
    
    for (const ListNode* current_node = head; current_node != nullptr; current_node = current_node->next) {
        const int rand_index = current_node->rand ? node_map[current_node->rand] : -1;
        if (!TryWriteNode(file, current_node, rand_index)) return;
    }
}

void List::Deserialize(FILE* file)
{
    if (!file) return;

    Clear();

    int node_count = 0;
    if(!fread(&node_count, sizeof(node_count), 1, file))
    {
        perror("Node count can't read");
        return;
    }
    
    if (node_count <= 0) return;

    std::vector<ListNode*> nodes(node_count);
    std::vector<int> random_nodes(node_count);
    
    for (int i = 0; i < node_count; ++i) {
        int len = 0;
        if (!TryReadNode(&len, sizeof(len), 1, file, "Node can't read length")) return;

        std::string data(len, '\0');
        if (!TryReadNode(&data[0], sizeof(char), len, file, "Node can't read data")) return;

        int rand_index = -1;
        if (!TryReadNode(&rand_index, sizeof(rand_index), 1, file, "Node can't read rand_index")) return;

        const auto node = new ListNode;
        node->data = std::move(data);

        if (i > 0) {
            nodes[i - 1]->next = node;
            node->prev = nodes[i - 1];
        }

        if (i == 0) head = node;
        tail = node;

        nodes[i] = node;
        random_nodes[i] = rand_index;
    }

    // restore random
    for (int i = 0; i < node_count; ++i) {
        nodes[i]->rand = ResolveRand(nodes, random_nodes[i]);
    }

    count = node_count;
}

void List::AddNode(const std::string& value)
{
    const auto node = new ListNode;
    node->data = value;
    
    if (!head)
    {
        tail = node;
        head = tail;
    }
    else {
        tail->next = node;
        node->prev = tail;
        tail = node;
    }

    
    count++;
}

void List::MakeRandom()
{
    if (!head) return;

    ListNode* current = head;
    std::mt19937 rng(std::random_device{}());     
    std::uniform_int_distribution<int> dist(0, 1);
    
    while (current) {
        if (dist(rng) == 0)  
            current->rand = current->next;
        else
            current->rand = nullptr;

        current = current->next;
    }
}

void List::PrintData()
{
    ListNode* n = GetHead();
    while (n) {
        std::cout << "Data: " << n->data;

        std::cout << ", Next: ";
        if (n->next) std::cout << n->next->data;
        else std::cout << "null";

        std::cout << ", Prev: ";
        if (n->prev) std::cout << n->prev->data;
        else std::cout << "null";

        std::cout << ", Rand: ";
        if (n->rand) std::cout << n->rand->data;
        else std::cout << "null";

        std::cout << "\n";
        n = n->next;
    }
}

void List::Clear()
{
    ListNode* current = head;
    while (current) {
        ListNode* next = current->next;
        delete current;
        current = next;
    }
    head = nullptr;
    tail = nullptr;
    count = 0;
}

ListNode* List::ResolveRand(const std::vector<ListNode*>& nodes, int index) {
    return (index >= 0 && index < static_cast<int>(nodes.size())) ? nodes[index] : nullptr;
}