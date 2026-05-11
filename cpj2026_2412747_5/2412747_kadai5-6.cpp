#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
using namespace std;

//HSVヒストグラム特徴量の抽出
//各8次元、合わせて24次元
const int H_BINS = 8;
const int S_BINS = 8;
const int V_BINS = 8;
const int FEATURE_DIM = H_BINS + S_BINS + V_BINS;

cv::Mat extractFeature(const cv::Mat& img_bgr)
{
    //BGRからHSVに変換
    cv::Mat img_hsv;
    cv::cvtColor(img_bgr, img_hsv, cv::COLOR_BGR2HSV);

    //HSVをそれぞれに分離
    vector<cv::Mat> hsv;
    cv::split(img_hsv, hsv);
    cv::Mat H = hsv[0];
    cv::Mat S = hsv[1];
    cv::Mat V = hsv[2];
    //総ピクセル数
    int total_pixels = img_bgr.rows * img_bgr.cols;
    //特徴量行列を0初期化で生成
    cv::Mat feature = cv::Mat::zeros(1, FEATURE_DIM, CV_32F);

    //Hヒストグラム(H:0~179)
    for(int i = 0; i < H.rows; i++)
        for(int j = 0; j < H.cols; j++)
        {
            int bin = (int)H.at<unsigned char>(i, j) * H_BINS / 180;
            if(bin >= H_BINS)
                bin = H_BINS - 1;
            feature.at<float>(0, bin)++;
        }

    //Sヒストグラム(S:0~255)
    for(int i = 0; i < S.rows; i++)
        for(int j = 0; j < S.cols; j++)
        {
            int bin = (int)S.at<unsigned char>(i, j) * S_BINS / 256;
            if(bin >= S_BINS)
                bin = S_BINS - 1;
            feature.at<float>(0, H_BINS + bin)++;
        }

    //Vヒストグラム(V:0~255)
    for(int i = 0; i < V.rows; i++)
        for(int j = 0; j < V.cols; j++)
        {
            int bin = (int)V.at<unsigned char>(i, j) * V_BINS / 256;
            if(bin >= V_BINS)
                bin = V_BINS - 1;
            feature.at<float>(0, H_BINS + S_BINS + bin)++;
        }

    //正規化
    feature /= (float)total_pixels;
    return(feature);
}

//ラベルと名前の対応(野球が1、バスケが2、サッカーが3)
string labelToName(int label)
{
    if(label == 1)
        return("野球ボール");
    else if(label == 2)
        return("バスケットボール");
    else if(label == 3)
        return("サッカーボール");
    else
        return "不明";
}

