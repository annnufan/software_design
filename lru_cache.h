#include <map>
#include <functional>
#include <cassert>
#include <algorithm>
#include <iostream>

template <typename K, typename V>
class lru_cache {
	struct node {
		node* next;
		node* prev;
		K key;
		node(K k, node* p = nullptr) : key(k), prev(p) {}
	};
	std::map<K, V> cache_values;
	std::function<V(K)> calc_function;
	node* head_of_recent_history_list;
	node* tail_of_recent_history_list;
	size_t cache_size;
	size_t used_size;

	void delete_latest_key() {
		K deleting_key = tail_of_recent_history_list->key;
		node * next_tail = tail_of_recent_history_list->next;
		if (next_tail != nullptr) {
			next_tail->prev = nullptr;
		} else {
			head_of_recent_history_list = nullptr;
		}
		delete tail_of_recent_history_list;
		tail_of_recent_history_list = next_tail;
		used_size--;
		cache_values.erase(deleting_key);
	}

	void update_history(K key) {
		node * key_for_up = head_of_recent_history_list;
		while (key_for_up->key != key) {
			key_for_up = key_for_up->prev;
		}
		assert(key_for_up != nullptr);
		if (key_for_up->next != nullptr) {
			key_for_up->next->prev = key_for_up->prev;
		}
		if (key_for_up->prev != nullptr) {
			key_for_up->prev->next = key_for_up->next;
		}
		head_of_recent_history_list->next = key_for_up;
		key_for_up->prev = head_of_recent_history_list;
		key_for_up->next = nullptr;
		while (head_of_recent_history_list->next != nullptr) {
			head_of_recent_history_list = head_of_recent_history_list->next;
		}
		while (tail_of_recent_history_list->prev != nullptr) {
			tail_of_recent_history_list = tail_of_recent_history_list->prev;
		}
	}

	void add_event_to_history_list(K key) {
		used_size++;
		if (head_of_recent_history_list == nullptr) {
			head_of_recent_history_list = new node(key);
			tail_of_recent_history_list = head_of_recent_history_list;
			return;
		}
		node* current_node = new node(key, head_of_recent_history_list);
		head_of_recent_history_list->next = current_node;
		head_of_recent_history_list = current_node;
	}

	V add_value(K key) {
		if (used_size == cache_size) {
			delete_latest_key();
		}
		V value = calc_function(key);
		add_event_to_history_list(key);
		cache_values.insert({key, value});
		assert(used_size <= cache_size);
		return value;
	}

public:
	lru_cache(std::function<V(K)> func, size_t c_size) : calc_function(func), used_size(0), head_of_recent_history_list(nullptr), tail_of_recent_history_list(nullptr) {
		assert(c_size != 0);
		cache_size = c_size;
	}

	lru_cache() = delete;

	V get_value(K key) {
		auto cached_value = cache_values.find(key);
		if (cached_value != cache_values.end()) {
			update_history(key);
			return cached_value->second;
		}
		auto val = add_value(key);
		return val;
	}

};