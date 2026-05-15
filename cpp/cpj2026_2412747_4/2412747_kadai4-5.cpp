#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <cerrno>
using namespace std;

//特徴量を保持するクラス
class DataPoint
{
    public:
        //初期化コンストラクタ
        DataPoint(double x, double y): feat1(x), feat2(y) {};
        //それぞれの特徴量を返す
        double getfeat1() const {return(feat1);};
        double getfeat2() const {return(feat2);};
    private:
        //特徴量
        double feat1;
        double feat2;
};

//マルウェアファミリークラス
class MalwareFile
{
    public:
        //初期化コンストラクタ
        MalwareFile(): a(0.0), b(0.0) {};
        //ファイルの読み込み
        bool loadFile(const string& filename);
        //最小二乗法
        void fitModel();
        //点と直線の距離の計算
        double distToLine(double x, double y) const
            {return(abs(a * x - y + b) / sqrt(pow(a, 2) + 1));};
        //モデルへの直線距離の平均を計算
        double avgDist(const MalwareFile& unknown) const;
        //モデルの出力
        void printModel(const string& label) const
            {cout << label << ":y = " << a << "x + " << b << '\n';};
        //データを返す
        const vector<DataPoint>& getData() const{return(data);};
    private:
        //特徴量クラスのvector
        vector<DataPoint> data;
        double a;
        double b;
};

//ファイルの読み込みと値の格納を行う関数
bool MalwareFile::loadFile(const string& filename)
{
    //ファイルのストリームとエラー対応
    ifstream fin(filename);
    if(!fin)
    {
        perror("ファイルが開けませんでした。\n");
        return(false);
    }

    //1行用の文字列型変数
    string line;
    //1行ずつ読み込み
    while(getline(fin, line))
    {
        //空行スキップ(念のため)
        if(line.empty())
            continue;
        //最後の\rsを削除
        if(!line.empty() && line.back() == '\r')
            line.pop_back();
        
        //csvの解析用
        stringstream ss(line);
        string token;
        vector <double> vals;
        //カンマによって区切って
        while(getline(ss, token, ','))
            //文字列をダブルに変換
            vals.push_back(stod(token));
        //データを保存
        data.push_back(DataPoint(vals[0], vals[1]));
    }
    //データが空でなければtrue(念の為)
    return (!data.empty());
}

//最小二乗法を行う計算
void MalwareFile::fitModel()
{
    int n = (int)data.size();
    //考えづらいが念の為
    if(n < 2)
        return;
    
    //ゲッターにより値を取得し平均を算出
    double meanX = 0, meanY = 0;
    for(const DataPoint& p: data)
    {
        meanX += p.getfeat1();
        meanY += p.getfeat2();
    }
    meanX /= n;
    meanY /= n;

    //分散と共分散を算出
    double Sxx = 0, Sxy = 0, Syy = 0;
    for(const DataPoint& p: data)
    {
        double dx = p.getfeat1() - meanX;
        double dy = p.getfeat2()- meanY;
        Sxx += dx * dx;
        Sxy += dx * dy;
        Syy += dy * dy;
    }

    //共分散があまりにも小さいならa=0とみなす
    if(abs(Sxy) < 1e-10)
        a = 0;
    else
        a = (Syy - Sxx +sqrt(pow(Syy - Sxx, 2) + 4 * pow(Sxy, 2))) / (2 * Sxy);
    //切片
    b = meanY - a * meanX;
}

//直線距離の平均を求める関数
double MalwareFile::avgDist(const MalwareFile& unknown) const
{
    //ゲッターによりデータを取得
    const vector<DataPoint>& points = unknown.getData();
    //無いがデータが空なら莫大な値を返すように設計
    if(points.empty())
        return(1e10);
    double total = 0;
    for(const DataPoint& p: points)
        total += distToLine(p.getfeat1(), p.getfeat2());
    //最後にサイズで割って平均を返す
    return(total / points.size());
}

int main(int argc, char *argv[])
{
    //引数エラー処理
    if(argc != 5)
    {
        cerr << "引数が4つではありません。\n"
            << "判定したいCSVファイル1つと判定元のCSVファイル3つのパス名を入力してください。\n";
        return(1);
    }

    //マルウェアクラスのオブジェクトを宣言(順番はPDFに合わせている)
    MalwareFile unknown, typeT, typeW, typeR;
    //ファイルの読み込み
    if(!unknown.loadFile(argv[1]))  return(1);
    if(!typeT.loadFile(argv[2]))    return(1);
    if(!typeW.loadFile(argv[3]))    return(1);
    if(!typeR.loadFile(argv[4]))    return(1);

    //モデルの適用
    typeT.fitModel();
    typeW.fitModel();
    typeR.fitModel();

    //モデルを出力
    typeT.printModel("T");
    typeW.printModel("W");
    typeR.printModel("R");

    //平均距離を記録
    double scoreT = typeT.avgDist(unknown);
    double scoreW = typeW.avgDist(unknown);
    double scoreR = typeR.avgDist(unknown);

    //出力
    cout << "それぞれの平均距離は\n"
        <<"T: " << scoreT << "\n"
        <<"W: " << scoreW << "\n"
        <<"R: " << scoreR << "\n";
    //条件分岐により出力
    cout << "よってunknownファイルはマルウェアファミリ";
    if(scoreT <= scoreW && scoreT <= scoreR)
        cout << 'T';
    else if(scoreW <= scoreT && scoreW <= scoreR)
        cout << 'W';
    else
        cout << 'R';
    cout << "に該当します。\n";
    return(0);
}
