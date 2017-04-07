#include <iostream>
#include <ctime>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <cmath>
using namespace std;

#define LEVEL_COMPLEXITY 0 /** 0 - easy | 1 - medium | 2 - hard */
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
    int move;
    int position;
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
            switch (LEVEL_COMPLEXITY) {
                case 0: { /** easy levels 2 rands */
                    if (j != 0 && rand() % 2 && rand() % 2) level[i][j].color = level[i][j - 1].color;
                    if (i != 0 && rand() % 2 && rand() % 2) level[i][j].color = level[i - 1][j].color;
                }

            }
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
        if ( colorgroups[n].id == level[start_x][start_y].groupID )     start = n;   /** set start and */
        if ( colorgroups[n].id == level[finish_x][finish_y].groupID )   finish = n;  /** final pos     */
    }

    for (int n = 0; n < (int)colorgroups.size(); n++) {
        cout << colorgroups[n].id << " : ";
        for (int z = 0; z < (int)colorgroups[n].neighbours.size(); z++)
            cout << colorgroups[n].neighbours[z] << " ";
        cout << endl;
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
    cout << "Breadth first Search starting from group ";
    cout << colorgroups[start].id << " : " << endl;

    /** unless the queue is empty */
    while (!Q.isEmpty()) {

        /** pop the group from the queue */
        int v = Q.dequeue();

        /** display the visited group */
        cout << colorgroups[v].id << " ";

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


    cout << endl << endl;

    for (int n = 0; n < (int)colorgroups.size(); n++) {
        cout << colorgroups[n].id << " : " << colorgroups[n].move << endl;
    }
}
/****************************************************/
int main() {

    /** Create a graph */
    Graph g;

    /** Generate the level */
    g.GenLay();
}