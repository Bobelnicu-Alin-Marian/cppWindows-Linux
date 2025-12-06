#pragma once 
#include <mutex>
#include <condition_variable>

// Enum pentru a gestiona prioritatea
enum Color { NONE, WHITE, BLACK };

class ResourceController {
private:
    std::mutex mtx;                 // Lac?tul pentru zona critic?
    std::condition_variable cv;     // Sala de a?teptare pentru thread-uri

    int active_white;   // Albi în?untru
    int active_black;   // Negri în?untru
    int waiting_white;  // Albi la coad?
    int waiting_black;  // Negri la coad?

    Color next_turn;    // Variabil? anti-înfometare (Starvation protection)

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