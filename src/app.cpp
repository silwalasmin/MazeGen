//
// Created by asmin on 2022-03-07.
//

#include <stack>
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
using namespace std;
constexpr int winWidth=160;
constexpr int winHeight=100;
#define delay 10ms

class MazeGen : public olc::PixelGameEngine
{
public:
    MazeGen(){
        sAppName="MazeName";
    }

private:
    int mazeWidth, mazeHeight, *maze;
    enum {
        CELL_PATH_N =0X01,
        CELL_PATH_E =0X02,
        CELL_PATH_S =0X04,
        CELL_PATH_W =0X08,
        CELL_VISITED =0X10,
    };
    int visitedCells;
    stack<pair<int, int>> mStack; // Here Paris means coordinate pairs (x,y)
    int pathWidth;

protected:
    virtual bool OnUserCreate(){
        mazeWidth=40;
        mazeHeight=25;
        maze=new int[mazeWidth * mazeHeight];
        memset(maze,0x00, mazeWidth * mazeHeight * sizeof(int));
        pathWidth=3;

        //Choosing random cell to start with
        int x=rand() % mazeWidth;
        int y=rand() % mazeHeight;
        mStack.push(make_pair(x,y));
        maze[y* mazeWidth+x]=CELL_VISITED;
        visitedCells=1;
        return true;
    }
    virtual bool OnUserUpdate(float elapsedTime)
    {
        //Slowing down for animation
        this_thread::sleep_for(delay);

        auto offset=[&](int x,int y) {
            return (mStack.top().second+y)*mazeWidth + (mStack.top().first+x);
        };

        //Maze Creating Algorithm
        if(visitedCells<mazeWidth*mazeHeight){
            vector<int> neighbours;

            //North Neighbour
            if(mStack.top().second>0 && (maze[offset(0,-1)] & CELL_VISITED)==0)
                neighbours.push_back(0);

            //East Neighbour
            if(mStack.top().first<mazeWidth-1 && (maze[offset(1,0)] & CELL_VISITED)==0)
                neighbours.push_back(1);

            //South Neighbour
            if(mStack.top().second<mazeHeight-1 && (maze[offset(0,1)] & CELL_VISITED)==0)
                neighbours.push_back(2);

            //West Neighbour
            if(mStack.top().first>0 && (maze[offset(-1,0)] & CELL_VISITED)==0)
                neighbours.push_back(3);

            if(!neighbours.empty()){
                int nextCellDir=neighbours[rand()%neighbours.size()];
                switch (nextCellDir) {
                    case 0:
                        maze[offset(0,-1)] | CELL_VISITED |CELL_PATH_S;
                        maze[offset(0,0)] |=CELL_PATH_N;
                        mStack.push(make_pair(mStack.top().first+0),(mStack.top().second-1));
                        break;
                }
            }
        }
    }

};




int main(){
    srand(clock());

    MazeGen maze;
    maze.Construct(winWidth,winHeight,8,8);
    maze.Start();
    return 0;
}