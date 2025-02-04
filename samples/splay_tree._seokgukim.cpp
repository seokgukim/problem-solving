// 간단한 Splay Tree
#include <iostream>

template <typename k_type>
class my_splay_tree
{
    struct node
    { // 기본적인 위치 관계 노드들에 더해 키와 사이즈
        node *p, *l, *r;
        k_type key;
        size_t sz;

        node(const k_type &key) : key(key), p(nullptr), l(nullptr), r(nullptr), sz(1) {}
    };

    // 루트 노드
    node *rt;

    void update(node *x)
    { // 업데이트 해 줄 값이 있는 경우의 업데이트
        x->sz = 1;
        if (x->l)
            x->sz += x->l->sz;
        if (x->r)
            x->sz += x->r->sz;
    }

    void rotate(node *x)
    {
        // x : 위로 올릴 노드, p : 부모 노드
        // b : x가 p의 왼쪽 자식일때 x의 오른쪽 자식, 오른쪽 자식일 때 왼쪽 자식
        node *p = x->p, *b = nullptr;
        if (!p)
            return; // x가 루트 노드라면 종료
        if (x == p->l)
        {                    // x가 왼쪽 자식이라면
            p->l = b = x->r; // x의 오른쪽 자식이 p의 새로운 왼쪽 자식
            x->r = p;        // p를 x의 자식으로 이동
        }
        else
        {                    // x가 오른쪽 자식이라면
            p->r = b = x->l; // x의 왼쪽 자식이 p의 새로운 오른쪽 자식
            x->l = p;        // p를 x의 자식으로 이동
        }
        // 부모 재설정
        x->p = p->p;
        p->p = x;
        if (b)
            b->p = p;
        if (x->p)
            (p == x->p->l ? x->p->l : x->p->r) = x;
        else
            rt = x;
        // 사이즈 값 갱신
        update(p);
        update(x);
    }

    void splay(node *x, node *tg = nullptr)
    { // 실행 결과 노드 x 를 노드 tg 밑에 붙이는 splay 연산
      // tg가 널 포인터라면 루트가 됨
        while (x->p != tg)
        { // x의 부모가 tg가 될 때까지
            node *p = x->p, *gp = p->p;
            if (gp != tg)
            {
                if ((x == p->l) == (p == gp->l))
                    rotate(p); // Zig-Zig Step
                else
                    rotate(x); // Zig-Zag Step
            }
            rotate(x); // Zig Step
        }
    }

    void propa(node *x)
    {
        // lazy propagation
    }

public:
    my_splay_tree() : rt(nullptr) {}

    size_t size()
    { // 사이즈
        if (rt)
            return rt->sz;
        return 0;
    }

    bool empty()
    { // 비어있는지 여부
        return rt ? false : true;
    }

    void insert(k_type key)
    { // 삽입
        node *p = rt;
        node **pp; // 넣을 위치
        if (!p)
        { // 트리 루트가 nullprt이면 노드 바로 삽입
            rt = new node(key);
            return;
        }
        while (1)
        { // 삽입할 위치 탐색
            // if (key == p->key)
            //     return; // 중복 처리가 필요하다면 중복일때 삽입 안 함
            if (key < p->key)
            { // 현재 노드보다 작으면 왼쪽이 삽입 지점
                if (!p->l)
                { // 왼쪽이 비어있으면 삽입
                    pp = &p->l;
                    break;
                }
                p = p->l; // 비어있지 않다면 왼쪽 서브 트리에서 다시 탐색
            }
            else
            { // 현재 노드보다 크다면 오른쪽에 삽입 지점
                if (!p->r)
                {
                    pp = &p->r;
                    break;
                }
                p = p->r;
            }
        }
        //  새 노드 삽입
        node *x = new node(key);
        *pp = x;
        x->l = x->r = nullptr;
        x->p = p;
        // splay
        splay(x);
    }

    bool find(k_type key)
    { // 찾을 수 있다/없다
        node *p = rt;
        if (!p)
            return false; // 빈 트리는 바로 못찾음
        while (p)
        { // 탐색
            if (key == p->key)
                break; // 탐색 성공시 break
            if (key < p->key)
            { // 현재 노드보다 키 값이 더 작다
                if (!p->l)
                    break; // 탐색 실패
                p = p->l;
            }
            else
            { // 현재 노드보다 키 값이 더 크다
                if (!p->r)
                    break; // 탐색 실패
                p = p->r;
            }
        }
        // splay 연산
        splay(p);
        // 찾은 경우에 true
        return rt->key == key;
    }

    void erase(k_type key)
    { // 삭제
        if (!find(key))
            return; // 검색하고 없다면 종료
        // 있는 경우 삭제할 노드는 루트임
        node *p = rt;
        if (p->l && p->r)
        { // 자식 노드 2개
            rt = p->l;
            rt->p = nullptr; // 왼쪽 자식이 새로운 루트

            // 오른쪽 서브 트리를 왼쪽 서브 트리 아래에 삽입
            node *x = rt;
            while (x->r)
                x = x->r;
            x->r = p->r;
            p->r->p = x;
            delete p;
            // 추가 업데이트
            update(rt);
            return; // 노드 삭제
        }
        if (p->l)
        { // 자식이 왼쪽만 있는 경우
            rt = p->l;
            rt->p = nullptr; // 왼쪽 자식이 새로운 루트
            delete p;
            // 추가 업데이트
            update(rt);
            return; // 노드 삭제
        }
        if (p->r)
        { // 자식이 오른쪽만 있는 경우
            rt = p->r;
            rt->p = nullptr; // 오른쪽 자식이 새로운 루트
            delete p;
            // 추가 업데이트
            update(rt);
            return; // 노드 삭제
        }

        // 자신밖에 없을 경우
        delete p;
        rt = nullptr;
    }

    node *kth(size_t k)
    { // k번째 노드
        // 현재 보는 노드
        node *x = rt;
        // propa(x);
        while (1)
        {
            // 왼쪽 서브트리의 합들이 더 클 경우엔 타고 내려감
            while (x->l && x->l->sz > k)
                x = x->l;
            // 작아지는 순간 합계만큼 빼주고 더 타고 내려가야 하는 경우 우측 서브트리로 감
            if (x->l)
                k -= x->l->sz;
            if (!k--)
                break;
            x = x->r;
            // propa(x);
        }
        // splay 연산 후 루트 반환
        splay(x);
        return rt;
    }

    node *get_range(int s, int e)
    { // 구간 [s, e]를 모은다
        kth(e + 1);
        node *tmp = rt;
        kth(s - 1);
        splay(tmp, rt);
        return rt->r->l;
    }
};

int static compare (const void* first, const void* second)
{ // 대충 비교 함수 qsort 쓸거임
    if (*(int*)first > *(int*)second)
        return 1;
    else if (*(int*)first < *(int*)second)
        return -1;
    else
        return 0;
}

int main()
{
    my_splay_tree<int> mtmt;
    
    int arr[1000] = {0,};
    for (int i = 0; i < 1000; i++)
    {
        int x = rand();
        arr[i] = x;
        mtmt.insert(x);
    }
    

    std::cout << mtmt.size() << '\n';
    for (int i = 1000; i > 0; i--) {
        qsort(arr, 1000, sizeof(int), compare);
        bool f = 1;
        for (int j = 0; j < i; j++) {
            if (arr[j] != mtmt.kth(j)->key) {
                f = 0;
                break;
            }
        }
        
        std::cout << "Test #" << 1000 - i << ": " << (f ? "OK\n" : "FUCKED\n");
        int rn = rand() % i;
        mtmt.erase(arr[rn]);
        arr[rn] = 999999;
    }
}