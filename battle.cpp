#include <iostream>
#include <string>
#include <vector>

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

class player
{
private:
    unsigned short no;
    std::string name;

public:
    player(): no(0) {}
    player(int num, std::string name)
    {
        if (num > 4)
        {
            std::cerr << "player ID must less than 5\n";
	    no = -1;
        } else 
	    no = num;
        this->name = name;
    }
    void showPlayerInfo() const 
    {
        std::cout << "ID: " << no << " Name: " << name << std::endl;
    }
};

class coordinate
{
protected:
    int degX, degY;

public:
    coordinate() {}
    coordinate(int x, int y): degX(x), degY(y)
    {
    }
    coordinate(const coordinate &coor): degX(coor.degX), degY(coor.degY)
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
};

class cell : public coordinate
{
private:
    int ownerId; // -1 unoccupied
    int sheeps;

public:
    cell() {}
    cell(int x, int y)
    {
        ownerId = -1;
        sheeps = 0;
        this->setDegXaY(x, y);
    }
    bool setowner(int own)
    {
        if (own != -1 && (own < 1 || own > 4))
        {
            std::cerr << "The range of ownerId is 1~4 or -1\n";
            return false;
        }
        ownerId = own;
        return true;
    }
    void setSheeps(int num)
    {
        sheeps = num;
    }
    int getOwnerId()
    {
        return ownerId;
    }
    int getSheeps()
    {
        return sheeps;
    }
    void showOwner()
    {
        std::cout << "OwnerID: " << ownerId << std::endl;
    }
    void showSheeps()
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
    groundToken(int x, int y, int dir)
    {
        coord[0].setDegXaY(x, y);
        if (dir == UP)
        {
            coord[1].setDegXaY(x - 1, y);
            coord[2].setDegXaY(x - 1, y + 1);
            coord[3].setDegXaY(x, y + 1);
        }
        else if (dir == RIGHT)
        {
            coord[1].setDegXaY(x, y + 1);
            coord[2].setDegXaY(x + 1, y + 1);
            coord[3].setDegXaY(x + 1, y);
        }
        else if (dir == DOWN)
        {
            coord[1].setDegXaY(x + 1, y);
            coord[2].setDegXaY(x + 1, y - 1);
            coord[3].setDegXaY(x, y - 1);
        }
        else
        {
            coord[1].setDegXaY(x, y - 1);
            coord[2].setDegXaY(x - 1, y - 1);
            coord[3].setDegXaY(x - 1, y);
        }
    }
    bool isPartOfToken(int x, int y)
    {
        for (int i = 0; i < 4; i++)
            if (coord[i].getDegX() == x && coord[i].getDegY() == y)
                return true;
        return false;
    }
};

