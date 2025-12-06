#include "ResourceController.h"
#include <iostream>

// Constructor - initializam toate contoarele cu 0
ResourceController::ResourceController()
    : active_white(0), active_black(0),
      waiting_white(0), waiting_black(0),
      next_turn(WHITE) {
}

// ================= LOGICA PENTRU ALB =================

void ResourceController::enterWhite(int id) {
    std::unique_lock<std::mutex> lock(mtx);
    waiting_white++; // Ne punem la coada

    // Asteptam daca:
    // 1. Sunt negri activi (Regula de excludere)
    // 2. SAU sunt negri care asteapta si e randul lor (Regula anti-starvation)
    while (active_black > 0 || (waiting_black > 0 && next_turn == BLACK)) {
        cv.wait(lock);
    }

    waiting_white--; // Iesim de la coada
    active_white++;  // Intram in resursa

    std::cout << "[ALB " << id << "] a intrat. (Albi activi: " << active_white << ")\n";
}

void ResourceController::exitWhite(int id) {
    std::lock_guard<std::mutex> lock(mtx);
    active_white--;
    std::cout << "[ALB " << id << "] a iesit.  (Albi activi: " << active_white << ")\n";

    if (active_white == 0) {
        // Daca a iesit ultimul alb, dam prioritate negrilor
        next_turn = BLACK;
        cv.notify_all(); // Trezim pe toata lumea sa verifice conditiile
    }
}

// ================= LOGICA PENTRU NEGRU =================

void ResourceController::enterBlack(int id) {
    std::unique_lock<std::mutex> lock(mtx);
    waiting_black++;

    // Asteptam daca sunt albi activi SAU (albi asteapta si e randul lor)
    while (active_white > 0 || (waiting_white > 0 && next_turn == WHITE)) {
        cv.wait(lock);
    }

    waiting_black--;
    active_black++;

    std::cout << "[NEGRU " << id << "] a intrat. (Negri activi: " << active_black << ")\n";
}

void ResourceController::exitBlack(int id) {
    std::lock_guard<std::mutex> lock(mtx);
    active_black--;
    std::cout << "[NEGRU " << id << "] a iesit.  (Negri activi: " << active_black << ")\n";

    if (active_black == 0) {
        // Daca a iesit ultimul negru, dam prioritate albilor
        next_turn = WHITE;
        cv.notify_all();
    }
}
