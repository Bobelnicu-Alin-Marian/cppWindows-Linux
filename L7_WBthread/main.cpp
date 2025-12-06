#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include "ResourceController.h" 

// Func?ia executat? de thread-ul ALB
void threadWhite(ResourceController& controller, int id) {
    // Simulare sosire
    std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 100));

    controller.enterWhite(id);

    // FOLOSIREA RESURSEI (Munc? simulat?)
    std::this_thread::sleep_for(std::chrono::milliseconds(100 + rand() % 200));

    controller.exitWhite(id);
}

// Func?ia executat? de thread-ul NEGRU
void threadBlack(ResourceController& controller, int id) {
    std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 100));

    controller.enterBlack(id);

    // FOLOSIREA RESURSEI
    std::this_thread::sleep_for(std::chrono::milliseconds(100 + rand() % 200));

    controller.exitBlack(id);
}

int main() {
    ResourceController controller;
    std::vector<std::thread> threads;

    int numar_perechi = 10; // Vom crea 10 perechi (20 thread-uri total)

    std::cout << "=== Pornire Simulare (Metoda Perechi) ===\n";

    for (int i = 0; i < numar_perechi; ++i) {
        // La fiecare pas al buclei, cre?m o pereche (un Alb ?i un Negru)

        // Cre?m firul ALB
        threads.emplace_back(threadWhite, std::ref(controller), i);

        // Cre?m firul NEGRU
        threads.emplace_back(threadBlack, std::ref(controller), i);
    }

    // A?tept?m finalizarea tuturor
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    std::cout << "=== Simulare finalizata. ===\n";
    return 0;
}