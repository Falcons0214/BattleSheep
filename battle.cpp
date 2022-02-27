#include <iostream>
#include <string>
#include <vector>

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3
#define DOWNLEFT 4
#define DOWNRIGHT 5
#define UPLEFT 6
#define UPRIGHT 7
#define UPLINCLINE 8
#define UPRINCLINE 9
#define DOWNLINCLINE 10
#define DOWNRINCLINE 11

class player
{
private:
    unsigned short no;
    std::string name;

public:
    player() : no(0) {}
    player(int num, std::string name)
    {
        if (num > 4)
        {
            std::cerr << "player ID must less than 5\n";
            no = -1;
        }
        else
            no = num;
        this->name = name;
    }
    void showPlayerInfo() const
    {
        std::cout << "ID: " << no << " Name: " << name << std::endl;
    }
    int getPlayerNum() const
    {
        return no;
    }
};

enum Dir
{
    Up = 0,
    RightUp,
    RightDown,
    Down,
    LeftDown,
    LeftUp
};

const int dirDegX[] = {-1, 0, 1, 1, 0, -1};
const int dirDegY[] = {1, 1, 0, -1, -1, 0};

class coordinate
{
protected:
    int degX, degY;

public:
    coordinate() {}
    coordinate(int x, int y) : degX(x), degY(y)
    {
    }
    coordinate(const coordinate &coor) : degX(coor.degX), degY(coor.degY)
    {
    }
    void setDegX(int x)
    {
        degX = x;
    }
    void setDegY(int y)
    {
        degY = y;
    }
    void setDegXaY(int x, int y)
    {
        degX = x;
        degY = y;
    }
    int getDegX() const
    {
        return degX;
    }
    int getDegY() const
    {
        return degY;
    }
    void showCoordInfo() const
    {
        std::cout << "X: " << degX << " Y: " << degY << std::endl;
    }
    friend bool operator==(const coordinate &c1, const coordinate &c2)
    {
        return ((c1.degX == c2.degX) && (c1.degY == c2.degY));
    }
    coordinate findNeighbor(const Dir &dir) const
    {
        return coordinate(degX + dirDegX[dir], degY + dirDegY[dir]);
    }
    bool isNeighborOf(const coordinate &c2) const
    {
        return (
            *this == c2.findNeighbor(Up) ||
            *this == c2.findNeighbor(RightUp) ||
            *this == c2.findNeighbor(LeftUp) ||
            *this == c2.findNeighbor(Down) ||
            *this == c2.findNeighbor(RightDown) ||
            *this == c2.findNeighbor(LeftDown));
    }
};

class cell : public coordinate
{
private:
    const player *owner = nullptr;
    int sheeps = 0;

public:
    cell() {}
    cell(int x, int y) : coordinate(x, y)
    {
    }
    cell(const cell &org) : coordinate(org), owner(org.owner), sheeps(org.sheeps) {}
    cell(const coordinate &org) : coordinate(org) {}
    void setowner(const player *own)
    {
        owner = own;
    }
    void setSheeps(int num)
    {
        sheeps = num;
    }
    int getOwnerId() const
    {
        return owner->getPlayerNum();
    }
    int getSheeps() const
    {
        return sheeps;
    }
    void showOwner() const
    {
        std::cout << "OwnerID: " << owner->getPlayerNum() << std::endl;
    }
    void showSheeps() const
    {
        std::cout << "Sheeps: " << sheeps << std::endl;
    }
};

