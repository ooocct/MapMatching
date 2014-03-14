#include <iostream>
#include<sstream>
#include <math.h>
#include "ReadInTrajs.h"
using namespace std;


string rootFilePath = "D:\\Document\\Subjects\\Computer\\Develop\\Data\\GISCUP2012_Data\\";
Map routeNetwork(rootFilePath, 500);
list<Traj*> trajList = list<Traj*>();


double CalculateMAD(list<double> &dist){
	dist.sort();
	int mid = dist.size() / 2;
	list<double>::iterator dist1Iter = dist.begin();
	for (int i = 0; i < mid; i++){
		dist1Iter++;
	}
	double median = -1;
	if (dist.size() % 2 != 0){
		median = *dist1Iter;
	}
	else
	{
		median = *dist1Iter;
		dist1Iter++;
		median += *dist1Iter;
		median /= 2;
	}
	for (dist1Iter = dist.begin(); dist1Iter != dist.end(); dist1Iter++){
		(*dist1Iter) = abs((*dist1Iter) - median);
	}
	dist.sort();
	dist1Iter = dist.begin();
	for (int i = 0; i < mid; i++){
		dist1Iter++;
	}
	if (dist.size() % 2 != 0){
		median = *dist1Iter;
	}
	else
	{
		median = *dist1Iter;
		dist1Iter++;
		median += *dist1Iter;
		median /= 2;
	}
	return median;
}

//����Viterbi�㷨�����õ���ת�Ƹ��ʺͷ�����ʵ���ز���
void CalculateParametersForViterbiAlgorithm(){
	long double sigma = 0, beta = 0;
	int indexOfTrajs = 0;
	for (list<Traj*>::iterator trajIter = trajList.begin(); trajIter != trajList.end(); trajIter++)
	{
		list<double> dist1 = list<double>();
		list<double> dist2 = list<double>();
		bool isStart = true;
		GeoPoint* formerTrajPoint = NULL;
		Edge* formerEdge = NULL;
		int indexOfTrajPoints = 0;
		for (list<GeoPoint*>::iterator trajPointIter = (*trajIter)->begin(); trajPointIter != (*trajIter)->end(); trajPointIter++){
			double shortestDist = 100000000.0;
			Edge* nearestEdge = routeNetwork.getNearestEdge((*trajPointIter)->lat, (*trajPointIter)->lon, shortestDist);
			if (nearestEdge != NULL){
				if (!isStart){
					list<Edge*> shortestPath;
					dist2.push_back(abs(GeoPoint::distM(formerTrajPoint, (*trajPointIter)) - routeNetwork.shortestPathLength(formerEdge->startNodeId, nearestEdge->startNodeId, shortestPath)));
				}
				dist1.push_back(shortestDist);
				formerTrajPoint = (*trajPointIter);
				formerEdge = nearestEdge;
				isStart = false;
			}
			indexOfTrajPoints++;
		}
		sigma += CalculateMAD(dist1);
		beta += CalculateMAD(dist2) / log(2);
		indexOfTrajs++;
		if (indexOfTrajs % 30 == 0){
			cout << "�����" << indexOfTrajs * 100 / trajList.size() << "%" << endl;
		}
	}
	sigma /= trajList.size();
	cout.precision(11);
	cout << "sigma = " << sigma << endl;
	beta /= trajList.size();
	cout << "beta = " << beta << endl;
	cout << "2*(sigma^2)/beta = " << 2 * sqrt(sigma) / beta << endl;
}

string ToString(int i){
	stringstream ss;
	ss << i;
	return ss.str();
}

//���һ���켣���������켣���ŷ�Ͼ������ʱ�����maxSpeed����λ����/�룩�����ڴ˴��ָ������켣���õ����µĹ켣�ļ������ںͳ�����ͬĿ¼��
void trajSplit(double maxSpeed){
	int trajIndex = 0;
	ofstream *fout;
	for (list<Traj*>::iterator trajIter = trajList.begin(); trajIter != trajList.end(); trajIter++){
		int goodTrajIndex = 0;
		fout = new ofstream("input_" + ToString(trajIndex) + "_" + ToString(goodTrajIndex) + ".txt");
		fout->precision(11);
		GeoPoint* formerTrajPoint = NULL;
		for each (GeoPoint* trajPoint in *(*trajIter))
		{
			if (formerTrajPoint != NULL && GeoPoint::distM(formerTrajPoint, trajPoint) / (trajPoint->time - formerTrajPoint->time) > maxSpeed){
				fout->close();
				delete fout;
				goodTrajIndex++;
				fout = new ofstream("input_" + ToString(trajIndex) + "_" + ToString(goodTrajIndex) + ".txt");
				cout << "input_" + ToString(trajIndex) + "_" + ToString(goodTrajIndex) + ".txt" << endl;
				fout->precision(11);
			}
			*fout << trajPoint->time << "," << trajPoint->lat << "," << trajPoint->lon << endl;
			formerTrajPoint = trajPoint;
		}
		trajIndex++;
		fout->close();
	}
	delete fout;
}

//����켣ƽ��������
void CalculateAverageSampleRate(){
	double totalAverageSampleRate = 0;
	for (list<Traj*>::iterator iter = trajList.begin(); iter != trajList.end(); iter++){
		double formerTrajPointTimeStamp = -1;
		double averageSampleRate = 0;
		for (Traj::iterator trajPointIter = (*iter)->begin(); trajPointIter != (*iter)->end(); trajPointIter++){
			if (formerTrajPointTimeStamp != -1){
				averageSampleRate += (*trajPointIter)->time - formerTrajPointTimeStamp;
			}
			formerTrajPointTimeStamp = (*trajPointIter)->time;
		}
		averageSampleRate /= ((*iter)->size() - 1);
		totalAverageSampleRate += averageSampleRate;
	}
	totalAverageSampleRate /= trajList.size();
	cout << "ƽ�������ʣ�" << totalAverageSampleRate << endl;
}

int main(){
	vector<string> outputFileNames;
	scanTrajFolder(rootFilePath, trajList, outputFileNames);
	cout << "�ļ�������ϣ�" << endl;
	CalculateParametersForViterbiAlgorithm();
	//CalculateAverageSampleRate();
	return 0;
}