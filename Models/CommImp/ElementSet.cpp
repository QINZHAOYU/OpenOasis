/** ***********************************************************************************
 *    @File      :  ElementSet.cpp
 *    @Brief     :  To describe a collection of spatial elements.
 *
 ** ***********************************************************************************/
#include "ElementSet.h"
#include "Models/Utils/Exception.h"
#include "Models/Utils/StringHelper.h"
#include <algorithm>


namespace OpenOasis::CommImp
{
using namespace Utils;
using namespace std;

ElementSet::ElementSet(const shared_ptr<IElementSet> &source)
{
    mDescription      = source->GetDescription();
    mCaption          = source->GetCaption();
    mElementType      = source->GetElementType();
    mSpatialReference = source->GetSpatialReferenceSystem();

    for (int i = 0; i < source->GetElementCount(); i++)
    {
        Element element(source->GetElementId(i));
        for (int j = 0; j < source->GetNodeCount(i); j++)
        {
            Coordinate vertex{
                FP(source->GetNodeXCoordinate(i, j)),
                FP(source->GetNodeYCoordinate(i, j)),
                FP(source->GetNodeZCoordinate(i, j))};
            element.AddVertex(vertex);
        }

        mElements.emplace_back(element);
    }
}

ElementSet::ElementSet(
    const std::string &caption, const std::string &description, ElementType type,
    const std::vector<Element> &elements, const std::string &spatialRef, int version)
{
    mCaption          = caption;
    mDescription      = description;
    mElementType      = type;
    mElements         = elements;
    mSpatialReference = spatialRef;
    mVersion          = version;
}

string ElementSet::GetDescription() const
{
    return mDescription;
}

void ElementSet::SetDescription(const string &value)
{
    mDescription = value;
}

string ElementSet::GetCaption() const
{
    return mCaption;
}

void ElementSet::SetCaption(const string &value)
{
    mCaption = value;
}

string ElementSet::GetSpatialReferenceSystem() const
{
    return mSpatialReference;
}

int ElementSet::GetElementCount() const
{
    return mElements.size();
}

int ElementSet::GetVersion() const
{
    return 0;
}

ElementType ElementSet::GetElementType() const
{
    return mElementType;
}

int ElementSet::GetElementIndex(const string &elementId)
{
    if (mElements.empty())
        return -1;

    for (std::size_t i = 0; i < mElements.size(); i++)
    {
        if (mElements[i].GetId() == elementId)
        {
            return i;
        }
    }

    return -1;
}

string ElementSet::GetElementId(int index)
{
    if (mElementType != ElementType::IdBased)
        return "";

    CheckElementIndex(index);

    return mElements[index].GetId();
}

int ElementSet::GetFaceCount(int index)
{
    CheckElementIndex(index);

    return mElements[index].GetFaceCount();
}

int ElementSet::GetNodeCount(int index)
{
    if (mElementType == ElementType::IdBased)
        throw IllegalArgumentException("Get vertex count from IdBased element set.");

    CheckElementIndex(index);

    return mElements[index].GetVertexCount();
}

vector<int> ElementSet::GetFaceNodeIndices(int elementIndex, int faceIndex)
{
    CheckFaceIndex(elementIndex, faceIndex);

    return mElements[elementIndex].GetFaceNodeIndices(faceIndex);
}

double ElementSet::GetNodeXCoordinate(int elementIndex, int vertexIndex)
{
    CheckVertexIndex(elementIndex, vertexIndex);

    return mElements[elementIndex].GetVertex(vertexIndex).x;
}

double ElementSet::GetNodeYCoordinate(int elementIndex, int vertexIndex)
{
    CheckVertexIndex(elementIndex, vertexIndex);

    return mElements[elementIndex].GetVertex(vertexIndex).y;
}

double ElementSet::GetNodeZCoordinate(int elementIndex, int vertexIndex)
{
    CheckVertexIndex(elementIndex, vertexIndex);

    return mElements[elementIndex].GetVertex(vertexIndex).z;
}

void ElementSet::CheckElementIndex(int elemIndex) const
{
    if (elemIndex < 0 || mElements.empty() || elemIndex >= (int)mElements.size())
        throw ArgumentOutOfRangeException(StringHelper::FormatSimple(
            "Element index [{}] out of range [{}] .", elemIndex, mElements.size()));
}

void ElementSet::CheckVertexIndex(int elemIndex, int vertIndex) const
{
    CheckElementIndex(elemIndex);

    int vertCount = mElements.at(elemIndex).GetVertexCount();
    if (vertIndex < 0 || vertIndex >= vertCount)
        throw ArgumentOutOfRangeException(StringHelper::FormatSimple(
            "Vertex index [{}] out of range [{}] .", vertIndex, vertCount));
}

void ElementSet::CheckFaceIndex(int elemIndex, int faceIndex) const
{
    CheckElementIndex(elemIndex);

    int faceCount = mElements.at(elemIndex).GetFaceCount();
    if (faceIndex < 0 || faceIndex >= faceCount)
        throw ArgumentOutOfRangeException(StringHelper::FormatSimple(
            "Face index [{}] out of range [{}] .", faceIndex, faceCount));
}

}  // namespace OpenOasis::CommImp