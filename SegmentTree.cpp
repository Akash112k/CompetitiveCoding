template<class T, class K = T>
class AbstractSegmentTree {
    
protected:
    AbstractSegmentTree(vector<K> &arr) {
        if (arr.empty()) {
            throw "Cannot create segment tree of size 0";
        }
        _size = arr.size();
        this -> _value.resize(4 * arr.size());
    }
    
    virtual T getOutOfRangeValue() = 0;
    
    virtual T calculateValueFromChildrenToUpdate(T leftChildValue, T rightchildValue) = 0;
    
    virtual T calculateRangeValue(T leftChildValue, T rightChildValue) = 0;

    void buildTree(vector<int> &arr) {
        buildTreeHelper(0, arr, 0, _size - 1);
    }
    
    T getSegmentTreeValueTypeFromInputValueType(K val) {
        if (!is_same_v<T, K>) {
            throw "Input data type and external data types differ.\n Override this method to convert the data type";
        }
        return val;
    }
    
private:
    vector<T> _value;
    int _size;
    
    int getLeftChildIndex(int n) {
        return n * 2 + 1;
    }
    
    int getRightChildIndex(int n) {
        return n * 2 + 2;
    }
    
    T buildTreeHelper(int val, vector<K> &arr, int low, int high) {
        if (low == high) {
            _value[val] = arr[low];
            return _value[val];
        }
        int left = getLeftChildIndex(val);
        int right = getRightChildIndex(val);
        int mid = (low + high) / 2;
        T leftValue = buildTreeHelper(left, arr, low, mid);
        T rightValue = buildTreeHelper(right, arr, mid + 1, high);
        T currentNodeValue = calculateValueFromChildrenToUpdate(leftValue, rightValue);
        return _value[val] = currentNodeValue;
    }
    
    void updateHelper(int val, int low, int high, int pos, K newValue) {
        if (low > high)
            return;
        if (low == high) {
            _value[val] = getSegmentTreeValueTypeFromInputValueType(newValue);
            return;
        }
        int left = getLeftChildIndex(val);
        int right = getRightChildIndex(val);
        int mid = (low + high) / 2;
        if (pos <= mid) {
            updateHelper(left, low, mid, pos, newValue);
        }
        if (pos > mid) {
            updateHelper(right, mid + 1, high, pos, newValue);
        }
        T leftValue = _value[left];
        T rightValue = _value[right];
        T currentNodeValue = calculateValueFromChildrenToUpdate(leftValue, rightValue);
        _value[val] = currentNodeValue;
        return;
    }
    
    T getRangeHelper(int node, int low, int high, int left, int right) {
        if (low > high) {
            return getOutOfRangeValue();
        }
        if(low >= left && high <= right) {
    		return _value[node];
        }
        if (high < left || low > right) {
            return getOutOfRangeValue();
        }
        int leftIndex = getLeftChildIndex(node);
        int rightIndex = getRightChildIndex(node);
        int mid = (low + high) / 2;
        T leftValue = getRangeHelper(leftIndex, low, mid, left, right);
        T rightValue = getRangeHelper(rightIndex, mid + 1, high, left, right);            
        return calculateRangeValue(leftValue, rightValue);
    }

public:
    
    void update(int index, K value) {
        updateHelper(0, 0, _size - 1, index, value);
    }
    
    T getRange(int left, int right) {
        return getRangeHelper(0, 0, _size - 1, left, right);
    }
};

template<class T, class K = T>
class SegmentTree: public AbstractSegmentTree<T, K> {
public:
    SegmentTree(vector<K> &arr): AbstractSegmentTree<T, K>(arr){
        this->buildTree(arr);
    }
    
private:
    T getOutOfRangeValue() {
        return 0;
    }
    
    T calculateValueFromChildrenToUpdate(T leftChildValue, T rightChildValue) {
        return leftChildValue + rightChildValue;
    }
    
    T calculateRangeValue(T leftChildValue, T rightChildValue) {
        return leftChildValue + rightChildValue;
    }
};
