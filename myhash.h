#include <iostream>
#include <vector>
#include <chrono>


// Inicializar la semilla solo una vez al comienzo del programa
void inicializarSemilla() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

int cual() {
    // No cambies la semilla aquí, ya que se inicializa solo una vez al comienzo del programa
    int i = std::rand() % 30;
    return (i % 2 == 0) ? -1 : 0;
}

std::string f_hash(std::string data) {
    // No cambies la semilla aquí, ya que se inicializa solo una vez al comienzo del programa
    std::string result = "          ";
    std::vector<int> azar;

    // Hash tendrá siempre 10 de tamaño
    for (int i = 0; i < 4; i++) {
        int j = cual();
        if (j == 0) {
            result[i] = data[j];
            result[2 * i + 1] = data[data.length() - 1];
        } else {
            result[i] = data[j + std::rand() % (data.length() - 1)];
            result[2 * i + 1] = data[j + 1];
        }

        azar.push_back(std::rand() % 20);
    }

    result[4] = static_cast<char>('a' + azar[0]);
    result[6] = static_cast<char>('a' + azar[1]);
    result[8] = static_cast<char>('a' + azar[2]);
    result[9] = static_cast<char>('a' + azar[3]);

    for (auto i : azar) {
        std::cout << i << " ";
    }

    azar.clear();
    return result;
}

