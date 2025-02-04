// 간단한 Treap
#include <iostream>

class my_treap
{
    typedef int key_type;

    struct t_node
    {
        key_type key;
        size_t p, sz;
        t_node *l = NULL, *r = NULL;

        t_node(const key_type &k) : key(k), p(rand()), sz(1) {}

        void calc_sz()
        {
            sz = 1;
            if (l)
                sz += l->sz;
            if (r)
                sz += r->sz;
        }
        void set_left(t_node *nl) { l = nl, calc_sz(); };
        void set_right(t_node *nr) { r = nr, calc_sz(); };
    };

    t_node *rt;

    typedef ::std::pair<t_node *, t_node *> pnn;
    pnn split(t_node *root, key_type key)
    { // first는 key 보다 작은 서브트리, second는 key 보다 큰 서브트리
        if (root == NULL)
            return pnn(NULL, NULL);

        if (root->key < key)
        {
            pnn r_sub = split(root->r, key);
            root->set_right(r_sub.first);
            return pnn(root, r_sub.second);
        }

        pnn l_sub = split(root->l, key);
        root->set_left(l_sub.second);
        return pnn(l_sub.first, root);
    }

    t_node *i_insert(t_node *root, t_node *node)
    {
        if (root == NULL)
            return node;

        if (root->p < node->p)
        {
            pnn got_split = split(root, node->key);
            node->set_left(got_split.first);
            node->set_right(got_split.second);
            return node;
        }
        else if (root->key > node->key)
        {
            root->set_left(i_insert(root->l, node));
        }
        else
        {
            root->set_right(i_insert(root->r, node));
        }

        return root;
    }

    t_node *i_merge(t_node *a, t_node *b)
    { // a 서브트리는 모든 b 서브트리보단 작음
        if (a == NULL)
            return b;
        if (b == NULL)
            return a;
        if (a->p < b->p)
        {
            b->set_left(i_merge(a, b->l));
            return b;
        }
        a->set_right(i_merge(a->r, b));
        return a;
    }

    t_node *i_erase(t_node *root, key_type key)
    {
        if (root == NULL)
            return root;

        if (root->key == key)
        {
            t_node *ret = i_merge(root->l, root->r);
            delete root;
            return ret;
        }
        else if (key < root->key)
        {
            root->set_left(i_erase(root->l, key));
        }
        else
        {
            root->set_right(i_erase(root->r, key));
        }

        return root;
    }

    t_node *i_k_th(t_node *root, size_t k)
    {
        size_t l_sz = root->l ? root->l->sz : 0;
        if (k <= l_sz)
            return i_k_th(root->l, k);
        if (k == l_sz + 1)
            return root;
        return i_k_th(root->r, k - l_sz - 1);
    }

    size_t i_count_less_than(t_node *root, key_type key)
    {
        if (root == NULL)
            return 0;
        if (root->key >= key)
            return i_count_less_than(root->l, key);
        size_t l_sz = root->l ? root->l->sz : 0;
        return l_sz + 1 + i_count_less_than(root->r, key);
    }

public:
    my_treap() : rt(NULL) {}

    bool empty()
    {
        if (rt)
            return false;
        return true;
    }

    size_t size()
    {
        if (rt)
            return rt->sz;
        return 0;
    }

    void insert(const key_type &key)
    {
        rt = i_insert(rt, new t_node(key));
    }

    void erase(const key_type &key)
    {
        rt = i_erase(rt, key);
    }

    t_node *k_th(size_t k)
    {
        return i_k_th(rt, k);
    }

    size_t count_less_than(key_type key)
    {
        return i_count_less_than(rt, key);
    }
};

int main()
{
    my_treap mtmt;
    for (int i = 0; i < 10; i++)
    {
        int x = rand();
        mtmt.insert(x);
        std::cout << x << '\n';
    }

    std::cout << mtmt.size() << '\n';
    std::cout << mtmt.k_th(3)->key << '\n';
    std::cout << mtmt.count_less_than(3000) << '\n';
}