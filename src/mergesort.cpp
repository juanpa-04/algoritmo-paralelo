#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

#define VECTOR_SIZE 10000000
#define LOW 0
#define HIGH 100000

void mergesort(std::vector<int>& list, int l_index, int r_index);
void print_vector(std::vector<int> &list);
void merge(std::vector<int> &list, int l_index, int midpoint, int r_index);
int gen(std::mt19937 engine);

int main() {

     /* Crear arreglo de N elementos aleatorios*/
    std::cout << "Generando..." << std::endl;
    std::random_device seeder;
    std::mt19937 engine {seeder()};
    std::uniform_int_distribution get {LOW, HIGH};
    std::vector<int> unsorted_list (VECTOR_SIZE);
    std::generate(unsorted_list.begin(), unsorted_list.end(), [&](){ return get(engine);});

    /* Ordenar la lista */
    std::cout << "Ordenando..." << std::endl;
    mergesort(unsorted_list, 0, unsorted_list.size() - 1);
    std::cout << "Terminado" << std::endl;
}

void mergesort(std::vector<int>& list, int l_index, int r_index) {

    if (list.size() == 1) return; // Si la lista en solo un elemento
    if (r_index <= l_index) return; // Sublista es de  1 elemento

    /* Dividir la list en dos*/
    int midpoint = l_index + (r_index - l_index) / 2;

    /* Ordenar la mitad izquierda*/
    mergesort(list, l_index, midpoint);

    /* Ordenar la mitad derecha*/
    mergesort(list, midpoint + 1, r_index);

    /* Merge las dos listas*/
    merge(list, l_index, midpoint, r_index);
}

void merge(std::vector<int> &list, int l_index, int midpoint, int r_index) {

    int left = l_index; // Primer indice de parte izquierda
    int right = midpoint + 1; // Primer indice de parte derecha

    std::vector<int> merged;

    /* Comparar las dos mitadas elemento por elemento*/
    while(left <= midpoint && right <= r_index) {
        if (list[left] < list[right]) {
            merged.push_back(list[left++]);
        } else {
            merged.push_back(list[right++]);
        }
    }

    /* Tranferir elementos que quedan de la lista izquierda*/
    while(left <= midpoint)
        merged.push_back(list[left++]);
    
     /* Tranferir elementos que quedan de la lista derecha*/
     while(right <= r_index)
        merged.push_back(list[right++]);

     /* Copiar elementos a la lista orignal*/
     for(int i {l_index}; i <= r_index; ++i) {
        list[i] = merged[i - l_index]; // Copiar de l_index a r_index
     }
}

void print_vector(std::vector<int> &list) {
    std::cout << "[ ";
    for (int element: list) {
        std::cout << element << " ";
    }
    std::cout << "]\n";
}