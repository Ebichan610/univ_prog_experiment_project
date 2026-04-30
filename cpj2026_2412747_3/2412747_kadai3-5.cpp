#include <iostream>
#include <fstream>
#include <cerrno>
using namespace std;

static const int PERSONS = 300;
static const int IMAGES = 5;
static const int DIMENSION = 1000;


int main(int argc, char *argv[])
{
    //引数エラー処理
    if(argc != 3)
    {
        cerr << "引数が2つではありません。\n";
        return(1);
    }

    //enrollment2026.txtを開く
    ifstream fin(argv[1]);
    if(!fin)
    {
        perror("enrollment2026.txtが開けませんでした。\n");
        return(errno);
    }

    //平均顔の作成
    //まずは二次元配列のメモリ
    double **avg = new(nothrow) double* [PERSONS];
    if(avg == NULL)
    {
        cerr << "メモリの確保に失敗しました。\n";
        return(1);
    }
    //次は各配列のメモリ
    for(int i = 0; i < PERSONS; i++)
    {
        avg[i] = new(nothrow) double[DIMENSION]();
        //エラー処理
        if(avg[i] == NULL)
        {
            while(i > 0)
                delete[] avg[i--];
            delete[] avg;
            cerr << "メモリの確保に失敗しました。\n";
            return(1);
        }
    }

    //
}
