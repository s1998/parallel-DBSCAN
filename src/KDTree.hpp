#ifndef __KDTREE_HPP
#define __KDTREE_HPP

/*
 * Reference implementation for serial version : 
 * https://github.com/crvs/KDTree
 */


#include <algorithm>
#include <functional>
#include <memory>
#include <vector>

using point_t = std::vector< float >;
using indexArr = std::vector< size_t >;
using pointIndex = typename std::pair< std::vector< float >, size_t >;

class KDNode {
   public:
    using KDNodePtr = std::shared_ptr< KDNode >;
    size_t index;
    point_t x;
    KDNodePtr left;
    KDNodePtr right;

    // initializer
    KDNode();
    KDNode(const point_t &, const size_t &, const KDNodePtr &,
           const KDNodePtr &);
    KDNode(const pointIndex &, const KDNodePtr &, const KDNodePtr &);
    ~KDNode();

    // getter
    float coord(const size_t &);

    // conversions
    explicit operator bool();
    explicit operator point_t();
    explicit operator size_t();
    explicit operator pointIndex();
};

using KDNodePtr = std::shared_ptr< KDNode >;

KDNodePtr NewKDNodePtr();

inline float dist(const point_t &, const point_t &);
inline float dist(const KDNodePtr &, const KDNodePtr &);

// Need for sorting
class comparer {
   public:
    size_t idx;
    explicit comparer(size_t idx_);
    inline bool compare_idx(
        const std::pair< std::vector< float >, size_t > &,  //
        const std::pair< std::vector< float >, size_t > &   //
    );
};

using pointIndexArr = typename std::vector< pointIndex >;

inline void sort_on_idx(const pointIndexArr::iterator &,  //
                        const pointIndexArr::iterator &,  //
                        size_t idx);

using pointVec = std::vector< point_t >;

class KDTree {
    KDNodePtr root;
    KDNodePtr leaf;

    KDNodePtr make_tree(const pointIndexArr::iterator &begin,  //
                        const pointIndexArr::iterator &end,    //
                        const size_t &length,                  //
                        const size_t &level                    //
    );

   public:
    KDTree() = default;
    explicit KDTree(pointVec point_array);

   private:
    indexArr neighborhood_(  //
        const KDNodePtr &branch,  //
        const point_t &pt,        //
        const float &rad,        //
        const size_t &level       //
    );

   public:

    indexArr neighborhood_indices(  //
        const point_t &pt,          //
        const float &rad);
};

#include "KDTree.cpp"
#endif