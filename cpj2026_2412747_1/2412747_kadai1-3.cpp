#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char *argv[])
{
    //引数のエラー処理
    if(argc != 2)
    {
        cerr << "引数が2つではありません\n";
        exit(EXIT_FAILURE);
    }
    //ファイル名の取得と読み込み
    ifstream fin(argv[1]);
    if(!fin)
    {
        cerr << "ファイルが読み込めません\n";
        exit(EXIT_FAILURE)
    }
    //
    //ファイルの生成と書き込み
    ofstream fout("result.txt");
    
    return(0);
}
