#include <iostream>
#include <fstream>
#include <cerrno>
#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include <dirent.h>
#include <sys/stat.h>
using namespace std;

//マルウェアの特徴であるバイト列を定義
const unsigned char SIGNATURE[] = {
    0x31, 0xC9, 0xE8, 0x8C, 0x32, 0x00, 0x00, 0x45, 0x31, 0xC0, 0x31
};
//上記のバイト列の長さを定義
const int SIG_LEN = sizeof(SIGNATURE) / sizeof(SIGNATURE[0]);

//通常ファイルなのかどうかを判定する関数
static bool is_regular_file(string &path)
{
    struct stat st;
    //ファイル情報の取得
    if(stat(path.c_str(), &st) != 0)
        return(false);
    //通常ファイルかのチェック
    return(S_ISREG(st.st_mode));
}

//ファイルをバイナリ形式で読む関数
static bool read_binary(string &path, vector<unsigned char> &buf)
{
    //ファイルをテキストではなくバイトで読む
    ifstream fin(path, ios::binary);
    if(!fin)
        return(false);
    //ファイル全体をbufにコピー
    buf.assign(istreambuf_iterator<char>(fin), istreambuf_iterator<char>());
    return(true);
}

//マルウェアAファミリーが存在するのかを判定する関数
static bool malware_search(vector<unsigned char> &data)
{
    //SEGV対策
    if(data.size() < SIG_LEN)
        return(false);
    //全探索により検索
    for(int i = 0; i < data.size() - SIG_LEN; i++)
    {
        bool is_find = true;

        for(int j = 0; j < SIG_LEN; j++)
        {
            if(data[i + j] != SIGNATURE[j])
            {
                is_find = false;
                break;
            }
        }
        if(is_find == true)
            return(true);
    }
    return(false);
}

//ファイルの検索順序を自然順にするためのソートの下準備用関数
static bool natural_num_comp(string &a, string &b)
{
    int i = 0, j = 0;

    while(i < (int)a.size() && j < (int)b.size())
    {
        //ファイル名が数字であったとき特有の処理
        if(isdigit(a[i]) && isdigit(b[j]))
        {
            int num_a = 0, num_b = 0;
            //ファイルの数字列を数値として合算
            while(i < (int)a.size() && isdigit(a[i]))
            {
                num_a = num_a * 10 + ((a[i]) - '0');
                i++;
            }
            //ファイルの数字列を数値として合算
            while(j < (int)b.size() && isdigit(b[j]))
            {
                num_b = num_b * 10 + ((b[j]) - '0');
                j++;
            }
            //両者を比較する
            if(num_a != num_b)
                return(num_a < num_b);
        }
        //そうでないときは純粋に比較
        else
        {
            if(a[i] != b[j])
                return(a[i] < b[j]);
            i++;
            j++;
        }
    }
    return(a.size() < b.size());
}

int main(int argc, char *argv[])
{
    //引数の数のエラー処理
    if(argc != 2)
    {
        cerr << "引数が2つではありません。\n";
        return(1);
    }
    //ディレクトリの読み込みとエラー処理
    string dir_path = argv[1];
    DIR *dir = opendir(dir_path.c_str());
    if(dir == NULL)
    {
        perror("ディレクトリが開けませんでした。");
        return(errno);
    }
    //バイナルファイル名を格納するstring型のコンテナ
    vector<string> filenames;
    struct dirent *entry;
    //ディレクトリ内のエントリをチェック
    while((entry = readdir(dir)) != nullptr)
    {
        string name = entry->d_name;
        //ディレクトリ内にある.と..は除外
        if(name != "." || name != "..")
        {
            string full_path = dir_path + "/" + name;
            //通常ファイルかどうかの判定
            if(is_regular_file(full_path) == true)
                filenames.push_back(name);
        }
    }
    //ディレクトリを閉じる
    closedir(dir);
    //ファイルを名前順にソート
    sort(filenames.begin(), filenames.end(), natural_num_comp);
    //マルウェア検出の変数
    bool is_find = false;
    for(string &name : filenames)
    {
        string full_path = dir_path + "/" + name;
        
        vector<unsigned char> buf;
        if(read_binary(full_path, buf) == false)
        {
            cerr << "読み込みが失敗しました\n";
            continue;
        }
        //マルウェアの判定
        if(malware_search(buf) == true)
        {
            cout << "[検出]:" << name << "\n";
            is_find = true;
        }
    }
    //マルウェアの有無でメッセージを分岐
    if(is_find == true)
        cout << "以上のファイルよりマルウェアAファミリーが検出されました。\n";
    else
        cout << "マルウェアAファミリーは検出されませんでした。\n";
    return(0);

}