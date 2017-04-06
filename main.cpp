#include <iostream>
#include <ctime>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cmath>
using namespace std;

#define COLORS_COUNT 5 /** colors numbers 0-4 */
#define LAYER_HEIGHT 10
#define LAYER_WIDTH 6
#define START_NR 5
#define FINISH_NR 6
#define BLOCK_NR 7

/****************************************************/
struct node {
    int info;
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
    void enqueue(int);
    int dequeue();
};

Queue::Queue() {
    front = NULL;
    rear = NULL;
}

Queue::~Queue() {
    delete front;
}

void Queue::enqueue(int data) {
    node *temp = new node();
    temp->info = data;
    temp->next = NULL;
    if( front == NULL ) {
        front = temp;
    } else {
        rear->next = temp;
    }
    rear = temp;
}

int Queue::dequeue() {
    node *temp = new node();
    int value;
    if( front != NULL ) {
        temp = front;
        value = temp->info;
        front = front->next;
        delete temp;
    }
    return value;
}

bool Queue::isEmpty() {
    return (front == NULL);
}
/****************************************************/
struct cell {
    int color;
    int groupID; /** ID of color group */
    int move;
    int position;
    int cntneighb;
    int x[4]; /** coordinates of      */
    int y[4]; /** neighbours on level */
    bool visited;
};

struct group {
    int id;
    vector <int> neighbours;
    bool visited;
};

class Graph {
private:
    cell level[LAYER_HEIGHT][LAYER_WIDTH];
    vector <group> colorgroups;

