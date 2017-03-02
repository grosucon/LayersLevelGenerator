#include <cstdlib>
#include <ctime>
#include <iostream>

#define COLORS_COUNT 5
#define LAYER_HEIGHT 10
#define LAYER_WIDTH 6
#define forlayer for (int i=0; i<LAYER_HEIGHT; i++) for (int j=0; j<LAYER_WIDTH; j++)

using namespace std;

int main() {

    int layer[LAYER_HEIGHT][LAYER_WIDTH];

    srand(time(NULL));
    forlayer {
            layer[i][j] = rand() % COLORS_COUNT + 1;
            cout << layer[i][j] << flush;
            if ( j == LAYER_WIDTH-1 ) cout << endl;
            else cout << " " << flush;
        };

    return 0;
}
