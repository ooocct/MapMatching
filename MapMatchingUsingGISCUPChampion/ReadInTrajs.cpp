#include "ReadInTrajs.h"


//�������·���Ĺ켣�ļ��е�һ���켣
Traj* readOneTrajectory(string &filePath)
{
	FILE *fpIn;
	fopen_s(&fpIn, filePath.c_str(), "r");
	double lat, lon;
	double db1, db2, db3, db4, db5, db6, db7, db8, db9, db10;
	double timeRaw;
	int time;
	Traj* traj = new Traj();
	while (true) {
		int flag = fscanf_s(fpIn, "%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf", &timeRaw, &lat, &lon, &db1, &db2, &db3, &db4, &db5, &db6, &db7, &db8, &db9, &db10);
		if (flag != 13) {
			// End of file reached or a reading error occurred
			break;
		}
		time = (int)timeRaw;
		GeoPoint* pt = new GeoPoint(lat, lon, time);
		traj->push_back(pt);
	}
	fclose(fpIn);
	return traj;
}

//�����ļ��������й켣�ļ�������켣�����ɶ�Ӧ������ļ���
void scanTrajFolder(string folderDir, list<Traj*> &trajList, vector<string> &outputFileNames)
{
	/*�ļ�Ŀ¼�ṹΪ
	* folderDir
	* |-input
	*   |-input_000011.txt ...
	* |-output
	*   |-output_000011.txt ...
	*/
	string completeInputFilesPath = folderDir + "test_input\\" + "*.txt";
	const char* dir = completeInputFilesPath.c_str();
	_finddata_t fileInfo;//�ļ���Ϣ
	long lf;//�ļ����
	if ((lf = _findfirst(dir, &fileInfo)) == -1l) {
		return;
	}
	else {
		do {
			string inputFileName = fileInfo.name;
			trajList.push_back(readOneTrajectory(folderDir + "test_input\\" + inputFileName));
			string outputFileName = inputFileName.substr(0, inputFileName.size() - 4);
			outputFileName = "output_" + outputFileName + ".txt";
			outputFileNames.push_back(outputFileName);
		} while (_findnext(lf, &fileInfo) == 0);
		_findclose(lf);
		return;
	}
}