int main(int argc, char* argv[])
{
    // 引数エラー処理
    if(argc != 2)
    {
        cerr << "引数の数が正しくありません。\n";
        return(1);
    }
    //ファイル名の構成
    string dirname = argv[1];
    if(dirname.back() != '/')
        dirname += '/';

    //学習画像ファイル名の定義(ラベルは野球が1、バスケが2、サッカーが3)
    //ファイル名とラベルを対応させるためpair
    vector<pair<string, int>> train_files =
    {
        {dirname + "baseball1.ppm", 1},
        {dirname + "baseball2.ppm", 1},
        {dirname + "baseball3.ppm", 1},
        {dirname + "basketball1.ppm", 2},
        {dirname + "basketball2.ppm", 2},
        {dirname + "basketball3.ppm", 2},
        {dirname + "soccer1.ppm", 3},
        {dirname + "soccer2.ppm", 3},
        {dirname + "soccer3.ppm", 3}
    };

    //学習データ行列とラベル行列の作成
    cv::Mat data_tr(0, FEATURE_DIM, CV_32F);
    cv::Mat labels_tr(0, 1, CV_32SC1);
    
    //学習データの特徴量抽出
    //vectorから一つずつ学習画像を読み込む
    for(auto& [filename, label] : train_files)
    {
        cv::Mat img = cv::imread(filename);
        if(img.empty())
        {
            cerr << "画像の読み込みに失敗しました: " << filename << "\n";
            return(1);
        }
        cv::Mat feat = extractFeature(img);
        //学習データ行列へ格納
        data_tr.push_back(feat);
        //ラベルも格納
        labels_tr.push_back(cv::Mat(1, 1, CV_32SC1, &label));
    }

    //RBFカーネルを選択
    //パラメータの選定を行う
    vector<double> C_values     = {0.1, 1.0, 10.0, 100.0, 1000.0};
    vector<double> gamma_values = {0.001, 0.01, 0.1, 1.0, 10.0};
    //初期化
    double best_acc   = -1.0;
    double best_C     = 1.0;
    double best_gamma = 0.1;
    //調査結果を表形式で出力
    cout << fixed << setprecision(2);
    cout << "C \\ gamma\t";
    for(double g : gamma_values)
        cout << g << "\t";
    cout << "\n";

    //全てのCのパターンで試す
    for(double C : C_values)
    {
        cout << C << "\t\t";
        //全てのgammaのパターンで試す
        for(double gamma : gamma_values)
        {
            //1つ抜き交差検証(LOOCV)の実行
            int correct = 0;
            int n = data_tr.rows;
            //テストデータの選択
            for(int leave = 0; leave < n; leave++)
            {
                //leaveを除いた学習データを作成
                cv::Mat tr_fold(0, FEATURE_DIM, CV_32F);
                cv::Mat lb_fold(0, 1, CV_32SC1);
                for(int k = 0; k < n; k++)
                {
                    //テストデータはスキップ
                    if(k == leave)
                        continue;
                    //LOOCV用の特徴量とラベルデータ
                    tr_fold.push_back(data_tr.row(k));
                    cv::Mat lb_row = labels_tr.row(k).clone();
                    lb_fold.push_back(lb_row);
                }
                //SVMの作成
                cv::Ptr<cv::ml::SVM> svm_cv = cv::ml::SVM::create();
                svm_cv->setType(cv::ml::SVM::C_SVC);
                svm_cv->setKernel(cv::ml::SVM::RBF);
                svm_cv->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER, 1000, 1e-6));
                svm_cv->setC(C);
                svm_cv->setGamma(gamma);
                //学習
                cv::Ptr<cv::ml::TrainData> td = cv::ml::TrainData::create(tr_fold, cv::ml::ROW_SAMPLE, lb_fold);
                svm_cv->train(td);
                //除外していたテストデータを参照
                cv::Mat test_sample = data_tr.row(leave);
                //予測して精度を評価する
                cv::Mat pred;
                svm_cv->predict(test_sample, pred);
                int predicted = (int)pred.at<float>(0, 0);
                int actual    = labels_tr.at<int>(leave, 0);
                if(predicted == actual)
                    correct++;
            }

            //正解率を算出し、最良を決定
            double acc = (double)correct / n * 100.0;
            cout << acc << "%\t";
            if(acc > best_acc)
            {
                best_acc   = acc;
                best_C     = C;
                best_gamma = gamma;
            }
        }
        cout << "\n";
    }
    //どれが最良であったかの出力
    cout << "\n最良パラメータ: C=" << best_C << ", gamma=" << best_gamma << '\n';

    //最良パラメータでSVMを作成
    cv::Ptr<cv::ml::SVM> svm = cv::ml::SVM::create();
    svm->setType(cv::ml::SVM::C_SVC);
    svm->setKernel(cv::ml::SVM::RBF);
    svm->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER, 1000, 1e-6));
    svm->setC(best_C);
    svm->setGamma(best_gamma);
    //学習データによる学習
    cv::Ptr<cv::ml::TrainData> train_data = cv::ml::TrainData::create(data_tr, cv::ml::ROW_SAMPLE, labels_tr);
    svm->train(train_data);

    //unknownデータの識別
    cout << "\nunknown画像の識別結果\n";
    vector<string> unknown_files =
    {
        dirname + "unknown1.ppm",
        dirname + "unknown2.ppm",
        dirname + "unknown3.ppm",
    };

    for(auto& filename : unknown_files)
    {
        cv::Mat img = cv::imread(filename);
        if(img.empty())
        {
            cerr << "画像の読み込みに失敗しました: " << filename << "\n";
            //テストは続行
            continue;
        }
        cv::Mat feat = extractFeature(img);
        cv::Mat pred;
        svm->predict(feat, pred);
        int predicted = (int)pred.at<float>(0, 0);
        cout << filename << "\n" << "\t-> 識別結果: " << labelToName(predicted) << "\n";
    }

    return(0);
}
