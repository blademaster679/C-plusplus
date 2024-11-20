// graph数组用来代表朋友列表，用于查找朋友
// queue数组用来代表已知八卦的人群
// visited数组用来判断这个人是否已知八卦,用true的个数来计数
#include <iostream>
#include <vector>
using namespace std;
int gossip(int N, int M, int K, int start, vector<pair<int, int>> &friendships)
{
    // 构建graph朋友数组
    vector<vector<int>> graph(N + 1);
    for (int i = 0; i < M; i++)
    {
        int u = friendships[i].first;
        int v = friendships[i].second;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }
    vector<int> queue;
    vector<bool> visited(N + 1, false);
    // 初始化queue数列
    queue.push_back(start);
    visited[start] = true;
    int front = 0;
    int round = 0;
    // front相当于一个指针，遍历整个数组
    while (front < queue.size() && round < K)
    {
        int size = queue.size();
        round++;
        while (front < size)
        {
            int person = queue[front];
            front++;
            for (int i = 0; i < graph[person].size(); i++)
            {
                int friends = graph[person][i];
                if (!visited[friends])
                {
                    visited[friends] = true;
                    queue.push_back(friends);
                }
            }
        }
    }
    // 统计人数，这里面变量是j，而不是friends
    int count = 0;
    for (int j = 0; j <= N; j++)
    {
        if (visited[j])
            count++;
    }
    return count;
}

int main()
{
    int N, M, K, start;
    cin >> N >> M >> K >> start;
    vector<pair<int, int>> friendships(M);
    for (int i = 0; i < M; i++)
    {
        int u, v;
        cin >> u >> v;
        // make_pair为内置函数，意思是将u，v当成一对输入friendships里面
        friendships[i] = make_pair(u, v);
    }
    cout << gossip(N, M, K, start, friendships) << endl;
    return 0;
}
