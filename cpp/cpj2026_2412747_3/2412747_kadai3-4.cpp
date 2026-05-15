#include <iostream>
#include <fstream>
#include <cerrno>
using namespace std;

//ファイル内の最初の3行をチェック
static void file_check(ifstream& fin, int *img_width, int *img_height)
{
    //識別子→画像幅→画像高→最大画素値の順に読み取り
    string identifier;
    int max_val;
    fin >> identifier  >> *img_width >> *img_height >> max_val;
    //うち2つは使用しないので
    (void)identifier;
    (void)max_val;
    fin.get();
}

//delete用関数
static void delete_window(int **window, int height)
{
    for(int i = 0; i < height; i++)
        delete[] window[i];
    delete[] window;
}

//PPM画像から画素値を入手してそれをグレイスケールにする関数
static bool get_gray_ppm(int**&window, ifstream& fin, int width, int height)
{
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            unsigned char rgb[3];
            //RGB値を読み込み
            fin.read((char *)rgb, 3);
            if(!fin)
                return(false);
            //グレイスケールに変換
            int gray = 0.299 * rgb[0] + 0.587 * rgb[1] + 0.114 * rgb[2];
            window[y][x] = gray;
        }
    }
    return(true);
}

//フィルター適用の関数
static void apply_filter(int**& filter, int**& img, int width, int height)
{
    for(int y = 1; y < height - 1; y++)
    {
        for(int x = 1; x < width - 1; x++)
        {
            //図1のフィルターを適用
            int val = img[y - 1][x] + img[y + 1][x] + img[y][x - 1] + img[y][x + 1] - 4 * img[y][x];
            if(val < 0)
                val = 0;
            if(val > 255)
                val = 255;
            filter[y][x] = val;
        }
    }
}

//PGM画像を保存する関数
static bool save_pgm(const char* filename, int **img, int width, int height)
{
    //出力ファイル
    ofstream fout(filename);
    if(!fout)
        return(false);
    //まずはPPM画像の形式に沿って最初3行を出力
    fout << "P2\n";
    fout << width << " " << height << "\n";
    fout << "255\n";
    //ループで出力
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            fout << img[y][x];
            if(x !=width - 1)
                fout << " ";
        }
        fout << "\n";
    }
    fout.close();
    return(true);
}

int main(int argc, char *argv[])
{
    //引数エラー処理
    if(argc != 2)
    {
        cerr << "引数を1つにしてください\n";
        return(1);
    }

    //ファイルの読み込み、バイナリ形式で読み込む
    ifstream fin(argv[1], ios::binary);
    if(!fin)
    {
        perror("ファイルが開けませんでした");
        return(errno);
    }

    //ファイルに書いてあるもののチェック
    int img_width;
    int img_height;
    file_check(fin, &img_width, &img_height);

    //画像出力用の二次元動的配列を作成
    int **window = new(nothrow) int*[img_height];
    //メモリエラー
    if(window == NULL)
    {
        cerr << "メモリの確保に失敗しました。\n";
        return(1);
    }
    for(int i = 0; i < img_height; i++)
    {
        window[i] = new(nothrow) int[img_width];
        //メモリエラー
        if(window[i] == NULL)
        {
            cerr << "メモリの確保に失敗しました。\n";
            //既に確保したメモリを全て解放
            delete_window(window, i);
            return(1);
        }
    }

    //PPM画像から値を読み取り
    if(get_gray_ppm(window, fin, img_width, img_height) == false)
    {
        //既に確保しているので解放
        delete_window(window, img_height);
        //ファイルをクローズ
        fin.close();
        cerr << "PPM画像取得に失敗しました。\n";
        return(errno);
    }
    //ファイルをクローズ
    fin.close();

    //フィルターを適用していくためのメモリ確保
    int **filter = new(nothrow) int*[img_height];
    if(filter == NULL)
    {
        delete_window(window, img_height);
        cerr << "メモリの確保に失敗しました。\n";
        return(1);
    }
    for(int j = 0; j < img_height; j++)
    {
        //メモリ確保兼0初期化
        filter[j] = new(nothrow) int[img_width]();
        //メモリエラー
        if(filter[j] == NULL)
        {
            cerr << "メモリの確保に失敗しました。\n";
            //既に確保したメモリを全て解放
            delete_window(window, img_height);
            delete_window(filter, j);
            return(1);
        }
    }

    //実際にフィルターを適用
    apply_filter(filter, window, img_width, img_height);
    //もうwindowのメモリは不要なので解放
    delete_window(window, img_height);

    //最後にPGM画像を出力
    if(save_pgm("output.pgm", filter, img_width, img_height) == false)
    {
        delete_window(filter, img_height);
        perror("ファイルが開けませんでした。\nfout:");
        return(errno);
    }
    //最後にメモリを解放
    delete_window(filter, img_height);
    //ユーザーに出力したことをメッセージで伝える
    cout << "output.pgmに保存しました。\n";
    return(0);
}
