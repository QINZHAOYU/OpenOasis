/** ***********************************************************************************
 *    Copyright (C) 2022, The OpenOasis Contributors. Join us in the Oasis!
 *
 *    @File      :  ElementSetChecker.h
 *    @License   :  Apache-2.0
 *
 *    @Desc      :  None.
 *
 ** ***********************************************************************************/
#pragma once
#include "Models/Inc/IElementSet.h"
#include <memory>


namespace OpenOasis::CommImp::DevSupports
{
/// @brief Summary description for ElementSetChecker.
class ElementSetChecker
{
public:
    /// @brief Static method that validates an object with an IElementSet interface.
    /// The method raises an Exception in case IElementSet doesn't describe a valid
    /// ElementSet. The checks made are:
    ///   ElementType: Check
    ///
    ///   XYPoint    : Only one vertex in each element.
    ///
    ///   XYPolyline : At least two vertices in each element.
    ///                All line segments in each element has length > 0.
    ///
    ///   XYPolygon  : At least three vertices in each element.
    ///                Area of each element is larger than 0.
    ///                All line segments in each element has length > 0.
    ///                No line segments within an element crosses.
    static void CheckElementSet(const std::shared_ptr<IElementSet> &elementSet);
};

}  // namespace OpenOasis::CommImp::DevSupports