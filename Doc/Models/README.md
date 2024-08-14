# 绿洲模型

*Oasis models*   

绿洲 项目参考 `OpenFoam` 的算子离散方案，针对`雷诺输运模型`实现一套通用的数值求解器。  
同时，基于 `OpenMI 2.0` 接口规范实现一套模型耦合框架。  


---------------------------------------------------------------------------------

## 模型几何描述

OpenOasis 中对几何的描述存在两套既有联系又有区别的方案，分别用于平面几何和作用域离散的描述：

- 基于平面几何的描述：端点、线段、多段线、多边形、多面体。

::: doxy.models.code
    file: Geom.h
    start: 11
    end: 15

- 基于作用域离散的描述：节点、界面、单元。

::: doxy.models.code
    file: Mesh.h
    start: 17
    end: 26

::: doxy.models.code
    file: Mesh.h
    start: 29
    end: 53

::: doxy.models.code
    file: Mesh.h
    start: 56
    end: 75

::: doxy.models.code
    file: Mesh.h
    start: 77
    end: 87    

---------------------------------------------------------------------------------

## 通用数值求解器

OpenOasis 参考 `OpenFoam` 的方程离散方案，将一般 `pde` 方程离散为定义域上的矩阵，同时采用 `Eigen` 作为矩阵求解器。

对于 `雷诺输运方程` 格式的物理模型，通过给定问题的初值、边界条件和源项定义，一方面可用使用内置封装的常用求解器，另一方面也可以通过自由选择方程各项的离散算子组装自定义求解器。

---------------------------------------------------------------------------------

### 接口规范

::: doxy.models.class
    name: OpenOasis::CommImp::Numeric::Boundary

::: doxy.models.class
    name: OpenOasis::CommImp::Numeric::Solver

::: doxy.models.class
    name: OpenOasis::CommImp::Numeric::Operator

::: doxy.models.class
    name: OpenOasis::CommImp::Numeric::Field

::: doxy.models.class
    name: OpenOasis::CommImp::Numeric::Matrix

---------------------------------------------------------------------------------

### FVM求解器

---------------------------------------------------------------------------------

---------------------------------------------------------------------------------

## 模型耦合框架

通过 `OpenMI 2.0` 接口规范，实现模型间自由耦合，包括 `pull-mode` 和 `loop-mode`。 

---------------------------------------------------------------------------------

### 接口规范

**对耦合组件的定义**

::: doxy.models.class   
    name: OpenOasis::IDescribable

::: doxy.models.class   
    name: OpenOasis::IIdentifiable    

::: doxy.models.class   
    name: OpenOasis::ILinkableComponent

::: doxy.models.class   
    name: OpenOasis::IArgument 

::: doxy.models.class   
    name: OpenOasis::IByteStateConverter 

**对耦合对象的定义**

::: doxy.models.class   
    name: OpenOasis::IBaseExchangeItem

::: doxy.models.class   
    name: OpenOasis::IInput

::: doxy.models.class
    name: OpenOasis::IOutput    

::: doxy.models.class
    name: OpenOasis::IAdaptedOutput

::: doxy.models.class
    name: OpenOasis::IAdaptedOutputFactory

**对耦合几何的定义**

::: doxy.models.class
    name: OpenOasis::ISpatialDefinition

::: doxy.models.class
    name: OpenOasis::IElementSet

**对耦合时间的定义**

::: doxy.models.class
    name: OpenOasis::ITime

::: doxy.models.class
    name: OpenOasis::ITimeSet

**对耦合数据的定义**

::: doxy.models.class
    name: OpenOasis::IValueDefinition

::: doxy.models.class
    name: OpenOasis::ICategory

::: doxy.models.class
    name: OpenOasis::IQuality

::: doxy.models.class
    name: OpenOasis::IDimension

::: doxy.models.class
    name: OpenOasis::IUnit

::: doxy.models.class
    name: OpenOasis::IQuality

::: doxy.models.class
    name: OpenOasis::IValueSet

---------------------------------------------------------------------------------
