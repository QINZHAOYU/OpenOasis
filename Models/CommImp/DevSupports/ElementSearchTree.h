/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  XYElementSearchTree.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  2D element search tree.
 *
 ** ***********************************************************************************/
#pragma once
#include "Models/CommImp/Spatial/Geom.h"
#include "Models/CommImp/Spatial/GeomCalculator.h"
#include "Models/Inc/IElementSet.h"
#include <memory>
#include <algorithm>
#include <stdexcept>


namespace OpenOasis::CommImp::DevSupports
{
using namespace Spatial;

/// @brief Leaf in the search tree.
template <typename T>
class Leaf
{
public:
    T          mElement;  // Element in the search tree.
    GeomExtent mExtent;   // Element extent.
};


/// @brief Node in the search tree.
template <typename T>
class TreeNode
{
public:
    int MaxPointsPerNode = 10;  // Max number of points in per node.

private:
    GeomExtent            mExtent;    // Node extent.
    std::vector<TreeNode> mChildren;  // Children node of current node.
    std::vector<Point>    mPoints;    // Node points.
    std::vector<Leaf<T>>  mElements;  // Node elements.

public:
    TreeNode(const GeomExtent &extent) : mExtent(extent)
    {}

    bool HasChildren() const
    {
        return !mChildren.empty();
    }

    bool Add(const Point &point)
    {
        bool added = false;

        // Check if inside this domain.
        if (!GeomCalculator::IsPointInExtent(point, mExtent))
        {
            return false;
        }

        // If has children, add recursively.
        if (HasChildren())
        {
            for (auto &child : mChildren)
            {
                added = child.Add(point);
            }
        }
        else  // it does not have children, add it here.
        {
            // Check if it already exists.
            for (auto &existPoint : mPoints)
            {
                if (point.x == existPoint.x && point.y == existPoint.y)
                {
                    return false;
                }
            }
            // Add point.
            mPoints.push_back(point);
            added = true;

            // Check if we should subdivide.
            if ((int)mPoints.size() > MaxPointsPerNode)
            {
                SubDivide();
            }
        }

        return added;
    }

    void Add(Leaf<T> elmtLeaf)
    {
        // If no overlap, do not add it here.
        if (!GeomCalculator::IsExtentOverlap(mExtent, elmtLeaf.mExtent))
        {
            return;
        }

        if (HasChildren())
        {
            for (auto &child : mChildren)
            {
                child.Add(elmtLeaf);
            }
        }
        else
        {
            mElements.push_back(elmtLeaf);
        }
    }

    void FindElements(const GeomExtent &extent, std::vector<T> &elmts)
    {
        // If no overlap, just return.
        if (!GeomCalculator::IsExtentOverlap(mExtent, extent))
        {
            return;
        }

        // If has children, ask those.
        if (HasChildren())
        {
            for (auto &child : mChildren)
            {
                child.FindElements(extent, elmts);
            }
        }
        else  // No children, search in elements of this node.
        {
            for (auto &elmtLeaf : mElements)
            {
                if (GeomCalculator::IsExtentOverlap(elmtLeaf.mExtent, extent))
                {
                    // Check if it is already there.
                    if (std::find(begin(elmts), end(elmts), elmtLeaf.mElement)
                        != elmts.end())
                    {
                        elmts.push_back(elmtLeaf.mElement);
                    }
                }
            }
        }
    }

    int Depth(int i)
    {
        int mydepth = i + 1;
        if (!HasChildren())
        {
            return mydepth;
        }

        int depth = mydepth;
        for (auto child : mChildren)
        {
            depth = std::max(depth, child.Depth(mydepth));
        }
        return depth;
    }

    int Nodes()
    {
        if (!HasChildren())
        {
            return 1;
        }

        int count = 1;
        for (auto child : mChildren)
        {
            count += child.Nodes();
        }
        return count;
    }

