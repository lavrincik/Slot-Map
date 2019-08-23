#include <vector>
#include <utility>

template< typename T >
struct slot_map {

	struct handle;

private:

	std::vector<handle> indices;
	std::vector<T> data;
	std::vector<std::size_t> eraseTable;

	bool freeList = false;
	std::size_t head;
	std::size_t tail;

	// Helper functions for const / non const functions

	template<typename HandlePtr, typename SelfPtr>
	static HandlePtr _find_ptr(SelfPtr self, const handle &val) {
		if (self->valid(val)) {
			HandlePtr ptr = &(self->data[self->indices[val.ind].ind]);
			return ptr;
		}

		return nullptr;
	}

	template<typename HandleRef, typename SelfRef>
	static HandleRef _find_ref(SelfRef self, const handle &val) {
		HandleRef ref = self->data[self->indices[val.ind].ind];
		return ref;
	}

public:

	struct handle {
		handle() = default;

	private:
		handle(std::size_t index, std::size_t generation) : ind(std::move(index)), gen(std::move(generation)) {}

		std::size_t ind;
		std::size_t gen;

		friend struct slot_map;
	};

	// implements http://en.cppreference.com/w/cpp/concept/Container

	handle insert( const T &v ) {
		if (!freeList) {
			handle indicesHandle(data.size(), 0);

			indices.push_back(indicesHandle);

			handle key(indices.size() - 1, 0);

			eraseTable.push_back(key.ind);
			data.push_back(v);

			return key;
		}

		handle key(head, indices[head].gen);
		std::size_t indToData = head;

		if (head == tail) {
			freeList = false;
		} else {
			head = indices[head].ind;
		}

		indices[indToData].ind = data.size();

		eraseTable.push_back(key.ind);
		data.push_back(v);

		return key;
	}

	bool valid( handle h ) const {
		return h.gen == indices[h.ind].gen;
	}
	
	void erase( handle h ) {
		if (!valid(h)) {
			return;
		}

		// swap last element with element that is going to be erased

		using std::swap;

		swap(data[indices[h.ind].ind], data.back());	
		data.pop_back();

		swap(eraseTable[indices[h.ind].ind], eraseTable[eraseTable.size() - 1]);
		eraseTable.pop_back();

		// update indices table
		
		indices[eraseTable[indices[h.ind].ind]].ind = indices[h.ind].ind;
		indices[h.ind].gen++;

		// add empty indices index to the freeList

		if (freeList) {
			indices[tail].ind = h.ind;
		} else {
			head = h.ind;
			freeList = true;
		}

		tail = h.ind;
	}

	T &operator[]( handle h ) {
		return _find_ref<T &>(this, h);
	}

	const T &operator[]( handle h ) const {
		return _find_ref<const T &>(this, h);
	}

	T *get( handle h ) {
		return _find_ptr<T *>(this, h);
	}

	const T *get( handle h ) const {
		return _find_ptr<const T *>(this, h);
	}

	using value_type = T;
	using reference = T&;
	using const_reference = const T&;
	using iterator = typename std::vector<T>::iterator;
	using const_iterator = typename std::vector<T>::const_iterator;
	using difference_type = typename std::vector<T>::difference_type;	
	using size_type = typename std::vector<T>::size_type;

	const_iterator begin() const {
		return data.begin();
	}

	iterator begin() {
		return data.begin();
	}

	const_iterator cbegin() const {
		return data.cbegin();
	}

	const_iterator end() const {
		return data.end();
	}

	iterator end() {
		return data.end();
	}

	const_iterator cend() const {
		return data.cend();
	}

	size_type size() const {
		return data.size();
	}

	size_type max_size() const {
		return data.max_size();
	}

	bool empty() const {
		return begin() == end();
	}

	template<class U>
	friend bool operator==(const slot_map<U>& lhs,const slot_map<U>& rhs);

	void swap(slot_map<T>& sm) {
		using std::swap;

		indices.swap(sm.indices);
		data.swap(sm.data);
		eraseTable.swap(sm.eraseTable);
		swap(head, sm.head);
		swap(tail, sm.tail);
		swap(freeList, sm.freeList);
	}
};

template<typename T>
bool operator==(const slot_map<T>& lhs,const slot_map<T>& rhs) {
	return lhs.data == rhs.data;
}

template<typename T>
bool operator!=(const slot_map<T>& lhs,const slot_map<T>& rhs) {
	return !(lhs == rhs);		
}

template<typename T>
void swap(slot_map<T>& lhs, slot_map<T>& rhs) {
	lhs.swap(rhs);
}
