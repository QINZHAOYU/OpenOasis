/** ***********************************************************************************
 *    @File      :  Element.cpp
 *    @Brief     :  To describe a spatial element.
 *
 ** ***********************************************************************************/
#include "Element.h"
#include "Models/Utils/Exception.h"
#include "Models/Utils/StringHelper.h"


namespace OpenOasis::CommImp::DevSupports
{
using namespace Utils;
using namespace std;


Element::Element(const Element &elem)
{
    mId                = elem.mId;
    mCaption           = elem.mCaption;
    mDescription       = elem.mDescription;
    mVertices          = elem.mVertices;
    mFaceVertexIndices = elem.mFaceVertexIndices;
}

Element::Element(
    const std::string &id, const std::string &caption, const std::string &description,
    const std::vector<Coordinate> &vertices, const std::vector<std::vector<int>> &faces)
{
    mId                = id;
    mCaption           = caption;
    mDescription       = description;
    mVertices          = vertices;
    mFaceVertexIndices = faces;
}

Element::Element(const string &id)
{
    mId          = id;
    mCaption     = id;
    mDescription = id;
}

string Element::GetCaption() const
{
    return mCaption;
}

void Element::SetCaption(const string &value)
{
    mCaption = value;
}

string Element::GetDescription() const
{
    return mDescription;
}

void Element::SetDescription(const string &value)
{
    mDescription = value;
}

string Element::GetId() const
{
    return mId;
}

vector<Coordinate> Element::GetVertices() const
{
    return mVertices;
}

void Element::SetVertices(const vector<Coordinate> &value)
{
    mVertices = value;
}

int Element::GetVertexCount() const
{
    return mVertices.size();
}

Coordinate Element::GetVertex(int index) const
{
    if (index < 0 || index >= (int)mVertices.size())
    {
        throw IllegalArgumentException(StringHelper::FormatSimple(
            "Invalid vertex query index [{}] out of range [{}] .",
            index,
            mVertices.size()));
    }
    return mVertices.at(index);
}

void Element::AddVertex(const Coordinate &vertex)
{
    mVertices.push_back(vertex);
}

int Element::GetFaceCount() const
{
    return mFaceVertexIndices.size();
}

void Element::AddFace(const vector<int> &vertexIndices)
{
    mFaceVertexIndices.push_back(vertexIndices);
}

vector<int> Element::GetFaceNodeIndices(int faceIndex) const
{
    return mFaceVertexIndices.at(faceIndex);
}

}  // namespace OpenOasis::CommImp::DevSupports