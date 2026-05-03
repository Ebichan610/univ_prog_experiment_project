#include <iostream>
#include <fstream>
#include <unordered_map>
#include <cerrno>
using namespace std;



//単語の頻度数などを格納するクラス
class WordFreq
{
    public:
        WordFreq();
        void read_file(ifstream& fin);
    private:
        unordered_map<int, int> word_map;
};

WordFreq::WordFreq()
{

}

void WordFreq::read_file(ifstream&fin)
{
    string tmp;
    while(fin >> tmp)
    {
        
    }
}

int main(int argc, char* argv[])
{
    //引数エラー処理
    if(argc != 3)
    {
        cerr << "引数を2つにしてください。\n";
        return(1);
    }

    //オブジェクトの宣言
    WordFreq wordsA, wordsB;

    ifstream fin1(argv[1]);
    if(!fin1)
    {
        perror("1つ目のファイルが開けませんでした。:");
        return(errno);
    }
    wordsA.read_file(fin1);
}