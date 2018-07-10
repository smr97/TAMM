#ifndef TAMM_INDEX_SPACE_INTERFACE_HPP_
#define TAMM_INDEX_SPACE_INTERFACE_HPP_

#include "tamm/types.hpp"
#include <algorithm>
#include <memory>
#include <vector>

namespace tamm {
/**
 * @class IndexSpaceInterface
 * @brief Base abstract implementation class as an interface
 *        for the different IndexSpace implementations
 */
class IndexSpaceInterface {
    public:
    /**
     * @brief Destroy the Index Space Interface object
     *
     * @todo specify (=default or =delete) the implicit functions
     */
    virtual ~IndexSpaceInterface() {}

    /**
     * @brief Accessor method for Index values in an IndexSpace
     *
     * @param [in] idx input Index value
     * @param [in] indep_index dependent Index values (mainly used for dependent
     * IndexSpace)
     * @returns an Index value from the IndexSpace for the corresponding input
     * Index
     */
    virtual Index index(Index idx,
                        const IndexVector& indep_index = {}) const = 0;

    /**
     * @brief
     *
     * @param [in] idx input Index value
     * @returns an Index value from the IndexSpace for the corresponding input
     * Index
     */
    virtual Index operator[](Index idx) const = 0;

    /**
     * @brief operator () for accessing IndexSpace objects associated with the
     * interface implementation
     *
     * @param [in] indep_index dependent Index values (mainly used for dependent
     * IndexSpaces)
     * @returns an IndexSpace object
     */
    virtual IndexSpace operator()(
      const IndexVector& indep_index = {}) const = 0;

    /**
     * @brief operator () overload with an input string value
     *
     * @param [in] named_subspace_id string value of the subspace name to be
     * accessed
     * @returns an IndexSpace corresponding to the subspace name
     */
    virtual IndexSpace operator()(
      const std::string& named_subspace_id) const = 0;

    /**
     * @brief Iterator accessor to the Index values associated with the
     * IndexSpace
     *
     * @returns const_iterator to the first element of the IndexVector
     */
    virtual IndexIterator begin() const = 0;
    /**
     * @brief Iterator accessor to the Index values associated with the
     * IndexSpace
     *
     * @returns const_iterator to the last element of the IndexVector
     */
    virtual IndexIterator end() const = 0;
    /**
     * @brief Returns the number of indices associated with the
     * IndexSpace
     *
     * @returns size of the index space
     */
    virtual std::size_t size() const = 0;

    /**
     * @brief Returns the maximum number of indices associated with
     * this index space. This is equal to size() for independent index
     * spaces. For dependent index spaces, this is the largest of the
     * sizes of the encapsulated index spaces.
     *
     * @returns maximum size of the index space
     */
    virtual std::size_t max_size() const = 0;

    /**
     * @brief Accessor methods to Spin value associated with the input Index
     *
     * @param [in] idx input Index value
     * @returns associated Spin value for the input Index value
     */
    virtual Spin spin(Index idx) const = 0;
    /**
     * @brief Accessor methods to Spatial value associated with the input Index
     *
     * @param [in] idx input Index value
     * @returns associated Spatial value for the input Index value
     */
    virtual Spatial spatial(Index idx) const = 0;

    /**
     * @brief Accessor method for the set of Ranges associated with a Spin value
     *
     * @param [in] spin input Spin value
     * @returns a vector of Ranges associated with the input Spin value
     */
    virtual std::vector<Range> spin_ranges(Spin spin) const = 0;

    /**
     * @brief Accessor method for the set of Ranges associated with a Spatial
     * value
     *
     * @param [in] spatial input Spatial value
     * @returns a vector of Ranges associated with the input Spatial value
     */
    virtual std::vector<Range> spatial_ranges(Spatial spatial) const = 0;