    int MaxElementsInNode()
    {
        if (!HasChildren())
        {
            return mElements.size();
        }

        int count = 0;
        for (auto child : mChildren)
        {
            count = std::max(count, child.MaxElementsInNode());
        }
        return count;
    }

private:
    void SubDivide()
    {
        // Create children.
        mChildren = std::vector<TreeNode>(4, GeomExtent());

        double xMid = 0.5 * (mExtent.xMin + mExtent.xMax);
        double yMid = 0.5 * (mExtent.yMin + mExtent.yMax);

        GeomExtent tempVar{xMid, mExtent.xMax, yMid, mExtent.yMax};
        mChildren[0] = TreeNode(tempVar);

        GeomExtent tempVar2{mExtent.xMin, xMid, yMid, mExtent.yMax};
        mChildren[1] = TreeNode(tempVar2);

        GeomExtent tempVar3{mExtent.xMin, xMid, mExtent.yMin, yMid};
        mChildren[2] = TreeNode(tempVar3);

        GeomExtent tempVar4{xMid, mExtent.xMax, mExtent.yMin, yMid};
        mChildren[3] = TreeNode(tempVar4);

        // Add points of this node to the new children.
        for (auto &point : mPoints)
        {
            for (auto &child : mChildren)
            {
                child.Add(point);
            }
        }

        // Clear points of this node.
        mPoints.clear();
    }
};


/// @brief 2D element search tree.
/// The search tree structure is build up by adding a number of coordinates
/// to the search tree, usually the coordinates of the nodes in the element.
template <typename T>
class ElementSearchTree
{
private:
    /// @brief Head of the tree.
    TreeNode<T> mHead;

    /// Counts the number of unique nodes used for building up the tree.
    int mNumNodes = 0;

    /// Counts the number of elements added to the tree.
    int mNumElmts = 0;

public:
    ElementSearchTree(const GeomExtent &extent) : mHead(extent)
    {}

    /// @brief Adds point to the search tree, thereby building the tree.
    void Add(const Point &point)
    {
        if (HasElements())
        {
            throw std::runtime_error("Can not add nodes when tree has elements");
        }

        bool added = mHead.Add(point);
        if (added)
        {
            mNumNodes++;
        }
    }

    /// @brief Adds element to the search tree.
    void AddElement(T element, const GeomExtent &extent)
    {
        Leaf<T> leaf  = Leaf<T>();
        leaf.mElement = element;
        leaf.mExtent  = extent;

        mHead.Add(leaf);
        mNumElmts++;
    }

    /// @brief Finds elements with extends that overlaps the provided "extent".
    ///
    /// @param extent Extent to look for elements within.
    /// @returns A list of elements with overlapping extents.
    std::vector<T> FindElements(const GeomExtent &extent)
    {
        std::vector<T> elmts;
        mHead.FindElements(extent, elmts);
        return elmts;
    }

    /// @brief Returns the depth of the search tree.
    int Depth() const
    {
        return mHead.Depth(0);
    }

    /// @brief Returns the maximum number of elements in one search tree node.
    int MaxElementsInNode() const
    {
        return mHead.MaxElementsInNode();
    }

    /// @brief Returns the total number of nodes in the search tree.
    int TreeNodes() const
    {
        return mHead.Nodes();
    }

    /// @brief Builds search tree based on an IElementSet, containing element
    /// index references.
    ///
    /// @param elmtSet Element set to build search tree around.
    /// @returns Search tree.
    static ElementSearchTree<int>
    BuildSearchTree(const std::shared_ptr<IElementSet> &elmtSet)
    {
        // Calculate start extent.
        GeomExtent extent;

        int elementCount = elmtSet->GetElementCount();
        for (int ielmt = 0; ielmt < elementCount; ielmt++)
        {
            int vertixCount = elmtSet->GetNodeCount(ielmt);
            for (int ivert = 0; ivert < vertixCount; ivert++)
            {
                double x = elmtSet->GetNodeXCoordinate(ielmt, ivert);
                double y = elmtSet->GetNodeYCoordinate(ielmt, ivert);
                GeomCalculator::UpdateExtent(extent, Point{x, y});
            }
        }

        // Create and build search tree, based on all vertex coordinates.
        ElementSearchTree<int> tree(extent);
        for (int ielmt = 0; ielmt < elementCount; ielmt++)
        {
            int vertixCount = elmtSet->GetNodeCount(ielmt);
            for (int ivert = 0; ivert < vertixCount; ivert++)
            {
                double x = elmtSet->GetNodeXCoordinate(ielmt, ivert);
                double y = elmtSet->GetNodeYCoordinate(ielmt, ivert);
                tree.Add(Point{x, y});
            }
        }

        // Add elements to the search tree.
        for (int ielmt = 0; ielmt < elementCount; ielmt++)
        {
            int        vertixCount = elmtSet->GetNodeCount(ielmt);
            GeomExtent elmtExtent;
            for (int ivert = 0; ivert < vertixCount; ivert++)
            {
                double x = elmtSet->GetNodeXCoordinate(ielmt, ivert);
                double y = elmtSet->GetNodeYCoordinate(ielmt, ivert);
                GeomCalculator::UpdateExtent(extent, Point{x, y});
            }

            tree.AddElement(ielmt, elmtExtent);
        }

        return tree;
    }

private:
    bool HasElements() const
    {
        return mNumElmts > 0;
    }
};

}  // namespace OpenOasis::CommImp::DevSupports