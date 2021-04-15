#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <stack>
#include <chrono>
#include <algorithm>
#include <ctime>
#include <chrono>
using namespace std;
using namespace std::chrono;

vector<vector<char>> build_input(int &boxes)
{
	ifstream input;
	input.open("input_1.txt");

	int m, n; //Dimensions of the game matrix
	input >> m; //rows
	input >> n; //columns

	//creating m vectors of size n
	vector <vector<char>> matrix(m, vector <char> (n));

	for (int i = 0; i < m; i++)
		for (int j = 0; j < n; j++)
        {
			input >> matrix[i][j];
			if (matrix[i][j] == '@')
				boxes++;
		}
	input.close();
	return matrix;
}
void print_state(vector<vector<char>> state)
{
	for (int i = 0; i < state.size(); i++)
	{
		for (int j = 0; j < state[i].size(); j++)
			cout << state[i][j];
		cout << endl;
	}
}
void output_goal_state(vector<vector<char>> state)
{
	ofstream output;
	output.open("result.txt");

	for (int i = 0; i < state.size(); i++)
	{
		for (int j = 0; j < state[i].size(); j++)
			output << state[i][j];
		output << "\n";
	}
}

bool is_goal(vector<vector<char>> &state)
{
    for (int i = 0; i < state.size(); i++)
    {
		for (int j = 0; j < state[i].size(); j++)
		{
			// S->@->X
			if (state[i][j] == 'S')
				if (state[i][j + 1] == '@')
					if (state[i][j + 2] == 'X')
                    {
						state[i][j] = '.';
						state[i][j + 1] = 'S';
						state[i][j + 2] = 'F';
						//print_state(state);
						return true;
					}
			// X<-@<-S
			if (state[i][j] == 'X')
				if (state[i][j + 1] == '@')
					if (state[i][j + 2] == 'S')
                    {
						state[i][j] = 'F';
						state[i][j + 1] = 'S';
						state[i][j + 2] = '.';
						//print_state(state);
						return true;
					}
			//top-down
			if (state[i][j] == 'S')
				if (state[i + 1][j] == '@')
					if (state[i + 2][j] == 'X')
                    {
						state[i][j] = '.';
						state[i + 1][j] = 'S';
						state[i + 2][j] = 'F';
						//print_state(state);
						return true;
					}
			//bottom-up
			if (state[i][j] == 'X')
				if (state[i + 1][j] == '@')
					if (state[i + 2][j] == 'S')
                    {
						state[i][j] = 'F';
						state[i + 1][j] = 'S';
						state[i + 2][j] = '.';
						//print_state(state);
						return true;
					}
		}
	}
	return false;
}

vector<vector<char>> walk(int S_index_row, int S_index_col, int p_index_row, int p_index_col, vector<vector<char>> state)
{
	state[S_index_row][S_index_col] = '.';
	state[p_index_row][p_index_col] = 'S';
	return state;
}
vector<vector<char>> push(int S_index_row, int S_index_col, int box_index_row, int box_index_col, vector<vector<char>> state)
{
	//push right
	if (S_index_col < box_index_col)
	{
		state[S_index_row][S_index_col] = '.';
		state[box_index_row][box_index_col] = 'S';
		state[box_index_row][box_index_col + 1] = '@';
	}

	//push left
	if (S_index_col > box_index_col)
	{
		state[S_index_row][S_index_col] = '.';
		state[box_index_row][box_index_col] = 'S';
		state[box_index_row][box_index_col - 1] = '@';
	}

	//push up
	if (S_index_row > box_index_row)
	{
		state[S_index_row][S_index_col] = '.';
		state[box_index_row][box_index_col] = 'S';
		state[box_index_row - 1][box_index_col] = '@';
	}

	//push down
	if (S_index_row < box_index_row)
	{
		state[S_index_row][S_index_col] = '.';
		state[box_index_row][box_index_col] = 'S';
		state[box_index_row + 1][box_index_col] = '@';
	}
	return state;
}

