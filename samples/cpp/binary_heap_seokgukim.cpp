// 간단한 binary heap 예시
#include <iostream>
using namespace std;

class my_heap
{                                     // 정수 최대 힙 클래스
    int *internal_arr, internal_size; // 내부에서 사용할 배열과 사이즈

    inline int parent_node(int idx)
    { // 부모 노드 반환
        return idx / 2;
    }

    inline int left_node(int idx)
    { // 왼쪽 자식 노드 반환
        if (idx * 2 > internal_size)
            return 0;
        return idx * 2;
    }

    inline int right_node(int idx)
    { // 오른쪽 자식 노드 반환
        if (idx * 2 + 1 > internal_size)
            return 0;
        return idx * 2 + 1;
    }

    void max_heapify(int idx)
    { // 최대 힙으로 만드는 내부 함수
        int l = left_node(idx), r = right_node(idx);

        int maximum_idx = idx; // 최댓값의 인덱스 찾기
        if (l > 0 && l <= internal_size && internal_arr[l] > internal_arr[idx])
            maximum_idx = l;
        if (r > 0 && r <= internal_size && internal_arr[r] > internal_arr[maximum_idx])
            maximum_idx = r;

        if (maximum_idx != idx)
        { // 현재 노드가 최댓값이 아닌 경우 스왑 후 재귀
            int tmp = internal_arr[maximum_idx];
            internal_arr[maximum_idx] = internal_arr[idx];
            internal_arr[idx] = tmp;
            max_heapify(maximum_idx);
        }
    }

    void build_max_heap()
    { // 최대 힙 정렬 상태 유지
        for (int idx = internal_size / 2; idx > 0; idx--)
            max_heapify(idx);
    }

    void heap_increase_key(int idx, int key)
    { // 힙의 idx번쨰 원소를 key로 증가시키고 힙 정렬 상태 유지
        if (key < internal_arr[idx])
        {
            std::cout << "Internal heap error: New key must be larger.\n";
            throw 1;
        }
        internal_arr[idx] = key;
        while (idx > 1 && internal_arr[parent_node(idx)] < internal_arr[idx])
        {
            int tmp = internal_arr[parent_node(idx)];
            internal_arr[parent_node(idx)] = internal_arr[idx];
            internal_arr[idx] = tmp;
            idx = parent_node(idx);
        }
    }

public:
    my_heap()
    { // 생성자. 기본 사이즈는 0이다
        internal_arr = new int[1];
        internal_size = 0;
    }

    bool empty()
    { // 힙이 비어있는지
        return !internal_size;
    }

    int size()
    { // 힙의 사이즈
        return internal_size;
    }

    void push(int x)
    {                    // 힙에 삽입 연산
        internal_size++; // 내부 사이즈 증가
        if (__builtin_popcount(internal_size) <= 1)
        { // 증가한 내부 사이즈가 2의 배수인 경우 내부 배열을 재조정
            int *new_arr = new int[internal_size * 2];
            for (int i = 1; i < internal_size; i++)
                new_arr[i] = internal_arr[i];
            free(internal_arr);
            internal_arr = new_arr;
        }
        // 말단 노드에 int min값을 삽입 후 x값으로 늘려줌
        internal_arr[internal_size] = 1 << 31;
        heap_increase_key(internal_size, x);
    }

    void pop()
    { // 제거 연산
        if (empty())
        { // 힙이 비면 오류
            std::cout << "Heap pop error: No elements in the Heap.\n";
            throw 1;
        }
        // 맨 끝의 노드와 스왑해주고 맨 끝 노드는 없는 셈 침
        internal_arr[1] = internal_arr[internal_size];
        if (__builtin_popcount(internal_size) <= 1)
        { // 줄어들기 전 사이즈가 2의 배수인 경우에 내부 배열 사이즈를 재조정
            int *new_arr = new int[internal_size / 2];
            for (int i = 1; i < internal_size; i++)
                new_arr[i] = internal_arr[i];
            free(internal_arr);
            internal_arr = new_arr;
        }
        // 사이즈를 줄이고 최대 힙 정렬을 유지
        internal_size--;
        max_heapify(1);
    }

    int top()
    { // 최댓값 반환
        if (empty())
        { // 비어있다면 오류
            std::cout << "Heap top error: No elements in the Heap.\n";
            throw 1;
        }
        return internal_arr[1];
    }

    int top_pop()
    { // 최댓값을 반환하면서 제거
        int ret = top();
        pop();
        return ret;
    }
};