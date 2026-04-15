#include <iostream>
#include <fstream>
#include <cerrno>
using namespace std;

#define BIT_LEN 2048
#define PEOPLE_NUM 300

//ハミング距離を計算する関数
int calc_hamming_distance(char *s1, char *s2)
{
    int dist = 0;

    for(int i = 0; i < BIT_LEN; i++)
    {
        if(s1[i] != s2[i])
            dist++;
    }
    return(dist);
}

int main(int argc, char *argv[])
{
    //引数エラー処理
    if(argc != 3)
    {
        cerr << "引数が2つではありません\n";
        exit(EXIT_FAILURE);
    }
    //ファイルの読み込み
    ifstream fin1(argv[1]), fin2(argv[2]);
    if(!fin1 || !fin2)
    {
        cerr << "ファイルが開けませんでした\n";
        exit(errno);
    }
    //fin1で読み込むidと虹彩情報の変数をそれぞれ宣言
    int ids[PEOPLE_NUM];
    char iris[PEOPLE_NUM][BIT_LEN + 1];
    //ループでid->irisの順に読み取り
    for(int i = 0; i < PEOPLE_NUM; i++)
    {
        fin1 >> ids[i];
        for(int j = 0; j < BIT_LEN; j++)
            fin1 >> iris[i][j];
    }
    //調べたい虹彩情報のデータを格納する配列を宣言
    char input_iris[BIT_LEN + 1];
    //調べたい虹彩情報を読み取り
    fin2 >> input_iris;
    //最小ハミング距離とそのidの変数を宣言
    int closest_dist = BIT_LEN + 1;
    int closest_id = -1;
    //ループで順番に計算していって最小距離だったらその距離とidを保存
    for(int l = 0; l < PEOPLE_NUM; l++)
    {
        int dist = calc_hamming_distance(iris[l], input_iris);
        if(dist < closest_dist)
        {
            closest_dist = dist;
            closest_id = ids[l];
        }
    }
    cout << "最も似ている人物のIDは" << closest_id << "で、\n";
    cout << "ハミング距離は" << closest_dist << "となります。\n";
    return(0);
}
