
#include <iostream>
#include "MaxHeap.hpp"

Heap::Heap(int capacity)
{
    this->capacity = capacity;
    this->size = 0;
    this->arr = new PlayerData[capacity];
}

bool Heap::isEmpty()
{
    return size == 0;
}

int Heap::parent(int i)
{
    return (i - 1) / 2;
}

int Heap::left(int i)
{
    return (2 * i + 1);
}

int Heap::right(int i)
{
    return (2 * i + 2);
}

PlayerData Heap::getMax()
{
    return arr[0];
}

void Heap::insert(PlayerData k)
{
    if (size == capacity)
    {
        std::cout << "Heap is full" << std::endl;
        return;
    }

    size++;
    int i = size - 1;
    arr[i] = k;

    while (i != 0 && arr[parent(i)].score < arr[i].score)
    {
        std::swap(arr[i], arr[parent(i)]);
        i = parent(i);
    }
}

void Heap::heapify(int i)
{
    int l = left(i);
    int r = right(i);
    int largest = i;

    if (l < size && arr[l].score > arr[i].score)
        largest = l;

    if (r < size && arr[r].score > arr[largest].score)
        largest = r;

    if (largest != i)
    {
        std::swap(arr[i], arr[largest]);
        heapify(largest);
    }
}

PlayerData Heap::extractMax()
{
    if (size <= 0)
        return PlayerData("NULL", INT_MAX);

    if (size == 1)
    {
        size--;
        return arr[0];
    }

    PlayerData root = arr[0];
    arr[0] = arr[size - 1];
    size--;
    heapify(0);

    return root;
}

void Heap::decreaseKey(int i, PlayerData new_val)
{
    arr[i] = new_val;
    while (i != 0 && arr[parent(i)].score < arr[i].score)
    {
        std::swap(arr[i], arr[parent(i)]);
        i = parent(i);
    }
}

void Heap::deleteKey(int i)
{
    decreaseKey(i, PlayerData("NULL", INT_MAX));
    extractMax();
}

void Heap::print()
{
    for (int i = 0; i < size; i++)
        std::cout << arr[i].name << ' ' << arr[i].score << " ";
    std::cout << std::endl;
}

// int main()
// {
//     Heap h(11);
//     h.insert(3);
//     h.insert(2);
//     h.deleteKey(1);
//     h.insert(15);
//     h.insert(5);
//     h.insert(4);
//     h.insert(45);
//     h.print();
//     std::cout << h.extractMax() << " ";
//     std::cout << h.getMax() << " ";
//     h.decreaseKey(2, 1);
//     std::cout << h.getMax();
//
//     return 0;
// }

