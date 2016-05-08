#include <vector>

const int player_count = 2;

int find_max(int* begin, int* end)
{
	//static assert to remind to change this if player_count ever changes
	typedef int player_count_not_equal_to_2[player_count == 2 ? 1 : -1];
	if(*begin > *(begin+1))
		return 0;
	else
		return 1;
}

class Cell
{
private:
	short int color;
public:
	Cell(short int col, bool state)
	{
		if(state)
			color = col+1;
		else
			color = 0;
	}
	Cell() : color(0) {}
	bool is_alive() const { return color != 0; }
	short int get_color() const { return color-1; }
};

class CellArr
{
private:
	std::vector<Cell> plansza;
	size_t dimension;
	Cell dead_cell;

public:
	CellArr(size_t dim) : dimension(dim)
	{
		plansza.resize(dimension*dimension, Cell(0, false));
	}

	Cell& at(size_t x, size_t y)
	{
		if(x >= dimension or y >= dimension)
		{
			dead_cell = Cell(0, false);
			return dead_cell;
		}
		else
			return plansza.at(x*dimension+y);
	}

	size_t size() const
	{
		return dimension;
	}

	void swap(CellArr& s)
	{
		this->plansza.swap(s.plansza);
		std::swap(this->dimension, s.dimension);
		std::swap(this->dead_cell, s.dead_cell);
	}
};

class GameOfLife;
class CellWrapper;

class CellWrapper
{
private:
	GameOfLife& gra;
	Cell& kom;
public:
	CellWrapper(GameOfLife& g, Cell& k) : gra(g), kom(k) {}

	operator Cell&() { return kom; }
	CellWrapper& operator=(const Cell& k);
	bool is_alive() const { return kom.is_alive(); }
	short int get_color() const { return kom.get_color(); }
};

class GameOfLife
{
private:
	CellArr first, second;
	int ilosc_komorek[player_count+1];
public:
	CellWrapper at(size_t x, size_t y)
	{
		return CellWrapper(*this, first.at(x,y));
	}

	size_t size() const
	{
		return first.size();
	}
	
	void refresh()
	{
		for(int i = 0; i < player_count+1; i++)
			ilosc_komorek[i] = 0;
		for(size_t i = 0; i < size(); i++)
		{
			for(size_t j = 0; j < size(); j++)
			{
				int neighbours = 0;
				int colnum[player_count] = {0};
				#define HANDLE_CASE(a,b) do { Cell k = first.at((a),(b)); if(k.is_alive()) { colnum[k.get_color()]++; neighbours++; } } while(0)
				HANDLE_CASE(i-1,j-1);
				HANDLE_CASE(i  ,j-1);
				HANDLE_CASE(i+1,j-1);
				HANDLE_CASE(i-1,j  );
				HANDLE_CASE(i+1,j  );
				HANDLE_CASE(i-1,j+1);
				HANDLE_CASE(i  ,j+1);
				HANDLE_CASE(i+1,j+1);
				if(!first.at(i,j).is_alive() and neighbours == 3)
				{
					int k = find_max(colnum, colnum+player_count);
					second.at(i,j) = Cell(k, true);
				}
				else if(neighbours < 2 or neighbours > 3)
				{
					second.at(i,j) = Cell(0, false);
				}
				else
				{
					second.at(i,j) = first.at(i,j);
				}
				ilosc_komorek[second.at(i,j).get_color()+1]++;
			}
		}
		first.swap(second);
	}

	int ilosc_komorek_gracza(size_t nr_gracza)
	{
		return ilosc_komorek[nr_gracza+1];
	}

	GameOfLife(size_t dim) : first(dim), second(dim)
	{
		for(int i = 0; i < player_count; i++)
			ilosc_komorek[i] = 0;
	}

	void reset()
	{
		for(size_t i = 0; i < size(); i++)
			for(size_t j = 0; j < size(); j++)
				first.at(i,j) = Cell(0, false);
		for(int i = 0; i < player_count+1; i++)
			ilosc_komorek[i] = 0;


	}

	friend class CellWrapper;
};

CellWrapper& CellWrapper::operator=(const Cell& k)
{
	gra.ilosc_komorek[kom.get_color()+1]--;
	kom = k;
	gra.ilosc_komorek[kom.get_color()+1]++;
	return *this;
}