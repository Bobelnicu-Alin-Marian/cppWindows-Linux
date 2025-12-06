#include "ResourceController.h"
#include <iostream>

// Constructor - ini?ializ?m toate contoarele cu 0
ResourceController::ResourceController()
    : active_white(0), active_black(0),
    waiting_white(0), waiting_black(0),
    next_turn(WHITE) {
}

// ================= LOGICA PENTRU ALB =================

void ResourceController::enterWhite(int id) {
    std::unique_lock<std::mutex> lock(mtx);
    waiting_white++; // Ne punem la coad?

    // A?tept?m dac?:
    // 1. Sunt negri activi (Regula de excludere)
    // 2. SAU sunt negri care a?teapt? ?i e rândul lor (Regula anti-starvation)
    while (active_black > 0 || (waiting_black > 0 && next_turn == BLACK)) {
        cv.wait(lock);
    }

    waiting_white--; // Ie?im de la coad?
    active_white++;  // Intr?m în resurs?

    std::cout << "[ALB " << id << "] a intrat. (Albi activi: " << active_white << ")\n";
}

void ResourceController::exitWhite(int id) {
    std::lock_guard<std::mutex> lock(mtx);
    active_white--;
    std::cout << "[ALB " << id << "] a iesit.  (Albi activi: " << active_white << ")\n";

    if (active_white == 0) {
        // Dac? a ie?it ultimul alb, d?m prioritate negrilor
        next_turn = BLACK;
        cv.notify_all(); // Trezim pe toat? lumea s? verifice condi?iile
    }
}

// ================= LOGICA PENTRU NEGRU =================

void ResourceController::enterBlack(int id) {
    std::unique_lock<std::mutex> lock(mtx);
    waiting_black++;

    // A?tept?m dac? sunt albi activi SAU (albi a?teapt? ?i e rândul lor)
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
        // Dac? a ie?it ultimul negru, d?m prioritate albilor
        next_turn = WHITE;
        cv.notify_all();
    }
}