    /**
     * @brief Boolean method for checking if an IndexSpace has SpinAttribute
     *
     * @returns true if there is a SpinAttribute associated with the IndexSpace
     */
    virtual bool has_spin() const = 0;
    /**
     * @brief Boolean method for checking if an IndexSpace has SpatialAttribute
     *
     * @returns true if there is a SpatialAttribute associated with the
     * IndexSpace
     */
    virtual bool has_spatial() const = 0;

    /**
     * @brief Get the spin attribute object associated with IndexSpace
     *
     * @returns const reference to associated SpinAttribute
     */
    virtual SpinAttribute get_spin() const = 0;

    /**
     * @brief Get the spatial attribute object associated with IndexSpace
     *
     * @returns const reference to associated SpatialAttribute
     */
    virtual SpatialAttribute get_spatial() const = 0;

    /**
     * @brief Get the named ranges map
     *
     * @returns NameToRangeMap object from IndexSpace
     */
    virtual const NameToRangeMap& get_named_ranges() const = 0;

    virtual IndexSpace root_index_space() const = 0;

    protected:
    std::weak_ptr<IndexSpaceInterface> this_weak_ptr_;

    /**
     * @brief Helper methods for string manupulation, the main use
     *        is to split a string into vector of strings with
     *        respect to a deliminator

    * @param [in] str string to be split
    * @param [in] delim used char deliminator
    * @returns a vector of split strings
    */
    static std::vector<std::string> split(const std::string& str, char delim) {
        std::vector<std::string> elems;
        std::size_t start = 0, end = 0;
        while((end = str.find(delim, start)) != std::string::npos) {
            if(end != start) {
                elems.push_back(str.substr(start, end - start));
            }
            start = end + 1;
        }
        if(end != start) { elems.push_back(str.substr(start)); }
        return elems;
    }

    /**
     * @brief Helper method checking if a vector of data
     *        has any duplicates by:
     *          - sorting a copy of the vector
     *          - check for adjacent repeation
     *
     * @tparam ContainerType stl container type with iterator
     * (RandomAccessIterator) support
     * @param [in] data_vec input vector
     * @returns true returned if there are duplicates
     */
    template<typename ContainerType>
    static bool has_duplicate(const ContainerType& data_vec) {
        ContainerType temp_vec = data_vec;
        std::sort(temp_vec.begin(), temp_vec.end());

        return (std::adjacent_find(temp_vec.begin(), temp_vec.end()) ==
                temp_vec.end());
    }

    /**
     * @brief Check if the input attributes is valid:
     *        - no-overlap between attribute ranges
     *        - covers all indicies
     *
     * @tparam AttributeType an attribute type (e.g. Spin, Spatial)
     * @param indices set of indices to check against
     * @param att attribute map to check for validity
     * @return true if there is no overlap on the ranges and fully covers
     *              indices
     * @return false otherwise
     */
    template<typename AttributeType>
    bool is_valid_attribute(const IndexVector& indices,
                            const AttributeToRangeMap<AttributeType>& att) {
        // Construct index vector from input attribute ranges
        IndexVector att_indices = {};
        for(const auto& kv : att) {
            for(const auto& range : kv.second) {
                for(const auto& index : construct_index_vector(range)) {
                    att_indices.push_back(indices[index]);
                }
            }
        }
        // Check no overlap on the ranges
        std::sort(att_indices.begin(), att_indices.end());
        EXPECTS(has_duplicate<IndexVector>(att_indices));

        // Check for full coverage of the indices
        EXPECTS(indices.size() == att_indices.size());
        // copy indicies
        IndexVector temp_indices(indices);
        // sort temporary index vector for equality check
        std::sort(temp_indices.begin(), temp_indices.end());
        EXPECTS(std::equal(temp_indices.begin(), temp_indices.end(),
                           att_indices.begin()));

        return true;
    }

    private:
    /**
     * @brief Set the weak ptr object for IndexSpaceInterface
     *
     * @param [in] weak_ptr std::weak_ptr to IndexSpaceInterface
     */
    void set_weak_ptr(std::weak_ptr<IndexSpaceInterface> weak_ptr) {
        this_weak_ptr_ = weak_ptr;
    }

