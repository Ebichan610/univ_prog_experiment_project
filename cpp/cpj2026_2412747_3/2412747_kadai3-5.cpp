#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cerrno>
using namespace std;

//人数、データ数、次元数の定数の定義
static const int PERSONS   = 300;
static const int NUM       = 5;
static const int DIMENSION = 1000;

//2種の配列(平均データ、インプットデータ)のグローバル変数の定義
static double avg[PERSONS][DIMENSION];
static double test_data[DIMENSION];

// カンマ区切りのテキストファイルから1000次元ベクトルを読み取る関数
static void read_vector(ifstream &fin, double *sum) 
{
    string line;
    //ファイルから1行ずつ読み取る
    while (getline(fin, line))
    {
        //カンマが含まれない行(ID行や空行)はデータではないので飛ばす
        if (line.find(',') == string::npos)
            continue;
        
        stringstream ss(line);
        string token;
        int count = 0;
        //カンマで区切られた値を格納
        while(count < DIMENSION && getline(ss, token, ','))
            sum[count++] = atof(token.c_str());
        
        //指定量の1000個を読み取ったら終了
        if (count == DIMENSION) 
            return; 
    }
}

int main(int argc, char *argv[])
{
    //引数エラー処理
    if (argc != 3)
    {
        cerr << "引数が2つではありません。\n";
        return(1);
    }

    //---- 各人の画像データの読み込みおよび平均顔の作成 ----
    //ファイルの読み込み
    ifstream fin1(argv[1]);
    if (!fin1)
    {
        perror("1つ目のファイルが開けません:");
        return(errno);
    }

    
    for (int i = 0; i < PERSONS; i++)
    {
        //初期化
        for (int j = 0; j < DIMENSION; j++)
            avg[i][j] = 0.0;

        //1人あたり5枚のデータを読み込み、加算していく
        for (int k = 0; k < NUM; k++)
        {
            read_vector(fin1, test_data);
            for (int l = 0; l < DIMENSION; l++)
                avg[i][l] += test_data[l];
        }
        //合計を枚数の5で割り、平均を算出
        for (int m = 0; m < DIMENSION; m++)
            avg[i][m] /= (double)NUM;
    }
    fin1.close();


    // ---- 各人物の正解ランクを求める ----
    ifstream fin2(argv[2]);
    if (!fin2)
    {
        perror("2つ目のファイルが開けません:");
        return(errno);
    }
    //各人物の正解ランクを記録する配列
    int rank_list[PERSONS];

    for (int total = 0; total < PERSONS; total++)
    {
        //値の読み取り
        read_vector(fin2, test_data);
        // 全300人との距離を計算
        double dists[PERSONS];
        for (int i = 0; i < PERSONS; i++)
        {
            double dist = 0.0;
            for (int j = 0; j < DIMENSION; j++)
            {
                //差分を取り
                double diff = test_data[j] - avg[i][j];
                //二乗値を加算
                dist += diff * diff;
            }
            dists[i] = dist;
        }

        //正解人物の距離より小さい距離の数を数える
        double correct_dist = dists[total];
        int rank = 1;
        for (int k = 0; k < PERSONS; k++)
        {
            if(dists[k] < correct_dist)
                rank++;
        }
        //これを配列の要素に代入
        rank_list[total] = rank;
    }
    fin2.close();

    // ---- 累積識別率の計算と出力 ----
    //正解(1位)がいくらかを計算
    int correct_rank = 0;
    for (int i = 0; i < PERSONS; i++)
    {
        if (rank_list[i] == 1)
            correct_rank++;
    }
    //結果を出力
    cout << "1位識別率: " << fixed << setprecision(2) << (double)correct_rank / PERSONS * 100.0 << " %\n";

    //result.csvに値を出力
    ofstream fout("result.csv");
    fout << "N,rate\n";
    //許容するランクを1ずつ増やして計算
    for (int n = 1; n <= PERSONS; n++)
    {
        int count = 0;
        for (int i = 0; i < PERSONS; i++)
        {
            if (rank_list[i] <= n)
                count++;
        }
        //結果を書き込み
        fout << n << "," << fixed << setprecision(2)
             << (double)count / PERSONS * 100.0 << "\n";
    }
    fout.close();
    cout << "累積識別率データをresult.csvに出力しました。\n";

    return(0);
}
