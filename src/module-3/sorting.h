#ifndef M3_SORTING_H
#define M3_SORTING_H

#include <vector>

namespace m3 {
    using Index = std::vector<int>::size_type;

    namespace detail {
        template <typename Before>
        void mergeRange(std::vector<Index>& indices, std::vector<Index>& scratch, Index first, Index last, const Before& before) {
            if(last - first < 2) {
                return;
            }
            const Index middle = first + (last - first) / 2;
            mergeRange(indices, scratch, first, middle, before);
            mergeRange(indices, scratch, middle, last, before);
            Index left = first;
            Index right = middle;
            Index out = first;
            while(left < middle && right < last) {
                scratch[out++] = before(indices[right], indices[left]) ? indices[right++] : indices[left++];
            }
            while(left < middle) {
                scratch[out++] = indices[left++];
            }
            while(right < last) {
                scratch[out++] = indices[right++];
            }
            for(Index i = first; i < last; ++i) {
                indices[i] = scratch[i];
            }
        }
    }
    template <typename Before>
    void mergeSortIndices(std::vector<Index>& indices, const Before& before) {
        std::vector<Index> scratch(indices.size());
        detail::mergeRange(indices, scratch, 0, indices.size(), before);
    }
}

 #endif
