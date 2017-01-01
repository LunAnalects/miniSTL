#ifndef MINISTL_SET_HPP
#define MINISTL_SET_HPP
#include <initializer_list>
#include <utility>
#include "allocator.hpp"

namespace ministl {
	template <class Key, class T, class Compare = std::less<Key>,
		class Allocator = allocator<std::pair<const Key, T>>>
		class map {
		public:
			// types:
			typedef Key key_type;
			typedef T mapped_type;
			typedef pair<const Key, T> value_type;
			typedef Compare key_compare;
			typedef Allocator allocator_type;
			typedef value_type& reference;
			typedef const value_type& const_reference;
			typedef implementation - defined iterator; // see 23.2
			typedef implementation - defined const_iterator; // see 23.2
			typedef implementation - defined size_type; // see 23.2
			typedef implementation - defined difference_type; // see 23.2
			typedef typename allocator_traits<Allocator>::pointer pointer;
			typedef typename allocator_traits<Allocator>::const_pointer const_pointer;
			typedef std::reverse_iterator<iterator> reverse_iterator;
			typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
			class value_compare {
				friend class map;
			protected:
				Compare comp;
				value_compare(Compare c) : comp(c) {}
			public:
				bool operator()(const value_type& x, const value_type& y) const {
					return comp(x.first, y.first);
				}
			};
			// 23.4.4.2, construct/copy/destroy:
			map() : map(Compare()) { }
			explicit map(const Compare& comp, const Allocator& = Allocator());
			template <class InputIterator>
			map(InputIterator first, InputIterator last,
				const Compare& comp = Compare(), const Allocator& = Allocator());
			map(const map& x);
			map(map&& x);
			explicit map(const Allocator&);
			map(const map&, const Allocator&);
			map(map&&, const Allocator&);
			map(initializer_list<value_type>,
				const Compare& = Compare(),
				const Allocator& = Allocator());
			template <class InputIterator>
			map(InputIterator first, InputIterator last, const Allocator& a)
				: map(first, last, Compare(), a) { }
			map(initializer_list<value_type> il, const Allocator& a)
				: map(il, Compare(), a) { }
			~map();
			map& operator=(const map& x);
			map& operator=(map&& x)
				noexcept(allocator_traits<Allocator>::is_always_equal::value &&
					is_nothrow_move_assignable<Compare>::value);
			map& operator=(initializer_list<value_type>);
			allocator_type get_allocator() const noexcept;
			// iterators:
			iterator begin() noexcept;
			const_iterator begin() const noexcept;
			iterator end() noexcept;
			const_iterator end() const noexcept;
			reverse_iterator rbegin() noexcept;
			const_reverse_iterator rbegin() const noexcept;
			reverse_iterator rend() noexcept;
			const_reverse_iterator rend() const noexcept;
			const_iterator cbegin() const noexcept;
			const_iterator cend() const noexcept;
			const_reverse_iterator crbegin() const noexcept;
			const_reverse_iterator crend() const noexcept;
			// capacity:
			bool empty() const noexcept;
			size_type size() const noexcept;
			size_type max_size() const noexcept;
			// 23.4.4.3, element access:
			T& operator[](const key_type& x);
			T& operator[](key_type&& x);
			T& at(const key_type& x);
			const T& at(const key_type& x) const;
			// 23.4.4.4, modifiers:
			template <class... Args> pair<iterator, bool> emplace(Args&&... args);
			template <class... Args> iterator emplace_hint(const_iterator position, Args&&... args);
			pair<iterator, bool> insert(const value_type& x);
			pair<iterator, bool> insert(value_type&& x);
			template <class P> pair<iterator, bool> insert(P&& x);
			iterator insert(const_iterator position, const value_type& x);
			iterator insert(const_iterator position, value_type&& x);
			template <class P>
			iterator insert(const_iterator position, P&&);
			template <class InputIterator>
			void insert(InputIterator first, InputIterator last);
			void insert(initializer_list<value_type>);
			template <class... Args>
			pair<iterator, bool> try_emplace(const key_type& k, Args&&... args);
			template <class... Args>
			pair<iterator, bool> try_emplace(key_type&& k, Args&&... args);
			template <class... Args>
			iterator try_emplace(const_iterator hint, const key_type& k, Args&&... args);
			template <class... Args>
			iterator try_emplace(const_iterator hint, key_type&& k, Args&&... args);
			template <class M>
			pair<iterator, bool> insert_or_assign(const key_type& k, M&& obj);
			template <class M>
			pair<iterator, bool> insert_or_assign(key_type&& k, M&& obj);
			template <class M>
			iterator insert_or_assign(const_iterator hint, const key_type& k, M&& obj);
			template <class M>
			iterator insert_or_assign(const_iterator hint, key_type&& k, M&& obj);

			iterator erase(iterator position);
			iterator erase(const_iterator position);
			size_type erase(const key_type& x);
			iterator erase(const_iterator first, const_iterator last);
			void swap(map&)
				noexcept(allocator_traits<Allocator>::is_always_equal::value &&
					is_nothrow_swappable_v<Compare>);
			void clear() noexcept;
			// observers:
			key_compare key_comp() const;
			value_compare value_comp() const;
			// map operations:
			iterator find(const key_type& x);
			const_iterator find(const key_type& x) const;
			template <class K> iterator find(const K& x);
			template <class K> const_iterator find(const K& x) const;
			size_type count(const key_type& x) const;
			template <class K> size_type count(const K& x) const;
			iterator lower_bound(const key_type& x);
			const_iterator lower_bound(const key_type& x) const;
			template <class K> iterator lower_bound(const K& x);
			template <class K> const_iterator lower_bound(const K& x) const;
			iterator upper_bound(const key_type& x);
			const_iterator upper_bound(const key_type& x) const;
			template <class K> iterator upper_bound(const K& x);
			template <class K> const_iterator upper_bound(const K& x) const;
			pair<iterator, iterator> equal_range(const key_type& x);
			pair<const_iterator, const_iterator> equal_range(const key_type& x) const;
			template <class K>
			pair<iterator, iterator> equal_range(const K& x);
			template <class K>
			pair<const_iterator, const_iterator> equal_range(const K& x) const;
	};
	template <class Key, class T, class Compare, class Allocator>
	bool operator==(const map<Key, T, Compare, Allocator>& x,
		const map<Key, T, Compare, Allocator>& y);
	template <class Key, class T, class Compare, class Allocator>
	bool operator< (const map<Key, T, Compare, Allocator>& x,
		const map<Key, T, Compare, Allocator>& y);
	template <class Key, class T, class Compare, class Allocator>
	bool operator!=(const map<Key, T, Compare, Allocator>& x,
		const map<Key, T, Compare, Allocator>& y);
	template <class Key, class T, class Compare, class Allocator>
	bool operator> (const map<Key, T, Compare, Allocator>& x,
		const map<Key, T, Compare, Allocator>& y);
	template <class Key, class T, class Compare, class Allocator>
	bool operator>=(const map<Key, T, Compare, Allocator>& x,
		const map<Key, T, Compare, Allocator>& y);
	template <class Key, class T, class Compare, class Allocator>
	bool operator<=(const map<Key, T, Compare, Allocator>& x,
		const map<Key, T, Compare, Allocator>& y);
	// 23.4.4.5, specialized algorithms:
	template <class Key, class T, class Compare, class Allocator>
	void swap(map<Key, T, Compare, Allocator>& x,
		map<Key, T, Compare, Allocator>& y)
		noexcept(noexcept(x.swap(y)));
}
#endif
