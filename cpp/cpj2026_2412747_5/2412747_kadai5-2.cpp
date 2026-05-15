#include <iostream>
#include <opencv2/opencv.hpp>
using namespace std;
//ウィンドウの名称
string win_dst = "destination";

int main(int argc, char* argv[])
{
    //引数エラー処理
    if(argc != 3)
    {
        cerr << "引数に閾値処理元の画像名と閾値の2つの引数を指定してください。\n";
        return(1);
    }

    //画像の読み込み
    cv::Mat src_img = cv::imread(argv[1]);
    if(!src_img.data)
    {
        cerr << "画像の読み込みに失敗しました。";
        return(1);
    }
    //グレースケールの画像
    cv::Mat gray_img;
    //出力結果の画像
    cv::Mat dst_img;

    //閾値の設定
    int threshold_val = stoi(argv[2]);
    if(threshold_val < 0 || threshold_val > 255)
    {
        cerr << "閾値は0~255以内で入力してください。\n";
        return(1);
    }
    //グレースケール化
    cv::cvtColor(src_img, gray_img, cv::COLOR_BGR2GRAY);
    //閾値による2値化
    cv::threshold(gray_img, dst_img, threshold_val, 255, cv::THRESH_BINARY);
    //ウィンドウ生成
    cv::namedWindow(win_dst, cv::WINDOW_AUTOSIZE);
    //画像の表示
    cv::imshow(win_dst, dst_img);
    //画像の保存
    cv::imwrite("result.bmp", dst_img);
    //キー入力待ち
    cv::waitKey(0);
    return(0);
}
