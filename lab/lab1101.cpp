#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

vector<int> intersect(vector<int>& nums1, vector<int>& nums2){
    unordered_map<int,int> inter_map;
    vector<int> results;
    for (int num: nums1){
        inter_map[num]++;
    }
    for (int num: nums2){
        if (inter_map[num] > 0){
            results.push_back(num);
            inter_map[num]--;
        }
    }
    sort(results.begin(), results.end());
    return results;
}

int main(){
    int m, n;
    cin >> m;
    vector<int> nums1(m);
    for (int i = 0; i < m; i++){
        cin >> nums1[i];
    }
    cin >> n;
    vector<int> nums2(n);
    for (int i = 0; i < n; i++){
        cin >> nums2[i];
    }
    vector<int> results = intersect(nums1, nums2);
    cout << "[";
    for (int i = 0; i < results.size(); i++){
        cout << results[i];
        if (i < results.size() - 1){
            cout << ", ";
        }
    }
    cout << "]" << endl;
    return 0;
}