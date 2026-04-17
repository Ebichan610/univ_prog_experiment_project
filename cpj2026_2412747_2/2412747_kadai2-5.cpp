#include <iostream>
#include <fstream>
#include <errno>
#include <string>
#include <vector>
using namespace std;

int main(int argc, char *argv[])
{
    //引数の数のエラー処理
    if(argc != 2)
    {
        cerr << "引数が2つではありません。\n";
        return(1);
    }
    //ファイルの読み込みとエラー処理
    ifstream fin(argv[1]);
    if(!fin)
    {
        perror("ディレクトリが開けませんでした。\n");
        return(errno);
    }

}