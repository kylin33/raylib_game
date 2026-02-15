#include "core/Game.h"

int main() {
    // 堆栈分配 Game 对象，生命周期随 main 函数
    Game game(800, 600, "Raylib C++ Refactored");
    game.Run();
    
    return 0;
}