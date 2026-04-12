#include <iostream>
#include <fstream>
#include <cerrno>
using namespace std;

int main()
{
    //入力ファイル名の取得とファイルの読み込み
    char in_filename[256];
    cout << "入力するファイル名は？\n";
    cin >> in_filename;
    ifstream fin(in_filename);
    if(!fin)
    {
        cerr << "ファイルがオープンできませんでした\n";
        exit(errno);
    }
    //ファイルは50行あり、1行あたり4項目ある
    int num = 50 * 4;
    int score[num];
    int math_mean = 0, eng_mean = 0, jap_mean = 0, sub_mean = 0;
    //ファイルの数値をintの配列に格納
    for(int i = 0; i<num; i++)
       fin >> score[i];
    //インデックスの値によって場合分け
    for(int j = 0; j<num; j++)
    {
        if(j % 4 == 1)
            math_mean += score[j];
        else if(j % 4 == 2)
            eng_mean += score[j];
        else if(j % 4 == 3)
            jap_mean += score[j];
        if(j % 4 != 0)
            sub_mean += score[j];
    }
    math_mean /= 50;
    eng_mean /= 50;
    jap_mean /= 50;
    sub_mean /= 50;
    //出力ファイル名の取得と生成
    char out_filename[256];
    cout << "出力するファイル名は？\n";
    cin >> out_filename;
    ofstream fout(out_filename);
    //ファイル及び画面に平均点を出力
    fout << "数学の平均点は" << math_mean << "点で、英語の平均点は" << eng_mean << "点で、国語の平均点は" << jap_mean << "点です。\n";
    fout << "また、合計点の平均点は" << sub_mean << "点です。\n";
    cout << "数学の平均点は" << math_mean << "点で、英語の平均点は" << eng_mean << "点で、国語の平均点は" << jap_mean << "点です。\n";
    cout << "また、合計点の平均点は" << sub_mean << "点です。\n";
    return(0);
}
