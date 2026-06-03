#include <cstdio>
#include <stack>
#include <algorithm>
using namespace std;

int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        int n;
        scanf("%d", &n);
        long long h[200001];
        for (int i = 0; i < n; i++) scanf("%lld", &h[i]);

        stack<int> st;
        long long ans = 0;

        for (int i = 0; i <= n; i++) {
            long long cur = (i == n) ? 0 : h[i];
            while (!st.empty() && h[st.top()] > cur) {
                long long height = h[st.top()]; st.pop();
                long long width = st.empty() ? i : i - st.top() - 1;
                ans = max(ans, height * width);
            }
            st.push(i);
        }
        printf("%lld\n", ans);
    }
    return 0;
}