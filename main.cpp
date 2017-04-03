#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>
using namespace std;

#define COLORS_COUNT 5 // colors numbers 0-4
#define LAYER_HEIGHT 10
#define LAYER_WIDTH 6
#define START_NR 5
#define FINISH_NR 6
#define BLOCK_NR 7

/****************************************************/
struct node {
    cell info;
    node *next;
};

class Queue {
private:
    node *front;
    node *rear;
public:
    Queue();
    ~Queue();
    bool isEmpty();
    void enqueue(cell);
    int dequeue();
};

Queue::Queue() {
    front = NULL;
    rear = NULL;
}

Queue::~Queue() {
    delete front;
}

void Queue::enqueue(cell data) {
    node *temp = new node();
    temp->info = data;
    temp->next = NULL;
    if(front == NULL){
        front = temp;
    }else{
        rear->next = temp;
    }
    rear = temp;
}

int Queue::dequeue() {
    node *temp = new node();
    cell value;
    if(front != NULL) {
        temp = front;
        value = temp->info;
        front = front->next;
        delete temp;
    }
    return value.position;
}

bool Queue::isEmpty() {
    return (front == NULL);
}
/****************************************************/
struct cell {
    int color;
    int move;
    int position;
    int cntneighb;
    int x[4]; /**  coordinates    */
    int y[4]; /**  of neighbours  */
    bool visited;
};

class Graph {
private:
    cell level[LAYER_HEIGHT][LAYER_WIDTH];

    bool isvalidI(int i) {
        if (i < 0 || i >= LAYER_HEIGHT) return false;
        return true;
    }
    bool isvalidJ(int j) {
        if (j >= LAYER_WIDTH || j < 0) return false;
        return true;
    }
public:
    int start_x, finish_x;
    int start_y, finish_y;
    Graph() {};
    ~Graph() {};
    void GenLay();
    void setNeighbours();
    void BFS();
};
/****************************************************/
void Graph::GenLay() {

    srand(time(NULL));
    for (int i=0; i<LAYER_HEIGHT; i++) {
        for (int j = 0; j < LAYER_WIDTH; j++) {
            level[i][j].color = rand() % COLORS_COUNT;
            level[i][j].visited = false;    /** Initailized all cells as unvisited */
            level[i][j].cntneighb = 0;
            level[i][j].move = 999;
            level[i][j].position = i*LAYER_HEIGHT + j + 1;
            if ( j != 0 && rand() % 2 && rand() % 2 ) level[i][j].color=level[i][j-1].color; /** easy levels 2 rands                 */
            if ( i != 0 && rand() % 2 && rand() % 2 ) level[i][j].color=level[i-1][j].color; /** for more hard levels use 3 rands    */
        }
    }

    level[start_x = (LAYER_HEIGHT-1)][start_y = rand() % LAYER_WIDTH].color=START_NR; /** rand start and */
    level[finish_x = 0][finish_y = rand() % LAYER_WIDTH].color=FINISH_NR;             /** final cell     */

    for (int i=0; i<LAYER_HEIGHT; i++) {
        for (int j = 0; j < LAYER_WIDTH; j++) {
            cout << level[i][j].color;
            if (j == LAYER_WIDTH - 1) cout << endl;
            else cout << " ";
        }
    }
}
/****************************************************/
void Graph::setNeighbours() {
    int cnt;
    for (int i = 0; i < LAYER_HEIGHT; i++) {
        for (int j = 0; j < LAYER_WIDTH; j++) {
            if ( isvalidI(i-1) ) { cnt=level[i][j].cntneighb++; level[i][j].x[cnt]=i-1; level[i][j].y[cnt]=j; }
            if ( isvalidI(i+1) ) { cnt=level[i][j].cntneighb++; level[i][j].x[cnt]=i+1; level[i][j].y[cnt]=j; }
            if ( isvalidJ(j-1) ) { cnt=level[i][j].cntneighb++; level[i][j].x[cnt]=i; level[i][j].y[cnt]=j-1; }
            if ( isvalidJ(j+1) ) { cnt=level[i][j].cntneighb++; level[i][j].x[cnt]=i; level[i][j].y[cnt]=j+1; }
        }
    }
}

/*************Breadth First Search*******************/
void Graph::BFS() {
    Queue Q;

    /** Push initial cell to the queue */
    Q.enqueue(level[start_x][start_y]);

    level[start_x][start_y].move = 0;         /** mark the first move on the start cell */
    level[start_x][start_y].visited = true;   /** mark it as visited */
    cout << "Breadth first Search starting from cell ";
    cout << level[start_x][start_y].position << " : " << endl;

    /** Unless the queue is empty */
    while (!Q.isEmpty()) {

        /** Pop the cell from the queue */
        int v = Q.dequeue();
        int x = v/(LAYER_HEIGHT-1);
        int y = v % (LAYER_HEIGHT-1);

        /** display the visited cell */
        cout << v << " ";

        /** From the visited cell v visit all neighbours */
        for (int i = 0; i<level[x][y].cntneighb; i++) {

            /** Visit the cell if it is unvisited */
            if ( !level[x][y].visited ) {

                /** adds the cell w to the queue */
                int x1 = level[x][y].x[i];
                int y1 = level[x][y].y[i];
                Q.enqueue(level[x1][y1]);

                /** marks the cell w as visited */
                level[x1][y1].visited = true;

                /** marks the move to the cell and his neighboor */
                if (level[x][y].move < level[x1][y1].move) level[x1][y1].move = level[x][y].move+1;

                for (int j = 0; j<level[x1][y1].cntneighb; j++) {

                    int x2 = level[x][y].x[j];
                    int y2 = level[x][y].y[j];

                    if (level[x1][y1].color == level[x2][y2].color) {
                        level[x2][y2].visited = true;
                        level[x2][y2].move = level[x1][y1].move;
                    }
                }
            }
        }
    }


    cout << endl;

    for (int i=0; i<LAYER_HEIGHT; i++) {
        for (int j = 0; j < LAYER_WIDTH; j++) {
            cout << level[i][j].move;
            if (j == LAYER_WIDTH - 1) cout << endl;
            else cout << " ";
        }
    }
}
/****************************************************/
int main() {

    /** Creates a graph */
    Graph g;

    /** Generate the level */
    g.GenLay();

    /** Set the neighbours cells */
    g.setNeighbours();

    /** Search the solution */
    g.BFS();
}