
using namespace std;

#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <queue>
#include <cmath>
#include <string>

int ROW, COL;
int ** MATRIX;
int Go_Up(int &R, int &C);
int Go_Down(int &R, int &C);
int Go_Left(int &R, int &C);
int Go_Right(int &R, int &C);
int(*Go[4])(int&, int&) = { Go_Up, Go_Left, Go_Down, Go_Right };
void WHereIsStartPoint(int& R, int &C);
void WHereIsFinishPoint(vector<pair<int, int> >& finishPoint);

pair<int, int> GBS(int &R, int &C);

int ** HU_MATRIX;
int ** visits;
pair<int, int> ** trace_MATRIX;

int score[5] = { 0, 1000, 10, 1000, 0 };

typedef struct __pqElement {

	int row;
	int col;
	int ex_row;
	int ex_col;
	int dir;
	int distance;
	int hu;
	bool operator<(const __pqElement & rhs) const
	{
		return  hu> rhs.hu;
	}

}pqElement;

int main(int argc, const char * argv[]) {

	int Start_R, Start_C;
	int Finish_R, Finish_C;
	int tmp_R, tmp_C;
	int tmptmp_R, tmptmp_C;
	int len = 0;
	pair<int, int> tempPair;
	vector<pair<int, int> > finishPoints;
	// 파일 입력
	if (argc != 2) {
		cout << "인자를 1개 입력해주세요" << endl;
		return -1;
	}
	string inputFileName = argv[1];
	ifstream in(inputFileName);
	size_t pos = inputFileName.find("input.txt");
	string outputFileName = inputFileName.substr(0, pos);
	outputFileName += "output.txt";
	ofstream off(outputFileName);

	if (in.is_open()) {
		in >> ROW;
		in >> COL;
		MATRIX = new int*[ROW];
		for (int i = 0; i < ROW; ++i)
			MATRIX[i] = new int[COL];

		while (!in.eof()) {
			for (int i = 0; i < ROW; ++i) {
				for (int j = 0; j < COL; ++j) {
					in >> MATRIX[i][j];
				}
			}
		}


		//시작점 구하기
		WHereIsStartPoint(Start_R, Start_C);
		WHereIsFinishPoint(finishPoints);


		HU_MATRIX = new int*[ROW];
		for (int i = 0; i < ROW; ++i)
			HU_MATRIX[i] = new int[COL];

		for (int i = 0; i < ROW; ++i)
			for (int j = 0; j < COL; ++j)
			{
				if (MATRIX[i][j] == 1)
				{
					HU_MATRIX[i][j] = 100000;
				}
				else if (MATRIX[i][j] == 2)
				{
					int min = 100000;

					int count = 1;
					for (vector<pair<int, int> >::iterator iter = finishPoints.begin(); iter != finishPoints.end(); ++iter)
					{
		
						int temp = (abs(i - iter->first) + abs(j - iter->second));
						if (min > temp) {
							min = temp;
						}
					}
					HU_MATRIX[i][j] = min;
	


				}
				else if (MATRIX[i][j] == 3)
				{
					HU_MATRIX[i][j] = 100000;
				}
				else if (MATRIX[i][j] == 4)
				{
					HU_MATRIX[i][j] = 0;
				}

			}

		trace_MATRIX = new pair<int, int>*[ROW];
		for (int i = 0; i < ROW; ++i) {
			trace_MATRIX[i] = new pair<int, int>[COL];
			for (int j = 0; j< COL; ++j) {
				trace_MATRIX[i][j] = make_pair(0, 0);
			}
		}

		visits = new int*[ROW];
		for (int i = 0; i < ROW; ++i) {
			visits[i] = new int[COL];
			for (int j = 0; j< COL; ++j) {
				visits[i][j] = 0;
			}
		}
	}
	else {
		cout << "파일을 찾을 수 없습니다!" << endl;
		return -1;
	}
	in.close();

	Finish_R = Start_R;
	Finish_C = Start_C;

	//미로찾기
	tempPair = GBS(Finish_R, Finish_C);
	
	tmp_R = trace_MATRIX[Finish_R][Finish_C].first;
	tmp_C = trace_MATRIX[Finish_R][Finish_C].second;

	while (tmp_R != Start_R || tmp_C != Start_C)
	{
		MATRIX[tmp_R][tmp_C] = 5;
		len++;
		tmptmp_R = trace_MATRIX[tmp_R][tmp_C].first;
		tmptmp_C = trace_MATRIX[tmp_R][tmp_C].second;
		tmp_R = tmptmp_R;
		tmp_C = tmptmp_C;

	}

	//output 출력
	for (int i = 0; i < ROW; ++i) {
		for (int j = 0; j < COL; ++j) {
			off << MATRIX[i][j] << " ";
		}
		off << endl;
	}
	off << "---" << endl;
	off << "length : " << len << endl;
	off << "time : " << tempPair.second << endl;
	off.close();

	cout << len << " " << tempPair.second << endl;
	return 0;
}

