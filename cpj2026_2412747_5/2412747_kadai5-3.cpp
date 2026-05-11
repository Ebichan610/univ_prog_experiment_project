#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;

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
    cv::Mat data_tr1 = tr1->getSamples();
    cv::Mat data_tr2 = tr2->getSamples();
    cv::Mat data_tr;
    cv::vconcat(data_tr1, data_tr2, data_tr);

    //データのラベル作成
    cv::Mat labels_tr(data_tr1.rows + data_tr2.rows, 1, CV_32SC1);
    //クラス1は1
    labels_tr.rowRange(0, data_tr1.rows).setTo(1);
    //クラス2は2
    labels_tr.rowRange(data_tr1.rows, data_tr1.rows + data_tr2.rows).setTo(2);

    //識別のためのSVMの構築
    cv::Ptr<cv::ml::SVM> svm = cv::ml::SVM::create();
    //SVMパラメータの設定
    svm->setType(cv::ml::SVM::C_SVC);
    //線形を指定
    svm->setKernel(cv::ml::SVM::LINEAR);
    svm->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER, 100, 1e-6));
    svm->setC(1.0);
    //学習
    data_tr.convertTo(data_tr, CV_32F);
    cv::Ptr<cv::ml::TrainData> train_data = cv::ml::TrainData::create(
        data_tr, cv::ml::ROW_SAMPLE, labels_tr);
    svm->train(train_data);
    
    //テストデータ
    //CSVファイルの読み込み
    cv::Ptr<cv::ml::TrainData> te1= cv::ml::TrainData::loadFromCSV(argv[3], 0, -2, 0);
    cv::Ptr<cv::ml::TrainData> te2= cv::ml::TrainData::loadFromCSV(argv[4], 0, -2, 0);
    //データの切り出し
    cv::Mat data_te1 = te1->getSamples();
    cv::Mat data_te2 = te2->getSamples();
    cv::Mat data_te;
    cv::vconcat(data_te1, data_te2, data_te);
    //ラベルの作成
    cv::Mat labels_te(data_te1.rows + data_te2.rows, 1, CV_32SC1);
    labels_te.rowRange(0, data_te1.rows).setTo(1);
    labels_te.rowRange(data_te1.rows, data_te1.rows + data_te2.rows).setTo(2);

    //予測
    cv::Mat pred;
    data_te.convertTo(data_te, CV_32F);
    svm->predict(data_te, pred);
    //評価
    //予測と実際を比較して判定
    //(True or False) (Positive or Negative)の頭文字を利用
    int TP = 0, TN = 0, FP = 0, FN = 0;
    for(int i = 0; i < pred.rows; i++)
    {
        int predicted = (int)pred.at<float>(i, 0);
        int actual    = labels_te.at<int>(i, 0);

        //クラス1を正しく判定
        if(actual == 1 && predicted == 1) TP++;
        //クラス2を正しく判定
        else if(actual == 2 && predicted == 2) TN++;
        //クラス2を誤って判定
        else if(actual == 2 && predicted == 1) FP++;
        //クラス1を誤って判定
        else if(actual == 1 && predicted == 2) FN++;
    }

    //ここで判別率を計算
    int total   = pred.rows;
    int correct = TP + TN;
    double accuracy = (double)correct / total * 100.0;

    //最後に行列形式で出力
    cout << "テストデータ数:\t" << total << '\n';
    cout << "正解数:\t" << correct << '\n';
    cout << "正解率:\t" << accuracy << "%\n\n";
    cout << "\t\t予測:クラス1 \t予測:クラス2\n";
    cout << "実際:クラス1\t\t" << TP << "\t\t" << FN << '\n';
    cout << "実際:クラス2\t\t" << FP << "\t\t" << TN << '\n';

    return(0);
}
