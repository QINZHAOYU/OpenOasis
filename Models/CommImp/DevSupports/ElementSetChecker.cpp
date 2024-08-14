/** ***********************************************************************************
 *    @File      :  ElementSetChecker.cpp
 *    @Brief     :  None.
 *
 ** ***********************************************************************************/
#include "ElementSetChecker.h"
#include "Models/CommImp/Spatial/GeomCalculator.h"
#include "Models/Utils/StringHelper.h"


namespace OpenOasis::CommImp::DevSupports
{
using namespace Spatial;
using namespace Utils;
using namespace std;

void ElementSetChecker::CheckElementSet(const shared_ptr<IElementSet> &elementSet)
{
    try
    {
        switch (elementSet->GetElementType())
        {
        case ElementType::Point:
            for (int i = 0; i < elementSet->GetElementCount(); ++i)
            {
                try
                {
                    if (elementSet->GetNodeCount(i) != 1)
                    {
                        throw runtime_error(
                            "Number of vertices in point element is different from 1.");
                    }
                }
                catch (const runtime_error &e)
                {
                    throw runtime_error(StringHelper::FormatSimple(
                        "{} ElementID = [{}] .",
                        e.what(),
                        elementSet->GetElementId(i)));
                }
            }
            break;
        case ElementType::Polyline:
            for (int i = 0; i < elementSet->GetElementCount(); ++i)
            {
                try
                {
                    Polyline polyline;
                    for (int j = 0; j < elementSet->GetNodeCount(i); j++)
                    {
                        polyline.emplace_back(Point{
                            elementSet->GetNodeXCoordinate(i, j),
                            elementSet->GetNodeYCoordinate(i, j)});
                    }
                }
                catch (const runtime_error &e)
                {
                    throw runtime_error(StringHelper::FormatSimple(
                        "{} ElementID = [{}] .",
                        e.what(),
                        elementSet->GetElementId(i)));
                }
            }
            break;
        case ElementType::Polygon:
            for (int i = 0; i < elementSet->GetElementCount(); ++i)
            {
                try
                {
                    Polygon polygon;
                    for (int j = 0; j < elementSet->GetNodeCount(i); j++)
                    {
                        polygon.emplace_back(Point{
                            elementSet->GetNodeXCoordinate(i, j),
                            elementSet->GetNodeYCoordinate(i, j)});
                    }
                }
                catch (const runtime_error &e)
                {
                    throw runtime_error(StringHelper::FormatSimple(
                        "{} ElementID = [{}] .",
                        e.what(),
                        elementSet->GetElementId(i)));
                }
            }
            break;
        default: throw runtime_error("Invalid ElementType.");
        }
    }
    catch (const runtime_error &)
    {
        throw runtime_error(StringHelper::FormatSimple(
            "ElementSet with Caption = [{}] is invalid.", elementSet->GetCaption()));
    }
}

}  // namespace OpenOasis::CommImp::DevSupports