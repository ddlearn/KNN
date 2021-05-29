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
    vector<double> A; //����
    string type; //��ǩ
    double distance; //�������ڴ����
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
        cout << "���ļ�ʧ��" << endl;
        return;
    }

    string line;
    while (getline(ifile, line)) 
    {
        //���ַ����ָ�Ķ��Ż��ɿո�,�Ϳ�����������������
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
            if (count == pos) //���˱�ǩtype��λ��
                temp.type = s;
            else 
                temp.A.push_back(stof(s));
            count ++;
        }

        data.push_back(temp);
    }

    ifile.close();
}

//����ŷ�Ͼ���
double E_distance(Data a, Data b)
{
    double dist = 0;
    for (int i = 0; i < a.A.size(); i ++ )
    {
        dist += (a.A[i] - b.A[i]) * (a.A[i] - b.A[i]);
    }
    return sqrt(dist);
}

//���������پ���
double M_distance(Data a, Data b)
{
    double dist = 0;
    for (int i = 0; i < a.A.size(); i ++ )
    {
        dist += abs(a.A[i] - b.A[i]);
    }
    return dist;
}

//��������ֵ
double cosine(Data a, Data b)
{
    double moda = 0, modb = 0; //������ģ��
    double product = 0; //���������

    for (int i = 0; i < a.A.size(); i ++ )
    {
        moda += a.A[i] * a.A[i];
        modb += b.A[i] * b.A[i];
        product += a.A[i] * b.A[i];
    }

    moda = sqrt(moda);
    modb = sqrt(modb);

    return -product / (moda * modb); //����ֵ��Խ��,���ƶ�Խ��,�����������������ͳһ
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
    string s = data[0].type; //��ų��ִ��������ַ���
    map<string, int> cnt; //ͳ�Ʊ�ǩ����Ƶ��
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
    //׼�������
    set<int> randoms;
    while (randoms.size() < data.size() / 10) //ȡ10%��Ϊ���Լ�,90%��Ϊѵ����
    {
        int r = rand() % data.size(); //��[0, data.size() - 1]���������
        randoms.insert(r);
    }

    //׼�����Լ�
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

    cout << "��ȷ��Ϊ:";
    cout << 1.0 * right / (wrong + right) * 100 << "%" << endl;
}


/*
argc:��������(ĿǰΪ5)(�����ִ�)
argv[0]:exe��ַ(��ִ���ļ���)
argv[1]:���ݼ���ַ(�������·��)
    iris.data
    wine.txt
argv[2]:��ǩtype����λ��
    �β��:5(�±��1��ʼ)
    ���:1(�±��1��ʼ)
argv[3]:k
argv[4]:ѡ�ñȽϾ���ķ���:
    1��ʾ����ŷ�Ͼ���
    2��ʾ���������پ���
    3��ʾ��������ֵ
*/
int main(int argc, char *argv[])
{
    srand((unsigned)time(NULL));

    string file_name = argv[1];
    int pos = stoi(argv[2]); //��ǩtype����λ��
    int k = stoi(argv[3]);
    int flag = stoi(argv[4]);

    vector<Data> data;
    ReadFile(file_name, data, pos);

    test(data, flag, k);

    return 0;
}