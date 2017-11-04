#pragma once

/// TODO: complete this implementation of a thread-safe (concurrent) hash
///       table of integers, implemented as an array of linked lists.  In
///       this implementation, each list should have a "sentinel" node that
///       contains the lock, so we can't just reuse the clist implementation
class shash
{

	struct Node {
		int value;
		Node *next;
	  	Node(int v) : value(v), next(nullptr) { }
	};

  	mutable std::vector<std::mutex> locks;
  	std::vector<Node*> sentinels;
  	std::vector<int> sizes;
  	int bucks;

  bool insert(int idx, int key)
  {
	  std::lock_guard<std::mutex> lock_guard(locks[idx]);
	  Node *p = sentinels[idx];
	  while (p->next && p->next->value < key) {
		  p = p->next;
	  }

	  if (p->next && p->next->value == key) {
		  return false;
	  }

	  sizes[idx]++;
	  Node *new_node = new Node(key);
	  new_node->next = p->next;
	  p->next = new_node;
	  return true;
  }

  bool remove(int idx, int key)
  {
	  std::lock_guard<std::mutex> lock_guard(locks[idx]);
	  Node *p = sentinels[idx];
	  while (p->next && p->next->value < key) {
		  p = p->next;
	  }

	  if (p->next && p->next->value == key) {
		  sizes[idx]--;
		  p->next = p->next->next;
		  return true;
	  }

	  return false;
  }

  bool lookup(int idx, int key) const
  {
	  std::lock_guard<std::mutex> lock_guard(locks[idx]);
	  Node *p = sentinels[idx];
	  while (p->next && p->next->value < key)
		  p = p->next;

	  return p->next && p->next->value == key;
  }


public:
	shash(unsigned _buckets) : bucks(_buckets), locks(_buckets), sizes(_buckets, 0)
	{
      for (int i = 0; i < bucks; ++i) {
        sentinels.push_back(new Node(-1));
      }
    }


	/// insert *key* into the appropriate linked list if it doesn't already
	/// exist; return true if the key was added successfully.
	bool insert(int key)
	{
		int idx = key % bucks;
		return insert(idx, key);
	}
	/// remove *key* from the appropriate list if it was present; return true
	/// if the key was removed successfully.
	bool remove(int key)
	{
		int idx = key % bucks;
		return remove(idx, key);
	}
	/// return true if *key* is present in the appropriate list, false
	/// otherwise
	bool lookup(int key) const
	{
		int idx = key % bucks;
		return lookup(idx, key);
	}

	//The following are not tested by the given tester but are required for grading
	//No locks are required for these.

	//This refers to the number of buckets not the total number of elements.
	size_t getSize() const
	{
		return bucks;
	}

	//This refers to the number of elements in a bucket, not the sentinel node.
	size_t getBucketSize(size_t bucket) const
	{
		return sizes[bucket];
	}

	int getElement(size_t bucket, size_t idx) const
	{
		if (idx >= sizes[bucket])
			return -1;
		Node *p = sentinels[bucket]->next;
		while (idx-- > 0) {
			p = p->next;
		}
		return p->value;
	}


	//These functions just need to exist, they do not need to do anything
	int getElement(size_t idx) const
	{
		return 0;
	}
};
