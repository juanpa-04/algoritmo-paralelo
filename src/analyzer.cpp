// Inclusión de bibliotecas necesarias
#include <iostream>     // Para entrada/salida estándar
#include <vector>       // Para usar vectores
#include <random>       // Para generación de números aleatorios
#include <chrono>       // Para medición de tiempos
#include <algorithm>    // Para funciones como generate()
#include <omp.h>        // Para paralelización con OpenMP

// Definición de la clase para analizar el rendimiento de Merge Sort
class MergeSortAnalyzer {
private:
    // Miembros privados de la clase
    std::vector<int> original_list;  // Vector para almacenar lista original
    const unsigned int SEED = 2287465853;  // Semilla para generador aleatorio
    const int LOW = 0;        // Límite inferior de números aleatorios
    const int HIGH = 10000;   // Límite superior de números aleatorios

    // Método privado para realizar Merge Sort recursivo
    void mergesort(std::vector<int>& list, int l_index, int r_index) {
        // Condiciones de terminación para recursión
        if (list.size() == 1) return;  // Lista de un solo elemento
        if (r_index <= l_index) return;  // Sublista de un elemento

        // Calcular punto medio para dividir el arreglo, dividir la lista en dos
        int midpoint = l_index + (r_index - l_index) / 2;

        // Región paralela de OpenMP
        #pragma omp parallel 
        {   
            // Directiva para que solo un hilo ejecute la división
            #pragma omp single 
            {
                // Tarea paralela para ordenar mitad izquierda
                #pragma omp task
                mergesort(list, l_index, midpoint);

                // Tarea paralela para ordenar mitad derecha
                #pragma omp task
                mergesort(list, midpoint + 1, r_index);
            }
        }
   
        // Esperar a que todas las tareas de ordenamiento terminen
        #pragma omp taskwait
        
        // Combinar las dos mitades ordenadas
        merge(list, l_index, midpoint, r_index);
    }

    // Método privado para combinar dos sublistas ordenadas
    void merge(std::vector<int> &list, int l_index, int midpoint, int r_index) {
        std::vector<int> merged;  // Vector temporal para almacenar resultado
        int left = l_index;       // Índice inicial de sublista izquierda
        int right = midpoint + 1; // Índice inicial de sublista derecha

        // Comparar y combinar elementos de ambas sublistas
        while(left <= midpoint && right <= r_index) {
            if (list[left] < list[right]) {
                merged.push_back(list[left++]);
            } else {
                merged.push_back(list[right++]);
            }
        }

        // Agregar elementos restantes de sublista izquierda
        while(left <= midpoint)
            merged.push_back(list[left++]);
    
        // Agregar elementos restantes de sublista derecha
        while(right <= r_index)
            merged.push_back(list[right++]);

        // Copiar elementos combinados de vuelta al arreglo original
        for(int i {l_index}; i <= r_index; ++i) {
            list[i] = merged[i - l_index];
        }
    }

public:
    // Constructor que genera lista de números aleatorios
    MergeSortAnalyzer(int size) {
        // Generador de números aleatorios con semilla fija
        std::mt19937 engine(SEED);
        
        // Distribución uniforme de enteros
        std::uniform_int_distribution<int> get(LOW, HIGH);
        
        // Redimensionar lista original
        original_list.resize(size);
        
        // Generar números aleatorios
        std::generate(original_list.begin(), original_list.end(), [&](){ return get(engine);});
    }

    // Método para análisis de rendimiento, imprime estadisticas 
    void performanceAnalysis() {
        // Tamaños de problema a probar
        std::vector<int> problem_sizes = {100000, 500000, 1000000, 2000000, 5000000};
        
        // Número de hilos a probar
        std::vector<int> thread_counts = {1, 2, 4, 8, 16};

        // Imprimir encabezado de resultados
        std::cout << "Performance Analysis for Merge Sort\n";
        std::cout << "Problem Size\tThreads\tTime (ms)\tSpeedup\n";

        // Iterar sobre diferentes tamaños de problema
        for (int size : problem_sizes) {
            // Crear copia del tamaño de problema actual
            std::vector<int> test_list(original_list.begin(), original_list.begin() + size);

            // Probar con diferentes números de hilos
            for (int thread_count : thread_counts) {
                // Establecer número de hilos
                omp_set_num_threads(thread_count);

                // Medición de tiempo secuencial
                auto start_seq = std::chrono::high_resolution_clock::now();
                {
                    std::vector<int> seq_list = test_list;
                    mergesort(seq_list, 0, seq_list.size() - 1);
                }
                auto end_seq = std::chrono::high_resolution_clock::now();
                auto seq_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_seq - start_seq).count();

                // Medición de tiempo paralelo
                auto start_par = std::chrono::high_resolution_clock::now();
                {
                    std::vector<int> par_list = test_list;
                    mergesort(par_list, 0, par_list.size() - 1);
                }
                auto end_par = std::chrono::high_resolution_clock::now();
                auto par_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_par - start_par).count();

                // Calcular speedup (aceleración)
                double speedup = static_cast<double>(seq_time) / par_time;

                // Imprimir resultados
                std::cout << size << "\t\t" << thread_count << "\t" 
                          << par_time << "\t\t" << speedup << "\n";
            }
            std::cout << "\n";
        }
    }
};

// Función principal
int main() {
    // Crear analizador con 5 millones de elementos
    MergeSortAnalyzer analyzer(5000000);
    
    // Realizar análisis de rendimiento
    analyzer.performanceAnalysis();
    
    return 0;
}