//BFS
bool visited(vector<vector<char>> state, vector<vector<vector<char>>>& list, int& bytes)
{
	for (int i = 0; i < list.size(); i++)
    {
		if (list[i] == state)
			return true;
    }
	list.push_back(state);
	bytes += sizeof(state);
	return false;
}
void BFS_successor(vector<vector<char>> state, queue<vector<vector<char>>> &state_q, int& count_push, int& count_move, int &c_succ, int& bytes)
{
	int x = 0, y = 0;
	for (int i = 0; i < state.size(); i++)
		for (int j = 0; j < state[i].size(); j++)
			if (state[i][j] == 'S')
			{
				x = i;
				y = j;
			}

	if (state[x][y - 1] == '.')
    {
		state_q.push(walk(x, y, x, y - 1, state));
		count_move++;
		c_succ++;
		bytes += sizeof(state);
    }
	if (state[x][y + 1] == '.')
    {
		state_q.push(walk(x, y, x, y + 1, state));
		count_move++;
		c_succ++;
		bytes += sizeof(state);
    }
	if (state[x - 1][y] == '.')
    {
		state_q.push(walk(x, y, x - 1, y, state));
		count_move++;
		c_succ++;
		bytes += sizeof(state);
    }
	if (state[x + 1][y] == '.')
    {
		state_q.push(walk(x, y, x + 1, y, state));
		count_move++;
		c_succ++;
		bytes += sizeof(state);
    }

	if (state[x][y - 1] == '@')
    {
		if (state[x][y - 2] == '.')
        {
			state_q.push(push(x, y, x, y - 1, state));
			count_push++;
			count_move++;
			c_succ++;
			bytes += sizeof(state);
        }
        else if (state[x][y - 2] == 'X')
        {
			state_q.push(push(x, y, x, y - 1, state));
			count_push++;
			count_move++;
			c_succ++;
			bytes += sizeof(state);
        }
    }
	if (state[x][y + 1] == '@')
    {
		if (state[x][y + 2] == '.')
        {
			state_q.push(push(x, y, x, y + 1, state));
			count_push++;
			count_move++;
			c_succ++;
			bytes += sizeof(state);
        }
		else if (state[x][y + 2] == 'X')
        {
			state_q.push(push(x, y, x, y + 1, state));
			count_push++;
			count_move++;
			c_succ++;
			bytes += sizeof(state);
        }
    }
	if (state[x - 1][y] == '@')
    {
		if (state[x - 2][y] == '.')
        {
			state_q.push(push(x, y, x - 1, y, state));
			count_push++;
			count_move++;
			c_succ++;
			bytes += sizeof(state);
        }
		else if (state[x - 2][y] == 'X')
        {
			state_q.push(push(x, y, x - 1, y, state));
			count_push++;
			count_move++;
			c_succ++;
			bytes += sizeof(state);
        }
    }
	if (state[x + 1][y] == '@')
    {
		if (state[x + 2][y] == '.')
        {
			state_q.push(push(x, y, x + 1, y, state));
			count_push++;
			count_move++;
			c_succ++;
			bytes += sizeof(state);
        }
		else if (state[x + 2][y] == 'X')
        {
			state_q.push(push(x, y, x + 1, y, state));
			count_push++;
			count_move++;
			c_succ++;
			bytes += sizeof(state);
        }
    }
}
vector<vector<char>> BFS(vector<vector<char>> &current_state, vector<vector<vector<char>>> &visited_list, int all_boxes, int &finished_boxes, int &c_push, int &c_succ, int &c_move, int& bytes)
{
	queue<vector<vector<char>>> state_q;
	BFS_successor(current_state, state_q,c_push,c_move,c_succ,bytes);

	while (!state_q.empty())
    {
		if (is_goal(current_state))
		{
			finished_boxes++;
			current_state = BFS(current_state, visited_list, all_boxes, finished_boxes, c_push, c_succ, c_move, bytes);
		}
		if (finished_boxes == all_boxes)
        {
			return current_state;
		}
		current_state = state_q.front();
		state_q.pop();
		if (!visited(current_state, visited_list,bytes))
			BFS_successor(current_state, state_q,c_push,c_move,c_succ,bytes);
	}
}

