#include <iostream>
#include <iomanip>
#include <vector>
#include <opencv2/opencv.hpp>
using namespace std;

//評価結果のための構造体
struct EvalResult {
    int TP, TN, FP, FN;
    int total, correct;
    double accuracy;
};

//識別器による評価を行う関数
EvalResult evaluate(const cv::Mat& pred, const cv::Mat& labels_te)
{
    //初期化
    EvalResult result = {0, 0, 0, 0, 0, 0, 0.0};
    for(int i = 0; i < pred.rows; i++)
    {
        //予測と実際のラベル
        int predicted = (int)pred.at<float>(i, 0);
        int actual    = labels_te.at<int>(i, 0);
        //正誤判定
        if     (actual ==  1 && predicted ==  1) result.TP++;
        else if(actual == -1 && predicted == -1) result.TN++;
        else if(actual == -1 && predicted ==  1) result.FP++;
        else if(actual ==  1 && predicted == -1) result.FN++;
    }
    //正当率の計算
    result.total    = pred.rows;
    result.correct  = result.TP + result.TN;
    result.accuracy = (double)result.correct / result.total * 100.0;
    return(result);
}

//混同行列の表示をする関数
void printConfusionMatrix(const EvalResult& result)
{
    //出力を整理するためにタブで調整
    cout << "テストデータ数:\t" << result.total   << "\n";
    cout << "正解数:\t\t" << result.correct << "\n";
    cout << "正解率:\t\t" << result.accuracy << "%\n\n";
    cout << "\t\t\t予測:良性\t予測:マルウェア\n";
    cout << "実際:良性\t\t" << result.TP << "\t\t" << result.FN << "\n";
    cout << "実際:マルウェア\t\t" << result.FP << "\t\t" << result.TN << "\n";
}

