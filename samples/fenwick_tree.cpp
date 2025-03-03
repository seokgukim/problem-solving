// fenwick tree by seokgukim
#include <iostream>

class my_fenwick_tree {
    int sz, *internal_arr;
    // 인덱스 넘어갈 경우 예외 처리
    void index_error() {
        std::cout << "Error: Index must be in range of a Fenwick Tree.";
        throw 1;
    }
    public:
        // 크기 n의 펜윅 트리 생성(1-based index)
        my_fenwick_tree(int n) {
            sz = n;
            internal_arr = new int[n + 1]();
        }
        // 기본 생성자는 크기 100으로 생성
        my_fenwick_tree() {
            sz = 100;
            internal_arr = new int[101]();
        }
        // index [1, i] 구간의 합
        int sum(int i) {
            if (i < 1 || i > sz) index_error();
            int ret = 0;
            while (i > 0) {
                ret += internal_arr[i];
                i -= i & -i;
            }
            return ret;
        }
        // index i에 x만큼 값을 더함
        void update(int i, int x) {
            if (i < 1 || i > sz) index_error();
            while (i <= sz) {
                internal_arr[i] += x;
                i += i & -i;
            }
        }
        // index [l, r] 구간의 합
        int range_sum(int l, int r) {
            return sum(r) - sum(l - 1);
        }
        // 내부적으로 저장된 값 반환
        int raw_point(int i) {
            if (i < 1 || i > sz) index_error();
            return internal_arr[i];
        }
        // 크기
        int size() {
            return sz;
        }
};

// int main() {
//     my_fenwick_tree fw;
//     fw.update(3, 1);
//     fw.update(3, 1);
//     fw.update(3, 1);
//     fw.update(6, 12);
//     fw.update(3, 1);
//     fw.update(4, 18);
//     std::cout << fw.sum(3);
//     std::cout << fw.sum(6);
// }