struct groundToken
{
    friend class world;

private:
    coordinate coord[4];
    groundToken() {}
    groundToken(int x, int y, int method) // less 2 methods
    {
        coord[0].setDegXaY(x, y);
        if (method == UP)
        {
            coord[1].setDegXaY(x - 1, y);
            coord[2].setDegXaY(x - 1, y + 1);
            coord[3].setDegXaY(x, y + 1);
        }
        else if (method == RIGHT)
        {
            coord[1].setDegXaY(x, y + 1);
            coord[2].setDegXaY(x + 1, y + 1);
            coord[3].setDegXaY(x + 1, y);
        }
        else if (method == DOWN)
        {
            coord[1].setDegXaY(x + 1, y);
            coord[2].setDegXaY(x + 1, y - 1);
            coord[3].setDegXaY(x, y - 1);
        }
        else if (method == LEFT)
        {
            coord[1].setDegXaY(x, y - 1);
            coord[2].setDegXaY(x - 1, y - 1);
            coord[3].setDegXaY(x - 1, y);
        }
        else if (method == UPLEFT)
        {
            coord[1].setDegXaY(x - 1, y + 1);
            coord[2].setDegXaY(x, y + 1);
            coord[3].setDegXaY(x - 1, y + 2);
        }
        else if (method == UPRIGHT)
        {
            coord[1].setDegXaY(x - 1, y + 1);
            coord[2].setDegXaY(x - 1, y);
            coord[3].setDegXaY(x - 2, y + 1);
        }
        else if (method == DOWNLEFT)
        {
            coord[1].setDegXaY(x + 1, y - 1);
            coord[2].setDegXaY(x, y - 1);
            coord[3].setDegXaY(x + 1, y - 2);
        }
        else if (method == DOWNRIGHT)
        {
            coord[1].setDegXaY(x + 1, y - 1);
            coord[2].setDegXaY(x + 1, y);
            coord[3].setDegXaY(x + 2, y - 1);
        }
        else if (method == UPRINCLINE)
        {
            coord[1].setDegXaY(x - 1, y + 1);
            coord[2].setDegXaY(x, y + 1);
            coord[3].setDegXaY(x + 1, y);
        }
        else if (method == UPLINCLINE)
        {
            coord[1].setDegXaY(x - 1, y + 1);
            coord[2].setDegXaY(x - 1, y);
            coord[3].setDegXaY(x, y - 1);
        }
        else if (method == DOWNLINCLINE)
        {
            coord[1].setDegXaY(x + 1, y - 1);
            coord[2].setDegXaY(x - 1, y);
            coord[3].setDegXaY(x, y - 1);
        }
        else if (method == DOWNRINCLINE)
        {
            coord[1].setDegXaY(x + 1, y - 1);
            coord[2].setDegXaY(x + 1, y);
            coord[3].setDegXaY(x, y + 1);
        }
    }
    bool isPartOfToken(coordinate coord)
    {
        for (int i = 0; i < 4; i++)
            if (this->coord[i] == coord)
                return true;
        return false;
    }
};