int main(int argc, char* argv[])
{
    // 引数エラー処理
    if(argc != 2)
    {
        cerr << "引数の数を1つにしてください。\n";
        return(1);
    }

    //ファイル名の定義
    string dirname = argv[1];
    if(dirname.back() != '/')
        dirname += '/';
    string file_benign_tr  = dirname + "benign.csv";
    string file_malware_tr = dirname + "malware.csv";
    string file_benign_te  = dirname + "benign_te.csv";
    string file_malware_te = dirname + "malware_te.csv";

    //学習データの読み込み
    cv::Ptr<cv::ml::TrainData> benign_tr  = cv::ml::TrainData::loadFromCSV(file_benign_tr,  0, -2, 0);
    cv::Ptr<cv::ml::TrainData> malware_tr = cv::ml::TrainData::loadFromCSV(file_malware_tr, 0, -2, 0);
    //特徴量の切り出し
    cv::Mat benign_data  = benign_tr->getSamples();
    cv::Mat malware_data = malware_tr->getSamples();
    cv::Mat data_tr;
    cv::vconcat(benign_data, malware_data, data_tr);
    //ラベルの作成(良性:1, マルウェア:-1)
    cv::Mat labels_tr(benign_data.rows + malware_data.rows, 1, CV_32SC1);
    labels_tr.rowRange(0, benign_data.rows).setTo(1);
    labels_tr.rowRange(benign_data.rows, benign_data.rows + malware_data.rows).setTo(-1);
    //学習データとして定義
    data_tr.convertTo(data_tr, CV_32F);
    cv::Ptr<cv::ml::TrainData> train_data = cv::ml::TrainData::create(data_tr, cv::ml::ROW_SAMPLE, labels_tr);

    //テストデータの読み込み
    cv::Ptr<cv::ml::TrainData> benign_te  = cv::ml::TrainData::loadFromCSV(file_benign_te,  0, -2, 0);
    cv::Ptr<cv::ml::TrainData> malware_te = cv::ml::TrainData::loadFromCSV(file_malware_te, 0, -2, 0);
    //特徴量の切り出し
    cv::Mat benign_te_data  = benign_te->getSamples();
    cv::Mat malware_te_data = malware_te->getSamples();
    cv::Mat data_te;
    cv::vconcat(benign_te_data, malware_te_data, data_te);
    //ラベルの作成
    cv::Mat labels_te(benign_te_data.rows + malware_te_data.rows, 1, CV_32SC1);
    labels_te.rowRange(0, benign_te_data.rows).setTo(1);
    labels_te.rowRange(benign_te_data.rows, benign_te_data.rows + malware_te_data.rows).setTo(-1);
    data_te.convertTo(data_te, CV_32F);

    // ==========================================================
    //(1)線形カーネル
    {
        //線形カーネルのSVM作成
        cv::Ptr<cv::ml::SVM> svm = cv::ml::SVM::create();
        svm->setType(cv::ml::SVM::C_SVC);
        svm->setKernel(cv::ml::SVM::LINEAR);
        svm->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER, 100, 1e-6));
        svm->setC(1.0);
        svm->train(train_data);
        //データの予測
        cv::Mat pred;
        svm->predict(data_te, pred);
        EvalResult result = evaluate(pred, labels_te);
        //線形カーネルの出力
        cout << "線形カーネル(C=1.0)\n";
        printConfusionMatrix(result);
    }

    // ==========================================================
    //(2)RBFカーネル、Cとgammaをグリッドサーチ
    {
        //C値とガンマ値をそれぞれ持つ
        vector<double> C_values     = {0.01, 0.1, 1.0, 10.0, 100.0};
        vector<double> gamma_values = {0.001, 0.01, 0.1, 1.0, 10.0};

        //センチネル値で初期化
        double best_acc   = -1.0;
        double best_C     = 0.0;
        double best_gamma = 0.0;
        EvalResult best_result = {0, 0, 0, 0, 0, 0, 0.0};

        cout << "\nRBFカーネル パラメータチューニング結果\n";
        //小数第3位までの出力
        cout << fixed << setprecision(3);

        //表形式で表示
        cout << "C \\ gamma\t";
        for(double g : gamma_values)
            cout << g << "\t";
        cout << "\n";

        for(double C : C_values)
        {
            cout << C << "\t\t";
            for(double gamma : gamma_values)
            {
                //SVM作成
                cv::Ptr<cv::ml::SVM> svm = cv::ml::SVM::create();
                svm->setType(cv::ml::SVM::C_SVC);
                //RBFカーネル
                svm->setKernel(cv::ml::SVM::RBF);
                svm->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER, 100, 1e-6));
                //ここを変動
                svm->setC(C);
                svm->setGamma(gamma);
                svm->train(train_data);
                //予測
                cv::Mat pred;
                svm->predict(data_te, pred);
                EvalResult result = evaluate(pred, labels_te);
                cout << result.accuracy << "%\t";
                //最良がどれかを保存
                if(result.accuracy > best_acc)
                {
                    best_acc    = result.accuracy;
                    best_C      = C;
                    best_gamma  = gamma;
                    best_result = result;
                }
            }
            cout << "\n";
        }
        //最良の結果について出力
        cout << "\n--- RBFカーネル 最良パラメータ ---\n";
        cout << "C=" << best_C << ", gamma=" << best_gamma << "\n";
        printConfusionMatrix(best_result);
    }

    // ==========================================================
    // (2)多項式カーネル、Cとdegreeをグリッドサーチ
    {
        //こちらもRBFカーネルと同様に処理
        vector<double> C_values      = {0.01, 0.1, 1.0, 10.0, 100.0};
        vector<double> degree_values = {1.0, 2.0, 3.0, 4.0, 5.0};

        double best_acc    = -1.0;
        double best_C      = 0.0;
        double best_degree = 0.0;
        EvalResult best_result = {0, 0, 0, 0, 0, 0, 0.0};

        cout << "\n多項式カーネル パラメータチューニング結果\n";
        cout << fixed << setprecision(2);

        cout << "C \\ degree\t";
        for(double d : degree_values)
            cout << (int)d << "\t";
        cout << "\n";

        for(double C : C_values)
        {
            cout << C << "\t\t";
            for(double degree : degree_values)
            {
                cv::Ptr<cv::ml::SVM> svm = cv::ml::SVM::create();
                svm->setType(cv::ml::SVM::C_SVC);
                //多項式カーネルを選択
                svm->setKernel(cv::ml::SVM::POLY);
                svm->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER, 100, 1e-6));
                //この2つを変動させる
                svm->setC(C);
                svm->setDegree(degree);
                svm->setGamma(1.0);
                svm->setCoef0(1.0);
                svm->train(train_data);

                cv::Mat pred;
                svm->predict(data_te, pred);
                EvalResult result = evaluate(pred, labels_te);

                cout << result.accuracy << "%\t";

                if(result.accuracy > best_acc)
                {
                    best_acc    = result.accuracy;
                    best_C      = C;
                    best_degree = degree;
                    best_result = result;
                }
            }
            cout << "\n";
        }

        cout << "\n--- 多項式カーネル 最良パラメータ ---\n";
        cout << "C=" << best_C << ", degree=" << (int)best_degree << "\n";
        printConfusionMatrix(best_result);
    }

    return(0);
}