    friend class IndexSpace;
}; // IndexSpaceInterface

/**
 * @brief IndexSpace implementation for range based
 *        IndexSpace construction.
 *
 */
class RangeIndexSpaceImpl : public IndexSpaceInterface {
    public:
    // @todo do we need a default constructor?
    // RangeIndexSpaceImpl() = default;

    /**
     * @brief Construct a new RangeIndexSpaceImpl object
     *
     * @todo optimization - constructing map<string, IndexSpace> can be delayed
     * until a specific subspace is requested
     *
     * @param [in] indices vector of Index values
     * @param [in] named_ranges a map from string value to a set of associated
     * ranges
     * @param [in] spin a map for Spin values to set of associated ranges
     * @param [in] spatial a map from Spatial values to set of associated ranges
     */
    RangeIndexSpaceImpl(const IndexVector& indices,
                        const NameToRangeMap& named_ranges,
                        const AttributeToRangeMap<Spin>& spin,
                        const AttributeToRangeMap<Spatial>& spatial) :
      indices_{indices},
      named_ranges_{named_ranges},
      named_subspaces_{construct_subspaces(named_ranges)},
      spin_{construct_spin(spin)},
      spatial_{construct_spatial(spatial)} {
        EXPECTS(has_duplicate<IndexVector>(indices_));
    }

    // @todo do we need these copy/move constructor/operators?
    RangeIndexSpaceImpl(RangeIndexSpaceImpl&&)      = default;
    RangeIndexSpaceImpl(const RangeIndexSpaceImpl&) = default;
    RangeIndexSpaceImpl& operator=(RangeIndexSpaceImpl&&) = default;
    RangeIndexSpaceImpl& operator=(const RangeIndexSpaceImpl&) = default;
    ~RangeIndexSpaceImpl()                                     = default;

    // Index Accessors
    Index index(Index i, const IndexVector& indep_index = {}) const override {
        return indices_[i];
    }
    Index operator[](Index i) const override { return indices_[i]; }

    // Subspace Accessors
    IndexSpace operator()(const IndexVector& indep_index = {}) const override {
        return IndexSpace{this_weak_ptr_.lock()};
    }

    IndexSpace operator()(const std::string& named_subspace_id) const override {
        return named_subspaces_.at(named_subspace_id);
    }

    // Iterators
    IndexIterator begin() const override { return indices_.begin(); }
    IndexIterator end() const override { return indices_.end(); }

    // Size of this index space
    std::size_t size() const override { return indices_.size(); }

    // Maximum size of this index space
    std::size_t max_size() const override { return indices_.size(); }

    // Attribute Accessors
    Spin spin(Index idx) const override { return spin_(idx); }
    Spatial spatial(Index idx) const override { return spatial_(idx); }

    std::vector<Range> spin_ranges(Spin spin) const override {
        return spin_.attribute_range(spin);
    }
    std::vector<Range> spatial_ranges(Spatial spatial) const override {
        return spatial_.attribute_range(spatial);
    }

    bool has_spin() const override { return !(spin_.empty()); }
    bool has_spatial() const override { return !(spatial_.empty()); }

    SpinAttribute get_spin() const override { return spin_; }
    SpatialAttribute get_spatial() const override { return spatial_; }

    const NameToRangeMap& get_named_ranges() const override {
        return named_ranges_;
    }

    IndexSpace root_index_space() const override {
        return IndexSpace{this_weak_ptr_.lock()};
    }

    protected:
    IndexVector indices_;
    NameToRangeMap named_ranges_;
    std::map<std::string, IndexSpace> named_subspaces_;
    SpinAttribute spin_;
    SpatialAttribute spatial_;

