#include <bits/stdc++.h>
using namespace std;

int main(void){
    int t; scanf("%d", &t);
    while (t--)
    {
        stack<int> st;
        int n; scanf("%d", &n);
        vector<long long> arr(n);
        for(int i = 0; i < n; i++) scanf("%lld", &arr[i]);

        long long ans = 0;
        for(int i = 0; i <=n; i++){
            long long cur = (i == n)? 0: arr[i];
            while (!st.empty() && arr[st.top()] > cur)
            {
                long long height = arr[st.top()];
                st.pop();
                int width = st.empty() ? i: i - st.top() - 1;
                ans = max(ans, width*height);
            }
            st.push(i);
            
        }
        printf("%lld\n", ans);

    }
    

    return 0;
}