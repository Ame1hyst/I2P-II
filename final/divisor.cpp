#include <bits/stdc++.h>
using namespace std;

vector<int> spf;

vector<int> divisor(int num){
    vector<int> div;
    div.push_back(1);
    int temp = num;
    while (temp > 1)
    {
        int prime_num = spf[temp], power = 0;
        while(temp % prime_num == 0) {temp/=prime_num; power++;}
        int sz = div.size();
        int pk = 1;
        for(int i = 0; i < power; i++){
            pk *= prime_num;
            for(int j = 0; j < sz; j++){
                div.push_back(div[j]*pk);
            }
        }
    }
    return div;
    
}

int main(void){
    int n; scanf("%d", &n);

    spf.resize(n+1);
    for(int i = 0; i <= n; i++) spf[i] = i;
    
    for(int i = 2; (long long)i*i <=n; i++){
        if(spf[i] == i){
            for(int j = i*i; j <= n; j+=i){
                if(spf[j] == j) spf[j] = i;
            }
        }
    }

    vector<int> dist(n + 1, -1);
    queue<int> q;
    dist[n] = 0;
    q.push(n);

    while (!q.empty())
    {
        int cur = q.front();
        q.pop();
        vector<int> divs = divisor(cur);
        for(int i = 0; i < (int)divs.size(); i++){
            int d = divs[i];
            int nb[2] = {cur - d, cur + d};
            for(int k = 0; k < 2; k++){
                int next = nb[k];
                if (next >= 1 && next <= n && dist[next] == -1) {
                    dist[next] = dist[cur] + 1;
                    q.push(next);
                }
            }

        }
    }
    
    for (int i = 1; i <= n; i++)
        printf("%d ", dist[i]);
    return 0;
}