    /**
     * @brief Helper method for generating the map between string values to
     * IndexSpaces. Mainly used for constructing the subspaces.
     *
     * @param [in] in_map NameToRangeMap argument holding string to Range vector
     * map
     * @return std::map<std::string, IndexSpace> returns the map from
     *                                           strings to subspaces
     */
    std::map<std::string, IndexSpace> construct_subspaces(
      const NameToRangeMap& in_map) {
        std::map<std::string, IndexSpace> ret;
        for(auto& kv : in_map) {
            std::string name         = kv.first;
            IndexVector temp_indices = {};
            for(auto& range : kv.second) {
                for(auto& i : construct_index_vector(range)) {
                    temp_indices.push_back(indices_[i]);
                }
            }
            ret.insert({name, IndexSpace{temp_indices}});
        }

        return ret;
    }

    /**
     * @brief Helper method for constructing  and validating the attributes for
     * IndexSpace
     *
     * @param [in] spin Spin attribute to Range map that is used for
     *             constructing Spin attribute
     * @returns a SpinAttribute constructed using input map
     */
    SpinAttribute construct_spin(const AttributeToRangeMap<Spin>& spin) {
        // return default spin value (Spin{0}) for the whole range
        if(spin.empty()) {
            return SpinAttribute(
              AttributeToRangeMap<Spin>{{Spin{0}, {range(indices_.size())}}});
        }

        // Check validity of the input attribute
        EXPECTS(is_valid_attribute<Spin>(indices_, spin));

        return SpinAttribute{spin};
    }

    /**
     * @brief Helper method for constructing  and validating the attributes for
     * IndexSpace
     *
     * @param [in] spatial a Spatial value to Range map that is used for
     *             constructing Spin attribute
     * @return [in] SpinAttribute returns a Spin attribute
     */
    SpatialAttribute construct_spatial(
      const AttributeToRangeMap<Spatial>& spatial) {
        // return default spatial value (Spatial{0}) for the whole range
        if(spatial.empty()) {
            return SpatialAttribute(AttributeToRangeMap<Spatial>{
              {Spatial{0}, {range(indices_.size())}}});
        }

        // Check validity of the input attribute
        EXPECTS(is_valid_attribute<Spatial>(indices_, spatial));

        return SpatialAttribute{spatial};
    }
}; // RangeIndexSpaceImpl

/**
 * @brief IndexSpace implementation for subspace based
 *        IndexSpace construction.
 *
 */
class SubSpaceImpl : public IndexSpaceInterface {
    public:
    // @todo do we need a default constructor?
    // SubSpaceImpl() = default;

    /**
     * @brief Construct a new SubSpaceImpl object
     *
     * @todo optimization - constructing map<string, IndexSpace> can be delayed
     * until a specific subspace is requested
     *
     * @param [in] is
     * @param [in] range
     * @param [in] named_ranges
     */
    SubSpaceImpl(const IndexSpace& is, const Range& range,
                 const NameToRangeMap& named_ranges) :
      ref_space_{is},
      ref_range_{range},
      indices_{construct_indices(is, range)},
      named_ranges_{named_ranges},
      named_subspaces_{construct_subspaces(named_ranges)},
      root_space_{is.root_index_space()} {}

    /**
     * @brief
     *
     * @todo: Implement
     *
     * @param [in] is
     * @param [in] indices
     * @param [in] named_ranges
     */
    SubSpaceImpl(const IndexSpace& is, const IndexVector& indices,
                 const NameToRangeMap& named_ranges) {}

    // @todo do we need these copy/move constructor/operators
    SubSpaceImpl(SubSpaceImpl&&)      = default;
    SubSpaceImpl(const SubSpaceImpl&) = default;
    SubSpaceImpl& operator=(SubSpaceImpl&&) = default;
    SubSpaceImpl& operator=(const SubSpaceImpl&) = default;
    ~SubSpaceImpl()                              = default;

    // Index Accessors
    Index index(Index i, const IndexVector& indep_index = {}) const override {
        return indices_[i];
    }
    Index operator[](Index i) const override { return indices_[i]; }

