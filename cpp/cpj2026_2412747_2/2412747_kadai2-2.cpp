#include <iostream>
#include <fstream>
#include <cerrno>
using namespace std;

template <class T>
static T sumt(T x, T y)
{
    return (x + y);
}

int main(int argc, char *argv[])
{
    //引数の数のエラー処理
    if(argc != 3)
    {
        cerr << "引数の数が2つではありません。\n";
        return(1);
    }
    //ファイルの読み取りおよびエラー処理
    ifstream fin1(argv[1]);
    if(!fin1)
    {
        cerr << "1つ目のファイルを開けませんでした。\n";
        return(errno);
    }
    ifstream fin2(argv[2]);
    if(!fin2)
    {
        cerr << "2つ目のファイルを開けませんでした。\n";
        return(errno);
    }
    //ファイルの行数の変数を宣言
    int fin1_len = 0, fin2_len = 0;
    //ファイル1と2それぞれの合計の変数と数字を一時的に保存する変数の宣言
    int fin1_sum = 0, fin1_tmp = 0;
    double fin2_sum = 0.0, fin2_tmp = 0.0;
    //ループ処理で合計をそれぞれ計算
    while(fin1 >> fin1_tmp)
    {
        fin1_sum = sumt(fin1_tmp, fin1_sum);
        fin1_len++;
    }
    while(fin2 >> fin2_tmp)
    {
        fin2_sum = sumt(fin2_tmp, fin2_sum);
        fin2_len++;
    }
    //出力
    cout << "1つ目のファイルは" << fin1_len << "行で、記載されている数字の合計は" << fin1_sum << "です\n";
    cout << "2つ目のファイルは" << fin2_len << "行で、記載されている数字の合計は" << fin2_sum << "です\n";
    return(0);
}
