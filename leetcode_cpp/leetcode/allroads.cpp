/**************************************************************
1087. All Roads Lead to Rome (30)

时间限制
200 ms
内存限制
65536 kB
代码长度限制
16000 B
判题程序
Standard
作者
CHEN, Yue
Indeed there are many different tourist routes from our city to Rome. You are supposed to find your clients the route with the least cost while gaining the most happiness.

Input Specification:

Each input file contains one test case. For each case, the first line contains 2 positive integers N (2<=N<=200), the number of cities, and K, the total number of routes between pairs of cities; followed by the name of the starting city. The next N-1 lines each gives the name of a city and an integer that represents the happiness one can gain from that city, except the starting city. Then K lines follow, each describes a route between two cities in the format "City1 City2 Cost". Here the name of a city is a string of 3 capital English letters, and the destination is always ROM which represents Rome.

Output Specification:

For each test case, we are supposed to find the route with the least cost. If such a route is not unique, the one with the maximum happiness will be recommended. If such a route is still not unique, then we output the one with the maximum average happiness -- it is guaranteed by the judge that such a solution exists and is unique.

Hence in the first line of output, you must print 4 numbers: the number of different routes with the least cost, the cost, the happiness, and the average happiness (take the integer part only) of the recommended route. Then in the next line, you are supposed to print the route in the format "City1->City2->...->ROM".

Sample Input:
6 7 HZH
ROM 100
PKN 40
GDN 55
PRS 95
BLN 80
ROM GDN 1
BLN ROM 1
HZH PKN 1
PRS ROM 2
BLN HZH 2
PKN GDN 1
HZH PRS 1
Sample Output:
3 3 195 97
HZH->PRS->ROM
****************************************************************/
#include <iostream>
#include <map>
#include <string>
#define INF 1<<30
using namespace std;
int mat[210][210];
int dist[210];
bool mark[210];
//记录开心值
int happiness[210];
//记录总的开心值
int sumhap[210];
//记录平均开心值
int avg[210];
int rout[210];
//记录有多少条最短路径
int difr[210];
//记录路径长度
int ct[210];
string cities[210];
map<string, int> m;
int num = -1;
int n;
int ston(string s){
	if (m.find(s) != m.end()) return m[s];
	else{
		num++;
		m[s] = num;
		return num;

	}

}
void init(){
	for (int i = 0; i<n; i++){
		dist[i] = INF;
		mark[i] = false;
		happiness[i] = 0;
		sumhap[i] = 0;
		difr[i] = 0;
		avg[i] = 0;
		rout[i] = 0;
		ct[i] = 0;
		for (int j = 0; j<n; j++){
			mat[i][j] = INF;
		}
	}
}
void dijk(){

	for (int i = 0; i<n; i++){
		int p;
		//选出最短加入路径
		int min = INF;
		for (int j = 0; j<n; j++) if (mark[j] == false && dist[j]<min) min = dist[p = j];
		if (min == INF) return;
		mark[p] = true;
		for (int j = 0; j<n; j++){
			if (mark[j] == true) continue;
			if (dist[j]>dist[p] + mat[p][j]){
				dist[j] = dist[p] + mat[p][j];
				rout[j] = p;
				sumhap[j] = sumhap[p] + happiness[j];
				ct[j] = ct[p] + 1;
				avg[j] = sumhap[j] / ct[j];
				difr[j] = difr[p];
			}
			else if (dist[j] == dist[p] + mat[p][j]){
				difr[j] += difr[p];
				if (sumhap[j]<sumhap[p] + happiness[j]){
					rout[j] = p;
					sumhap[j] = sumhap[p] + happiness[j];
					ct[j] = ct[p] + 1;
					avg[j] = sumhap[j] / ct[j];
				}
				else if (sumhap[j] == sumhap[p] + happiness[j]){
					if (avg[j]<sumhap[j] / (ct[p] + 1)){
						rout[j] = p;
						ct[j] = ct[p] + 1;
						avg[j] = sumhap[j] / ct[j];

					}

				}
			}
		}
	}

}
void printfrout(int x){
	if (rout[x] != 0) printfrout(rout[x]);

	cout << cities[rout[x]] << "->";

}

int main(){
	int k;
	string scity;
	string city;
	int hp;
	string sa, sb;
	int ia, ib, cost;
	while (cin >> n >> k >> scity){
		init();
		num = ston(scity);
		cities[num] = scity;

		for (int i = 0; i<n - 1; i++){
			cin >> city >> hp;
			num = ston(city);
			happiness[num] = hp;
			cities[num] = city;
		}
		for (int i = 0; i<k; i++){
			cin >> sa >> sb >> cost;
			ia = ston(sa);
			ib = ston(sb);
			mat[ia][ib] = mat[ib][ia] = cost;
		}


		dist[0] = 0;
		difr[0] = 1;
		dijk();
		ib = ston("ROM");
		cout << difr[ib] << " " << dist[ib] << " " << sumhap[ib] << " " << avg[ib] << endl;
		printfrout(ib);
		cout << "ROM" << endl;
	}
}
/**************************************************************
author: ws
Language: C++
****************************************************************/