    // Subspace Accessors
    IndexSpace operator()(const IndexVector& indep_index = {}) const override {
        return IndexSpace{this_weak_ptr_.lock()};
    }

    IndexSpace operator()(const std::string& named_subspace_id) const override {
        return named_subspaces_.at(named_subspace_id);
    }

    // Iterators
    IndexIterator begin() const override { return indices_.begin(); }
    IndexIterator end() const override { return indices_.end(); }

    // Size of this index space
    std::size_t size() const override { return indices_.size(); }

    // Maximum size of this index space
    std::size_t max_size() const override { return indices_.size(); }

    // Attribute Accessors
    Spin spin(Index idx) const override { return ref_space_.spin(idx); }
    Spatial spatial(Index idx) const override {
        return ref_space_.spatial(idx);
    }

    std::vector<Range> spin_ranges(Spin spin) const override {
        return ref_space_.spin_ranges(spin);
    }
    std::vector<Range> spatial_ranges(Spatial spatial) const override {
        return ref_space_.spatial_ranges(spatial);
    }

    bool has_spin() const override { return ref_space_.has_spin(); }
    bool has_spatial() const override { return ref_space_.has_spatial(); }

    SpinAttribute get_spin() const override { return ref_space_.get_spin(); }
    SpatialAttribute get_spatial() const override {
        return ref_space_.get_spatial();
    }

    const NameToRangeMap& get_named_ranges() const override {
        return named_ranges_;
    }

    IndexSpace root_index_space() const override { return root_space_; }

    protected:
    IndexSpace ref_space_;
    Range ref_range_;
    IndexVector indices_;
    NameToRangeMap named_ranges_;
    std::map<std::string, IndexSpace> named_subspaces_;
    IndexSpace root_space_;

    /**
     * @brief Helper method for constructing the new set of
     *        indicies from the reference IndexSpace
     *
     * @param ref_space reference IndexSpace argument
     * @param range     Range argument for generating the subspace
     * @return IndexVector returns a vector of Indicies
     */
    IndexVector construct_indices(const IndexSpace& ref_space,
                                  const Range& range) {
        IndexVector ret = {};
        for(const auto& i : construct_index_vector(range)) {
            ret.push_back(ref_space[i]);
        }

        return ret;
    }

    /**
     * @brief Helper method for generating the map between
     *        string values to IndexSpaces. Mainly used for
     *        constructing the subspaces.
     *
     * @param in_map NameToRangeMap argument holding string to Range map
     * @return std::map<std::string, IndexSpace> returns the map from
     *                                           strings to subspaces
     */
    std::map<std::string, IndexSpace> construct_subspaces(
      const NameToRangeMap& in_map) {
        std::map<std::string, IndexSpace> ret;
        for(auto& kv : in_map) {
            std::string name    = kv.first;
            IndexVector indices = {};
            for(auto& range : kv.second) {
                for(auto& i : construct_index_vector(range)) {
                    indices.push_back(indices_[i]);
                }
            }
            ret.insert({name, IndexSpace{indices}});
        }

        return ret;
    }
}; // SubSpaceImpl

/**
 * @brief IndexSpace implementation for aggregation
 *        based IndexSpace construction.
 *
 */
class AggregateSpaceImpl : public IndexSpaceInterface {
    public:
    // @todo do we need a default constructor?
    // AggregateSpaceImpl() = default;

    /**
     * @brief Construct a new Aggregate Space Impl object
     *
     * @param [in] spaces reference IndexSpace objects for aggregating
     * @param [in] names string names associated with each reference IndexSpace
     * @param [in] named_ranges additional string names to Range vector map
     * @param [in] subspace_references named subspace relations using reference
     * IndexSpace named subspaces
     */
    AggregateSpaceImpl(const std::vector<IndexSpace>& spaces,
                       const std::vector<std::string>& names,
                       const NameToRangeMap& named_ranges,
                       const std::map<std::string, std::vector<std::string>>&
                         subspace_references) :
      ref_spaces_(spaces),
      indices_{construct_indices(spaces)},
      named_ranges_{named_ranges},
      named_subspaces_{construct_subspaces(named_ranges)} {
        EXPECTS(has_duplicate<IndexVector>(indices_));
        if(names.size() > 0) { add_ref_names(spaces, names); }
        if(subspace_references.size() > 0) {
            add_subspace_references(subspace_references);
        }
    }

