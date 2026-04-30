#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <cmath>

using namespace std;

static const int PERSONS   = 300;
static const int NUM       = 5;
static const int DIMENSION = 1000;

static double avg[PERSONS][DIMENSION];
static double test_data[DIMENSION];

// カンマ区切りの1000次元ベクトルを読み込む（ID行・空行はスキップ）
bool read_next_vector(ifstream &fin, double *out) {
    string line;
    while (getline(fin, line)) {
        if (line.find(',') == string::npos) continue; // ID行・空行をスキップ

        stringstream ss(line);
        string token;
        int cnt = 0;
        while (cnt < DIMENSION && getline(ss, token, ','))
            out[cnt++] = atof(token.c_str());

        if (cnt == DIMENSION) return true;
    }
    return false;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cerr << "使用法: " << argv[0] << " enrollment2026.txt input2026.txt" << endl;
        return 1;
    }

    // --- 学習フェーズ: 各人物の平均ベクトルを作成 ---
    ifstream fin1(argv[1]);
    if (!fin1) { cerr << "学習ファイルが開けません: " << argv[1] << endl; return 1; }

    for (int i = 0; i < PERSONS; i++) {
        for (int j = 0; j < NUM; j++) {
            if (!read_next_vector(fin1, test_data)) {
                cerr << "学習データ不足: person=" << i << ", sample=" << j << endl;
                return 1;
            }
            for (int k = 0; k < DIMENSION; k++)
                avg[i][k] += test_data[k];
        }
        for (int k = 0; k < DIMENSION; k++)
            avg[i][k] /= NUM;
    }
    fin1.close();

    // --- 識別フェーズ: 最近傍法（ユークリッド距離の二乗）で識別 ---
    ifstream fin2(argv[2]);
    if (!fin2) { cerr << "識別ファイルが開けません: " << argv[2] << endl; return 1; }

    int correct = 0, total = 0;
    while (total < PERSONS && read_next_vector(fin2, test_data)) {
        double min_dist = -1.0;
        int predicted = -1;
        for (int p = 0; p < PERSONS; p++) {
            double dist = 0.0;
            for (int d = 0; d < DIMENSION; d++) {
                double diff = test_data[d] - avg[p][d];
                dist += diff * diff;
            }
            if (min_dist < 0 || dist < min_dist) { min_dist = dist; predicted = p; }
        }
        if (predicted == total) correct++;
        total++;
    }
    fin2.close();

    // --- 結果出力 ---
    cout << "正解数: " << correct << " / " << total << endl;
    cout << "識別率: " << fixed << setprecision(2)
         << (total > 0 ? (double)correct / total * 100.0 : 0.0) << " %" << endl;

    return 0;
}
