# Tree Policy

Tree collection

Test on Intel i9-9900KF, 32GB 3200MHz memory

Complied with MSVC 2019

#### N=1e6 Random Data

| Name          | Insert(ms) | Query(ms) | Remove(ms) | Tree Height |
| ------------- | ---------- | --------- | ---------- | ----------- |
| RB Tree       | 698.061    | 522.185   | 681.705    | 24          |
| AVL           | 743.144    | 519.798   | 722.017    | 24          |
| BST           | 685.298    | 623.136   | 751.113    | 52          |
| Treap         | 961.47     | 920.845   | 784.155    | 48          |
| Splay         | 900.919    | 1043.44   | 1062.16    | -           |
| std::multiset | 688.41     | 650.588   | 913.145    | -           |

#### N=1e6 Ascend Data

| Name          | Insert(ms) | Query(ms) | Remove(ms) | Tree Height |
| ------------- | ---------- | --------- | ---------- | ----------- |
| RB Tree       | 139.632    | 49.103    | 78.668     | 37          |
| AVL           | 110.304    | 48.922    | 84.070     | 20          |
| BST           | -          | -         | -          | -           |
| Treap         | 104.54     | 58.848    | 69.091     | 51          |
| Splay         | 61.145     | 114.029   | 142.181    | -           |
| std::multiset | 142.211    | 60.666    | 107.007    | -           |

#### N=1e6 Descend Data

| Name          | Insert (ms) | Query (ms) | Remove (ms) | Tree Height |
| ------------- | ----------- | ---------- | ----------- | ----------- |
| RB Tree       | 145.360     | 52.924     | 77.444      | 37          |
| AVL           | 112.695     | 49.651     | 86.035      | 20          |
| BST           | -           | -          | -           | -           |
| Treap         | 106.264     | 64.552     | 77.722      | 54          |
| Splay         | 62.644      | 116.194    | 149.772     | -           |
| std::multiset | 152.483     | 59.628     | 138.373     | -           |

