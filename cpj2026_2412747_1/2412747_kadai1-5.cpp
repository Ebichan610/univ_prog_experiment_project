#include <iostream>
#include <fstream>
#include <cerrno>
using namespace std;

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
    
}
