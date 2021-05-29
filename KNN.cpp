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

void ReadFile(string file_name, vector<Data>& data)
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
        int count = 1; //��¼����ά��
        //���ַ����ָ�Ķ��Ż��ɿո�,�Ϳ�����������������
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

int main()
{
    srand((unsigned)time(NULL));

    vector<Data> data;
    ReadFile("d://iris.data", data);

    int flag;
    cout << "����1��ʾ����ŷ�Ͼ���" << endl;
    cout << "����2��ʾ���������پ���" << endl;
    cout << "����3��ʾ��������ֵ" << endl;
    cout << "������ѡ�õľ�����㷽��:";
    cin >> flag;

    int k;
    cout << "������kֵ:";
    cin >> k;

    test(data, flag, k);

    return 0;
}