    // @todo do we need these constructor/operators
    AggregateSpaceImpl(AggregateSpaceImpl&&)      = default;
    AggregateSpaceImpl(const AggregateSpaceImpl&) = default;
    AggregateSpaceImpl& operator=(AggregateSpaceImpl&&) = default;
    AggregateSpaceImpl& operator=(const AggregateSpaceImpl&) = default;
    ~AggregateSpaceImpl()                                    = default;

    // Index Accessors
    Index index(Index i, const IndexVector& indep_index = {}) const override {
        return indices_[i];
    }
    Index operator[](Index i) const override { return indices_[i]; }

    // Subspace Accessors
    IndexSpace operator()(const IndexVector& indep_index = {}) const override {
        return IndexSpace{this_weak_ptr_.lock()};
    }

    IndexSpace operator()(const std::string& named_subspace_id) const override {
        return named_subspaces_.at(named_subspace_id);
    }

    // Iterators
    IndexIterator begin() const override { return indices_.begin(); }
    IndexIterator end() const override { return indices_.end(); }

    // Size of this index space
    std::size_t size() const override { return indices_.size(); }

    // Maximum size of this index space
    std::size_t max_size() const override { return indices_.size(); }

    // @todo what should these return? Currently, it returns the
    // first reference space's spin and spatial attributes.
    // Attribute Accessors
    Spin spin(Index idx) const override {
        NOT_ALLOWED();
        return Spin{0};
    }
    Spatial spatial(Index idx) const override {
        NOT_ALLOWED();
        return Spatial{0};
    }

    std::vector<Range> spin_ranges(Spin spin) const override {
        NOT_ALLOWED();
        return {};
    }
    std::vector<Range> spatial_ranges(Spatial spatial) const override {
        NOT_ALLOWED();
        return {};
    }

    bool has_spin() const override {
        for(const auto& space : ref_spaces_) {
            if(space.has_spin() == false) { return false; }
        }
        return true;
    }
    bool has_spatial() const override {
        for(const auto& space : ref_spaces_) {
            if(space.has_spatial() == false) { return false; }
        }
        return true;
    }

    SpinAttribute get_spin() const override {
        NOT_ALLOWED();
        return SpinAttribute{};
    }
    SpatialAttribute get_spatial() const override {
        NOT_ALLOWED();
        return SpatialAttribute{};
    }

    const NameToRangeMap& get_named_ranges() const override {
        return named_ranges_;
    }

    IndexSpace root_index_space() const override {
        return IndexSpace{this_weak_ptr_.lock()};
    }

    protected:
    std::vector<IndexSpace> ref_spaces_;
    IndexVector indices_;
    NameToRangeMap named_ranges_;
    std::map<std::string, IndexSpace> named_subspaces_;

    /**
     * @brief Add subspaces reference names foreach aggregated
     *        IndexSpace
     *
     * @param [in] ref_spaces a vector of reference IndexSpaces
     * @param [in] ref_names  a vector of associated names for each
     *                   reference IndexSpace
     */
    void add_ref_names(const std::vector<IndexSpace>& ref_spaces,
                       const std::vector<std::string>& ref_names) {
        EXPECTS(ref_spaces.size() == ref_names.size());
        std::size_t i        = 0;
        std::size_t curr_idx = 0;
        for(const auto& space : ref_spaces) {
            named_subspaces_.insert({ref_names[i], space});
            named_ranges_.insert(
              {ref_names[i], {range(curr_idx, curr_idx + space.size())}});
            i++;
            curr_idx += space.size();
        }
    }

