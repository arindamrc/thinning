#ifndef ArrayND_H
#define ArrayND_H

#include <type_traits>
#include <vector>
#include <array>

#include "globals.h"

/**
 * @class ArrayND
 *
 * An N-dimensional array allowing indexing in row-major order.
 *
 * The underlying data is stored contiguously in a row-major order.
 *
 * The memory is completely owned and managed by this class. A copy operation
 * performs a deep copy, i.e., the underlying data container is
 * copied too.
 *
 * It is possible to grow or wane the allocated memory during operating
 * an instance.
 *
 * The index order is from slowest changing to quickest
 * changing dimension.
 *
 * The number of dimensions is a compile time constant and
 * passed as a template parameter during instantiation.
 *
 * The access/modification operator is of the form:
 * arr(index0, index1, ..., indexN) where index0 is an index
 * into the first dimension  and so on.
 * The number of parameters passed to operator() is also a
 * compile time constant and equal to the number of dimensions
 * of the N-dimensional array.
 */
template<typename T, unsigned_t NDims>
class ArrayND {

    using container_t = std::vector<T>;
    using container_ndims_t = std::array<unsigned_t, NDims>;

public:

    /**
     * @brief ArrayND
     *
     * Default constructor. Note that this must be followed with
     * ArrayND#updateDimSizes to reserve memory for the array.
     */
    ArrayND() {
        for(unsigned_t i = 0; i < NDims; i++) {
            N_[i] = 0;
        }
    }

    /**
     * @brief ArrayND
     * @param sz
     *
     * Creates a uniform sized grid with size = `sz` in all dimensions.
     * The dimensions are ordered from slowest changing to quickest
     * changing.
     */
    ArrayND(unsigned_t sz)
    {
        for(unsigned_t i = 0; i < NDims; i++) {
            N_[i] = sz;
        }

        init();
    }

    /**
     * @brief ArrayND
     * @param sizes...
     *
     * Creates a grid based on the sizes of each dimension supplied.
     * The dimensions are ordered from slowest changing to quickest
     * changing.
     */
    template<
            typename... Sizes,
            typename std::enable_if<sizeof...(Sizes) == NDims, unsigned_t>::type = 0,
            typename std::enable_if<std::conjunction_v<std::is_integral<Sizes>...>, unsigned_t>::type = 0>
    ArrayND(Sizes... sizes) {
        updateDimSizes(std::forward<Sizes>(sizes)...);
    }

    /**
     * @brief capacity
     * @return The capacity of this grid, i.e., the count of items for which
     * memory has been reserved in the grid.
     */
    unsigned_t capacity() {
        return data_.capacity();
    }

    /**
     * @brief operator()
     *
     * Accessor for the array elements.
     */
    template<
            typename... Indices,
            typename std::enable_if<sizeof...(Indices) == NDims, unsigned_t>::type = 0,
            typename std::enable_if<std::conjunction_v<std::is_integral<Indices>...>, unsigned_t>::type = 0>
    const T& operator()(Indices... indices) const {
        unsigned_t idx[]{indices...}; // Unpack
        unsigned_t flat = idx[NDims - 1];
        for (unsigned_t i = 1; i < NDims; i++) {
            flat += idx[NDims - i - 1] * strides_[i];
        }
        return data_[flat];
    }

    /**
     * @brief operator()
     *
     * Modifier for array elements.
     */
    template<
            typename... Indices,
            typename std::enable_if<sizeof...(Indices) == NDims, unsigned_t>::type = 0,
            typename std::enable_if<std::conjunction_v<std::is_integral<Indices>...>, unsigned_t>::type = 0>
    T& operator()(Indices... indices) {
        return const_cast<T&>(static_cast<const ArrayND<T,NDims>&>(*this).operator()(std::forward<Indices>(indices)...));
    }

    /**
     * @brief dimCount
     * @return The number of dimensions in the array.
     */
    unsigned_t dimCount() const {
        return NDims;
    }

    /**
     * @brief dimSize
     * @param i
     * @return The size of dimension `i`.
     */
    const unsigned_t& dimSize(const unsigned_t i) const {
        return N_[NDims - i - 1];
    }


    /**
     * @brief setDimSizes
     * @param sizes
     */
    template<
            typename... Sizes,
            typename std::enable_if<sizeof...(Sizes) == NDims, unsigned_t>::type = 0,
            typename std::enable_if<std::conjunction_v<std::is_integral<Sizes>...>, unsigned_t>::type = 0>
    void updateDimSizes(Sizes... sizes) {
        unsigned_t sz[]{sizes...}; // Unpack

        for(unsigned_t i = 0; i < NDims; i++) {
            N_[i] = sz[NDims - i - 1];
        }

        init();
    }


    /**
     * @brief fill
     * @param val
     *
     * Fill the array with value `val`.
     */
    void fill(const T& val) {
        data_.assign(data_.capacity(), val);
    }

    /**
     * @brief clear
     *
     * Clear the vector. Memory is not released.
     */
    void clear() {
        unsigned_t total = data_.size();
        data_.clear();
        data_.resize(total);
    }

private:

    container_ndims_t N_; /**> Number of elements in each dimension. */

    container_ndims_t strides_; /**> Strides in each dimension: useful for flat indexing. A count of elements to skip when changing dimension. */

    container_t data_; /**> Underlying data. */

    /**
     * @brief init
     *
     * Initialize/reserve space for the underlying container.
     */
    void init() {
        unsigned_t total = N_[0];

        strides_[0] = 1;
        for (unsigned_t i = 1; i < NDims; i++) {
            total *= N_[i];
            strides_[i] = strides_[i-1] * N_[i-1];
        }

        data_.clear();
        data_.resize(total);
    }

};

#endif // ArrayND_H
