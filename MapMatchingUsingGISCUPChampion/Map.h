#pragma once
#include <list>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <queue>
#include <set>
#include "GeoPoint.h"
using namespace std;

#define eps 1e-10
#define INF  1e7 //最短路径所用
#define MAXSPEED 50 //最大速度
#define min(a,b)	(((a) < (b)) ? (a) : (b))
#define max(a,b)	(((a) > (b)) ? (a) : (b))
typedef list<GeoPoint*> Figure; //代表一条路形，每个GeoPoint*代表路形点，首尾节点即路的两个端点
typedef std::pair<double, double> simplePoint; //内部类型，勿改动
typedef list<GeoPoint*> Traj;//轨迹类型

//最短路径长度所用数据结构
struct NODE_DIJKSTRA {
	int t; double dist;
	NODE_DIJKSTRA(int i, double dist){
		this->t = i;
		this->dist = dist;
	}
	bool operator < (const NODE_DIJKSTRA &rhs) const {
		return dist > rhs.dist;
	}
};

struct  Edge 
{
	Figure* figure;  //路形信息
	double lengthM;  //记录路段总长，单位为m
	int startNodeId; //路段起始点id
	int endNodeId;  //路段终止点id
	bool visited;  //辅助字段，外部调用勿改动
	int id;
};

struct AdjNode //邻接表结点
{
	int endPointId;
	int edgeId;
	AdjNode* next;
};

class Map
{
public:
	vector<Edge*> edges; //保存所有边的集合，如果边的两个端点至少一个不在范围内则为NULL，【逐个遍历需手动判断NULL】
	vector<GeoPoint*> nodes; //保存所有点的集合,如果点不在范围内则为NULL，【逐个遍历需手动判断NULL】
	vector<AdjNode*> adjList; //邻接表
	
	//默认构造函数,需要手动调用open()函数来初始化
	Map(); 
	//在folderDir路径下载入地图文件,并以gridWidth列的粒度来创建索引
	Map(string folderDir, int gridWidth);  

	//在folderDir路径下载入地图文件,并以gridWidth列的粒度来创建索引,适用于无参构造函数	
	void open(string folderDir, int gridWidth);
	//返回所有距离(lat, lon)点严格小于threshold米的所有Edge*
	vector<Edge*> getNearEdges(double lat, double lon, double threshold) const; 
	//找出所有距离(lat, lon)点严格小于threshold米的所有Edge*，保存在dest容器内
	void getNearEdges(double lat, double lon, double threshold, vector<Edge*>& dest); //推荐版本
	//返回离(lat, lon)点距离最近的k条路段，存入dest
	void getNearEdges(double lat, double lon, int k, vector<Edge*>& dest); 
	//返回距离(lat, lon)点最近的Edge*
	Edge* getNearestEdge(double lat, double lon, double &shortestDist);
	//找出距离(lat, lon)点最近的k条路段
	vector<Edge*> getKNearEdges(double lat, double lon, int k);
	//返回(lat,lon)点到edge的距离，单位为米
	double distM(double lat, double lon, Edge* edge) const; 
	//同上，同时记录投影点到edge起点的距离存入prjDist，无投影则记为0
	double distM(double lat, double lon, Edge* edge, double& prjDist) const;
	//判断startNodeId与endNodeId之间有无边,没有边返回-1，有边返回edgeId
	int hasEdge(int startNodeId, int endNodeId) const; 
	//插入一个新结点,返回新结点id
	int insertNode(double lat, double lon); 
	//在当前图中插入边,返回新边id
	int insertEdge(Figure* figure, int startNodeId, int endNodeId); 
	//将edge在(lat,lon)点处分开成两段,(lat,lon)作为新结点加入,返回新结点的nodeId
	int splitEdge(int edgeId, double lat, double lon); 
	void delEdge(int edgeId);
	void getMinMaxLatLon(string nodeFilePath);
	/*
	A路段起点到B路段起点的最小路网距离
	参数：
	ID1：A路段起点
	ID2：B路段起点
	dist1：基于隐马尔科夫模型地图匹配算法中轨迹点到B路段起点的距离，默认值为0
	dist2：基于隐马尔科夫模型地图匹配算法中轨迹点到A路段起点的距离，默认值为0
	deltaT：基于隐马尔科夫模型地图匹配算法中两轨迹点的时间差，默认为100000000
	*/
	double shortestPathLength(int ID1, int ID2, double dist1 = 0, double dist2 = 0, double deltaT = INF);

//private:
	int gridWidth, gridHeight;
	double gridSizeDeg;
	double strictThreshold = 0;
	list<Edge*>* **grid;
	/*singapore half
	double minLat = 1.22;
	double maxLat = 1.5;
	double minLon = 103.620;
	double maxLon = 104.0;*/
	
	//singapore full
	//double minLat = 0.99999;
	//double maxLat = 1.6265;
	//double minLon = 103.548;
	//double maxLon = 104.1155;

	//HongKong test
	//double minLat = 22.28;
	//double maxLat = 22.32;
	//double minLon = 114.135;
	//double maxLon = 114.20;
	//HongKong full
	double minLat = 22.14;
	double maxLat = 22.58;
	double minLon = 113.81;
	double maxLon = 114.43;

	//washington full
	//double minLat = 45.0;
	//double maxLat = 49.5;
	//double minLon = -125.0;
	//double maxLon = -116.5;

	int getRowId(double lat) const;
	int getColId(double lon) const;
	//给定行号row和列号row，找出相应单元格中离(lat,lon)点最近的路段，保存在currentResultEdge中，shortestDist保存相应的最短距离
	void getNearestEdgeInAGridCell(double lat, double lon, int row, int col, Edge*& currentResultEdge, double &shortestDist);
	//给定行号row和列号row，返回相应单元格中所有路段保存在resultEdges中，另外把相应的EdgeId保存在visitedEdgeIdSet中
	bool Map::getEdgesInAGridCell(double lat, double lon, int row, int col, vector<pair<Edge*, double>> &resultEdges, set<int> &visitedEdgeIdSet);
	//返回(lat,lon)点到edge的距离上界,提前预判优化版本
	double distM_withThres(double lat, double lon, Edge* edge, double threshold) const; 	
	double calEdgeLength(Figure* figure) const;
	bool inArea(double lat, double lon) const;
	bool inArea(int nodeId) const;
	void createGridIndex();
	void createGridIndexForEdge(Edge *edge);
	void createGridIndexForSegment(Edge *edge, GeoPoint* fromPT, GeoPoint* toPt);
	void insertEdgeIntoGrid(Edge* edge,int row, int col);
	void insertEdge(int edgeId, int startNodeId, int endNodeId);
	
	void split(const string& src, const string& separator, vector<string>& dest);
	void split(const string& src, const char& separator, vector<string>& dest);
	double cosAngle(GeoPoint* pt1, GeoPoint* pt2, GeoPoint* pt3) const;
	void test();
};

