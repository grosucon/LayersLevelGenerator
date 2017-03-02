#include <cstdlib>
#include <ctime>
#include <iostream>

#define COLORS_COUNT 5 //colors numbers 0-4
#define LAYER_HEIGHT 10
#define LAYER_WIDTH 6
#define START 5
#define FINISH 6
#define BLOCK 7

using namespace std;

int main() {

    int layer[LAYER_HEIGHT][LAYER_WIDTH];

    srand(time(NULL));
    for (int i=0; i<LAYER_HEIGHT; i++) {
        for (int j = 0; j < LAYER_WIDTH; j++) {
            layer[i][j] = rand() % COLORS_COUNT;
            if ( j != 0 && rand() % 2 && rand() % 2 ) layer[i][j]=layer[i][j-1];
            if ( i != 0 && rand() % 2 && rand() % 2 ) layer[i][j]=layer[i-1][j];
        }
    }

    layer[LAYER_HEIGHT-1][rand() % LAYER_WIDTH]=START; //rand start and
    layer[0][rand() % LAYER_WIDTH]=FINISH;             //final cell

    for (int i=0; i<LAYER_HEIGHT; i++) {
        for (int j = 0; j < LAYER_WIDTH; j++) {
            cout << layer[i][j];
            if (j == LAYER_WIDTH - 1) cout << endl;
            else cout << " ";
        }
    }


    return 0;
}
