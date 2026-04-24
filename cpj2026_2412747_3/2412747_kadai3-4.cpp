#include <iostream>
#include <fstream>
#include <cerrno>
using namespace std;

//ファイル内の最初の3行をチェック
static void file_check(ifstream& fin, int *win_width, int * win_height)
{
    //識別子→画像幅→画像高→最大画素値の順に読み取り
    string identifier;
    int color_max;
    fin >> identifier;
    fin >> *win_width;
    fin >> *win_height;
    fin >> color_max;
    //うち2つは使用しないので
    (void)identifier;
    (void)color_max;
}

//delete用関数
static void delete_window(int **window)
{
    int i = 0;
    while(window[i] != NULL)
        delete window[i++];
    delete window;
}

//PPM画像から画素値を入手する関数
static void get_ppm(int ***window, ifstream& fin)
{

}

//グレースケールの演算をする関数
static int **save_pgm(int **window)
{

}

int main(int argc, char *argv[])
{
    //引数エラー処理
    if(argc != 2)
    {
        cerr << "引数を1つにしてください\n";
        return(1);
    }
    //ファイルの読み込み
    ifstream fin(argv[1]);
    if(!fin)
    {
        perror("ファイルが開けませんでした");
        return(errno);
    }
    //ファイルに書いてあるもののチェック
    int win_width;
    int win_height;
    file_check(fin, &win_width, &win_height);
    //画像出力用の二次元動的配列を作成
    int **window = new int*[win_height];
    //メモリエラー
    if(window == NULL)
    {
        perror("メモリの確保に失敗しました\nnew");
        return(errno);
    }
    for(int i = 0; i < win_height; i++)
    {
        window[i] = new int[win_width];
        //メモリエラー
        if(window[i] == NULL)
        {
            perror("メモリの確保に失敗しました\nnew");
            //既に確保したメモリを全て解放
            delete_window(window);
            return(errno);
        }
    }
    //PPM画像から値を読み取り
    get_ppm(&window, fin);
    //PPMをPGMに
    int **result;
    result = save_pgm(window);
    
}
