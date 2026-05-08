#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cerrno>
using namespace std;



//単語の頻度数などを格納するクラス
class WordFreq
{
    public:
        //コンストラクタ
        WordFreq(): len(0), freq(0){};
        //コピーコンストラクタ
        WordFreq(int length, int frequency)
            :len(length), freq(frequency){};
        //長さを返す
        int getLen() const {return(len);};
        //出現頻度を返す
        int getFreq() const {return(freq);};
        //出現頻度をインクリメントする
        void addFreq(){freq++;};
        
    private:
        int len;
        int freq;
};

//文章解析用クラス
class TextAnalyzer
{
    public:
        //ファイルの読み込み
        bool loadFile(const string& filename);
        //単語の出現頻度の計算
        void countFreq();
        //出現頻度の多い順にソート
        void sortFreq();
        //出力用関数
        void printFreq() const;
        //2つのファイルによる解析結果の比較を出力
        void printCompare(const TextAnalyzer& a, const TextAnalyzer& b);
        //文の数を返す
        int getSentenceCount() const;
        //総単語数を返す
        int getTotalWord() const {return((int)words.size());};
        //1文あたりの単語数を返す
        double getWordAve() const {return((double)getTotalWord() / getSentenceCount());};
    private:
        //単語のvector
        vector<string> words;
        //出現頻度のvector
        vector<WordFreq> freqtable;
        //解析する文を格納
        string text;
};

//ファイルの読み込みをして単語毎に格納していく関数
bool TextAnalyzer::loadFile(const string& filename)
{
    //ファイルの読み込み
    ifstream fin(filename);
    if(!fin)
    {
        perror("ファイルが開けませんでした。:");
        return(false);
    }
    //空文字で初期化
    text = "";
    //読み取り用の文字列型
    string line;
    //読み取り部分
    while(getline(fin, line))
    {
        //\rの条件はテストの結果必要だった
        if(!line.empty() && line.back() == '\r')
            line.pop_back();
        //文を空白毎に区切る
        text += line + " ";
    }
    return(true);
}

//単語の頻度を計算
void TextAnalyzer::countFreq()
{
    //初期化
    words.clear();
    //処理用文字列型の宣言
    string tmp = "";
    for(int i = 0; i <= (int)text.size(); i++)
    {
        //最後の単語を確定させる(範囲外アクセスを防ぐ)
        char c = (i < (int)text.size()) ? text[i] : ' ';
        //単語部分を探索している
        //単語途中に数字が来る場合は許可
        if((unsigned char)isalpha(c) || ((unsigned char)isdigit(c) && !tmp.empty()))
            //小文字に統一
            tmp += (unsigned char)tolower(c);
        //空白が来たのならば
        else
        {
            //そこまでが単語なのでvectorに格納
            if(!tmp.empty())
            {
                words.push_back(tmp);
                tmp = "";
            }
        }
    }

    int max_len = 0;
    //単語の最大長がいくらかを求める
    for(const string& w: words)
    {
        if((int)w.size() > max_len)
            max_len = (int)w.size();
    }
    //テーブルを初期化
    freqtable.clear();
    //単語の長さ毎にテーブルを作成
    for(int i = 1; i <= max_len; i++)
        freqtable.push_back(WordFreq(i, 0));
    //出現頻度カウント
    for(const string& w: words)
    {
        int len = (int)w.size();
        if(len >= 1 && len <= max_len)
            //始まりが0からなのでlen-1
            freqtable[len - 1].addFreq();
    }
}

//出現頻度のバブルソート用関数
void TextAnalyzer::sortFreq()
{
    int size = (int)freqtable.size();
    for(int i = 0; i < size - 1; i++)
    {
        for(int j = 0; j < size - i - 1; j++)
        {
            if(freqtable[j].getFreq() < freqtable[j + 1].getFreq())
            {
                WordFreq tmp = freqtable[j];
                freqtable[j] = freqtable[j + 1];
                freqtable[j + 1] = tmp;
            }
        }
    }
}

//出力用関数
void TextAnalyzer::printFreq() const
{
    cout << "単語長\t出現頻度\n";
    for(const WordFreq& wf: freqtable)
    {
        if(wf.getFreq() > 0)
            cout << wf.getLen() << "文字\t" << wf.getFreq() << "回\n";
    }
}

//2つの文章解析クラスの比較をして出力する関数
void TextAnalyzer::printCompare(const TextAnalyzer& a, const TextAnalyzer& b)
{
    //平均を取得
    double aveA = a.getWordAve();
    double aveB = b.getWordAve();
    //それぞれ出力
    cout << "\n平均単語数\n";
    cout << "A: " << aveA << "語/文\n";
    cout << "B: " << aveB << "語/文\n";
    //条件分岐により出力
    if(aveA > aveB)
        cout << "ファイルAの方が長い文章を書きます。\n";
    else if(aveA < aveB)
        cout << "ファイルBの方が長い文章を書きます。\n";
    else
        cout << "どちらのファイルも同じ長さの文章を書きます。\n";
}

//文の数を数える関数
int TextAnalyzer::getSentenceCount() const
{
    int count = 0;
    for(int i = 0; i < (int)text.size(); i++)
    {
        //文の区切り
        if(text[i] == '!' || text[i] == '?')
            count++;
        //ファイルBでピリオドが連続している箇所があったのでそれの対策
        else if(text[i] == '.')
        {
            //次の要素を見る
            int next = i + 1;
            //空白であればスキップ
            while(next < (int)text.size() && text[next] == ' ')
                next++;
            //次が大文字であればそれまでが文章
            if(next >= (int)text.size() || isupper(text[next]))
                count++;
        }
    }
    //三項演算子(0除算回避)
    return(count == 0) ? 1 : count;
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
    TextAnalyzer textA, textB;

    //1つ目のファイルAについて
    if(!textA.loadFile(argv[1]))
        return(1);
    textA.countFreq();
    textA.sortFreq();
    cout << "ファイルA:" << argv[1] << "\n";
    textA.printFreq();

    //２つ目のファイルBについて
    if(!textB.loadFile(argv[2]))
        return(1);
    textB.countFreq();
    textB.sortFreq();
    cout << "ファイルB:" << argv[2] << "\n";
    textB.printFreq();

    textA.printCompare(textA, textB);
    return(0);
}
