/*
 Copyright (c) 2017
        Hugo "hrkz" Frezat <hugo.frezat@gmail.com>

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
*/

#ifndef IG_MATH_BUILDER_SAH_H
#define IG_MATH_BUILDER_SAH_H

#include "imagine/math/geom/structure/tree.h"
#include "imagine/math/geom/structure/heuristic/binned.h"

namespace ig      {
namespace builder {

template <typename Hierarchy, typename Heuristic>
class sah {
public:
  using build_node = typename Hierarchy::ref;
  using record     = typename Heuristic::record;
  static constexpr size_t branching = 4;

  explicit sah(Hierarchy& tree, size_t leaf_size, size_t block_size, float intersect, float traversal)
    : tree_{tree}
    , heuristic_{primitives_, 32}
    , leaf_size_{leaf_size}
    , block_size_{block_size * Hierarchy::M}
    , leaf_block_{static_cast<size_t>(std::log2(block_size_))}
    , build_track_{0}
    , intersect_cost_{intersect}
    , traversal_cost_{traversal}
    , leaves_(tree_.objects_.size())
    , info_{0, 0} {}

  void build();
  auto recurse(record& current) -> build_node;

private:
  auto leaf(const record& record) -> build_node;
  template <typename PrimIterator> auto node(PrimIterator begin, PrimIterator end);

  Hierarchy& tree_;
  Heuristic heuristic_;

  size_t leaf_size_;
  size_t block_size_;
  size_t leaf_block_;
  size_t build_track_;

  float intersect_cost_;
  float traversal_cost_;

  std::vector<prim_info> primitives_;
  std::vector<size_t> leaves_;
  record info_;
};

template <typename Hierarchy, typename Heuristic>
void sah<Hierarchy, Heuristic>::build() {
  for (auto& object : tree_.objects_) {
    auto& id = primitives_.emplace_back(leaves_.emplace_back(object->id), object->bounds());
    info_.expand(id.bounds);
  }
  
  auto root = recurse(info_);
  assert(
    root.index == 0 
    && "Tree building process failed");
  std::sort(
    tree_.objects_.begin(), 
    tree_.objects_.end(), 
    [&](auto& lhs, auto& rhs) { return leaves_[lhs->id] < leaves_[rhs->id]; });

  tree_.root_   = root;
  tree_.bounds_ = info_.geometry_bounds;
  tree_.optimize();
}

template <typename Hierarchy, typename Heuristic>
auto sah<Hierarchy, Heuristic>::recurse(record& current) -> build_node {
  auto split = heuristic_.find(current);
  auto leaf_cost =
    intersect_cost_ * current.geometry_bounds.half_area() * 
    current.size() * leaf_block_;
  auto split_cost = 
    traversal_cost_ * current.geometry_bounds.half_area() + 
    intersect_cost_ * split.cost;

  if (current.size() <= leaf_size_ || (current.size() <= block_size_ && leaf_cost < split_cost))
    return leaf(current);

  std::array<record, branching> children;
  std::tie(children[0], children[1]) = heuristic_.partition(split, current.begin, current.end);

  size_t n = 2;
  while (n < branching) {
    int32_t child = -1;
    float   cost  = -std::numeric_limits<float>::infinity();
    
    for (size_t i = 0; i < n; ++i) {
      auto child_cost = children[i].geometry_bounds.half_area();
      if (children[i].size() <= leaf_size_) continue;
      if (child_cost > cost) {
        child = static_cast<int32_t>(i);
        cost = child_cost;
      }
    }

    if (child == -1)
      break;

    auto& current = children[child];
    auto split = heuristic_.find(current);
    std::tie(children[child], children[n++]) = heuristic_.partition(split, current.begin, current.end);
  } return node(children.begin(), std::next(children.begin(), n));
}

template <typename Hierarchy, typename Heuristic>
auto sah<Hierarchy, Heuristic>::leaf(const record& record) -> build_node {
  auto blocks = record.size();

  build_node leaf;
  leaf.index = static_cast<uint32_t>(build_track_);
  leaf.count = static_cast<uint32_t>(blocks);

  for (size_t i = 0; i < blocks; ++i, ++build_track_) {
    leaves_
    [
      primitives_[record.begin + i].geom
    ] = build_track_;
  } return leaf;
}

template <typename Hierarchy, typename Heuristic>
template <typename PrimIterator>
auto sah<Hierarchy, Heuristic>::node(PrimIterator begin, PrimIterator end) {
  build_node node;
  node.index = static_cast<uint32_t>(tree_.nodes_.size());
  node.count = static_cast<uint32_t>(~0);

  tree_.nodes_.resize(node.index + 1);
  std::array<build_node, branching> children;
  std::transform(
    begin, 
    end,
    children.begin(),
    [this](auto& child) { return recurse(child); });

  size_t i = 0;
  std::for_each(
    begin,
    end, 
    [this, &node, &children, &i](auto& child) { 
      tree_.nodes_[node.index].invalidate(i);
      tree_.nodes_[node.index].encode_packet(i, children[i], child.geometry_bounds);
      i++; 
    }
  ); return node;
}

} // namespace builder
} // namespace ig

#endif // IG_MATH_BUILDER_SAH_H
