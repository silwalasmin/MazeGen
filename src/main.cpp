//
// Created by asmin on 2022-03-06.
//
#include <stack>
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
using namespace std;
#define delay 10ms //ms
class MazeGen : public olc::PixelGameEngine
{
public:
    MazeGen()
    {
        sAppName="MazeGen";
    }
private:
    int  mazeWidth;
    int  mazeHeight;
    // This stores value for cells and will be used to tell to which the cell is connected to.
    int *maze;

    //These bits are to locate the direction of the walls. It's for making the task easier. HEX values is bit mask
    enum
    {
        CELL_PATH_N = 0x01,
        CELL_PATH_E = 0x02,
        CELL_PATH_S = 0x04,
        CELL_PATH_W = 0x08,
        CELL_VISITED = 0x10,
    };
    // Algorithm variables
    int  visitedCells;
    stack<pair<int, int>> mStack;	// pair helps to store two value together i.e. (x,y) coordinates.
    int  pathWidth;

protected:
    // Called by olcConsoleGameEngine
    virtual bool OnUserCreate()
    {
        // Maze Parameters
        mazeWidth = 25;
        mazeHeight = 25;
        maze = new int[mazeWidth * mazeHeight];
        memset(maze, 0x00, mazeWidth * mazeHeight * sizeof(int)); // Allocating the memory base on size
        pathWidth =3;

        // A random cell is used to start the maze.
        int x = rand() % mazeWidth;
        int y = rand() % mazeHeight;
        mStack.push(make_pair(x, y));
        maze[y * mazeWidth + x] = CELL_VISITED;
        visitedCells = 1;
        return true;
    }
// Inside this function the changes is updated and viewed in the screen.
    virtual bool OnUserUpdate(float elapsedTime)
    {
        // this slows down the process. It's not for slowing the algorithm process. Rather it slows the animation
        this_thread::sleep_for(delay);

        // Little lambda function to calculate index in a readable way
        auto offset = [&](int x, int y)
        {
            return (mStack.top().second + y) * mazeWidth + (mStack.top().first + x);
        };

        // Maze Algorithm
        if (visitedCells < mazeWidth * mazeHeight)
        {
            // Create a set of unvisted neighbours
            vector<int> neighbours;

            // North neighbour
            if (mStack.top().second > 0 && (maze[offset(0, -1)] & CELL_VISITED) == 0)
                neighbours.push_back(0);
            // East neighbour
            if (mStack.top().first < mazeWidth - 1 && (maze[offset(1, 0)] & CELL_VISITED) == 0)
                neighbours.push_back(1);
            // South neighbour
            if (mStack.top().second < mazeHeight - 1 && (maze[offset(0, 1)] & CELL_VISITED) == 0)
                neighbours.push_back(2);
            // West neighbour
            if (mStack.top().first > 0 && (maze[offset(-1, 0)] & CELL_VISITED) == 0)
                neighbours.push_back(3);

            // Are there any neighbours available?
            if (!neighbours.empty())
            {
// Next probable cell is choosen randomly
                int next_cell_dir = neighbours[rand() % neighbours.size()];
                // Creating the path between the current and next cell. The code below helps to delete the line between current and next cells.
                switch (next_cell_dir)
                {
                    case 0: // North
                        maze[offset(0, -1)] |= CELL_VISITED | CELL_PATH_S;
                        maze[offset(0,  0)] |= CELL_PATH_N;
                        mStack.push(make_pair((mStack.top().first + 0), (mStack.top().second - 1)));
                        break;

                    case 1: // East
                        maze[offset(+1, 0)] |= CELL_VISITED | CELL_PATH_W;
                        maze[offset( 0, 0)] |= CELL_PATH_E;
                        mStack.push(make_pair((mStack.top().first + 1), (mStack.top().second + 0)));
                        break;

                    case 2: // South
                        maze[offset(0, +1)] |= CELL_VISITED | CELL_PATH_N;
                        maze[offset(0,  0)] |= CELL_PATH_S;
                        mStack.push(make_pair((mStack.top().first + 0), (mStack.top().second + 1)));
                        break;

                    case 3: // West
                        maze[offset(-1, 0)] |= CELL_VISITED | CELL_PATH_E;
                        maze[offset( 0, 0)] |= CELL_PATH_W;
                        mStack.push(make_pair((mStack.top().first - 1), (mStack.top().second + 0)));
                        break;
                }
                visitedCells++;
            }
            else
            {
                // if next cell is not found. the cell is popped from the stack and we go back till next path is found!
                mStack.pop();
            }
        }
        // Let's start drawing the necessary items in the screen.
        // Clear Screen by drawing 'spaces' everywhere
        FillRect(0, 0, ScreenWidth(), ScreenHeight(), olc::BLACK);

        // Draw Maze with pathwidth
        for (int x = 0; x < mazeWidth; x++)
        {
            for (int y = 0; y < mazeHeight; y++)
            {
                // Each cell is inflated by pathWidth, so fill it in
                for (int py = 0; py < pathWidth; py++)
                    for (int px = 0; px < pathWidth; px++)
                    {
                        if (maze[y * mazeWidth + x] & CELL_VISITED)
                            Draw(x * (pathWidth + 1) + px, y * (pathWidth + 1) + py, olc::WHITE); // Draw Cell
                        else
                            Draw(x * (pathWidth + 1) + px, y * (pathWidth + 1) + py, olc::RED); // Draw Cell
                    }
                // Draw passageways between cells
                for (int p = 0; p < pathWidth; p++)
                {
                    if (maze[y * mazeWidth + x] & CELL_PATH_S)
                        Draw(x * (pathWidth + 1) + p, y * (pathWidth + 1) + pathWidth); // Draw South Passage

                    if (maze[y * mazeWidth + x] & CELL_PATH_E)
                        Draw(x * (pathWidth + 1) + pathWidth, y * (pathWidth + 1) + p); // Draw East Passage
                }
            }
        }
        // Draw Unit - the top of the stack
        for (int py = 0; py < pathWidth; py++)
            for (int px = 0; px < pathWidth; px++)
                Draw(mStack.top().first * (pathWidth + 1) + px, mStack.top().second * (pathWidth + 1) + py,olc::GREEN); // Draw Cell
        return true;
    }
};
int main()
{
    // Seed random number generator
    srand(clock());

    MazeGen maze;
    maze.Construct(160, 100, 8, 8);
    maze.Start();
    return 0;
}