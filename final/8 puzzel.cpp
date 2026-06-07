#include <bits/stdc++.h>
using namespace std;

int bfs(string start){
    string goal = "123456780";
    if(start == goal) return 0;
    map<string, int> dist;
    queue<string> q;

    int move[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    //start base
    dist[start] = 0;
    q.push(start);

    while (!q.empty())
    {
        string cur = q.front();
        q.pop();

        int d = dist[cur];
        if(d >= 14) continue;

        int cpos = cur.find('0');
        int cr = cpos / 3, cc = cpos %3;

        for(int i = 0; i < 4; i++){
            int nr = cr + move[i][0];
            int nc = cc + move[i][1];
            
            if(nc >= 3 || nr >= 3 || nc < 0|| nr < 0) continue;
            int npos = nr * 3 + nc;
            string next = cur;
            swap(next[npos], next[cpos]);
            if(next == goal) return d+1;
            if(dist.count(next)) continue;
            dist[next] = d + 1;
            q.push(next);
        }
    }
    return -1;
    
}

int main(void){
    int t; scanf("%d", &t);
    while (t--)
    {
        string s = "";
        for(int i = 0; i < 9; i++){
            int x; scanf("%d", &x);
            s += (char)('0' + x);
        }
        int ans = bfs(s);
        if(ans == -1) printf("You'd better skip this game.\n");
        else printf("You can solve it within %d steps.\n", ans);
    }
    

    return 0;
}