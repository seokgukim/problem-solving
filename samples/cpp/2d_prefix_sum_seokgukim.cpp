// 2d prefix sum by seokgukim
#include <iostream>
#include <vector>

int get_range_sum(std::vector<std::vector<int>> &psum, int x1, int y1, int x2, int y2) {
    //2D prefix sum 쿼리
    return psum[x2][y2] - psum[x1 - 1][y2] - psum[x2][y1 - 1] + psum[x1 - 1][y1 - 1];
}

void make_prefix_sum(std::vector<std::vector<int>> &psum) {
    // 2D prefix sum을 1-based로 전처리
    for (int i = 1; i < psum.size(); i++) {
        for (int j = 1; j < psum[i].size(); j++) {
            psum[i][j] += psum[i - 1][j] + psum[i][j - 1] - psum[i - 1][j - 1];
        }
    }    
}

// int main() {
//    int N, M; cin >> N >> M;
//    vector<vector<int>> v(N + 1, vector<int> (M + 1));
//     
//    문제 조건대로 입력받기...
// 
//    make_prefix_sum(v);
// }