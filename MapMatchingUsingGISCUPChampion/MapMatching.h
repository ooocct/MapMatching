#pragma once
#include <iostream>
#include <fstream>
#include <direct.h>
#include <io.h>
#include <vector>
#include "GeoPoint.h"
#include "Map.h"
using namespace std;

//地图匹配所用参数by Wang Yin：
#define COEFFICIENT_FOR_EMISSIONPROB 140.2384599822997282786640971977//原始值为0.01402384599822997282786640971977，现扩大10000倍
#define COEFFICIENT_FOR_TRANSATIONPROB 93.1003342301998864175922391561//原始值为0.00931003342301998864175922391561，现扩大10000倍
//地图匹配所用参数by宋壬初
//#define SIGMAZ 4.591689
//#define N2_SIGMAZ2 -0.0237151062104234
//#define SQR_2PI_SIGMAZ 0.0868835586212075
//地图匹配通用参数
#define RANGEOFCANADIDATEEDGES 50.0 //候选路段选取范围
#define MINPROB 1e-150 //整体概率的下限

extern Map map;
extern list<Edge*> MapMatching(list<GeoPoint*> &trajectory, double canadidatesPickRange);
extern GeoPoint* GetProjection(GeoPoint* trajPoint, Edge* edge);
extern GeoPoint* GetLineSegmentProjection(GeoPoint* pt, GeoPoint* lineStart, GeoPoint* lineEnd);