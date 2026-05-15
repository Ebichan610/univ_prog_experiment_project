#include <iostream>
#include <fstream>
#include <cerrno>
using namespace std;

int main(int argc, char *argv[])
{
    //引数のエラー処理
    if(argc != 2)
    {
        cerr << "引数が1つではありません\n";
        exit(EXIT_FAILURE);
    }
    //ファイル名の取得と読み込み
    ifstream fin(argv[1]);
    if(!fin)
    {
        cerr << "ファイルが読み込めません\n";
        exit(errno);
    }
    //ファイルの生成
    ofstream fout("result.txt");
    //ファイル内の文字列を順番に読み込んで書き込んでいく
    char word;
    while(fin >> noskipws >> word)//今回は空白や改行を無視しない
    {
        //もし小文字なら大文字へ
        if(word >= 'a' && word <= 'z')
            word -= 'a' - 'A';
        //もし大文字なら小文字へ
        else if(word >= 'A' && word <= 'Z')
            word += 'a' - 'A';
        fout << word;
    }
    return(0);
}
