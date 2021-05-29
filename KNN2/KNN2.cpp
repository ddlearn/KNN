#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>
#include <cmath>
#include <map>
#include <set>
#include <cstdlib>
#include <ctime>

using namespace std;

struct Data
{
    vector<double> A; //向量
    string type; //标签
    double distance; //方便用于存距离
};

double E_distance(Data a, Data b);
double M_distance(Data a, Data b);
double cosine(Data a, Data b);
double distance(Data a, Data b, int flag);

void ReadFile(string file_name, vector<Data>& data, int pos)
{
    ifstream ifile(file_name);

    if (!ifile)
    {
        cout << "打开文件失败" << endl;
        return;
    }

    string line;
    while (getline(ifile, line)) 
    {
        //将字符串分割的逗号换成空格,就可以利用流快速输入
        for (int i = 0; i < line.size(); i ++ )
            if (line[i] == ',') 
            {
                line[i] = ' ';
            }

        int count = 1;
        istringstream ssin(line);
        Data temp;
        string s;
        while (ssin >> s)
        {
            if (count == pos) //到了标签type的位置
                temp.type = s;
            else 
                temp.A.push_back(stof(s));
            count ++;
        }

        data.push_back(temp);
    }

    ifile.close();
}

//计算欧氏距离
double E_distance(Data a, Data b)
{
    double dist = 0;
    for (int i = 0; i < a.A.size(); i ++ )
    {
        dist += (a.A[i] - b.A[i]) * (a.A[i] - b.A[i]);
    }
    return sqrt(dist);
}

//计算曼哈顿距离
double M_distance(Data a, Data b)
{
    double dist = 0;
    for (int i = 0; i < a.A.size(); i ++ )
    {
        dist += abs(a.A[i] - b.A[i]);
    }
    return dist;
}

//计算余弦值
double cosine(Data a, Data b)
{
    double moda = 0, modb = 0; //两向量模长
    double product = 0; //两向量点乘

    for (int i = 0; i < a.A.size(); i ++ )
    {
        moda += a.A[i] * a.A[i];
        modb += b.A[i] * b.A[i];
        product += a.A[i] * b.A[i];
    }

    moda = sqrt(moda);
    modb = sqrt(modb);

    return -product / (moda * modb); //余弦值是越大,相似度越高,因此添负号与其他距离统一
}

double distance(Data a, Data b, int flag)
{
    if (flag == 1) return E_distance(a, b);
    if (flag == 2) return M_distance(a, b);
    return cosine(a, b);
}

bool cmp(Data a, Data b)
{
    return a.distance < b.distance;
}

void find_k_neighbours(Data& test, vector<Data> data, int flag, int k)
{
    for (int i = 0; i < data.size(); i ++ )
    {
        Data temp = data[i];
        data[i].distance = distance(temp, test, flag);
    }

    sort(data.begin(), data.end(), cmp);
    
    int maxv = 0;
    string s = data[0].type; //存放出现次数最大的字符串
    map<string, int> cnt; //统计标签出现频率
    for (int i = 0; i < k; i ++ )
    {
        cnt[data[i].type] ++;
        if (maxv < cnt[data[i].type])
        {
            maxv = cnt[data[i].type];
            s = data[i].type;
        }
    }

    test.type = s;
}

void test(vector<Data> data, int flag, int k)
{
    //准备随机数
    set<int> randoms;
    while (randoms.size() < data.size() / 10) //取10%作为测试集,90%作为训练集
    {
        int r = rand() % data.size(); //在[0, data.size() - 1]产生随机数
        randoms.insert(r);
    }

    //准备测试集
    vector<Data> tests;
    vector<Data> train;

    for (int i = 0; i < data.size(); i ++ )
    {
        if (randoms.count(i)) tests.push_back(data[i]);
        else train.push_back(data[i]);
    }

    int right = 0, wrong = 0;

    for (int i = 0; i < tests.size(); i ++ )
    {
        Data test = tests[i];
        Data backup = test;
        test.type = "";

        find_k_neighbours(test, train, flag, k);

        if (test.type == backup.type) right ++;
        else wrong ++;
    }

    cout << "正确率为:";
    cout << 1.0 * right / (wrong + right) * 100 << "%" << endl;
}


/*
argc:参数个数(目前为5)(不用手传)
argv[0]:exe地址(可执行文件名)
argv[1]:数据集地址(可以相对路径)
    iris.data
    wine.txt
argv[2]:标签type所在位置
    鸢尾花:5(下标从1开始)
    红酒:1(下标从1开始)
argv[3]:k
argv[4]:选用比较距离的方法:
    1表示计算欧氏距离
    2表示计算曼哈顿距离
    3表示计算余弦值
*/
int main(int argc, char *argv[])
{
    srand((unsigned)time(NULL));

    string file_name = argv[1];
    int pos = stoi(argv[2]); //标签type所在位置
    int k = stoi(argv[3]);
    int flag = stoi(argv[4]);

    vector<Data> data;
    ReadFile(file_name, data, pos);

    test(data, flag, k);

    return 0;
}