#include <bits/stdc++.h>
using namespace std;

int main() {
    int t;
    scanf("%d", &t);
    while (t--) {
        long long target;
        int n;
        scanf("%lld %d", &target, &n);
        vector<int> nums(n);
        for (int i = 0; i < n; i++) scanf("%d", &nums[i]);
        
        map<long long, int> seen;
        int a = -1, b = -1;
        for (int i = 0; i < n; i++) {
            long long need = target - nums[i];
            if (seen.count(need)) {
                a = seen[need];
                b = i;
                break;
            }
            seen[nums[i]] = i;
        }
        
        if (a == -1) printf("None\n");
        else printf("%d %d\n", a, b);
    }
}