    bool isvalidI(int i) {
        if ( i < 0 || i >= LAYER_HEIGHT ) return false;
        return true;
    }
    bool isvalidJ(int j) {
        if ( j >= LAYER_WIDTH || j < 0 ) return false;
        return true;
    }
public:
    int start_x, finish_x;
    int start_y, finish_y;
    Graph() {};
    ~Graph() {};
    void GenLay();
    void setNeighbours();
    void setIDgroupcolor();
    void createColGr();
    void BFS();
};
/****************************************************/
void Graph::GenLay() {

    srand(time(NULL));
    for (int i=0; i<LAYER_HEIGHT; i++) {
        for (int j = 0; j < LAYER_WIDTH; j++) {
            level[i][j].color = rand() % COLORS_COUNT;
            level[i][j].visited = false;    /** initailized all cells as unvisited */
            level[i][j].cntneighb = 0;
            level[i][j].move = 999;
            level[i][j].position = i*LAYER_WIDTH + j + 1;
            level[i][j].groupID = level[i][j].position+10;
            if ( j != 0 && rand() % 2 && rand() % 2 ) { level[i][j].color=level[i][j-1].color; } /** easy levels 2 rands                 */
            if ( i != 0 && rand() % 2 && rand() % 2 ) { level[i][j].color=level[i-1][j].color; } /** for more hard levels use 3 rands    */
        }
    }

    level[start_x = (LAYER_HEIGHT-1)][start_y = rand() % LAYER_WIDTH].color=START_NR; /** rand start and */
    level[finish_x = 0][finish_y = rand() % LAYER_WIDTH].color=FINISH_NR;             /** final cell     */

    for (int i = 0; i < LAYER_HEIGHT; i++) {
        for (int j = 0; j < LAYER_WIDTH; j++) {
            cout << level[i][j].color;
            if ( j == LAYER_WIDTH - 1 ) cout << endl;
            else cout << " ";
        }
    }

    /** Set the neighbours cells */
    setNeighbours();
}
/****************************************************/
void Graph::setNeighbours() {
    int cnt;
    for (int i = 0; i < LAYER_HEIGHT; i++) {
        for (int j = 0; j < LAYER_WIDTH; j++) {
            if ( isvalidJ(j-1) ) { cnt=level[i][j].cntneighb++; level[i][j].x[cnt]=i; level[i][j].y[cnt]=j-1; }
            if ( isvalidI(i-1) ) { cnt=level[i][j].cntneighb++; level[i][j].x[cnt]=i-1; level[i][j].y[cnt]=j; }
            if ( isvalidJ(j+1) ) { cnt=level[i][j].cntneighb++; level[i][j].x[cnt]=i; level[i][j].y[cnt]=j+1; }
            if ( isvalidI(i+1) ) { cnt=level[i][j].cntneighb++; level[i][j].x[cnt]=i+1; level[i][j].y[cnt]=j; }
        }
    }

    /** Set the different ID to each group color */
    setIDgroupcolor();
}
/****************************************************/
void Graph::setIDgroupcolor() {
    for (int i = 0; i < LAYER_HEIGHT; i++) {
        for (int j = 0; j < LAYER_WIDTH; j++) {
            int x,y;
            if ( isvalidJ(j-1) && (level[i][j].color==level[i][j-1].color) ) level[i][j].groupID = level[i][j-1].groupID;
            if ( isvalidI(i-1) && (level[i][j].color==level[i-1][j].color) ) level[i][j].groupID = level[i-1][j].groupID;
            if ( isvalidJ(j+1) && (level[i][j].color==level[i][j+1].color) ) level[i][j+1].groupID = level[i][j].groupID;
            if ( isvalidI(i+1) && (level[i][j].color==level[i+1][j].color) ) level[i+1][j].groupID = level[i][j].groupID;
            for (int n = 0; n < level[i][j].cntneighb; n++) {
                x = level[i][j].x[n];
                y = level[i][j].y[n];
                if ( level[x][y].color==level[i][j].color ) {
                    if ( x > i || y > j )
                        level[i][j].groupID = level[x][y].groupID;
                    else
                        level[x][y].groupID = level[i][j].groupID;
                }
            }
        }
    }

    for (int i = LAYER_HEIGHT-1; i >= 0; i--) {
        for (int j = LAYER_WIDTH-1; j >= 0; j--) {
            int x,y;
            if ( isvalidJ(j+1) && (level[i][j].color==level[i][j+1].color) ) level[i][j].groupID = level[i][j+1].groupID;
            if ( isvalidI(i+1) && (level[i][j].color==level[i+1][j].color) ) level[i][j].groupID = level[i+1][j].groupID;
            if ( isvalidJ(j-1) && (level[i][j].color==level[i][j-1].color) ) level[i][j-1].groupID = level[i][j].groupID;
            if ( isvalidI(i-1) && (level[i][j].color==level[i-1][j].color) ) level[i-1][j].groupID = level[i][j].groupID;
            for (int n = level[i][j].cntneighb-1; n >= 0 ; n--) {
                x = level[i][j].x[n];
                y = level[i][j].y[n];
                if ( level[x][y].color==level[i][j].color ) {
                    if ( x > i || y > j )
                        level[i][j].groupID = level[x][y].groupID;
                    else
                        level[x][y].groupID = level[i][j].groupID;
                }
            }
        }
    }

    /** Create the color group and its neighbours */
    createColGr();

    cout << endl;

    for (int i = 0; i < LAYER_HEIGHT; i++) {
        for (int j = 0; j < LAYER_WIDTH; j++) {
            cout << level[i][j].groupID;
            if ( j == LAYER_WIDTH - 1 ) cout << endl;
            else cout << " ";
        }
    }
}
/****************************************************/
void Graph::createColGr() {
    for (int i = 0; i < LAYER_HEIGHT; i++) {
        for (int j = 0; j < LAYER_WIDTH; j++) {
            bool exist = false;
            int m,x,y;

            for (int n = 0; n < colorgroups.size(); n++) {
                if ( colorgroups[n].id == level[i][j].groupID ) {
                    exist = true;
                    m = n;
                    break;
                }
            }

            if ( exist ) {
                for (int a = 0; a < level[i][j].cntneighb; a++) {
                    x = level[i][j].x[a];   y = level[i][j].y[a];

                    if ( level[x][y].groupID != level[i][j].groupID )
                        colorgroups[m].neighbours.insert(colorgroups[m].neighbours.begin(), level[x][y].groupID);
                }
            }
            else {
                group newgroup;
                vector <int> neigh;
                for (int a = 0; a < level[i][j].cntneighb; a++) {
                    x = level[i][j].x[a];   y = level[i][j].y[a];

                    if ( level[x][y].groupID != level[i][j].groupID )
                        neigh.insert(neigh.begin(), level[x][y].groupID);
                }

                newgroup = {level[i][j].groupID, neigh, false};
                colorgroups.insert(colorgroups.end(), newgroup);
            }
        }
    }

    for (int n = 0; n < colorgroups.size(); n++) {
        sort( colorgroups[n].neighbours.begin(), colorgroups[n].neighbours.end() );
        colorgroups[n].neighbours.erase( unique( colorgroups[n].neighbours.begin(), colorgroups[n].neighbours.end() ), colorgroups[n].neighbours.end() );
    }

    for (int n = 0; n < colorgroups.size(); n++) {
        cout << colorgroups[n].id << " : ";
        for (int z = 0; z < colorgroups[n].neighbours.size(); z++)
            cout << colorgroups[n].neighbours[z] << " ";
        cout << endl;
    }
}
/*************Breadth Firs t Search*******************/
void Graph::BFS() {
    Queue Q;

    /** push initial cell to the queue */
    Q.enqueue(level[start_x][start_y].position);

    level[start_x][start_y].move = 0;         /** mark the first move on the start cell */
    level[start_x][start_y].visited = true;   /** mark it as visited */
    cout << "Breadth first Search starting from cell ";
    cout << level[start_x][start_y].position << " : " << endl;

    /** unless the queue is empty */
    while (!Q.isEmpty()) {

        /** pop the cell from the queue */
        int v = Q.dequeue();
        int x = (v-1) / LAYER_WIDTH;
        int y = (v-1) % LAYER_WIDTH;

        /** display the visited cell */
        cout << v << " ";

        /** from the visited cell visit all neighbours */
        for (int i = 0; i < level[x][y].cntneighb; i++) {
            int x1 = level[x][y].x[i];
            int y1 = level[x][y].y[i];

            /** visit the cell if it is unvisited */
            if (!level[x1][y1].visited) {

                /** adds the cell to the queue */
                Q.enqueue(level[x1][y1].position);

                /** marks the cell as visited */
                level[x1][y1].visited = true;

                if ( level[x][y].color == level[x1][y1].color ) level[x1][y1].move = level[x][y].move;

                /** marks the move to the cell and his neighboor */
                if ( level[x][y].move < level[x1][y1].move ) level[x1][y1].move = level[x][y].move+1;
            }
        }
    }


    cout << endl << endl;

    for (int i = 0; i < LAYER_HEIGHT; i++) {
        for (int j = 0; j < LAYER_WIDTH; j++) {
            cout << level[i][j].move;
            if ( j == LAYER_WIDTH - 1 ) cout << endl;
            else cout << " ";
        }
    }
}
/****************************************************/
int main() {

    /** Create a graph */
    Graph g;

    /** Generate the level */
    g.GenLay();

    /** Search the solution */
    g.BFS();
}