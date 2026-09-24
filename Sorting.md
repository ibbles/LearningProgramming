# Bubble Sort

I like bubble-sort.
It solves an easy to understand problem in an easy to understand way.
It also lends itself to numerous variations and experimentation to find faster variants.
It is often "bad" in the sense that there are more efficient algorithms out there in the general case, but not all cases are the general case.
Bubble sort is very efficient for sorted and many almost sorted lists, even with the straight-forward implementation, at the best case terminating after a single pass over the list.
I don't think it is possible to beat that.

Bubble sort lends itself well as an intrusive algorithm.
By that I mean that we don't call a `sort` function that sorts the list and does nothing else.
Instead we incorporate additional logic into the sorting process itself.
Bubble sort is a swap-based algorithm and each swap that is performed provides information about the state of the system that the list represents.
For example, in a sweep-and-prune collision detection implementation the list contains begin and end markers for the potentially colliding objects and each swap represents a pair of objects that either was not colliding but now are, or that was colliding but now aren't anymore.
If we keep track of these changes by the time the sort has completed, which is likely to be quick since collision detection often has high temporal coherency, we have constructed the complete set of overlapping objects.


# Sort Based On Knowledge Of The Data Being Sorted

Most standard libraries provide a `sort` function.
These functions are often using state-of-the-art sorting algorithms with high-quality implementations.
Despite this, if we have some knowledge about the data being sorted we can sometimes exploit this knowledge to write a faster `sort` function.

Let's consider an extreme case, we have a collection of linked lists and we want to sort them by size.
Our linked list implementation doesn't store the size of the list, to get the size it will traverse the list and count the nodes.
A generic `sort` implementation will be very slow if we have many linked lists and each linked list is long.
```cpp
void sort_by_length(std::vector<linked_list>& lists)
{
	auto less = [](linked_list const& lhs, linked_list const& rhs) { return lhs.size() < rhs.size(); };
	std::sort(lists, less);
}
```

There are a few ways we can try to improve this.
An restriction we can enforce is to require that the lists cannot change length while we are sorting.
A reasonable restriction.
With that restriction we can pre-compute the sizes and sort based on the stored sizes.
It costs O(n) memory to store the sizes, and some other temporary data, which is probably fine.
```cpp
void sort_by_length(std::vector<linked_list>& lists)
{
	// This thing got more complicated than I thought it would.
	// I think there is a simpler way to express the same idea.
	//
	// I didn't think this through properly.

	std::vector<std::pair<linked_list*, size_t>> lists_and_sizes;
	list_and_sizes.reserve(lists.size);
	for (const linked_list& list : lists)
	{
		list_and_sizes.emplace_back(list.length, &list);
	}
	
	std::sort(lists_and_sizes, std::get<1>); // This is meant to be a projection to the size_t member, I'm not sure the syntax is correct.
	
	std::vector<linked_list> old_lists = std::move(lists);
	lists.reserve(old_lists.size());
	for (size_t i = 0; i < old_lists.size(); ++i)
	{
		lists[i] = std::move(std::get<0>(lists_and_sizes[i]));
	}
}
```

My idea was to declare that we are not allowed to precompute the list sizes and then find a case where a custom `sort` implementation can be faster than the generic one by having our `sort` algorithm traverse the two linked lists in parallel, something that is only possible when we know that the elements of the list being sorted are linked lists, but I realize now that this logic would live in the comparison operator, now the `sort` implementation itself, so that trick can be applied just as well with the generic `sort`.
```cpp
void first_is_shorter(linked_list const& lhs, linked_list const& rhs)
{
	linked_node* lhs_node = lhs.head;
	linked_node* rhs_node = rhs.node;
	
	// Loop until either list has run out of nodes.
	while (lhs_node != nullptr || rhs_node != nullptr)
	{
		// At least one list still has another element.
		if (lhs_node == nullptr) {
			// The FIRST list ran out, the first list is SHORTER.
			return true;
		}
		if (rhs_node == nullptr) {
			// The SECOND list ran out, the first list is LONGER.
			return false;
		}
		
		// Neither list have run out of nodes yet, step to the next nodes.
		lhs_node = lhs_node->next;
		rhs_node = rhs_node->next;
	}
	
	// Both lists ran out of nodes at the same time, meaning they have the same length,
	// meaning the first list is not shorter.
	return false;
}

void sort_by_length(std::vector<linked_list>& lists)
{
	auto less = [](linked_list const& lhs, linked_list const& rhs) { return first_is_shorter(lhs, rhs); };
	return std::sort(lists, less);
}
```