    /**
     * @brief Add extra references for subspace names
     *        associated with the reference subspaces.
     *
     * @param [in] subspace_references a map from subspace names
     *                            to reference subspace names
     */
    void add_subspace_references(
      const std::map<std::string, std::vector<std::string>>&
        subspace_references) {
        for(const auto& kv : subspace_references) {
            IndexVector temp_indices;
            std::vector<Range> ranges;

            std::string key = kv.first;
            for(const auto& ref_str : kv.second) {
                std::vector<std::string> ref_names = split(ref_str, ':');
                IndexSpace temp_is   = named_subspaces_.at(ref_names[0]);
                const auto& temp_map = temp_is.get_named_ranges();

                auto ref_it =
                  std::find(ref_spaces_.begin(), ref_spaces_.end(), temp_is);
                EXPECTS(ref_it != ref_spaces_.end());
                auto it            = ref_spaces_.begin();
                std::size_t offset = 0;

                while(it != ref_it) {
                    offset += (*it).size();
                    it++;
                }
                // for(size_t i = 1; i < ref_names.size(); i++) {
                //     temp_is = temp_is(ref_names[i]);
                // }

                // for each range for the corresponding sub-name
                for(const auto& rng : temp_map.at(ref_names[1])) {
                    // update the range values and push to the ranges
                    ranges.push_back(
                      range(rng.lo() + offset, rng.hi() + offset));
                }

                // for now we will support 1 depth references (e.g. "occ:alpha")
                temp_is = temp_is(ref_names[1]);

                temp_indices.insert(temp_indices.end(), temp_is.begin(),
                                    temp_is.end());
            }
            named_subspaces_.insert({key, IndexSpace{temp_indices}});
            named_ranges_.insert({key, ranges});
        }
    }

    /**
     * @brief Construct set of indicies from the aggregated
     *        IndexSpaces
     *
     * @param [in] spaces vector of IndexSpaces
     * @returns a vector of Index values
     */
    IndexVector construct_indices(const std::vector<IndexSpace>& spaces) {
        IndexVector ret = {};
        for(const auto& space : spaces) {
            ret.insert(ret.end(), space.begin(), space.end());
        }

        return ret;
    }

    /**
     * @brief Helper method for generating the map between
     *        string values to IndexSpaces. Mainly used for
     *        constructing the subspaces.
     *
     * @param [in] in_map NameToRangeMap argument holding string to Range map
     * @returns the map from strings to subspaces
     */
    std::map<std::string, IndexSpace> construct_subspaces(
      const NameToRangeMap& in_map) {
        std::map<std::string, IndexSpace> ret;
        for(auto& kv : in_map) {
            std::string name    = kv.first;
            IndexVector indices = {};
            for(auto& range : kv.second) {
                for(auto& i : construct_index_vector(range)) {
                    indices.push_back(indices_[i]);
                }
            }
            ret.insert({name, IndexSpace{indices}});
        }

        return ret;
    }

}; // AggregateSpaceImpl

/**
 * @brief IndexSpace implementation for constructing
 *        dependent IndexSpaces
 *
 */
class DependentIndexSpaceImpl : public IndexSpaceInterface {
    public:
    // @todo do we need a default constructor?
    // DependentIndexSpaceImpl() = default;

    /**
     * @brief Construct a new Dependent Index Space Impl object
     *
     * @param [in] indep_spaces a vector of dependent IndexSpace objects
     * @param [in] dep_space_relation a relation map between IndexVectors to
     * IndexSpaces
     */
    DependentIndexSpaceImpl(
      const std::vector<IndexSpace>& indep_spaces,
      const std::map<IndexVector, IndexSpace>& dep_space_relation) :
      dep_spaces_{indep_spaces},
      dep_space_relation_{dep_space_relation},
      named_ranges_{} {
        max_size_ = 0;
        for(const auto& pair: dep_space_relation) {
          max_size_ = std::max(max_size_, pair.second.size());
        }
      }

