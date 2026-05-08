#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char* argv[])
{
    //引数エラー処理
    if(argc != 5)
    {
        cerr << "引数の数が正しくありません。4つの引数を入力してください。\n";
        return(1);
    }

    //学習データの処理
    //CSVファイルの読み込み
    cv::Ptr<cv::ml::TrainData> tr1= cv::ml::TrainData::loadFromCSV(argv[1], 0, -2, 0);
    cv::Ptr<cv::ml::TrainData> tr2= cv::ml::TrainData::loadFromCSV(argv[2], 0, -2, 0);
    //データの切り出し
    cv::Mat data_tr1(200, 2, CV_64FC1);
    cv::Mat data_tr2(200, 2, CV_64FC1);
    data_tr1 = tr1->getSamples();
    data_tr2 = tr2->getSamples();
    //データのラベル作成
    

    //識別のためのSVMの構築
    cv::Ptr<cv::ml::SVM> svm = cv::ml::SVM::create();
    //SVMパラメータの設定
    svm->setType(cv::ml::SVM::C_SVC);
    svm->setKernel(cv::ml::SVM::LINEAR);
    svm->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER, 100, 1e-6));
    svm->setC(1.0);
    
    //テストデータ
    //CSVファイルの読み込み
    cv::Ptr<cv::ml::TrainData> te1= cv::ml::TrainData::loadFromCSV(argv[3], 0, -2, 0);
    cv::Ptr<cv::ml::TrainData> te2= cv::ml::TrainData::loadFromCSV(argv[4], 0, -2, 0);
    //データの切り出し
    cv::Mat data_te1(50, 2, CV_64FC1);
    cv::Mat data_te2(50, 2, CV_64FC1);
    data_te1 = te1->getSamples();
    data_te2 = te2->getSamples();
    return(0);
}