void WHereIsStartPoint(int& R, int &C)
{
	pair<int, int> ret;
	for (int i = 0; i < ROW; ++i)
		for (int j = 0; j < COL; ++j)
			if (MATRIX[i][j] == 3)
			{
				R = i;
				C = j;
				break;
			}
}


void WHereIsFinishPoint(vector<pair<int, int> >& finishPoint)
{
	pair<int, int> tmp_pair;
	for (int i = 0; i < ROW; ++i)
		for (int j = 0; j < COL; ++j)
			if (MATRIX[i][j] == 4)
			{
				tmp_pair.first = i;
				tmp_pair.second = j;
				finishPoint.push_back(tmp_pair);
			}
}

int Go_Up(int &R, int &C)
{
	if (R - 1 < 0)
		return 0;
	R--;
	if (visits[R][C])
		return -1;

	if (MATRIX[R][C] != 2 && MATRIX[R][C] != 4)
		return 0;
	else
		return 1;
}
int Go_Down(int &R, int &C)
{
	if (R + 1 == ROW)
		return 0;
	R++;
	if (visits[R][C])
		return -1;

	if (MATRIX[R][C] != 2 && MATRIX[R][C] != 4)
		return 0;
	else
		return 1;
}
int Go_Left(int &R, int &C)
{
	if (C - 1 < 0)
		return 0;
	C--;

	if (visits[R][C])
		return -1;

	if (MATRIX[R][C] != 2 && MATRIX[R][C] != 4)
		return 0;
	else
		return 1;
}
int Go_Right(int &R, int &C)
{
	if (C + 1 == COL)
		return 0;
	C++;

	if (visits[R][C])
		return -1;

	if (MATRIX[R][C] != 2 && MATRIX[R][C] != 4)
		return 0;
	else
		return 1;
}

pair<int, int> GBS(int &R, int &C)
{
	int cur_R, cur_C, cur_dir, cur_walk;
	int tmp_R, tmp_C;

	int time = 0;
	int walk = 100000;

	priority_queue<pqElement> PQ;
	pqElement tmp_pqElement;

	for (int i = 0; i < 4; ++i) {
		tmp_R = R;
		tmp_C = C;
		if (Go[i](tmp_R, tmp_C) == 1) {

			tmp_pqElement = { tmp_R, tmp_C, R,C,i, walk, HU_MATRIX[tmp_R][tmp_C]};
			PQ.push(tmp_pqElement);
		}
	}

	while (!PQ.empty()) {

		//pop 과정
		cur_R = PQ.top().row;
		cur_C = PQ.top().col;
		tmp_R = PQ.top().ex_row;
		tmp_C = PQ.top().ex_col;
		cur_dir = PQ.top().dir;
		cur_walk = PQ.top().distance;
	
		PQ.pop();

		if (visits[cur_R][cur_C]  < cur_walk)
			visits[cur_R][cur_C] = cur_walk;
		else
			continue;
		trace_MATRIX[cur_R][cur_C] = make_pair(tmp_R, tmp_C);

			//도착확인
		if (MATRIX[cur_R][cur_C] == 4)
		{
			// 끝점 반환
			R = cur_R;
			C = cur_C;
			return make_pair(0, time);
		}

		//시도(time)증가
		time++;

		// 스텍에 가능한 이동 집어넣기
		for (int i = 0; i < 4; ++i) {
			if (cur_dir == (i + 2) % 4)
				continue;
			tmp_R = cur_R;
			tmp_C = cur_C;
			int gogo = Go[i](tmp_R, tmp_C);
			//  if(Go[i](tmp_R,tmp_C) == 1){
			if (gogo == 1) {
				tmp_pqElement = { tmp_R, tmp_C, cur_R,cur_C,i, cur_walk - 1, HU_MATRIX[tmp_R][tmp_C] };
				PQ.push(tmp_pqElement);

			}
		}
	}
	return make_pair(0, 0);
}