class world
{
private:
    void _pushToGroundCandList(int x, int y, groundToken &puzzle)
    {
        int candsCoord[6][2] = {{x - 1, y + 1}, {x, y + 1}, {x + 1, y}, {x + 1, y - 1}, {x, y - 1}, {x - 1, y}}; // six coordinates beside center hexagon
        for (int i = 0; i < 6; i++)
        {
            int flag = 0;
            for (std::vector<coordinate>::iterator it = _groundCandList.begin(); it != _groundCandList.end(); it++)
            {
                if (puzzle.isPartOfToken(candsCoord[i][0], candsCoord[i][1]) || (candsCoord[i][0] == it->getDegX() && candsCoord[i][1] == it->getDegY()))
                {
                    flag = 1;
                    break;
                }
            }
            if (!flag)
                _groundCandList.push_back(coordinate(candsCoord[i][0], candsCoord[i][1]));
        }
    }
    void _moveFromGroundCandList(int x, int y)
    {
        for (std::vector<coordinate>::iterator it = _groundCandList.begin(); it != _groundCandList.end(); it++)
        {
            if (x == it->getDegX() && y == it->getDegY())
            {
                _groundCandList.erase(it);
                return;
            }
        }
    }
    bool _isTokenLegal(int x, int y, int dir)
    {
        groundToken puzzle(x, y, dir);
        for (int i = 0; i < 4; i++)
        {
            for (std::vector<cell>::iterator it = cellList.begin(); it != cellList.end(); it++)
                if (it->getDegX() == puzzle.coord[i].getDegX() && it->getDegY() == puzzle.coord[i].getDegY())
                    return false;
        }
        return true;
    }
    void _findTargetCell(int x, int y, int vecX, int vecY)
    {
        int flag = 1, tmpX = x, tmpY = y;
        while (1)
        {
            for (std::vector<cell>::iterator it = cellList.begin(); it != cellList.end(); it++)
            {
                if (it->getOwnerId() == -1 && it->getDegX() == tmpX + vecX && it->getDegY() == tmpY + vecY)
                {
                    flag = 0;
                    break;
                }
            }
            if (flag)
                break;
            tmpX += vecX;
            tmpY += vecY;
        }
        if (x == tmpX && y == tmpY)
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
    std::vector<coordinate> _groundCandList;
    std::vector<cell> cellList;

public:
    world()
    {
        putGround(0, 0, DOWN);
    }
    // groundToken operation
    bool putGround(int x, int y, int dir)
    {
        groundToken puzzle(x, y, dir);
        for (int i = 0; i < 4; i++)
        {
            cellList.push_back(cell(puzzle.coord[i].getDegX(), puzzle.coord[i].getDegY()));
            _moveFromGroundCandList(puzzle.coord[i].getDegX(), puzzle.coord[i].getDegY());
            _pushToGroundCandList(puzzle.coord[i].getDegX(), puzzle.coord[i].getDegY(), puzzle);
        }
    }
    void listGroundCandidates()
    {
        for (std::vector<coordinate>::iterator it = _groundCandList.begin(); it != _groundCandList.end(); it++)
            it->showCoordInfo();
    }
    void getGroundTokenMethod(int x, int y)
    {
        for (int i = 0; i < 4; i++)
        {
            if (_isTokenLegal(x, y, i))
            {
                if (i == UP)
                    std::cout << "UP\n";
                else if (i == RIGHT)
                    std::cout << "RIGHT\n";
                else if (i == DOWN)
                    std::cout << "DOWN\n";
                else
                    std::cout << "LEFT\n";
            }
        }
    }
    // sheep init & operation
    bool initCellSheep(int x, int y, int owner)
    {
        for (std::vector<cell>::iterator it = cellList.begin(); it != cellList.end(); it++)
        {
            if (it->getDegX() == x && it->getDegY() == y)
            {
                it->setowner(owner);
                it->setSheeps(16);
                return true;
            }
        }
        std::cerr << "Can't find the cell\n";
        return false;
    }
    void listPlayerSheepCand(int player)
    {
        for (std::vector<cell>::iterator it = cellList.begin(); it != cellList.end(); it++)
            if (it->getOwnerId() == player && it->getSheeps() > 1)
                it->showCoordInfo();
    }
    void listMoveCandidates(int x, int y)
    {
        int vec[6][2] = {{-1, 1}, {0, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, 0}};
        for (int i = 0; i < 6; i++)
        {
            _printDir(i);
            _findTargetCell(x, y, vec[i][0], vec[i][1]);
        }
    }
    cell &getTargetCell(int x, int y)
    {
        for (std::vector<cell>::iterator it = cellList.begin(); it != cellList.end(); it++)
        {
            if (it->getDegX() == x && it->getDegY() == y)
                return *it;
        }
        std::cerr << "No instence\n";
        return;
    }
    bool moveSheep(cell &from, cell &to, int num, int player)
    {
        if (from.getSheeps() > num)
            from.setSheeps(from.getSheeps() - num);
        else
            return false;
        to.setSheeps(num);
        to.setowner(player);
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

    return 0;
}
