import random
import time
import matplotlib.pyplot as plt


# Implementación del algoritmo de ordenamiento Bubble Sort
def bubble_sort(arr):
    n = len(arr)
    for i in range(n):
        for j in range(0, n - i - 1):
            if arr[j] > arr[j + 1]:
                arr[j], arr[j + 1] = arr[j + 1], arr[j]


# Implementación del algoritmo de ordenamiento Quick Sort
def quick_sort(arr):
    if len(arr) <= 1:
        return arr
    else:
        pivot = arr[0]
        left = []
        right = []
        for i in arr[1:]:
            if i < pivot:
                left.append(i)
            else:
                right.append(i)
        return quick_sort(left) + [pivot] + quick_sort(right)


# Función que mide el tiempo de ejecución de un algoritmo de ordenamiento
def measure_time(sort_func, arr):
    start_time = time.time()
    sort_func(arr)
    end_time = time.time()
    return end_time - start_time


# Valores de entrada para las pruebas
input_sizes = [10, 100, 1000, 10000, 100000, 1000000]

# Listas para almacenar los tiempos de ejecución de cada algoritmo
bubble_sort_times = []
quick_sort_times = []

# Realizar las pruebas para cada tamaño de entrada
for size in input_sizes:
    # Generar una lista aleatoria de números
    arr = [random.randint(0, 1000000) for _ in range(size)]

    # Medir el tiempo de ejecución de Bubble Sort
    bubble_sort_time = measure_time(bubble_sort, arr.copy())
    bubble_sort_times.append(bubble_sort_time)

    # Medir el tiempo de ejecución de Quick Sort
    quick_sort_time = measure_time(quick_sort, arr.copy())
    quick_sort_times.append(quick_sort_time)

# Graficar los resultados
plt.plot(input_sizes, bubble_sort_times, label="Bubble Sort")
plt.plot(input_sizes, quick_sort_times, label="Quick Sort")
plt.xlabel("Tamaño de la lista")
plt.ylabel("Tiempo de ejecución (segundos)")
plt.title("Comparación de complejidad algorítmica")
plt.legend()
plt.show()
