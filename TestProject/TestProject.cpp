// Ядыгин Илья Дмитриевич
// Дата выполнения: 05.05.2025
// Время выполнения: 4 часа

#include <cinttypes>
#include <cstdint>
#include <cstdio>
#include <limits>
#include <type_traits>
#include <cstdlib> 
#include <iomanip>
#include <iostream>
#include <random>

#include "List.h"

using namespace std;

template<typename T>
void SafePrintBits(T value) {
   static_assert(std::is_same<T, int64_t>::value, "SafePrintBits can use only int64_t");
    
    if (value > std::numeric_limits<int64_t>::max()) {
        printf("SafePrintBits Value can't have more than int64_t\n");
        return;
    }

    if (value < std::numeric_limits<int64_t>::min()) {
        printf("SafePrintBits Value can't have less than int64_t\n");
        return;
    }

    printf("Value is : %lld\n", static_cast<long long>(value));
    printf("HEX: 0x%llX\n", static_cast<unsigned long long>(value));
    printf("Bytes format is:\n");
    
    char buffer[80];
    int pos = 0;
    
    for (int i = 63; i >= 0; --i) {
        buffer[pos++] = '0' + ((value >> i) & 1);
        if (i % 8 == 0) buffer[pos++] = ' ';
    }

    buffer[pos] = '\0'; 
    puts(buffer);
    printf("\n");
}

//Напишите функцию, которая принимает на вход знаковое целое число и печатает его двоичное представление как в памяти.
void RunFirstTask()
{
    int64_t test_positive_val1 = 9223372036854775807;
    SafePrintBits(test_positive_val1);
    
    int64_t test_negative_val1 = -9223372036854775807;
    SafePrintBits(test_negative_val1);
}

void RemoveDups(char* str)
{
    if (!str || !*str) return;

    const char* read = str;
    char* write = str;

    while (*read) {
        const char current = *read;
        *write++ = current;
        
        while (*read == current)
            ++read;
    }

    *write = '\0';
}

//Напишите функцию, удаляющую последовательно дублирующиеся символы в строке: 
void RunSecondTask()
{
    char data[] = "AAA BBB   AAA";
    
    RemoveDups(data);
    printf("%s\n", data);// "A B A"
}

void RunThirdTask()
{
    List original;
    
    original.AddNode("one");
    original.AddNode("two");
    original.AddNode("three");

    original.AddNode("four");
    original.AddNode("five");
    original.AddNode("six");
    
    original.MakeRandom();
    
    FILE* f = nullptr;

    errno_t err = fopen_s(&f, "list.bin", "wb");
    if(err != 0)
    {
        perror("File can't fopen_s");
        return;
    }
    
    if (f) {
        original.Serialize(f);

        err = fclose(f);
        if(err != 0)
        {
            perror("File can't fclose");
            return;
        }
    }

    original.PrintData();
    std::cout << "\n\n\n";
    
    FILE* in = nullptr;
    if (fopen_s(&in, "list.bin", "rb") == 0 && in) {
        List restored;
        restored.Deserialize(in);

        err = fclose(in);
        if(err != 0)
        {
            perror("File can't fclose");
            return;
        }

        restored.PrintData();
    }
} 

int main(int argc, char* argv[])
{
    RunFirstTask();
    RunSecondTask();
    RunThirdTask();
    
    return 0;
}
