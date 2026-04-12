#include <iostream>
#include <fstream>
#include <cerrno>
#include <set>
using namespace std;

//単語を予め小文字および文末記号を削除しておく関数
string normalize(string s)
{
    for(int i = 0; i < s.length(); i++)
    {
        if(s[i] >= 'A' && (s[i] <= 'Z'))
            s[i] += 'a' - 'A';
    }
    char last = s[s.length() - 1];
    if(last == ':' || last == ';' || last == ',' || last == '.')
        s.erase(s.length() - 1);
    return(s);
}

int main(int argc, char *argv[])
{
    //引数エラー処理
    if(argc != 3)
    {
        cerr << "引数が2つではありません\n";
        exit(EXIT_FAILURE);
    }
    //ファイル名の取得と読み込み
    ifstream fin1(argv[1]), fin2(argv[2]);
    if(!fin1 || !fin2)
    {
        cerr << "ファイルが開けません\n";
        exit(errno);
    }
    //diff1とdiff2のそれぞれの単語を格納
    set<string> s1, s2;
    string tmp;
    while(fin1 >> tmp)
        s1.insert(normalize(tmp));
    while(fin2 >> tmp)
        s2.insert(normalize(tmp));
    //共通部分や単独の部分をそれぞれ判定して格納
    set<string> common, only1, only2;
    for(string word : s1)
    {
        if(s2.count(word))
            common.insert(word);
        else
            only1.insert(word);
    }
    for(string word : s2)
    {
        if(!s1.count(word))
            only2.insert(word);
    }
    //結果をファイルと画面にそれぞれ出力
    ofstream fout("result.txt");
    cout << "2つのファイルに共通して出現する単語(単語数:" << common.size() << "個)\n";
    fout << "2つのファイルに共通して出現する単語(単語数:" << common.size() << "個)\n";
    for(string word : common)
    {
        cout << word << " ";
        fout << word << " ";
    }
    cout << "\n\n";
    fout << "\n\n";
    cout << argv[1] <<"のみに記載されている単語(単語数:" << only1.size() << "個)\n";
    fout << argv[1] <<"のみに記載されている単語(単語数:" << only1.size() << "個)\n";
    for(string word : only1)
    {
        cout << word << " ";
        fout << word << " ";
    }
    cout << "\n\n";
    fout << "\n\n";
    cout << argv[2] <<"のみに記載されている単語(単語数:" << only2.size() << "個)\n";
    fout << argv[2] <<"のみに記載されている単語(単語数:" << only2.size() << "個)\n";
    for(string word : only2)
    {
        cout << word << " ";
        fout << word << " ";
    }
    cout << "\n\n";
    fout << "\n\n";
    return(0);
}
