# Tree Policy

Tree collection

Test on Intel i9-9900KF, 32GB 3200MHz memory

Complied with MSVC 2022

#### N=1e6 Random Data

| Name          | Insert(ms) | Query(ms) | Remove(ms) | Tree Height |
| ------------- | ---------- | --------- | ---------- | ----------- |
| RB Tree       | 784.658    | 636.702   | 920.995    | 24          |
| AVL           | 812.613    | 615.549   | 958.611    | 24          |
| BST           | 807.024    | 755.249   | 979.226    | 57          |
| Treap         | 1101.245   | 995.942   | 1077.163   | 47          |
| Splay         | 1010.890   | 1176.792  | 1362.036   | 55          |
| 2-3-4 Tree    | 1178.889   | 1178.099  | 1314.973   | 16          |
| std::multiset | 783.249    | 732.043   | 1108.711   | -           |

#### N=1e6 Ascend Data

| Name          | Insert(ms) | Query(ms) | Remove(ms) | Tree Height |
| ------------- | ---------- | --------- | ---------- | ----------- |
| RB Tree       | 143.017    | 54.348    | 76.294     | 37          |
| AVL           | 111.719    | 52.748    | 92.470     | 20          |
| BST           | -          | -         | -          | -           |
| Treap         | 106.164    | 63.573    | 71.766     | 47          |
| Splay         | 65.478     | 121.018   | 143.867    | 1000000     |
| 2-3-4 Tree    | 221.918    | 90.295    | 322.096    | 19          |
| std::multiset | 150.762    | 63.255    | 109.617    | -           |

#### N=1e6 Descend Data

| Name          | Insert (ms) | Query (ms) | Remove (ms) | Tree Height |
| ------------- | ----------- | ---------- | ----------- | ----------- |
| RB Tree       | 141.437     | 49.231     | 76.075      | 37          |
| AVL           | 114.281     | 49.582     | 83.127      | 20          |
| BST           | -           | -          | -           | -           |
| Treap         | 103.433     | 63.623     | 71.713      | 47          |
| Splay         | 57.825      | 109.387    | 154.094     | 1000000     |
| 2-3-4 Tree    | 277.765     | 100.804    | 277.628     | 19          |
| std::multiset | 144.584     | 59.446     | 144.394     | -           |

