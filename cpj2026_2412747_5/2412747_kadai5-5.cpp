#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <opencv2/opencv.hpp>
using namespace std;

//評価結果のための構造体
struct EvalResult {
    int TP, TN, FP, FN;
    int total, correct;
    double accuracy;
};

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

//各クラスの特徴量ごとの平均・分散を計算
void computeMeanVar(const cv::Mat& data, cv::Mat& mean, cv::Mat& var)
{
    int rows = data.rows;
    int cols = data.cols;
    mean = cv::Mat::zeros(1, cols, CV_64F);
    var  = cv::Mat::zeros(1, cols, CV_64F);

    //平均の計算
    for(int i = 0; i < rows; i++)
        for(int j = 0; j < cols; j++)
            mean.at<double>(0, j) += data.at<float>(i, j);
    mean /= rows;

    //分散の計算
    for(int i = 0; i < rows; i++)
        for(int j = 0; j < cols; j++)
        {
            double diff = data.at<float>(i, j) - mean.at<double>(0, j);
            var.at<double>(0, j) += diff * diff;
        }
    var /= rows;
}

//ガウス分布の対数確率密度を計算
double logGaussian(double x, double mean, double var)
{
    //分散が極端に小さい場合の数値保護
    if(var < 1e-10)
        var = 1e-10;
    return(-0.5 * log(2.0 * M_PI * var) - (x - mean) * (x - mean) / (2.0 * var));
}

//ベイズ識別器による予測(ラベルは通常を1、スパムを-1とした)
EvalResult bayesPredict(cv::Mat& data_te, cv::Mat& labels_te, cv::Mat& normal_mean, cv::Mat& normal_var, cv::Mat& spam_mean, cv::Mat& spam_var, double log_prior_normal, double log_prior_spam)
{
    EvalResult result = {0, 0, 0, 0, 0, 0, 0.0};
    int n_features = data_te.cols;

    for(int i = 0; i < data_te.rows; i++)
    {
        // 対数事後確率 = 対数事前確率 + 各特徴量の対数尤度の和
        double log_post_normal = log_prior_normal;
        double log_post_spam = log_prior_spam;

        for(int j = 0; j < n_features; j++)
        {
            double x = data_te.at<float>(i, j);
            log_post_normal += logGaussian(x, normal_mean.at<double>(0, j), normal_var.at<double>(0, j));
            log_post_spam += logGaussian(x, spam_mean.at<double>(0, j), spam_var.at<double>(0, j));
        }

        // 対数事後確率が大きいクラスに分類
        int predicted = (log_post_normal >= log_post_spam) ? 1 : -1;
        int actual = labels_te.at<int>(i, 0);

        if (actual ==  1 && predicted ==  1) result.TP++;
        else if(actual == -1 && predicted == -1) result.TN++;
        else if(actual == -1 && predicted ==  1) result.FP++;
        else if(actual ==  1 && predicted == -1) result.FN++;
    }

    result.total = data_te.rows;
    result.correct = result.TP + result.TN;
    result.accuracy = (double)result.correct / result.total * 100.0;
    return(result);
}

int main(int argc, char* argv[])
{
    // 引数エラー処理
    if(argc != 3)
    {
        cerr << "引数の数が正しくありません。\n";
        return(1);
    }

    //ファイル名の定義
    string dirname = argv[1];
    if(dirname.back() != '/')
        dirname += '/';
    int n_train = stoi(argv[2]);
    //ディレクトリ内のファイル
    string file_normal_tr = dirname + "Nmail2026.csv";
    string file_spam_tr = dirname + "Amail2026.csv";
    string file_normal_te = dirname + "Nmail_t2026.csv";
    string file_spam_te = dirname + "Amail_t2026.csv";

    //学習データの読み込み
    cv::Ptr<cv::ml::TrainData> normal_tr_raw = cv::ml::TrainData::loadFromCSV(file_normal_tr, 0, -2, 0);
    cv::Ptr<cv::ml::TrainData> spam_tr_raw = cv::ml::TrainData::loadFromCSV(file_spam_tr, 0, -2, 0);
    //特徴量の切り出し
    cv::Mat normal_all = normal_tr_raw->getSamples();
    cv::Mat spam_all = spam_tr_raw->getSamples();
    // 行数チェック
    if(n_train <= 0 || n_train > 200)
    {
        cerr << "学習行数が範囲外です。1以上200以下の値を指定してください。\n";
        return(1);
    }

    //先頭n行を学習データとして使用
    cv::Mat normal_data = normal_all.rowRange(0, n_train).clone();
    cv::Mat spam_data = spam_all.rowRange(0, n_train).clone();
    normal_data.convertTo(normal_data, CV_32F);
    spam_data.convertTo(spam_data, CV_32F);

    //データの平均と分散を計算
    cv::Mat normal_mean, normal_var;
    cv::Mat spam_mean, spam_var;
    computeMeanVar(normal_data, normal_mean, normal_var);
    computeMeanVar(spam_data, spam_mean, spam_var);

    //事前確率の計算
    int total_tr = 2 * n_train;
    double log_prior_normal = log((double)n_train / total_tr);
    double log_prior_spam = log((double)n_train / total_tr);

    //テストデータの読み込み
    cv::Ptr<cv::ml::TrainData> normal_te_raw = cv::ml::TrainData::loadFromCSV(file_normal_te, 0, -2, 0);
    cv::Ptr<cv::ml::TrainData> spam_te_raw =cv::ml::TrainData::loadFromCSV(file_spam_te, 0, -2, 0);
    cv::Mat normal_te = normal_te_raw->getSamples();
    cv::Mat spam_te = spam_te_raw->getSamples();
    //テストデータを結合
    cv::Mat data_te;
    cv::vconcat(normal_te, spam_te, data_te);
    data_te.convertTo(data_te, CV_32F);
    //ラベルの作成(通常:1、スパム:-1)
    cv::Mat labels_te(normal_te.rows + spam_te.rows, 1, CV_32SC1);
    labels_te.rowRange(0, normal_te.rows).setTo(1);
    labels_te.rowRange(normal_te.rows, normal_te.rows + spam_te.rows).setTo(-1);

    //テストデータの予測
    EvalResult result = bayesPredict(data_te, labels_te, normal_mean, normal_var,spam_mean, spam_var, log_prior_normal, log_prior_spam);
    //結果を出力
    cout << "学習データ数: " << n_train << " 行\n";
    printConfusionMatrix(result);

    return(0);
}
