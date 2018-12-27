//2018-05-19 10:19PM: Just finished : adjacent to the 'current' node node evalution.
//2018-05-25 8:16PM: WARNING: REMEMBER THAT THIS PROGRAM WORKS BACKWARDS, FROM END NODE TO START NODE!
#include <iostream>
#include <string>
#include <cstdlib>
#define MAX_X 10
#define MAX_Y 10
#define MAX_Z 4
using namespace std;


//0=free to move
//1=blocked by wall
//2=stairs
//3=empty
int map[MAX_Z][MAX_Y][MAX_X] =
{
    0,0,0,0,0,1,0,0,0,0,
    0,0,0,0,0,1,0,0,0,0,
    0,0,0,0,0,1,0,0,0,0,
    0,0,0,0,0,1,0,0,0,0,
    0,0,0,0,0,1,0,0,0,0,
    0,0,0,0,0,1,0,0,0,0,
    0,0,0,0,0,1,0,0,0,0,
    0,0,0,0,0,1,0,0,0,0,
    0,0,0,0,0,1,0,0,0,0,
    0,0,0,0,2,1,2,0,0,0,

    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,1,0,0,0,0,

    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,2,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,

    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,

};

std::string moveCheck[3][3] =
{
    {"0","1","2"},
    {"7","9","3"},
    {"6","5","4"}
};


string textCheck[3][3] =
{
    {"ru","u","lu"},
    {"r","","l"},
    {"rd","d","ld"}
};

