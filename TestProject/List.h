#pragma once
#include <string>
#include <vector>

//3. Реализуйте функции сериализации и десериализации двусвязного списка. Данные должны быть записаны в бинарном формате. Ожидаемая алгоритмическая сложность – меньше квадратичной. 
// структуру ListNode модифицировать нельзя 
struct ListNode { 
    ListNode* prev = nullptr; // указатель на предыдущий элемент списка, либо `nullptr` в случае начала списка
    ListNode* next = nullptr; 
    ListNode* rand = nullptr; // указатель на произвольный элемент данного списка, либо `nullptr` 
    std::string data; // произвольные пользовательские данные
}; 


class List { 
public:
    void Serialize(FILE* file);
    void Deserialize(FILE* file);
    
    void AddNode(const std::string& value);
    
    void MakeRandom();
    
    ListNode* GetHead() const {return head;}
    
    void PrintData();
    
private: 
    ListNode* head = nullptr;
    ListNode* tail = nullptr; 
    int count = 0;

    void Clear();
    
    bool TryWriteNode(FILE* file, const ListNode* node, int rand_index);
    bool TryReadNode(void* buffer, size_t element_size, size_t count, FILE* file, const char* error_message);
    ListNode* ResolveRand(const std::vector<ListNode*>& nodes, int index);
};

