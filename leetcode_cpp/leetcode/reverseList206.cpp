/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
	ListNode* reverseList(ListNode* head) {
		vector<int> data;
		auto* helper = head;
		while (helper)
		{
			data.push_back(helper->val);
			helper = helper->next;
		}
		helper = head;
		for (int i = data.size() - 1; i >= 0; i--)
		{
			helper->val = data[i];
			helper = helper->next;
		}
		return head;
	}
};