void drawMap()
{
    for (int z = 0; z < MAX_Z; z++)
    {
        for (int y = 0; y < MAX_Y; y++)
        {
            for (int x = 0; x < MAX_X; x++)
            {
                if (map[z][y][x] == 9)
                {
                    std::cout << "P";
                }
                else if (map[z][y][x] == 0)
                {
                    std::cout << ".";
                }
                else
                {
                    std::cout << map[z][y][x];
                }
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}


class Node
{
  public:
      int gCost;    //Cost of the current node
      int state;    //0 = undefined(unevaluated,visitable), 1 = closed(visited)/blocked,2 = open(available,visitable)
      std::string previous; //the code for movement
      //Node * prev;  //The node that this one is pointing to
};

class Coord
{
    public:
        Coord()
        {
        }
        Coord(int x, int y, int z)
        {
            this->x = x;
            this->y = y;
            this->z = z;
        }
        int x;
        int y;
        int z;
};

bool outOfBounds(int x, int y, int z)
{
    //If any of these conditions are met, then we fail
    return (x < 0 || x >= MAX_X || y < 0 || y >= MAX_Y || z < 0 || z >= MAX_Z); //current element compared to map size
}


void drawStates(Node node[MAX_Z][MAX_Y][MAX_X])
{
    std::cout << '\n';
    for (int z = 0; z < MAX_Z; z++)
    {
        for (int y = 0; y < MAX_Y; y++)
        {
            for (int x = 0; x < MAX_X; x++)
            {
                if (node[z][y][x].state == 0)
                {
                    std::cout << ".";
                }
                else
                {
                    std::cout << node[z][y][x].state;
                }
            }
            std::cout << '\n';
        }
        std::cout << "\n\n";
    }
}

void evaluteAdjacent(Node (*node)[MAX_Y][MAX_X],Coord currentNode) //Graph,currentNode
{
    node[currentNode.z][currentNode.y][currentNode.x].state = 1; //close the current node
    int gValue = 0; //Are we moving diagonally or not? If yes, the value is 14, otherwise 10
    int z = 0; //z only used on stairs
    for (int x = -1; x <= 1; x++)
    {
        for (int y = -1; y <= 1; y++)
        {
            if (x == 0 || y == 0)   //If we're moving non-diagonally
                gValue = 10;
            else
                gValue = 14;
            if ((y == 0) && (x == 0) && (z==0))
            {
                //nothing
            }
            else if (!outOfBounds(currentNode.x+x,currentNode.y+y,currentNode.z+z))  //If the element is not out of bounds
            {
                if ((node[currentNode.z+z][currentNode.y+y][currentNode.x+x].state == 0))    //setundefined node
                {
                    //...then evalute it, or re-evaluate it
                    node[currentNode.z+z][currentNode.y+y][currentNode.x+x].state = 2;    //Make it available as an open node first
                    node[currentNode.z+z][currentNode.y+y][currentNode.x+x].gCost = node[currentNode.z][currentNode.y][currentNode.x].gCost + gValue; //Adjacent node gCost = oldGCost + distance to old current
                    node[currentNode.z+z][currentNode.y+y][currentNode.x+x].previous = moveCheck[y+1][x+1];
                    //Link to old node
                    //node[currentNode.y+y][currentNode.x+x].prev = &node[currentNode.y][currentNode.x];
                }
                else if ((node[currentNode.z+z][currentNode.y+y][currentNode.x+x].state == 2))    //reevalute...
                {
                    if (node[currentNode.z+z][currentNode.y+y][currentNode.x+x].gCost > node[currentNode.z][currentNode.y][currentNode.x].gCost + gValue)   //...if changing it would give a better value...
                    {
                        node[currentNode.z+z][currentNode.y+y][currentNode.x+x].gCost = node[currentNode.z][currentNode.y][currentNode.x].gCost + gValue;
                        node[currentNode.z+z][currentNode.y+y][currentNode.x+x].previous = moveCheck[y+1][x+1];
                        //Pointer to old node
                        //node[currentNode.y+y][currentNode.x+x].prev = &node[currentNode.y][currentNode.x];

                    }
                    //...Otherwise just exit.
                }
            }
            //std::cout << "bean" << std::endl;
        }
    }
    //Do cases for stairs here.
    if (map[currentNode.z][currentNode.y][currentNode.x] == 2 //If current tile is stair or
        || (!outOfBounds(currentNode.x,currentNode.y,currentNode.z-1)
        && map[currentNode.z-1][currentNode.y][currentNode.x] == 2)) //...the one we are moving to is a stair
    {

        //Up
        if (!outOfBounds(currentNode.x,currentNode.y,currentNode.z+1))
        {
            if (node[currentNode.z+1][currentNode.y][currentNode.x].state == 0) //Node not evaluated
            {
                node[currentNode.z+1][currentNode.y][currentNode.x].state = 2;
                node[currentNode.z+1][currentNode.y][currentNode.x].gCost = node[currentNode.z][currentNode.y][currentNode.x].gCost + 10;
                node[currentNode.z+1][currentNode.y][currentNode.x].previous = "8";
            }
            else if (node[currentNode.z+1][currentNode.y][currentNode.x].state == 2) //Node has already been evaluated
            {
                if (node[currentNode.z+1][currentNode.y][currentNode.x].gCost > node[currentNode.z][currentNode.y][currentNode.x].gCost + 10)   //...if changing it would give a better value...
                {
                    node[currentNode.z+1][currentNode.y][currentNode.x].gCost = node[currentNode.z][currentNode.y][currentNode.x].gCost + 10;
                    node[currentNode.z+1][currentNode.y][currentNode.x].previous = "8";
                }
            }
        }
        //Down
        if(!outOfBounds(currentNode.x,currentNode.y,currentNode.z-1))
        {
            if  (node[currentNode.z-1][currentNode.y][currentNode.x].state == 0) //Node not evaluated
            {
                node[currentNode.z-1][currentNode.y][currentNode.x].state = 2;
                node[currentNode.z-1][currentNode.y][currentNode.x].gCost = node[currentNode.z][currentNode.y][currentNode.x].gCost + 10;
                node[currentNode.z-1][currentNode.y][currentNode.x].previous = "9";
            }
            else if (node[currentNode.z-1][currentNode.y][currentNode.x].state == 2) //Node has already been evaluated
            {
                node[currentNode.z-1][currentNode.y][currentNode.x].gCost = node[currentNode.z][currentNode.y][currentNode.x].gCost + 10;
                node[currentNode.z-1][currentNode.y][currentNode.x].previous = "9";
            }
        }
    }


    //drawStates(node);
}

void selectBestNode(Node node[MAX_Z][MAX_Y][MAX_X], Coord * currentNode)
{
    Coord bestNode;
    bestNode.x = MAX_X; //Non-reachable value
    bestNode.y = MAX_Y; //Non-reachable value
    bestNode.z = MAX_Z; //Non-reachable value'
    for (int z = 0 ; z < MAX_Z; z++)
    {
        for (int y = 0; y < MAX_Y; y++)
        {
            for (int x = 0; x < MAX_X; x++)
            {
                if(bestNode.x == MAX_X && bestNode.y == MAX_Y && bestNode.z == MAX_Z && node[z][y][x].state == 2) //If there is no best value yet...
                {
                    //...we do this so we can actually compare the next node. Otherwise we'd be comparing the default (wrong) node!
                    bestNode.x = x;
                    bestNode.y = y;
                    bestNode.z = z;
                }
                //If open and less gCost than current best
                else if ((node[z][y][x].state == 2)  && (node[z][y][x].gCost) < (node[bestNode.z][bestNode.y][bestNode.x].gCost))
                {
                    bestNode.x = x;
                    bestNode.y = y;
                    bestNode.z = z;
                }
            }
        }
    }
    //The best node will be the next currentNode
    currentNode->x = bestNode.x;
    currentNode->y = bestNode.y;
    currentNode->z = bestNode.z;
    return;
}

void setUp(Node * node)
{
    for (int z = 0 ; z < MAX_Z; z++)
    {
        for (int y = 0; y < MAX_Y; y++)
        {
            for (int x = 0; x < MAX_X; x++)
            {
                if (map[z][y][x] == 1)
                {
                    (node+((z*MAX_Y*MAX_X)+(y*MAX_X)+x))->state = 1;  //blokd
                }
                else
                {
                    (node+((z*MAX_Y*MAX_X)+(y*MAX_X)+x))->state = 0;  //ready
                }
            }
        }
    }
    return;
}

string djikstra(int x1, int y1, int z1 , int x2, int y2, int z2)
{
    string movecode = "S";
    Node node[MAX_Z][MAX_Y][MAX_X];    //All the nodes
    setUp(&node[0][0][0]);
    node[z1][y1][x1].gCost = 0; //Start point starts with no cost
    Coord currentNode;
    currentNode.x = x1;
    currentNode.y = y1;
    currentNode.z = z1;
    drawStates(node);
    while(true)
    {
        //Evalutate the nodes adjacent to the current node
        evaluteAdjacent(node,currentNode);
        //Draw the current nodes
        //drawStates(node);
        //Select the best node to be the current node
        selectBestNode(node,&currentNode);
        //There is no path if there is no better node
        if(currentNode.x == MAX_X && currentNode.y == MAX_Y && currentNode.z == MAX_Z) //Didn't work out: No path.
        {
            std::cout << "No path" << std::endl;
            return "X"; //No path magic number
        }
        std::cout << currentNode.x << std::endl;
        std::cout << currentNode.y << std::endl;
        std::cout << currentNode.z << std::endl;
        std::cout << node[currentNode.z][currentNode.y][currentNode.x].gCost << std::endl;
        //If it's the last thingoid
        if (currentNode.y == y2 && currentNode.x == x2 && currentNode.z == z2 )
        {

            int zs = z2;
            //x-search
            int xs = x2;
            //y-search
            int ys = y2;
            //Search back through the pathfinding and make a string
            std::cout << "vvvvvvv" << std::endl;
            while(true)
            {
                //At the end of route
                if ((x1 == xs) && (y1 == ys) && (z1 == zs))
                {
                    break;
                }
                std::cout << "bbbarffff!: " << std::endl;
                //move our "pointer" moving back through the bardingo and append the code
                movecode.append(node[zs][ys][xs].previous);
                std::cout << "Mcs: " << node[zs][ys][xs].previous << std::endl;
                std::cout << "Mc: " << std::atoi(node[zs][ys][xs].previous.c_str()) << std::endl;
                std::cout << "X: " << xs << std::endl;
                std::cout << "Y: " << ys << std::endl;
                std::cout << "Z: " << zs << std::endl;
                drawMap();
                switch(std::atoi(node[zs][ys][xs].previous.c_str()))
                {
                    case 0:xs++;ys++;break;
                    case 1:ys++;     break;
                    case 2:xs--;ys++;break;
                    case 3:xs--;     break;
                    case 4:xs--;ys--;break;
                    case 5:ys--;     break;
                    case 6:xs++;ys--;break;
                    case 7:xs++;     break;
                    case 8:zs--;     break;
                    case 9:zs++;     break;
                }
                map[zs][ys][xs] = 9;
            }
            //Return the movement value
            return movecode;
        }
    }
}

int main()
{
    //Note: The algorithm works backwards for optimisation reasons.
    //Also it's a one step Djistra for optimisation.
    //If you want it to work with an actual path, add a coord to Node class and follow pointers.
    //I won't do it because it is out of specs!!
    std::cout << djikstra(1,1,0,MAX_X-2,1,0) << std::endl;
    drawMap();
    return 0;
}