class world
{
private:
    void _pushToGroundCandList(coordinate coord, groundToken &puzzle)
    {
        coordinate coords[6] = {{coord.getDegX() - 1, coord.getDegY() + 1},
                                {coord.getDegX(), coord.getDegY() + 1},
                                {coord.getDegX() + 1, coord.getDegY()},
                                {coord.getDegX() + 1, coord.getDegY() - 1},
                                {coord.getDegX(), coord.getDegY() - 1},
                                {coord.getDegX() - 1, coord.getDegY()}}; // six coordinates beside center hexagon
        for (int i = 0; i < 6; i++)
        {
            int flag = 0;
            for (std::vector<coordinate>::iterator it = _groundCandList.begin(); it != _groundCandList.end(); it++)
            {
                if (puzzle.isPartOfToken(coords[i]) || (*it == coords[i]))
                {
                    flag = 1;
                    break;
                }
            }
            if (!flag)
                _groundCandList.push_back(coordinate(coords[i]));
        }
    }
    void _moveFromGroundCandList(coordinate coord)
    {
        for (std::vector<coordinate>::iterator it = _groundCandList.begin(); it != _groundCandList.end(); it++)
        {
            if (coord.getDegX() == it->getDegX() && coord.getDegY() == it->getDegY())
            {
                _groundCandList.erase(it);
                return;
            }
        }
    }
    bool _isTokenLegal(coordinate coord, int dir)
    {
        groundToken puzzle(coord.getDegX(), coord.getDegY(), dir);
        for (int i = 0; i < 4; i++)
        {
            for (std::vector<cell>::iterator it = cellList.begin(); it != cellList.end(); it++)
                if (*it == puzzle.coord[i])
                    return false;
        }
        return true;
    }
    bool _isInitLegal(coordinate coord)
    {
        unsigned short count = 0;
        for (int i = 0; i < 6; i++)
            for (std::vector<cell>::iterator it = cellList.begin(); it != cellList.end(); it++)
                if (coord.getDegX() + dirDegX[i] == it->getDegX() && coord.getDegY() + dirDegY[i] == it->getDegY())
                    count++;
        
        return (count == 6 || count == 0) ? false : true;
    }
    void _findTargetCell(coordinate coord, coordinate vector)
    {
        int flag = 1, tmpX = vector.getDegX(), tmpY = vector.getDegY();
        while (1)
        {
            for (std::vector<cell>::iterator it = cellList.begin(); it != cellList.end(); it++)
            {
                if (it->getOwnerId() == -1 && it->getDegX() == tmpX + vector.getDegX() && it->getDegY() == tmpY + vector.getDegX())
                {
                    flag = 0;
                    break;
                }
            }
            if (flag)
                break;
            tmpX += vector.getDegX();
            tmpY += vector.getDegX();
        }
        if (coord.getDegX() == tmpX && coord.getDegY() == tmpY)
            std::cout << "This direction is blind alley\n";
        else
            std::cout << "X: " << tmpX << " Y: " << tmpY << std::endl;
    }
    void _printDir(int dir)
    {
        if (dir == 0)
            std::cout << "UP\n";
        else if (dir == 1)
            std::cout << "UPRIGHT\n";
        else if (dir == 2)
            std::cout << "DOWNRIGHT\n";
        else if (dir == 3)
            std::cout << "DOWN\n";
        else if (dir == 4)
            std::cout << "DOWNLEFT\n";
        else
            std::cout << "UPLEFT\n";
    }
    bool init = false;
    std::vector<coordinate> _groundCandList;
    std::vector<cell> cellList;

public:
    world()
    {
        _groundCandList.push_back(coordinate(0, 0));
        init = true;
    }
    // groundToken operation
    bool putGround(coordinate coord, int method)
    {
        int flag = 1;
        for (std::vector<coordinate>::iterator it = _groundCandList.begin(); it != _groundCandList.end(); it++)
            if (*it == coord)
                flag = 0;
        if (flag && init)
        {
            std::cout << "Error coordinate\n";
            return false;
        }
        groundToken puzzle(coord.getDegX(), coord.getDegY(), method);
        for (int i = 0; i < 4; i++)
        {
            cellList.push_back(cell(puzzle.coord[i]));
            _moveFromGroundCandList(puzzle.coord[i]);
            _pushToGroundCandList(puzzle.coord[i], puzzle);
        }
        return true;
    }
    void listGroundCandidates()
    {
        for (std::vector<coordinate>::iterator it = _groundCandList.begin(); it != _groundCandList.end(); it++)
            it->showCoordInfo();
    }
    void listGroundTokenMethod(coordinate coord)
    {
        std::string a[10] = {
            "UP",
            "RIGHT",
            "DOWN",
            "LEFT",
            "DOWNLEFT",
            "DOWNRIGHT",
            "UPLEFT",
            "UPRIGHT",
            "UPLINCLINE",
            "UPRINCLINE"};
        for (int i = 0; i < 10; i++)
            if (_isTokenLegal(coord, i))
                std::cout << a[i] << std::endl;
    }
    // sheep init & operation
    bool initCellSheep(coordinate coord, player *own) // not yet: check start location
    {
        if(!_isInitLegal(coord))
        {
            std::cerr << "Please check your coordinate is legal\n";    
            return false;
        }
        for (std::vector<cell>::iterator it = cellList.begin(); it != cellList.end(); it++)
        {
            if (*it == coord)
            {
                it->setowner(own);
                it->setSheeps(16);
                return true;
            }
        }
    }
    void listPlayerSheepCand(const player &player)
    {
        for (std::vector<cell>::iterator it = cellList.begin(); it != cellList.end(); it++)
            if (it->getOwnerId() == player.getPlayerNum() && it->getSheeps() > 1)
                it->showCoordInfo();
    }
    void listMoveCandidates(coordinate coord)
    {
        coordinate vectors[6] = {{-1, 1}, {0, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, 0}};
        for (int i = 0; i < 6; i++)
        {
            _printDir(i);
            _findTargetCell(coord, vectors[i]);
        }
    }
    cell &getTargetCell(coordinate coord)
    {
        for (std::vector<cell>::iterator it = cellList.begin(); it != cellList.end(); it++)
            if (*it == coord)
                return *it;
        std::cerr << "No instence\n";
    }
    bool moveSheep(cell &from, cell &to, int num, player *own)
    {
        if (from.getSheeps() > num)
            from.setSheeps(from.getSheeps() - num);
        else
            return false;
        to.setSheeps(num);
        to.setowner(own);
        return true;
    }
    void showCellsCoord()
    {
        for (std::vector<cell>::iterator it = cellList.begin(); it != cellList.end(); it++)
            it->showCoordInfo();
    }
};

int main()
{
    world game;
    game.putGround(coordinate(0, 0), UP);
    game.listGroundCandidates();

    return 0;
}