    /**
     * @brief Construct a new Dependent Index Space Impl object
     *
     * @param [in] indep_spaces a vector of dependent IndexSpace objects
     * @param [in] ref_space a reference IndexSpace
     * @param [in] dep_space_relation a relation map between IndexVectors to
     * IndexSpaces
     */
    DependentIndexSpaceImpl(
      const std::vector<IndexSpace>& indep_spaces, const IndexSpace& ref_space,
      const std::map<IndexVector, IndexSpace>& dep_space_relation) :
      dep_spaces_{indep_spaces},
      dep_space_relation_{dep_space_relation},
      named_ranges_{} {}

    // @todo do we need these constructor/operators
    DependentIndexSpaceImpl(DependentIndexSpaceImpl&&)      = default;
    DependentIndexSpaceImpl(const DependentIndexSpaceImpl&) = default;
    DependentIndexSpaceImpl& operator=(DependentIndexSpaceImpl&&) = default;
    DependentIndexSpaceImpl& operator=(const DependentIndexSpaceImpl&) =
      default;
    ~DependentIndexSpaceImpl() = default;

    // Index Accessors
    /**
     * @brief Given an Index and a IndexVector return
     *        corresponding Index from the dependent IndexSpace
     *
     * @param i an Index argument
     * @param indep_index a vector of Index
     * @return Index an Index value from the dependent IndexSpace
     */
    Index index(Index i, const IndexVector& indep_index = {}) const override {
        return dep_space_relation_.at(indep_index)[i];
    }

    // @todo what should ve returned?
    Index operator[](Index i) const override {
        NOT_ALLOWED();
        return Index{0};
    }

    // Subspace Accessors
    IndexSpace operator()(const IndexVector& indep_index = {}) const override {
        return dep_space_relation_.at(indep_index);
    }

    // @todo What should this return, currently returning itself
    IndexSpace operator()(const std::string& named_subspace_id) const override {
        return IndexSpace{this_weak_ptr_.lock()};
    }

    // Iterators
    // @todo Error on call
    IndexIterator begin() const override {
        NOT_ALLOWED();
        return IndexIterator();
    }
    IndexIterator end() const override {
        NOT_ALLOWED();
        return IndexIterator();
    }

    // @todo What should this return?
    std::size_t size() const override {
        NOT_ALLOWED();
        return 0;
    }

    std::size_t max_size() const override {
      return max_size_;
    }

    // Attribute Accessors
    Spin spin(Index idx) const override {
        NOT_ALLOWED();
        return Spin{0};
    }
    Spatial spatial(Index idx) const override {
        NOT_ALLOWED();
        return Spatial{0};
    }

    std::vector<Range> spin_ranges(Spin spin) const override {
        NOT_ALLOWED();
        return {};
    }
    std::vector<Range> spatial_ranges(Spatial spatial) const override {
        NOT_ALLOWED();
        return {};
    }

    bool has_spin() const override {
        NOT_ALLOWED();
        return false;
    }
    bool has_spatial() const override {
        NOT_ALLOWED();
        return false;
    }

    SpinAttribute get_spin() const override {
        NOT_ALLOWED();
        return SpinAttribute();
    }
    SpatialAttribute get_spatial() const override {
        NOT_ALLOWED();
        return SpatialAttribute();
    }

    const NameToRangeMap& get_named_ranges() const override {
        NOT_ALLOWED();
        return named_ranges_;
    }

    IndexSpace root_index_space() const override {
        return IndexSpace{this_weak_ptr_.lock()};
    }

    protected:
    std::vector<IndexSpace> dep_spaces_;
    IndexSpace ref_space_;
    std::map<IndexVector, IndexSpace> dep_space_relation_;
    NameToRangeMap named_ranges_;
    std::size_t max_size_;
}; // DependentIndexSpaceImpl

} // namespace tamm

#endif // TAMM_INDEX_SPACE_INTERFACE_HPP_
