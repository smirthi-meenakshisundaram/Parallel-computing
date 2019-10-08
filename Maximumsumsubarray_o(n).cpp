#include<bits/stdc++.h> 
using namespace std; 
  struct Node { 
    long long maximum; 
    long long prefix; 
      long long suffix; 
     long long totalsum; 
  
}; 
  Node getNode(long long x){ 
    Node a; 
    a.maximum = x; 
    a.prefix = x; 
    a.suffix = x; 
    a.totalsum = x; 
    return a; 
} 
Node merg(const Node &l, const Node &r){ 
      Node b ; 
  b.maximum = b.prefix = b.suffix = b.totalsum = 0;
     b.prefix = max({l.prefix, l.totalsum+r.prefix, l.totalsum+r.totalsum}); 
     b.suffix = max({r.suffix, r.totalsum+l.suffix, l.totalsum+r.totalsum}); 
      b.totalsum = l.totalsum + r.totalsum; 
    b.maximum = max({b.prefix, b.suffix, b.totalsum,l.maximum, r.maximum, l.suffix+r.prefix}); 
  return b; 
} 
Node getMaxSumSubArray(int l, int r, vector<long long> &ar){ 
  
    if (l == r) return getNode(ar[l]); 
  
    int mid = (l + r) >> 1; 
    Node leftside = getMaxSumSubArray(l, mid, ar); 
    Node rightside = getMaxSumSubArray(mid+1, r, ar); 
return merg(leftside, rightside); 
  
} 
int main(){ 
  
    vector<long long> ar = {1,1,2,3,10,20,20,3,-1,-2}; 
    int n = ar.size(); 
     Node b = getMaxSumSubArray(0, n-1, ar); 
    cout << "The maximum sum using divide and conquer for time complexity o(n) using merge function =" << b.maximum << "\n"; 
  return 0; 
} 