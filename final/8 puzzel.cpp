#include <bits/stdc++.h>
using namespace std;

int bfs(string start) {
    string goal = "123456780";
    if (start == goal) return 0;
    
    map<string, int> dist;
    queue<string> q;
    dist[start] = 0;
    q.push(start);
    
    int moves[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
    
    while (!q.empty()) {
        string cur = q.front(); q.pop();
        int d = dist[cur];
        if (d >= 14) continue;
        
        int pos = (int)cur.find('0');
        int r = pos / 3, c = pos % 3;
        
        for (int i = 0; i < 4; i++) {
            int nr = r + moves[i][0], nc = c + moves[i][1];
            if (nr < 0 || nr >= 3 || nc < 0 || nc >= 3) continue;
            int npos = nr * 3 + nc;
            string next = cur;
            swap(next[pos], next[npos]);
            if (dist.count(next)) continue;
            dist[next] = d + 1;
            if (next == goal) return d + 1;
            q.push(next);
        }
    }
    return -1;
}

int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        string s = "";
        for (int i = 0; i < 9; i++) {
            int x; scanf("%d", &x);
            s += (char)('0' + x);
        }
        int ans = bfs(s);
        if (ans == -1)
            printf("You'd better skip this game.\n");
        else
            printf("You can solve it within %d steps.\n", ans);
    }
}