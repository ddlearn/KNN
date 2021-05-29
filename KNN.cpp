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

void ReadFile(string file_name, vector<Data>& data)
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
        int count = 1; //记录向量维度
        //将字符串分割的逗号换成空格,就可以利用流快速输入
        for (int i = 0; i < line.size(); i ++ )
            if (line[i] == ',') 
            {
                line[i] = ' ';
                count ++;
            }

        istringstream ssin(line);
        Data temp;
        double number;
        while (count > 1 && ssin >> number)
        {
            temp.A.push_back(number);
            count --;
        }

        ssin >> temp.type;
        
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

int main()
{
    srand((unsigned)time(NULL));

    vector<Data> data;
    ReadFile("d://iris.data", data);

    int flag;
    cout << "输入1表示计算欧氏距离" << endl;
    cout << "输入2表示计算曼哈顿距离" << endl;
    cout << "输入3表示计算余弦值" << endl;
    cout << "请输入选用的距离计算方法:";
    cin >> flag;

    int k;
    cout << "请输入k值:";
    cin >> k;

    test(data, flag, k);

    return 0;
}