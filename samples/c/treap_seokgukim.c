#define DEFINE_TREAP(TypeName, KeyType) \
    typedef struct treap_node_##TypeName { \
        KeyType key; \
        int priority; \
        int size; \
        struct treap_node_##TypeName *left, *right; \
    } treap_node_##TypeName; \
    \
    typedef struct treap_##TypeName { \
        treap_node_##TypeName *root; \
    } treap_##TypeName; \
    \
    typedef struct treap_split_result_##TypeName { \
        treap_node_##TypeName *first; \
        treap_node_##TypeName *second; \
    } treap_split_result_##TypeName; \
    \
    /* Forward declarations for internal static functions */ \
    static treap_node_##TypeName* _treap_node_new_##TypeName(KeyType k); \
    static void _treap_node_update_##TypeName(treap_node_##TypeName *node); \
    static void _treap_node_set_left_##TypeName(treap_node_##TypeName *node, treap_node_##TypeName *l); \
    static void _treap_node_set_right_##TypeName(treap_node_##TypeName *node, treap_node_##TypeName *r); \
    static treap_split_result_##TypeName _treap_split_##TypeName(treap_node_##TypeName *node, KeyType key); \
    static treap_node_##TypeName* _treap_merge_##TypeName(treap_node_##TypeName *left_node, treap_node_##TypeName *right_node); \
    static treap_node_##TypeName* _treap_insert_internal_##TypeName(treap_node_##TypeName *parent, treap_node_##TypeName *new_node_to_insert); \
    static treap_node_##TypeName* _treap_erase_internal_##TypeName(treap_node_##TypeName *parent, KeyType key); \
    static treap_node_##TypeName* _treap_kth_internal_##TypeName(treap_node_##TypeName *node, int k); \
    static int _treap_count_less_internal_##TypeName(treap_node_##TypeName *node, KeyType key); \
    \
    /* Function implementations */ \
    static treap_node_##TypeName* _treap_node_new_##TypeName(KeyType k) { \
        treap_node_##TypeName *new_node = (treap_node_##TypeName*)malloc(sizeof(treap_node_##TypeName)); \
        if (!new_node) { \
            /* fprintf(stderr, "Memory allocation failed for new treap node\n"); */ \
            return NULL; \
        } \
        new_node->key = k; \
        new_node->priority = rand(); \
        new_node->size = 1; \
        new_node->left = NULL; \
        new_node->right = NULL; \
        return new_node; \
    } \
    \
    static void _treap_node_update_##TypeName(treap_node_##TypeName *node) { \
        if (!node) return; \
        node->size = 1 + (node->left ? node->left->size : 0) + (node->right ? node->right->size : 0); \
    } \
    \
    static void _treap_node_set_left_##TypeName(treap_node_##TypeName *node, treap_node_##TypeName *l) { \
        if (!node) return; \
        node->left = l; \
        _treap_node_update_##TypeName(node); \
    } \
    \
    static void _treap_node_set_right_##TypeName(treap_node_##TypeName *node, treap_node_##TypeName *r) { \
        if (!node) return; \
        node->right = r; \
        _treap_node_update_##TypeName(node); \
    } \
    \
    static treap_split_result_##TypeName _treap_split_##TypeName(treap_node_##TypeName *node, KeyType key) { \
        if (!node) return (treap_split_result_##TypeName){NULL, NULL}; \
        if (node->key < key) { \
            treap_split_result_##TypeName res = _treap_split_##TypeName(node->right, key); \
            _treap_node_set_right_##TypeName(node, res.first); \
            return (treap_split_result_##TypeName){node, res.second}; \
        } \
        treap_split_result_##TypeName res = _treap_split_##TypeName(node->left, key); \
        _treap_node_set_left_##TypeName(node, res.second); \
        return (treap_split_result_##TypeName){res.first, node}; \
    } \
    \
    static treap_node_##TypeName* _treap_merge_##TypeName(treap_node_##TypeName *left_node, treap_node_##TypeName *right_node) { \
        if (!left_node || !right_node) return left_node ? left_node : right_node; \
        if (left_node->priority < right_node->priority) { \
            _treap_node_set_left_##TypeName(right_node, _treap_merge_##TypeName(left_node, right_node->left)); \
            return right_node; \
        } \
        _treap_node_set_right_##TypeName(left_node, _treap_merge_##TypeName(left_node->right, right_node)); \
        return left_node; \
    } \
    \
    static treap_node_##TypeName* _treap_insert_internal_##TypeName(treap_node_##TypeName *parent, treap_node_##TypeName *new_node_to_insert) { \
        if (!parent) return new_node_to_insert; \
        if (new_node_to_insert->priority > parent->priority) { \
            treap_split_result_##TypeName res = _treap_split_##TypeName(parent, new_node_to_insert->key); \
            _treap_node_set_left_##TypeName(new_node_to_insert, res.first); \
            _treap_node_set_right_##TypeName(new_node_to_insert, res.second); \
            return new_node_to_insert; \
        } \
        if (new_node_to_insert->key < parent->key) { \
            _treap_node_set_left_##TypeName(parent, _treap_insert_internal_##TypeName(parent->left, new_node_to_insert)); \
        } else { \
            _treap_node_set_right_##TypeName(parent, _treap_insert_internal_##TypeName(parent->right, new_node_to_insert)); \
        } \
        return parent; \
    } \
    \
    static treap_node_##TypeName* _treap_erase_internal_##TypeName(treap_node_##TypeName *parent, KeyType key) { \
        if (!parent) return NULL; \
        if (key < parent->key) { \
            _treap_node_set_left_##TypeName(parent, _treap_erase_internal_##TypeName(parent->left, key)); \
        } else if (key > parent->key) { \
            _treap_node_set_right_##TypeName(parent, _treap_erase_internal_##TypeName(parent->right, key)); \
        } else { \
            treap_node_##TypeName *merged_children = _treap_merge_##TypeName(parent->left, parent->right); \
            free(parent); \
            return merged_children; \
        } \
        return parent; \
    } \
    \
    static treap_node_##TypeName* _treap_kth_internal_##TypeName(treap_node_##TypeName *node, int k) { \
        if (!node || k >= node->size) return NULL; \
        int left_size = node->left ? node->left->size : 0; \
        if (k < left_size) { \
            return _treap_kth_internal_##TypeName(node->left, k); \
        } else if (k > left_size) { \
            return _treap_kth_internal_##TypeName(node->right, k - left_size - 1); \
        } \
        return node; \
    } \
    \
    static int _treap_count_less_internal_##TypeName(treap_node_##TypeName *node, KeyType key) { \
        if (!node) return 0; \
        if (key <= node->key) { \
            return _treap_count_less_internal_##TypeName(node->left, key); \
        } \
        return (node->left ? node->left->size : 0) + 1 + _treap_count_less_internal_##TypeName(node->right, key); \
    } \
    \
    static void _treap_node_free_##TypeName(treap_node_##TypeName *node) { \
        if (!node) return; \
        _treap_node_free_##TypeName(node->left); \
        _treap_node_free_##TypeName(node->right); \
        free(node); \
    } \
    /* Public interface functions */ \
    static void treap_destroy_##TypeName(treap_##TypeName *t) { \
        if (!t) return; \
        _treap_node_free_##TypeName(t->root); \
        t->root = NULL; \
    } \
    \
    static void treap_init_##TypeName(treap_##TypeName *t) { \
        if (t) t->root = NULL; \
    } \
    \
    static int treap_empty_##TypeName(const treap_##TypeName *t) { \
        return !t || t->root == NULL; \
    } \
    \
    static int treap_size_##TypeName(const treap_##TypeName *t) { \
        return (t && t->root) ? t->root->size : 0; \
    } \
    \
    static void treap_insert_##TypeName(treap_##TypeName *t, KeyType key) { \
        if (!t) return; \
        treap_node_##TypeName *new_node = _treap_node_new_##TypeName(key); \
        if (!new_node) return; /* Allocation failed, _treap_node_new_ handles message */ \
        t->root = _treap_insert_internal_##TypeName(t->root, new_node); \
    } \
    \
    static void treap_erase_##TypeName(treap_##TypeName *t, KeyType key) { \
        if (!t) return; \
        t->root = _treap_erase_internal_##TypeName(t->root, key); \
    } \
    \
    static treap_node_##TypeName* treap_find_##TypeName(treap_##TypeName *t, KeyType key) { \
        if (!t) return NULL; \
        treap_node_##TypeName *current = t->root; \
        while (current) { \
            if (key < current->key) { \
                current = current->left; \
            } else if (key > current->key) { \
                current = current->right; \
            } else { \
                return current; \
            } \
        } \
        return NULL; \
    } \
    \
    static treap_node_##TypeName* treap_kth_##TypeName(treap_##TypeName *t, int k) { \
        if (!t) return NULL; \
        return _treap_kth_internal_##TypeName(t->root, k); \
    } \
    \
    static int treap_count_less_##TypeName(treap_##TypeName *t, KeyType key) { \
        if (!t) return 0; \
        return _treap_count_less_internal_##TypeName(t->root, key); \
    }