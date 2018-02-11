#include <iostream>
#include <vector> 
#include <board/board.h>
int main(int argc, char**argv) {
    Board board("rnbqk2r/p1ppp1bp/5n2/1p6/8/2NP1N2/PPP2PPP/R1BQKB1R w KQkq -");
    std::vector<Move> moves = board.get_legal_moves();
    for(int i = 0; i < moves.size(); i++) {
        std::cout << moves[i].get_uci() << std::endl;
    }
    return 0;
}