//DFS
void DFS_successor(vector<vector<char>> state, stack<vector<vector<char>>>& state_s, int& count_push, int& count_move, int& c_succ, int& bytes)
{
	int x = 0, y = 0;
	for (int i = 0; i < state.size(); i++)
		for (int j = 0; j < state[i].size(); j++)
			if (state[i][j] == 'S')
			{
				x = i;
				y = j;
			}

	if (state[x][y - 1] == '.')
	{
		state_s.push(walk(x, y, x, y - 1, state));
		count_move++;
		c_succ++;
		bytes += sizeof(state);
	}
	if (state[x][y + 1] == '.')
	{
		state_s.push(walk(x, y, x, y + 1, state));
		count_move++;
		c_succ++;
		bytes += sizeof(state);
	}
	if (state[x - 1][y] == '.')
	{
		state_s.push(walk(x, y, x - 1, y, state));
		count_move++;
		c_succ++;
		bytes += sizeof(state);
	}
	if (state[x + 1][y] == '.')
	{
		state_s.push(walk(x, y, x + 1, y, state));
		count_move++;
		c_succ++;
		bytes += sizeof(state);
	}
	if (state[x][y - 1] == '@')
	{
		if (state[x][y - 2] == 'X')
		{
			state_s.push(push(x, y, x, y - 1, state));
			count_push++;
			count_move++;
			c_succ++;
			bytes += sizeof(state);
		}
		else if (state[x][y - 2] == '.')
		{
			state_s.push(push(x, y, x, y - 1, state));
			count_push++;
			count_move++;
			c_succ++;
			bytes += sizeof(state);
		}
	}
	if (state[x][y + 1] == '@')
	{
		if (state[x][y + 2] == 'X')
		{
			state_s.push(push(x, y, x, y + 1, state));
			count_push++;
			count_move++;
			c_succ++;
			bytes += sizeof(state);
		}
		else if (state[x][y + 2] == '.')
		{
			state_s.push(push(x, y, x, y + 1, state));
			count_push++;
			count_move++;
			c_succ++;
			bytes += sizeof(state);
		}
	}
	if (state[x - 1][y] == '@')
	{
		if (state[x - 2][y] == 'X')
		{
			state_s.push(push(x, y, x - 1, y, state));
			count_push++;
			count_move++;
			c_succ++;
			bytes += sizeof(state);
		}
		else if (state[x - 2][y] == '.')
		{
			state_s.push(push(x, y, x - 1, y, state));
			count_push++;
			count_move++;
			c_succ++;
			bytes += sizeof(state);
		}
	}
	if (state[x + 1][y] == '@')
	{
		if (state[x + 2][y] == 'X')
		{
			state_s.push(push(x, y, x + 1, y, state));
			count_push++;
			count_move++;
			c_succ++;
			bytes += sizeof(state);
		}
		else if (state[x + 2][y] == '.')
		{
			state_s.push(push(x, y, x + 1, y, state));
			count_push++;
			count_move++;
			c_succ++;
			bytes += sizeof(state);
		}
	}
}
vector<vector<char>> DFS(vector<vector<char>> &current_state, vector<vector<vector<char>>> &visited_list, int &all_boxes, int &finished_boxes,int& c_push,int& c_succ,int& c_move,int& bytes)
{
	stack<vector<vector<char>>> state_stack;
	DFS_successor(current_state, state_stack,c_push,c_move,c_succ,bytes);

	while (!state_stack.empty())
    {
		if (is_goal(current_state))
		{
			finished_boxes++;
			visited_list.push_back(current_state);
			current_state = DFS(current_state, visited_list, all_boxes, finished_boxes, c_push, c_succ, c_move, bytes);
		}
		if (finished_boxes == all_boxes)
		{
			return current_state;
		}
		current_state = state_stack.top();
		state_stack.pop();
		if (!visited(current_state, visited_list,bytes))
			DFS_successor(current_state, state_stack,c_push,c_move,c_succ,bytes);
	}
	cout << "goal not found" << endl;
	return current_state;
}


int main(void)
{
    ofstream output;
    output.open("result.txt");

    int count_push = 0;
    int count_succ = 0;
    int count_move = 0;
    int bytes = 0;

	int all_boxes = 0;
	int finished_boxes = 0;

	vector<vector<char>> init_state = build_input(all_boxes);
	vector<vector<vector<char>>> visited_list;

	cout << "input: " << endl << endl;
	print_state(init_state);
	cout << endl;
	cout << "running..." << endl << endl;

    //bfs
    auto search_start_bfs = high_resolution_clock::now();
    vector<vector<char>> goal_state = BFS(init_state, visited_list, all_boxes, finished_boxes,count_push,count_succ,count_move,bytes);
    auto search_end_bfs= high_resolution_clock::now();
    auto duration_bfs = duration_cast<milliseconds>(search_end_bfs - search_start_bfs);
    output_goal_state(goal_state);
    cout << "\nTime taken by BFS: " <<  duration_bfs.count() << " milliseconds" << endl;

    //dfs
    auto search_start_dfs = high_resolution_clock::now();
    vector<vector<char>> goal_state = DFS(init_state, visited_list, all_boxes, finished_boxes,count_push,count_succ,count_move,bytes);
    auto search_end_dfs= high_resolution_clock::now();
    auto duration_dfs = duration_cast<milliseconds>(search_end_dfs - search_start_dfs);
    output_goal_state(goal_state);
    cout << "\nTime taken by DFS: " <<  duration_dfs.count() << " milliseconds" << endl;

    cout << "Number of Pushes: " << count_push << endl;
    cout << "Number of Successors: " << count_succ << endl;
    cout << "Occupied memory: " << bytes << " Bytes" << endl;
    cout << "Number of Moves: " << count_move << endl;

    return 0;
}
