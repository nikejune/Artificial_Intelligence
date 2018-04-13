using namespace std;

#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <utility>
#include <string>
#include <cmath>


int ROW, COL;
int ** MATRIX;

int Go_Up(int &R, int &C);
int Go_Down(int &R, int &C);
int Go_Left(int &R, int &C);
int Go_Right(int &R, int &C);
int(*Go[4])(int&, int&) = { Go_Up, Go_Left, Go_Down, Go_Right };
void WHereIsStartPoint(int& R, int &C);
void WHereIsFinishPoint(vector<pair<int, int> >& finishPoint);
pair<int, int> IDS(int &R, int &C);

int ** visits;
pair<int, int> ** trace_MATRIX;

typedef struct __stackElement {

	int row;
	int col;
	int dir;
	int limit;

}stackElement;


stack<pair<int, int> > trace;

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
	string outputFileName = inputFileName.substr(0,pos);
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
	tempPair = IDS(Finish_R, Finish_C);

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

pair<int, int> IDS(int& R, int& C)
{
	int cur_R, cur_C, cur_dir, cur_limit;
	int tmp_R, tmp_C;
	int limit = 0;
	int time = 0;

	stack<stackElement> S;
	stackElement tmpStackElement;


	while (true)
	{
		limit++;
		cur_limit = limit;
	//	cout << limit << "'s trial" << endl;
		cur_R = R;
		cur_C = C;
		cur_dir = -1;

		//시작점 집어넣기
		for (int i = 0; i < 4; ++i) {
			tmp_R = cur_R;
			tmp_C = cur_C;
			if (Go[i](tmp_R, tmp_C)) {
				tmpStackElement = { tmp_R, tmp_C, i, cur_limit - 1 };
				S.push(tmpStackElement);
				trace_MATRIX[tmp_R][tmp_C] = make_pair(R, C);
			}
		}

		while (!S.empty()) {
			//pop 과정
			cur_R = S.top().row;
			cur_C = S.top().col;
			cur_dir = S.top().dir;
			cur_limit = S.top().limit;
			visits[cur_R][cur_C] = cur_limit;
			S.pop();

			//도착확인
			if (MATRIX[cur_R][cur_C] == 4)
			{
				R = cur_R;
				C = cur_C;
			//	cout << "find !! " << cur_R << "," << cur_C << endl;
				return make_pair(trace.size(), time);
			}

			//시도(time)증가
			time++;

			// 스텍에 가능한 이동 집어넣기
			if (cur_limit != 0) {
				for (int i = 0; i < 4; ++i) {
					if (cur_dir == (i + 2) % 4)
						continue;
					tmp_R = cur_R;
					tmp_C = cur_C;
					int gogo = Go[i](tmp_R, tmp_C);
					if (gogo == 1 || (gogo == -1 && cur_limit > visits[tmp_R][tmp_C])) {
						tmpStackElement = { tmp_R, tmp_C, i, cur_limit - 1 };
						S.push(tmpStackElement);
						trace_MATRIX[tmp_R][tmp_C] = make_pair(cur_R, cur_C);
					
					}
				}
			}
		}
	}
}
