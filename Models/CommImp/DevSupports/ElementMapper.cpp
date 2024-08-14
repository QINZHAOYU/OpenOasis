/** ***********************************************************************************
 *    @File      :  ElementMapper.cpp
 *    @Brief     :  To converts one ValueSet(inputValues) associated one ElementSet
 *    (fromElements) to a new ValuesSet that corresponds to another ElementSet
 *    (toElements).
 *
 ** ***********************************************************************************/
#include "ElementMapper.h"
#include "ExtensionMethods.h"
#include "ElementSetChecker.h"
#include "ElementSearchTree.h"
#include "Models/CommImp/Spatial/GeomCalculator.h"
#include "Models/CommImp/ValueSet2D.h"
#include "Models/CommImp/SpaceAdaptedOutputFactory.h"
#include "Models/Utils/Exception.h"
#include <numeric>


namespace OpenOasis::CommImp::DevSupports
{
using namespace Spatial;
using namespace Utils;
using namespace std;


ElementMapper::ElementMapper()
{
    mNumberOfToRows      = 0;
    mNumberOfFromColumns = 0;
    mIsInitialised       = false;
}

shared_ptr<DoubleSparseMatrix> ElementMapper::GetMappingMatrix() const
{
    return mMappingMatrix;
}

bool ElementMapper::IsInitialized() const
{
    return mIsInitialised;
}

bool ElementMapper::GetUseSearchTree() const
{
    return mUseSearchTree;
}

void ElementMapper::SetUseSearchTree(bool value)
{
    mUseSearchTree = value;
}

void ElementMapper::Initialise(
    const shared_ptr<IIdentifiable> &method,
    const shared_ptr<IElementSet>   &fromElements,
    const shared_ptr<IElementSet>   &toElements)
{
    UpdateMappingMatrix(method, fromElements, toElements);
    mIsInitialised = true;
}

shared_ptr<IValueSet> ElementMapper::MapValues(const shared_ptr<IValueSet> &inputValues)
{
    if (!mIsInitialised)
    {
        throw runtime_error(
            "ElementMapper needs to be initialised before the MapValue() can be used");
    }

    if (ExtensionMethods::ElementCount(inputValues) != mNumberOfFromColumns)
    {
        throw runtime_error(
            "Dimension mismatch between inputValues and mapping matrix");
    }

    // Make a time-space valueset of the correct size
    auto result = CreateResultValueSet(
        ExtensionMethods::TimesCount(inputValues), mNumberOfToRows);

    MapValues(result, inputValues);
    return result;
}

shared_ptr<IValueSet> ElementMapper::CreateResultValueSet(int numtimes, int numElements)
{
    vector<vector<real>> outValues;
    for (int i = 0; i < numtimes; i++)
    {
        outValues.push_back(vector<real>(numElements));
    }

    return make_shared<ValueSetFP>(outValues, nullptr);
}

void ElementMapper::MapValues(
    const shared_ptr<IValueSet> &outputValues, const shared_ptr<IValueSet> &inputValues)
{
    for (int i = 0; i < ExtensionMethods::TimesCount(inputValues); i++)
    {
        int          elemCount = outputValues->GetIndexCount({i});
        vector<real> resultDbl(elemCount);

        mMappingMatrix->Product(
            resultDbl, ExtensionMethods::GetElementValuesForTime<real>(inputValues, i));

        vector<any> result(resultDbl.begin(), resultDbl.end());
        outputValues->SetElementValuesForTime(i, result);
    }
}

void ElementMapper::UpdateMappingMatrix(
    const shared_ptr<IIdentifiable> &methodIdentifier,
    const shared_ptr<IElementSet>   &fromElements,
    const shared_ptr<IElementSet>   &toElements)
{
    try
    {
        ElementSetChecker::CheckElementSet(fromElements);
        ElementSetChecker::CheckElementSet(toElements);

        mMethod = make_optional(SpaceAdaptedOutputFactory::GetMethod(methodIdentifier));
        mNumberOfFromColumns = fromElements->GetElementCount();
        mNumberOfToRows      = toElements->GetElementCount();

        mMappingMatrix =
            make_shared<DoubleSparseMatrix>(mNumberOfToRows, mNumberOfFromColumns);

        if (fromElements->GetElementType() == ElementType::Point
            && toElements->GetElementType() == ElementType::Point)
        {
            MapFromPointToPoint(fromElements, toElements);
        }
        else if (
            fromElements->GetElementType() == ElementType::Point
            && toElements->GetElementType() == ElementType::Polyline)
        {
            MapFromPointToPolyline(fromElements, toElements);
        }
        else if (
            fromElements->GetElementType() == ElementType::Point
            && toElements->GetElementType() == ElementType::Polygon)
        {
            MapFromPointToPolygon(fromElements, toElements);
        }
        else if (
            fromElements->GetElementType() == ElementType::Polyline
            && toElements->GetElementType() == ElementType::Point)
        {
            MapFromPolylineToPoint(fromElements, toElements);
        }
        else if (
            fromElements->GetElementType() == ElementType::Polyline
            && toElements->GetElementType() == ElementType::Polyline)
        {
            MapFromPolylineToPolyline(fromElements, toElements);
        }
        else if (
            fromElements->GetElementType() == ElementType::Polyline
            && toElements->GetElementType() == ElementType::Polygon)
        {
            MapFromPolylineToPolygon(fromElements, toElements);
        }
        else if (
            fromElements->GetElementType() == ElementType::Polygon
            && toElements->GetElementType() == ElementType::Point)
        {
            MapFromPolygonToPoint(fromElements, toElements);
        }
        else if (
            fromElements->GetElementType() == ElementType::Polygon
            && toElements->GetElementType() == ElementType::Polyline)
        {
            MapFromPolygonToPolyline(fromElements, toElements);
        }
        else if (
            fromElements->GetElementType() == ElementType::Polygon
            && toElements->GetElementType() == ElementType::Polygon)
        {
            MapFromPolygonToPolygon(fromElements, toElements);
        }
        else
        {
            throw runtime_error(
                "Mapping of specified ElementTypes not included in ElementMapper");
        }
    }
    catch (const runtime_error &e)
    {
        throw runtime_error("UpdateMappingMatrix failed to update mapping matrix");
    }
}

void ElementMapper::MapFromPointToPoint(
    const shared_ptr<IElementSet> &fromElements,
    const shared_ptr<IElementSet> &toElements)
{
    try
    {
        for (int i = 0; i < mNumberOfToRows; i++)
        {
            Point toPoint = CreateXYPoint(toElements, i);
            for (int j = 0; j < mNumberOfFromColumns; j++)
            {
                Point fromPoint = CreateXYPoint(fromElements, j);
                mMappingMatrix->mValues[DoubleSparseMatrix::Index(i, j)] =
                    GeomCalculator::CalculatePointsDistance(toPoint, fromPoint);
            }
        }

        if (mMethod == ElementMapperMethod::Nearest)
        {
            for (int i = 0; i < mNumberOfToRows; i++)
            {
                double minDist = mMappingMatrix->At(i, 0);
                for (int j = 1; j < mNumberOfFromColumns; j++)
                {
                    if (mMappingMatrix->At(i, j) < minDist)
                    {
                        minDist = mMappingMatrix->At(i, j);
                    }
                }

                int denominator = 0;
                for (int j = 0; j < mNumberOfFromColumns; j++)
                {
                    if (mMappingMatrix->At(i, j) == minDist)
                    {
                        mMappingMatrix->SetValue(i, j, 1);
                        denominator++;
                    }
                    else
                    {
                        mMappingMatrix->SetValue(i, j, 0);
                    }
                }

                for (int j = 0; j < mNumberOfFromColumns; j++)
                {
                    mMappingMatrix->SetValue(
                        i, j, mMappingMatrix->At(i, j) / denominator);
                }
            }
        }
        else if (mMethod == ElementMapperMethod::Inverse)
        {
            for (int i = 0; i < mNumberOfToRows; i++)
            {
                double minDist = mMappingMatrix->At(i, 0);
                for (int j = 1; j < mNumberOfFromColumns; j++)
                {
                    if (mMappingMatrix->At(i, j) < minDist)
                    {
                        minDist = mMappingMatrix->At(i, j);
                    }
                }

                if (minDist == 0)
                {
                    int denominator = 0;
                    for (int j = 0; j < mNumberOfFromColumns; j++)
                    {
                        if (mMappingMatrix->At(i, j) == minDist)
                        {
                            mMappingMatrix->SetValue(i, j, 1);
                            denominator++;
                        }
                        else
                        {
                            mMappingMatrix->SetValue(i, j, 0);
                        }
                    }

                    for (int j = 0; j < mNumberOfFromColumns; j++)
                    {
                        mMappingMatrix->SetValue(
                            i, j, mMappingMatrix->At(i, j) / denominator);
                    }
                }
                else
                {
                    double denominator = 0;
                    for (int j = 0; j < mNumberOfFromColumns; j++)
                    {
                        mMappingMatrix->SetValue(i, j, 1 / mMappingMatrix->At(i, j));
                        denominator = denominator + mMappingMatrix->At(i, j);
                    }

                    for (int j = 0; j < mNumberOfFromColumns; j++)
                    {
                        mMappingMatrix->SetValue(
                            i, j, mMappingMatrix->At(i, j) / denominator);
                    }
                }
            }
        }
        else
        {
            throw runtime_error("methodDescription unknown for point point mapping");
        }
    }
    catch (const runtime_error &e)
    {
        throw runtime_error("Point to point mapping failed");
    }
}

void ElementMapper::MapFromPointToPolyline(
    const shared_ptr<IElementSet> &fromElements,
    const shared_ptr<IElementSet> &toElements)
{
    try
    {
        for (int i = 0; i < mNumberOfToRows; i++)
        {
            Polyline toPolyline = CreateXYPolyline(toElements, i);
            for (int j = 0; j < mNumberOfFromColumns; j++)
            {
                Point fromPoint = CreateXYPoint(fromElements, j);
                mMappingMatrix->mValues[DoubleSparseMatrix::Index(i, j)] =
                    GeomCalculator::CalculatePointToPolylineDistance(
                        toPolyline, fromPoint);
            }
        }

        if (mMethod == ElementMapperMethod::Nearest)
        {
            for (int i = 0; i < mNumberOfToRows; i++)
            {
                double minDist = (*mMappingMatrix)(i, 0);
                for (int j = 1; j < mNumberOfFromColumns; j++)
                {
                    if (mMappingMatrix->At(i, j) < minDist)
                    {
                        minDist = mMappingMatrix->At(i, j);
                    }
                }

                int denominator = 0;
                for (int j = 0; j < mNumberOfFromColumns; j++)
                {
                    if (mMappingMatrix->At(i, j) == minDist)
                    {
                        mMappingMatrix->mValues[DoubleSparseMatrix::Index(i, j)] = 1;
                        denominator++;
                    }
                    else
                    {
                        mMappingMatrix->mValues[DoubleSparseMatrix::Index(i, j)] = 0;
                    }
                }

                for (int j = 0; j < mNumberOfFromColumns; j++)
                {
                    mMappingMatrix->mValues[DoubleSparseMatrix::Index(i, j)] =
                        mMappingMatrix->At(i, j) / denominator;
                }
            }
        }
        else if (mMethod == ElementMapperMethod::Inverse)
        {
            for (int i = 0; i < mNumberOfToRows; i++)
            {
                double minDist = (*mMappingMatrix)(i, 0);
                for (int j = 1; j < mNumberOfFromColumns; j++)
                {
                    if (mMappingMatrix->At(i, j) < minDist)
                    {
                        minDist = mMappingMatrix->At(i, j);
                    }
                }

                if (minDist == 0)
                {
                    int denominator = 0;
                    for (int j = 0; j < mNumberOfFromColumns; j++)
                    {
                        if (mMappingMatrix->At(i, j) == minDist)
                        {
                            mMappingMatrix->mValues[DoubleSparseMatrix::Index(i, j)] =
                                1;
                            denominator++;
                        }
                        else
                        {
                            mMappingMatrix->mValues[DoubleSparseMatrix::Index(i, j)] =
                                0;
                        }
                    }

                    for (int j = 0; j < mNumberOfFromColumns; j++)
                    {
                        mMappingMatrix->mValues[DoubleSparseMatrix::Index(i, j)] =
                            mMappingMatrix->At(i, j) / denominator;
                    }
                }
                else
                {
                    double denominator = 0;
                    for (int j = 0; j < mNumberOfFromColumns; j++)
                    {
                        mMappingMatrix->mValues[DoubleSparseMatrix::Index(i, j)] =
                            1 / mMappingMatrix->At(i, j);
                        denominator = denominator + mMappingMatrix->At(i, j);
                    }

                    for (int j = 0; j < mNumberOfFromColumns; j++)
                    {
                        mMappingMatrix->mValues[DoubleSparseMatrix::Index(i, j)] =
                            mMappingMatrix->At(i, j) / denominator;
                    }
                }
            }
        }
        else
        {
            throw runtime_error(
                "methodDescription unknown for point to Polyline mapping");
        }
    }
    catch (const runtime_error &e)
    {
        throw runtime_error("Point to Polyline mapping failed");
    }
}

void ElementMapper::MapFromPointToPolygon(
    const shared_ptr<IElementSet> &fromElements,
    const shared_ptr<IElementSet> &toElements)
{
    try
    {
        for (int i = 0; i < mNumberOfToRows; i++)
        {
            Polygon polygon = CreateXYPolygon(toElements, i);
            int     count   = 0;
            Point   point;
            for (int n = 0; n < mNumberOfFromColumns; n++)
            {
                point = CreateXYPoint(fromElements, n);
                if (GeomCalculator::IsPointInPolygon(point, polygon))
                {
                    if (mMethod == ElementMapperMethod::Mean)
                    {
                        count = count + 1;
                    }
                    else if (mMethod == ElementMapperMethod::Sum)
                    {
                        count = 1;
                    }
                    else
                    {
                        throw runtime_error(
                            "methodDescription unknown for point to polygon mapping");
                    }
                }
            }

            for (int n = 0; n < mNumberOfFromColumns; n++)
            {
                point = CreateXYPoint(fromElements, n);
                if (GeomCalculator::IsPointInPolygon(point, polygon))
                {
                    mMappingMatrix->mValues[DoubleSparseMatrix::Index(i, n)] =
                        1.0 / count;
                }
            }
        }
    }
    catch (const runtime_error &e)
    {
        throw runtime_error("Point to polygon mapping failed");
    }
}

void ElementMapper::MapFromPolylineToPoint(
    const shared_ptr<IElementSet> &fromElements,
    const shared_ptr<IElementSet> &toElements)
{
    try
    {
        for (int i = 0; i < mNumberOfToRows; i++)
        {
            Point toPoint = CreateXYPoint(toElements, i);
            for (int j = 0; j < mNumberOfFromColumns; j++)
            {
                Polyline fromPolyline = CreateXYPolyline(fromElements, j);
                mMappingMatrix->mValues[DoubleSparseMatrix::Index(i, j)] =
                    GeomCalculator::CalculatePointToPolylineDistance(
                        fromPolyline, toPoint);
            }
        }

        if (mMethod == ElementMapperMethod::Nearest)
        {
            for (int i = 0; i < mNumberOfToRows; i++)
            {
                double minDist = (*mMappingMatrix)(i, 0);
                for (int j = 1; j < mNumberOfFromColumns; j++)
                {
                    if (mMappingMatrix->At(i, j) < minDist)
                    {
                        minDist = mMappingMatrix->At(i, j);
                    }
                }
                int denominator = 0;
                for (int j = 0; j < mNumberOfFromColumns; j++)
                {
                    if (mMappingMatrix->At(i, j) == minDist)
                    {
                        mMappingMatrix->mValues[DoubleSparseMatrix::Index(i, j)] = 1;
                        denominator++;
                    }
                    else
                    {
                        mMappingMatrix->mValues[DoubleSparseMatrix::Index(i, j)] = 0;
                    }
                }
                for (int j = 0; j < mNumberOfFromColumns; j++)
                {
                    mMappingMatrix->mValues[DoubleSparseMatrix::Index(i, j)] =
                        mMappingMatrix->At(i, j) / denominator;
                }
            }
        }
        else if (mMethod == ElementMapperMethod::Inverse)
        {
            for (int i = 0; i < mNumberOfToRows; i++)
            {
                double minDist = (*mMappingMatrix)(i, 0);
                for (int j = 1; j < mNumberOfFromColumns; j++)
                {
                    if (mMappingMatrix->At(i, j) < minDist)
                    {
                        minDist = mMappingMatrix->At(i, j);
                    }
                }
                if (minDist == 0)
                {
                    int denominator = 0;
                    for (int j = 0; j < mNumberOfFromColumns; j++)
                    {
                        if (mMappingMatrix->At(i, j) == minDist)
                        {
                            mMappingMatrix->mValues[DoubleSparseMatrix::Index(i, j)] =
                                1;
                            denominator++;
                        }
                        else
                        {
                            mMappingMatrix->mValues[DoubleSparseMatrix::Index(i, j)] =
                                0;
                        }
                    }
                    for (int j = 0; j < mNumberOfFromColumns; j++)
                    {
                        mMappingMatrix->mValues[DoubleSparseMatrix::Index(i, j)] =
                            mMappingMatrix->At(i, j) / denominator;
                    }
                }
                else
                {
                    double denominator = 0;
                    for (int j = 0; j < mNumberOfFromColumns; j++)
                    {
                        mMappingMatrix->mValues[DoubleSparseMatrix::Index(i, j)] =
                            1 / mMappingMatrix->At(i, j);
                        denominator = denominator + mMappingMatrix->At(i, j);
                    }
                    for (int j = 0; j < mNumberOfFromColumns; j++)
                    {
                        mMappingMatrix->mValues[DoubleSparseMatrix::Index(i, j)] =
                            mMappingMatrix->At(i, j) / denominator;
                    }
                }
            }
        }
        else
        {
            throw runtime_error(
                "methodDescription unknown for Polyline to point mapping");
        }
    }
    catch (const runtime_error &e)  // For all of the Point to Polyline part
    {
        throw runtime_error("Polyline to point mapping failed");
    }
}

void ElementMapper::MapFromPolylineToPolyline(
    const shared_ptr<IElementSet> &fromElements,
    const shared_ptr<IElementSet> &toElements)
{
    throw NotImplementedException(
        "ElementMapper::MapFromPolylineToPolyline not implemented.");
}

void ElementMapper::MapFromPolylineToPolygon(
    const shared_ptr<IElementSet> &fromElements,
    const shared_ptr<IElementSet> &toElements)
{
    try
    {
        // For each polygon in target.
        for (int i = 0; i < mNumberOfToRows; i++)
        {
            Polygon polygon = CreateXYPolygon(toElements, i);

            if (mMethod == ElementMapperMethod::WeightedMean)
            {
                double totalLineLengthInPolygon = 0;
                for (int n = 0; n < mNumberOfFromColumns; n++)
                {
                    Polyline Polyline = CreateXYPolyline(fromElements, n);
                    mMappingMatrix->mValues[DoubleSparseMatrix::Index(i, n)] =
                        GeomCalculator::CalculateLengthOfPolylineInsidePolygon(
                            Polyline, polygon);
                    totalLineLengthInPolygon += (*mMappingMatrix)(i, n);
                }

                if (totalLineLengthInPolygon > 0)
                {
                    for (int n = 0; n < mNumberOfFromColumns; n++)
                    {
                        mMappingMatrix->mValues[DoubleSparseMatrix::Index(i, n)] =
                            (*mMappingMatrix)(i, n) / totalLineLengthInPolygon;
                    }
                }
            }
            else if (mMethod == ElementMapperMethod::WeightedSum)
            {
                // For each line segment in Polyline
                for (int n = 0; n < mNumberOfFromColumns; n++)
                {
                    Polyline Polyline = CreateXYPolyline(fromElements, n);
                    mMappingMatrix->mValues[DoubleSparseMatrix::Index(i, n)] =
                        GeomCalculator::CalculateLengthOfPolylineInsidePolygon(
                            Polyline, polygon)
                        / GeomCalculator::CalculateLengthOfPolyline(Polyline);
                }
            }
            else
            {
                throw runtime_error(
                    "methodDescription unknown for Polyline to polygon mapping");
            }
        }
    }
    catch (const runtime_error &e)
    {
        throw runtime_error("Polyline to polygon mapping failed");
    }
}

void ElementMapper::MapFromPolygonToPoint(
    const shared_ptr<IElementSet> &fromElements,
    const shared_ptr<IElementSet> &toElements)
{
    try
    {
        if (mMethod != ElementMapperMethod::Value)
        {
            throw runtime_error(
                "methodDescription unknown for polygon to point mapping");
        }

        // Only create search tree if number of cols/rows is larger than 10/10.
        bool useSearchTree = mNumberOfFromColumns > 10 && mNumberOfToRows > 10;
        ElementSearchTree<int> fromSearchTree({});
        vector<int>            fromCandidateElmts;
        if (useSearchTree)
        {
            fromSearchTree = ElementSearchTree<int>::BuildSearchTree(fromElements);
        }
        else
        {
            fromCandidateElmts = vector<int>(mNumberOfFromColumns);
            iota(begin(fromCandidateElmts), end(fromCandidateElmts), 0);
        }

        for (int n = 0; n < mNumberOfToRows; n++)
        {
            Point point = CreateXYPoint(toElements, n);
            if (useSearchTree)
            {
                GeomExtent toExtent = GenerateExtent(point);
                fromCandidateElmts  = fromSearchTree.FindElements(toExtent);
            }

            int count = 0;

            // Check first for strict inclusion.
            for (auto i : fromCandidateElmts)
            {
                Polygon polygon = CreateXYPolygon(fromElements, i);
                if (GeomCalculator::IsPointInPolygon(point, polygon))
                {
                    mMappingMatrix->mValues[DoubleSparseMatrix::Index(n, i)] = 1.0;
                    count++;
                }
            }

            if (count == 0)
            {
                // Not strictly inside any polygon, check also edges.
                for (auto i : fromCandidateElmts)
                {
                    Polygon polygon = CreateXYPolygon(fromElements, i);
                    if (GeomCalculator::IsPointInPolygon(point, polygon))
                    {
                        mMappingMatrix->mValues[DoubleSparseMatrix::Index(n, i)] = 1.0;
                        count++;
                    }
                }
            }

            if (count > 1)
            {
                // In case of more than one hit, use average.
                for (auto i : fromCandidateElmts)
                {
                    if ((*mMappingMatrix)(n, i) != 0.0)
                    {
                        mMappingMatrix->mValues[DoubleSparseMatrix::Index(n, i)] =
                            1.0 / count;
                    }
                }
            }
        }
    }
    catch (const runtime_error &e)
    {
        throw runtime_error("Polygon to point mapping failed");
    }
}

void ElementMapper::MapFromPolygonToPolyline(
    const shared_ptr<IElementSet> &fromElements,
    const shared_ptr<IElementSet> &toElements)
{
    // Polygon to Polyline
    try
    {
        for (int i = 0; i < mNumberOfToRows; i++)
        {
            Polyline Polyline = CreateXYPolyline(toElements, i);

            if (mMethod == ElementMapperMethod::WeightedMean)
            {
                for (int n = 0; n < mNumberOfFromColumns; n++)
                {
                    Polygon polygon = CreateXYPolygon(fromElements, n);
                    mMappingMatrix->mValues[DoubleSparseMatrix::Index(i, n)] =
                        GeomCalculator::CalculateLengthOfPolylineInsidePolygon(
                            Polyline, polygon)
                        / GeomCalculator::CalculateLengthOfPolyline(Polyline);
                }

                double sum = 0;
                for (int n = 0; n < mNumberOfFromColumns; n++)
                {
                    sum += (*mMappingMatrix)(i, n);
                }

                for (int n = 0; n < mNumberOfFromColumns; n++)
                {
                    mMappingMatrix->mValues[DoubleSparseMatrix::Index(i, n)] =
                        (*mMappingMatrix)(i, n) / sum;
                }
            }
            else if (mMethod == ElementMapperMethod::WeightedSum)
            {
                for (int n = 0; n < mNumberOfFromColumns; n++)
                {
                    Polygon polygon = CreateXYPolygon(fromElements, n);
                    mMappingMatrix->mValues[DoubleSparseMatrix::Index(i, n)] =
                        GeomCalculator::CalculateLengthOfPolylineInsidePolygon(
                            Polyline, polygon)
                        / GeomCalculator::CalculateLengthOfPolyline(Polyline);
                }
            }
            else
            {
                throw runtime_error(
                    "methodDescription unknown for polygon to Polyline mapping");
            }
        }
    }
    catch (const runtime_error &e)  // catch for all of Polygon to Polyline
    {
        throw runtime_error("Polygon to Polyline mapping failed");
    }
}

void ElementMapper::MapFromPolygonToPolygon(
    const shared_ptr<IElementSet> &fromElements,
    const shared_ptr<IElementSet> &toElements)
{
    // Polygon to Polygon
    try
    {
        // Only create search tree if number of cols/rows is larger than 100/10.
        bool useSearchTree = mNumberOfFromColumns > 10 && mNumberOfToRows > 10;
        ElementSearchTree<int> fromSearchTree({});
        vector<int>            fromCandidateElmts;
        if (useSearchTree)
        {
            fromSearchTree = ElementSearchTree<int>::BuildSearchTree(fromElements);
        }
        else
        {
            fromCandidateElmts = vector<int>(mNumberOfFromColumns);
            iota(begin(fromCandidateElmts), end(fromCandidateElmts), 0);
        }

        for (int i = 0; i < mNumberOfToRows; i++)
        {
            Polygon toPolygon = CreateXYPolygon(toElements, i);
            if (useSearchTree)
            {
                GeomExtent toExtent = GenerateExtent(toPolygon);
                fromCandidateElmts  = fromSearchTree.FindElements(toExtent);
            }

            for (auto j : fromCandidateElmts)
            {
                Polygon fromPolygon = CreateXYPolygon(fromElements, j);
                mMappingMatrix->mValues[DoubleSparseMatrix::Index(i, j)] =
                    GeomCalculator::CalculatePolygonSharedArea(toPolygon, fromPolygon);
                if (mMethod == ElementMapperMethod::Distribute)
                {
                    mMappingMatrix->mValues[DoubleSparseMatrix::Index(i, j)] /=
                        GeomCalculator::CalculateAreaOfPolygon(fromPolygon);
                }
            }

            if (mMethod == ElementMapperMethod::WeightedMean)
            {
                double denominator = 0;
                for (auto j : fromCandidateElmts)
                {
                    denominator = denominator + mMappingMatrix->At(i, j);
                }

                for (auto j : fromCandidateElmts)
                {
                    if (denominator != 0)
                    {
                        mMappingMatrix->mValues[DoubleSparseMatrix::Index(i, j)] =
                            mMappingMatrix->At(i, j) / denominator;
                    }
                }
            }
            else if (mMethod == ElementMapperMethod::WeightedSum)
            {
                for (auto j : fromCandidateElmts)
                {
                    mMappingMatrix->mValues[DoubleSparseMatrix::Index(i, j)] =
                        mMappingMatrix->At(i, j)
                        / GeomCalculator::CalculateAreaOfPolygon(toPolygon);
                }
            }
            else if (mMethod != ElementMapperMethod::Distribute)
            {
                throw runtime_error(
                    "methodDescription unknown for polygon to polygon mapping");
            }
        }
    }
    catch (const runtime_error &e)  // catch for all of Polygon to Polygon.
    {
        throw runtime_error("Polygon to polygon mapping failed");
    }
}

double ElementMapper::GetValueFromMappingMatrix(int row, int column)
{
    try
    {
        ValidateIndicies(row, column);
    }
    catch (const runtime_error &e)
    {
        throw runtime_error("GetValueFromMappingMatrix failed.");
    }
    return (*mMappingMatrix)(row, column);
}

void ElementMapper::SetValueInMappingMatrix(double value, int row, int column)
{
    try
    {
        ValidateIndicies(row, column);
    }
    catch (const runtime_error &e)
    {
        throw runtime_error("SetValueInMappingMatrix failed.");
    }
    mMappingMatrix->mValues[DoubleSparseMatrix::Index(row, column)] = value;
}

void ElementMapper::ValidateIndicies(int row, int column)
{
    if (row < 0)
    {
        throw runtime_error(
            "Negative row index not allowed. GetValueFromMappingMatrix failed.");
    }
    if (row >= mNumberOfToRows)
    {
        throw runtime_error(
            "Row index exceeds mapping matrix dimension. GetValueFromMappingMatrix failed.");
    }
    if (column < 0)
    {
        throw runtime_error(
            "Negative column index not allowed. GetValueFromMappingMatrix failed.");
    }
    if (column >= mNumberOfFromColumns)
    {
        throw runtime_error(
            "Column index exceeds mapping matrix dimension. GetValueFromMappingMatrix failed.");
    }
}


GeomExtent ElementMapper::GenerateExtent(const Point &point, real adjacent)
{
    adjacent = max(adjacent, GeomCalculator::EPSILON);

    GeomExtent extent;
    extent.xMin = point.x - adjacent;
    extent.xMax = point.x + adjacent;
    extent.yMin = point.y - adjacent;
    extent.yMax = point.y + adjacent;
    extent.zMin = point.z - adjacent;
    extent.zMax = point.z + adjacent;
    return extent;
}

GeomExtent ElementMapper::GenerateExtent(const Polygon &polygon)
{
    GeomExtent extent;
    for (auto point : polygon)
    {
        GeomCalculator::UpdateExtent(extent, point);
    }

    return extent;
}

Point ElementMapper::CreateXYPoint(const shared_ptr<IElementSet> &elementSet, int index)
{
    if (elementSet->GetElementType() != ElementType::Point)
    {
        throw ArgumentOutOfRangeException(
            "elementSet cannot create Point, the element type of the element set is not Point");
    }

    return Point{
        elementSet->GetNodeXCoordinate(index, 0),
        elementSet->GetNodeYCoordinate(index, 0)};
}

Polyline
ElementMapper::CreateXYPolyline(const shared_ptr<IElementSet> &elementSet, int index)
{
    if (!(elementSet->GetElementType() == ElementType::Polyline))
    {
        throw runtime_error("Cannot create Polyline");
    }

    Polyline xyPolyline;
    for (int i = 0; i < elementSet->GetNodeCount(index); i++)
    {
        Point tempVar{
            elementSet->GetNodeXCoordinate(index, i),
            elementSet->GetNodeYCoordinate(index, i)};
        xyPolyline.push_back(tempVar);
    }

    return xyPolyline;
}

Polygon
ElementMapper::CreateXYPolygon(const shared_ptr<IElementSet> &elementSet, int index)
{
    if (elementSet->GetElementType() != ElementType::Polygon)
    {
        throw runtime_error("Cannot create Polyline");
    }

    Polygon xyPolygon;

    for (int i = 0; i < elementSet->GetNodeCount(index); i++)
    {
        Point tempVar{
            elementSet->GetNodeXCoordinate(index, i),
            elementSet->GetNodeYCoordinate(index, i)};
        xyPolygon.push_back(tempVar);
    }

    return xyPolygon;
}

ElementType ElementMapper::GetTargetElementType()
{
    throw NotImplementedException();
}

Polygon
ElementMapper::CreateFromXYPolygon(const shared_ptr<IElementSet> &elementSet, int index)
{
    if (elementSet->GetElementType() != ElementType::Polygon)
    {
        throw runtime_error("Cannot create Polyline");
    }

    Polygon xyPolygon;

    for (int i = 0; i < elementSet->GetNodeCount(index); i++)
    {
        Point tempVar{
            elementSet->GetNodeXCoordinate(index, i),
            elementSet->GetNodeYCoordinate(index, i)};
        xyPolygon.push_back(tempVar);
    }

    return xyPolygon;
}


}  // namespace OpenOasis::CommImp::DevSupports