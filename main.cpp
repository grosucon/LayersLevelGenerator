#include <iostream>
#include <ctime>
#include <vector>
#include <algorithm>
#include <fstream>
#include <assert.h>
using namespace std;

#define LEVEL_COMPLEXITY 0  /** 0 - easy | 1 - medium | 2 - hard */
#define COLORS_COUNT 6      /** colors numbers 0 - COLORS_COUNT-1 */
#define START_NR 6          /** equal with COLORS_COUNT */
#define FINISH_NR 7         /** equal with COLORS_COUNT+1 */
#define LAYER_WIDTH 6
#define LAYER_HEIGHT 10

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
    int position;
    int cntneighb;
    int x[4];  /** coordinates of      */
    int y[4];  /** neighbours on level */
};

struct group {
    int id;
    vector <int> neighbours;
    bool visited;
    int move;
    int position;
};

struct move_cmp {
    bool operator()(const group& x, const group& y) const {
        return x.move < y.move;
    }
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

    int IDtoPos(int id) {
        for (int n = 0; n < (int)colorgroups.size(); n++)
            if ( colorgroups[n].id == id )
                return n;
        return -1;
    };
public:
    int start_x, finish_x;
    int start_y, finish_y;
    int start, finish;
    Graph() {};
    ~Graph() {};
    void GenLay();
    void setNeighbours();
    void setIDgroupcolor();
    void createColGr();
    void BFS();
    void testandWrite();
};
/****************************************************/
void Graph::GenLay() {

    srand(time(NULL));
    for (int i=0; i<LAYER_HEIGHT; i++) {
        for (int j = 0; j < LAYER_WIDTH; j++) {
            level[i][j].color = rand() % COLORS_COUNT;
            level[i][j].cntneighb = 0;
            level[i][j].position = i * LAYER_WIDTH + j + 1;
            level[i][j].groupID = level[i][j].position+10;
            switch (LEVEL_COMPLEXITY) {
                case 0: { /** easy levels 2 rands */
                    if (j != 0 && rand() % 2 && rand() % 2) level[i][j].color = level[i][j - 1].color;
                    if (i != 0 && rand() % 2 && rand() % 2) level[i][j].color = level[i - 1][j].color;
                    break;
                }
                case 1: { /** medium levels 3 rands */
                    if (j != 0 && rand() % 2 && rand() % 2 && rand() % 2) level[i][j].color = level[i][j - 1].color;
                    if (i != 0 && rand() % 2 && rand() % 2 && rand() % 2) level[i][j].color = level[i - 1][j].color;
                    break;
                }
                case 2: { /** hard levels 4 rands */
                    if (j != 0 && rand() % 2 && rand() % 2 && rand() % 2 && rand() % 2) level[i][j].color = level[i][j - 1].color;
                    if (i != 0 && rand() % 2 && rand() % 2 && rand() % 2 && rand() % 2) level[i][j].color = level[i - 1][j].color;
                    break;
                }
            }
        }
    }

    level[start_x = (LAYER_HEIGHT-1)][start_y = rand() % LAYER_WIDTH].color=START_NR; /** rand start and */
    level[finish_x = 0][finish_y = rand() % LAYER_WIDTH].color=FINISH_NR;             /** final cell     */

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
}
/****************************************************/
void Graph::createColGr() {
    for (int i = 0; i < LAYER_HEIGHT; i++) {
        for (int j = 0; j < LAYER_WIDTH; j++) {
            bool exist = false;
            int m,x,y;

            for (int n = 0; n < (int)colorgroups.size(); n++) {
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

                newgroup = {level[i][j].groupID, neigh, false, 999, 0};
                colorgroups.insert(colorgroups.end(), newgroup);
            }
        }
    }

    for (int n = 0; n < (int)colorgroups.size(); n++) {
        sort( colorgroups[n].neighbours.begin(), colorgroups[n].neighbours.end() );
        colorgroups[n].neighbours.erase( unique( colorgroups[n].neighbours.begin(), colorgroups[n].neighbours.end() ), colorgroups[n].neighbours.end() );
        colorgroups[n].position = n;
        if ( colorgroups[n].id == level[start_x][start_y].groupID )     start  = n;  /** set start and */
        if ( colorgroups[n].id == level[finish_x][finish_y].groupID )   finish = n;  /** final pos     */
    }

    /** Set the min move to each group */
    BFS();
}
/*************Breadth Firs t Search*******************/
void Graph::BFS() {
    Queue Q;

    /** push initial group to the queue */
    Q.enqueue(start);

    colorgroups[start].move = 0;         /** mark the first move on the start group */
    colorgroups[start].visited = true;   /** mark it as visited */

    /** unless the queue is empty */
    while (!Q.isEmpty()) {

        /** pop the group from the queue */
        int v = Q.dequeue();

        /** from the visited group visit all neighbours */
        for (int i = 0; i < (int)colorgroups[v].neighbours.size(); i++) {

            int w = IDtoPos(colorgroups[v].neighbours[i]);

            /** visit the group if it is unvisited */
            if ( !colorgroups[w].visited ) {
                /** adds the group to the queue */
                Q.enqueue(colorgroups[w].position);

                /** marks the group as visited */
                colorgroups[w].visited = true;
            }

            /** marks the move to the group */
            if ( colorgroups[w].move > (colorgroups[v].move + 1) ) colorgroups[w].move = colorgroups[v].move+1;
        }
    }

    level[finish_x][finish_y].color = START_NR;
}
/****************************************************/
void Graph::testandWrite() {
    ofstream outfile("1.lvl");

    /** Generate the level */
    GenLay();

    switch (LEVEL_COMPLEXITY) {
        case 0: {
            outfile << LAYER_WIDTH << " " << LAYER_HEIGHT << " " << start_x << " " << start_y << " " << finish_x
                    << " " << finish_y << " " << colorgroups[finish].move << endl;
            for (int i = 0; i < LAYER_HEIGHT; i++) {
                for (int j = 0; j < LAYER_WIDTH; j++) {
                    outfile << level[i][j].color;
                    if (j == LAYER_WIDTH - 1) outfile << endl;
                    else outfile << " ";
                }
            }
            break;
        }
        case 1: {
            outfile << LAYER_WIDTH << " " << LAYER_HEIGHT << " " << start_x << " " << start_y << " " << finish_x
                    << " " << finish_y << " " << colorgroups[finish].move << endl;
            for (int i = 0; i < LAYER_HEIGHT; i++) {
                for (int j = 0; j < LAYER_WIDTH; j++) {
                    outfile << level[i][j].color;
                    if (j == LAYER_WIDTH - 1) outfile << endl;
                    else outfile << " ";
                }
            }
            break;
        }
        case 2: {
            outfile << LAYER_WIDTH << " " << LAYER_HEIGHT << " " << start_x << " " << start_y << " " << finish_x
                    << " " << finish_y << " " << colorgroups[finish].move << endl;
            for (int i = 0; i < LAYER_HEIGHT; i++) {
                for (int j = 0; j < LAYER_WIDTH; j++) {
                    outfile << level[i][j].color;
                    if (j == LAYER_WIDTH - 1) outfile << endl;
                    else outfile << " ";
                }
            }
            break;
        }
    }
    outfile.close();
}
/****************************************************/
int main() {

    assert(LEVEL_COMPLEXITY >= 0 && LEVEL_COMPLEXITY <= 2);
    assert(COLORS_COUNT >= 0 && LAYER_HEIGHT >= 0 && LAYER_HEIGHT >= 0);
    assert(COLORS_COUNT == START_NR && COLORS_COUNT == FINISH_NR - 1);

    /** Create a graph */
    Graph g;

    /** Testing and write n levels to file */
    g.testandWrite();

    return 0;
}