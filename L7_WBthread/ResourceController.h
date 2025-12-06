#pragma once 
#include <mutex>
#include <condition_variable>

// Enum pentru a gestiona prioritatea
enum Color { NONE, WHITE, BLACK };

class ResourceController {
private:
    std::mutex mtx;                 // Lock pentru zona critica
    std::condition_variable cv;     // threadurile asteapta

    int active_white;   // Albi activi
    int active_black;   // Negri activi
    int waiting_white;  // Albi la coada
    int waiting_black;  // Negri la coada

    Color next_turn;    // Starvation protection

public:
    // Constructor
    ResourceController();

    // Metode pentru firele ALBE
    void enterWhite(int id);
    void exitWhite(int id);

    // Metode pentru firele NEGRE
    void enterBlack(int id);
    void exitBlack(int id);

};
