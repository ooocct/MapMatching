#include <iostream>
#include <map>
#include "FileIO.h"
#include "Map.h"
#include "MapMatching.h"
#include "MapMatchingUsingBiasStatistic.h"
using namespace std;

string rootFilePath = "E:\\Documents\\Computer\\Data\\TrajData\\WashingtonState\\";
string inputDirectory = "input_120";//输入的轨迹文件名要求：以“input_”开头
string outputDirectory = "output_120";//输出的匹配结果文件名均以“output_”开头
string gridCellBiasFileName = "biasStatistic.txt";
Map routeNetwork = Map(rootFilePath, 1000);

vector<string> outputFileNames;//匹配结果文件名集合
list<Traj*> trajList;//轨迹集合
map<pair<int, int>, map<Edge*, int>> biasSet;

void biasStatistic(Traj* traj, list<Edge*> result){
	int trajPointIndex = 0;
	Traj::iterator trajIter = traj->begin();
	for each (Edge* edge in result)
	{
		GeoPoint* trajPoint = *trajIter;
		trajIter++;
		if (edge != NULL){
			pair<int, int> gridCellIndex = routeNetwork.findGridCellIndex(trajPoint->lat, trajPoint->lon);
			if (biasSet.find(gridCellIndex) == biasSet.end()){
				biasSet[gridCellIndex] = map<Edge*, int>();
			}
			if (biasSet[gridCellIndex].find(edge) == biasSet[gridCellIndex].end()){
				biasSet[gridCellIndex][edge] = 0;
			}
			biasSet[gridCellIndex][edge]++;
		}
		else{
			continue;
		}
	}
}

void main(){
	scanTrajFolder(rootFilePath, inputDirectory, trajList, outputFileNames);
	readGridCellBias(gridCellBiasFileName, biasSet,routeNetwork);
	int trajIndex = 0;
	cout << "开始地图匹配！" << endl;
	for (list<Traj*>::iterator trajIter = trajList.begin(); trajIter != trajList.end(); trajIter++){
		list<Edge*> resultList = MapMatchingUsingBiasStatisticAsPriorProb(*(*trajIter));
		outputMatchedEdges(rootFilePath + outputDirectory + "\\" + outputFileNames[trajIndex], *trajIter, resultList);
		//cout << "第" << trajIndex << "条轨迹匹配完毕！" << endl;
		//biasStatistic(*trajIter, resultList);
		trajIndex++;
	}
	cout << "地图匹配完成！" << endl;
	//outputGridCellBias(gridCellBiasFileName, biasSet);
	system("pause");
}