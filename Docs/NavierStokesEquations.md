<p align="center">
  <a href="https://github.com/OurForce2020/OpenOasis"><img src="../Resources/logo/logo.png" alt=""></a>
</p>

---------------------------------------------------------------------------

## Navier-Stokes Equations

*流体力学·NS方程组*  

<div align="center">

**流场理论**，将流体运动看作是充满一定空间而由无数流体质点组成的连续介质运动。其中，
运动流体所占据的空间，叫做 **流场**。

不同时刻，流场中每个流体质点都有各自的空间位置、流速、加速度、压强等，研究流体运动
规律就是求解流场中这些运动要素的变化情况。

分析方法是在流场中任意取出一个 **微分平行六面体** 来研究，应用机械运动的一般原理，
求出表达流体运动规律的微分方程。
在该方法中，将流体运动看作是三维流动，各项运动要素均是空间坐标 (x, y, z) 的函数。

*质点无限多，要如何描述整个流体的运动规律？*

**欧拉法**，通过观察不同流体质点通过固定的空间点的运动情况来了解整个流场的流动情况，
即着眼于研究各种运动要素的 **分布场**。
采用欧拉法，可把流场中运动要素 f 表示为空间和时间的函数：$ f = f(x, y, z, t) $。

采用欧拉法，将目光聚焦在空间内水流的流动情况，分析对象是空间点处的运动要素变化情况，
而不去追究具体的流体质点的运动轨迹。

</div>

---------------------------------------------------------------------------

### Velocity、Acceleration、Density & Pressure

*流速、加速度、密度 和 压力*

<div align="center">

一般情况下，同一时刻不同空间点 (x, y, z) 上流体的运动要素是不同的，即使在同一点上
运动要素也是随时间 t 变化的。
不同时刻流体质点在不同空间点的 **流速 u** 在个方向的投影：

$$\begin{cases}
u_x = f_x(x, y, z, t) \\
u_y = f_y(x, y, z, t) \\
u_z = f_z(x, y, z, t) \\
\end{cases}$$

1. 若令 x,y,z 为常数，t 为变数，则得到不同时刻同一空间点处流体质点的流速变化情况；
2. 若令 t 为常数，x,y,z 为变数，则得到同一瞬间流场内不同空间点上质点流速分布情况。

其中，情况2 即 **流速场**，一般的：
+ 同一空间点上不同质点通过时的速度是不同的，即流速随时间变化；
+ 同一瞬间下不同空间点的流速也是不同的，即流速随空间位置变化。

<img src="./imgs/1.jpg" width=440 height=200>

在时刻 t，流场中 $ A, A' $ 点 x 方向投影流速分别为 $ u_x, u_x + \frac{\partial u_x}{\partial x}\mathrm{d}x $；  
在时刻 t+dt，流场中 $ A, A' $ 点 x 方向投影流速分别为 $ u_x + \frac{\partial u_x}{\partial t}\mathrm{d}t $, $ (u_x + \frac{\partial u_x}{\partial x}\mathrm{d}x) + \frac{\partial }{\partial t}(u_x + \frac{\partial u_x}{\partial x}\mathrm{d}x)\mathrm{d}t $。  


假设一个流体质点在时刻 t 从空间点 $A$ 经过时间 dt 后运动到 $A'$，**加速度 $a_x$**：
$$a_x = \frac{(u_x + \frac{\partial u_x}{\partial x}\mathrm{d}x + \frac{\partial u_x}{\partial t}\mathrm{d}t) - u_x}{\mathrm{d}t} = \frac{\partial u_x}{\partial t} + u_x\frac{\partial u_x}{\partial x}$$


分析流体质点的加速度，其中由两部分构成：
1. $ \frac{\partial u_x}{\partial t} $，由空间定点上流体质点流速随时间的变化产生，称为 **时变加速度**，又称为 **当地加速度**；
2. $ u_x\frac{\partial u_x}{\partial x} $，由同一时刻下流体质点流速随位置变化产生，称为 **位变加速度**，又称为 **位移加速度**。

在上面的例子中，流体质点的运动轨迹与 x 轴重合，dx = dy = 0，故：
$$\begin{cases}
\frac{\partial u_x}{\partial y}\mathrm{d}y = 0 \\
\frac{\partial u_x}{\partial z}\mathrm{d}z = 0 \\
\end{cases}$$

更一般的情况下，流体质点的 **加速度 $a$** 在个方向的投影：

$$\begin{cases}
a_x = \frac{\mathrm{d}u_x}{\mathrm{d}t} = \frac{\partial u_x}{\partial t} + \frac{\partial u_x}{\partial x}\frac{\mathrm{d}x}{\mathrm{d}t} + \frac{\partial u_x}{\partial y}\frac{\mathrm{d}y}{\mathrm{d}t} + \frac{\partial u_x}{\partial z}\frac{\mathrm{d}z}{\mathrm{d}t}\\
a_y = \frac{\mathrm{d}u_y}{\mathrm{d}t} = \frac{\partial u_y}{\partial t} + \frac{\partial u_y}{\partial x}\frac{\mathrm{d}x}{\mathrm{d}t} + \frac{\partial u_y}{\partial y}\frac{\mathrm{d}y}{\mathrm{d}t} + \frac{\partial u_y}{\partial z}\frac{\mathrm{d}z}{\mathrm{d}t}\\
a_z = \frac{\mathrm{d}u_z}{\mathrm{d}t} = \frac{\partial u_z}{\partial t} + \frac{\partial u_z}{\partial x}\frac{\mathrm{d}x}{\mathrm{d}t} + \frac{\partial u_z}{\partial y}\frac{\mathrm{d}y}{\mathrm{d}t} + \frac{\partial u_z}{\partial z}\frac{\mathrm{d}z}{\mathrm{d}t}\\
\end{cases}$$

因为存在 $ u_x = \frac{\mathrm{d}x}{\mathrm{d}t}, u_y = \frac{\mathrm{d}y}{\mathrm{d}t}, u_z = \frac{\mathrm{d}z}{\mathrm{d}t} $，代入上式又有：

$$\begin{cases}
a_x = \frac{\mathrm{d}u_x}{\mathrm{d}t} = \frac{\partial u_x}{\partial t} + u_x\frac{\partial u_x}{\partial x} + u_y\frac{\partial u_x}{\partial y} + u_z\frac{\partial u_x}{\partial z}\\
a_y = \frac{\mathrm{d}u_y}{\mathrm{d}t} = \frac{\partial u_y}{\partial t} + u_x\frac{\partial u_y}{\partial x} + u_y\frac{\partial u_y}{\partial y} + u_z\frac{\partial u_y}{\partial z}\\
a_z = \frac{\mathrm{d}u_z}{\mathrm{d}t} = \frac{\partial u_z}{\partial t} + u_x\frac{\partial u_z}{\partial x} + u_y\frac{\partial u_z}{\partial y} + u_z\frac{\partial u_z}{\partial z}\\
\end{cases}$$

同样的推理过程，可以得到流体 **密度** 在流场内的变化率（密度作为标量，无方向差别）：

$$\frac{\mathrm{d}\rho}{\mathrm{d}t} = \frac{\partial \rho}{\partial t} + u_x\frac{\partial \rho}{\partial x} + u_y\frac{\partial \rho}{\partial y} + u_z\frac{\partial \rho}{\partial z}$$

同样的推理过程，可以得到流体 **压力** 在流场内的变化率（压力各向同性，无方向差别）：

$$\frac{\mathrm{d}p}{\mathrm{d}t} = \frac{\partial p}{\partial t} + u_x\frac{\partial p}{\partial x} + u_y\frac{\partial p}{\partial y} + u_z\frac{\partial p}{\partial z}$$

</div>

*--- 注意：---*
1. 在推导过程中忽略了高阶微量（二阶及以上）。

2. 特别的，对于恒定流（流场任意点处运动要素不随时间变化），
必然满足：
$\begin{cases}
\frac{\partial u_x}{\partial t} = \frac{\partial u_y}{\partial t} = \frac{\partial u_z}{\partial t} = 0 \\
\frac{\partial \rho}{\partial t} = 0 \\
\frac{\partial p}{\partial t} = 0 \\
\end{cases}$
故，恒定流下流场内运动要素仅为空间坐标的函数：
$\begin{cases}
u_x = f_x(x, y, z) \\
u_y = f_y(x, y, z) \\
u_z = f_z(x, y, z) \\
\end{cases}$

3. 特别的，对于均匀流（流场任意点处运动要素相同），
必然满足：
$\begin{cases}
\frac{\partial u_x}{\partial x} = \frac{\partial u_x}{\partial y} = \frac{\partial u_x}{\partial z} = 0 \\
... \\
\frac{\partial \rho}{\partial x} = \frac{\partial \rho}{\partial y} = \frac{\partial \rho}{\partial z} = 0 \\
\frac{\partial p}{\partial x} = \frac{\partial p}{\partial y} = \frac{\partial p}{\partial z} = 0 \\
\end{cases}$
故，均匀流下流场内运动要素仅为时间的函数：
$\begin{cases}
u_x = f_x(t) \\
u_y = f_y(t) \\
u_z = f_z(t) \\
\end{cases}$

4. 恒定流中，时变加速度为 0；均匀流中，位变加速度为 0。 

---------------------------------------------------------------------------

### StreamLine & PathLine

*流线 和 迹线*

<div align="center">

**拉格朗日法**，研究流体中各个质点在不同时刻下运动的变化情况，引出迹线的概念；
**欧拉法**，研究流场内同一时刻不同流体质点的运动情况，引出流线的概念。

**流线**，是某一时刻下流速场内的一条几何曲线，在该曲线上每个流体质点的速度向量与该曲线相切。

<img src="./imgs/2.jpg" width=440 height=200>

如图所示，若在流线 AB 上取一微分段 ds，因其无限小，所以可以看作是直线。根据流线的定义可知，
A 点处的流速向量 u 与此流线微分段相切。
这里，分别以ux、uy、uz 和 dx、dy、dz 表示流速向量 u 和微分段 ds 在各坐标轴上的投影。所以有，
$$\begin{cases}
\cos{\alpha} = \frac{\mathrm{d}x}{\mathrm{d}s} = \frac{u_x}{u} \\
\cos{\beta}  = \frac{\mathrm{d}y}{\mathrm{d}s} = \frac{u_y}{u} \\
\cos{\gamma} = \frac{\mathrm{d}z}{\mathrm{d}s} = \frac{u_z}{u} \\
\end{cases}$$

得到，流线的微分方程：
$$ \frac{dx}{u_x} = \frac{dy}{u_y} = \frac{dz}{u_z} = \frac{ds}{u} $$

其中，$ u_i $ 都是 x, y, z, t 的函数；求取流速场内某时刻 t 下流线时，把 t 作为常数代入该方程，积分即可。

<img src="./imgs/10.jpg" width=420 height=180>

**流束**，流场中任意取一微小面积，通过该面积上的各点作出的一束流线群；**流管**，即流束空间的表面积。

**迹线**，是流体流动时，其中某一流体质点在不同时刻下流动经历的路线。

在上图中，若将微分段 ds 看作流体质点在时间 dt 内的位移，dx、dy、dz 表示位移 ds 在各轴上投影。
根据迹线的定义，所以有：
$$\begin{cases}
\mathrm{d}x = u_x\mathrm{d}t \\
\mathrm{d}y = u_y\mathrm{d}t \\
\mathrm{d}z = u_z\mathrm{d}t \\
\end{cases}$$

得到，迹线的微分方程：
$$ \frac{\mathrm{d}x}{u_x} = \frac{\mathrm{d}y}{u_y} = \frac{\mathrm{d}z}{u_z} = \mathrm{d}t $$

其中，$ u_i $ 都是 x, y, z, t 的函数；同时，时间 t 是自变量，质点坐标 x, y, z 均是 t 的函数。

</div>

*--- 注意：---*
1. 流线是流场的一个瞬时快照；迹线是不同时刻质点位置集合。
质点运动各时刻下、当前位置上，迹线与流速相切；但同时回看，之前位置上流速已经改变、不再相切。

2. 恒定流中，迹线与流线重合。

---------------------------------------------------------------------------

### The Basic Form of Particle Motion

*质点运动的基本形式*

<div align="center">

<img src="./imgs/3.jpg" width=240 height=280>
<img src="./imgs/4.jpg" width=440 height=280>

在流体中取一个微分平行六面体，各边长 dx, dy, dz；其中，角点 P(x, y, z) 处速度矢量 u(ux, uy, uz)； 
其它角点处速度可以通过 **泰勒级数** 表达。
由于六面体各个角点速度不同，经过时间dt 之后，不仅其位置发生移动，而且其形状也将发生变化。
作为组成六面体的每个面都将发生这种变化，因此可以取一个面来分析整个微分六面体的运动情况。

这里，选取 x-z 平面上 PQRS 作分析对象：
$$\begin{cases}
点P, (u_x, u_y, u_z) \\
点Q, (u_x + \frac{\partial u_x}{\partial x}\mathrm{d}x, u_y + \frac{\partial u_y}{\partial x}\mathrm{d}x, u_z + \frac{\partial u_z}{\partial x}\mathrm{d}x) \\
点S, (u_x + \frac{\partial u_x}{\partial z}\mathrm{d}z, u_y + \frac{\partial u_y}{\partial z}\mathrm{d}z, u_z + \frac{\partial u_z}{\partial z}\mathrm{d}z) \\
点R, (u_x + \frac{\partial u_x}{\partial x}\mathrm{d}x + \frac{\partial u_x}{\partial z}\mathrm{d}z, u_y + \frac{\partial u_y}{\partial x}\mathrm{d}x + \frac{\partial u_y}{\partial z}\mathrm{d}z, u_z + \frac{\partial u_z}{\partial x}\mathrm{d}x + \frac{\partial u_z}{\partial z}\mathrm{d}z) \\
\end{cases}$$

**位置平移**，分析面 PQRS 各角点速度，ux, uz是共有的速度组成部分；暂时不考虑角点 Q,R,S 分速度
与角点P 的相差部分，则面PQRS 将沿 x 轴移动 uxdt、沿 y 轴移动 uydt，从而到达面P'Q1R1S1 位置。
在此过程中，面 PQRS 位置发生移动，但是其形状大小均未发生变化。
由此推论，ux, uy, uz 是整个微分平行六面体在 x, y, z 方向的**位移速度**。

**线变形**，分析面 PQRS 各角点速度，点R 对 S、点Q 对 P在 x 方向均有相同的速度差值 $ \frac{\partial u_x}{x}\mathrm{d}x $，
所以，经过时间 dt 之后，边线 SR、PQ 在 x 方向的变形均为 $ \frac{\partial u_x}{x}\mathrm{d}x\mathrm{d}t $；同理，边 PS、QR 在 z 方向
的变形均为 $ \frac{\partial u_z}{z}\mathrm{d}z\mathrm{d}t $。因此，面 PQRS经过时间 dt、经过平移和边线变形之后，变成矩形 P'Q2R2S2.
在各坐标轴方向每秒边线单位长度的变形称为 **线变形速率**，又称 **线应变**。
根据定义，微分平行六面体在各坐标轴方向的线变形速率 ( 线应变) 即为：
$$\begin{cases}
x 方向,  \frac{\frac{\partial u_x}{x}\mathrm{d}x\mathrm{d}t}{\mathrm{d}x\mathrm{d}t} = \frac{\partial u_x}{x} \\
y 方向,  \frac{\frac{\partial u_y}{y}\mathrm{d}y\mathrm{d}t}{\mathrm{d}y\mathrm{d}t} = \frac{\partial u_y}{y} \\
z 方向,  \frac{\frac{\partial u_z}{z}\mathrm{d}z\mathrm{d}t}{\mathrm{d}z\mathrm{d}t} = \frac{\partial u_z}{z} \\
\end{cases}$$

现在，分析矩形平面各角点与边线垂直方向上的分速度差异对运动的影响。
边线PQ、沿z 方向上，角点Q 的分速度与P 相差 $ \frac{\partial u_z}{\partial x}\mathrm{d}x $，经时间dt 后角点Q 将比P 多移动 $ \frac{\partial u_z}{\partial x}\mathrm{d}x\mathrm{d}t $；
边线PS、沿x 方向上，角点S 的分速度与P 相差 $ \frac{\partial u_x}{\partial z}\mathrm{d}z $ ，经时间dt 后角点S 将比P 多移动 $ \frac{\partial u_x}{\partial z}\mathrm{d}z\mathrm{d}t $ ；
前者使边线PQ 逆时针偏转角度 $ \mathrm{d}\beta $，后者使边线PS 顺时针偏转角度 $ \mathrm{d}\alpha $；
最后，经过平移、线变形、边线偏转，微分矩形平面PQRS 转变为平行四边形 P'Q'R'S'，由图分析知：
$$\begin{cases}
\mathrm{d}\alpha \approx \tan(\alpha) = \frac{\frac{\partial u_x}{\partial z}\mathrm{d}z\mathrm{d}t}{\mathrm{d}z + \frac{\partial u_z}{\partial z}\mathrm{d}z\mathrm{d}t}, \Rightarrow \mathrm{d}\alpha = \frac{\partial u_x}{\partial z}\mathrm{d}t \\
\mathrm{d}\beta \approx \tan(\beta) = \frac{\frac{\partial u_z}{\partial x}\mathrm{d}x\mathrm{d}t}{\mathrm{d}x + \frac{\partial u_x}{\partial x}\mathrm{d}x\mathrm{d}t}, \Rightarrow \mathrm{d}\beta = \frac{\partial u_z}{\partial x}\mathrm{d}t \\
\end{cases}$$

对于因为边线偏转导致的矩形平面 P'Q2R2S2 的变形过程，其实可以分解为两个部分：角变形 和 旋转。

**角变形**，边线P'Q2 首先逆时针偏转角度 $ \mathrm{d}\alpha - \gamma $，而边线P'S2 同时顺时针偏转角度 $ \mathrm{d}\beta + \gamma $，
要求两个偏转角度相等。如此，矩形平面P'Q2R2S2变形为平行四边形P'Q3R3S3，二者等分角线重合，
所以矩形平面 P'Q2R2S2 此时只有直角纯变形。由于角变形中两条边线的偏转角度是相等的，所以有：
$$ \mathrm{d}\alpha - \gamma = \mathrm{d}\beta + \gamma, \Rightarrow \gamma = \frac{\mathrm{d}\alpha - \mathrm{d}\beta}{2} $$

绕各坐标轴方向每秒直角边线的角度变形称为 **角变形速率**，又称 **角应变**。
$$ y 方向，\theta_y = \frac{\mathrm{d}\alpha - \gamma}{\mathrm{d}t} = \frac{1}{2}(\frac{\mathrm{d}\alpha + \mathrm{d}\beta}{\mathrm{d}t}) = \frac{1}{2}(\frac{\partial u_x}{\partial z} + \frac{\partial u_z}{\partial x})$$

由此推论，微分六面体的角变形速率：
$$\begin{cases}
\theta_x = \frac{1}{2}(\frac{\partial u_y}{\partial z} + \frac{\partial u_z}{\partial y}) \\
\theta_y = \frac{1}{2}(\frac{\partial u_x}{\partial z} + \frac{\partial u_z}{\partial x}) \\
\theta_z = \frac{1}{2}(\frac{\partial u_x}{\partial y} + \frac{\partial u_y}{\partial x}) \\
\end{cases}$$

**旋转**，平行四边形P'Q3R3S3 绕通过P' 点的 y方向的轴顺时针旋转角度 $ \gamma $ 变成P'Q'R'S'。
( 旋转是由于边线偏转角度 $ \mathrm{d}\alpha, \mathrm{d}\beta $ 不等所产生的。P'Q3R3S3 的等分角线P'M 将旋转 $ \gamma $ 到达P'M'。)

绕各坐标轴方向每秒平面的纯旋转角称为 **旋转角速度**。
$$ y 方向，\omega_y = \frac{\gamma}{\mathrm{d}t} = \frac{1}{2}(\frac{\mathrm{d}\alpha - \mathrm{d}\beta}{2}) = \frac{1}{2}(\frac{\partial u_x}{\partial z} - \frac{\partial u_z}{\partial x}) $$

由此推论，微分六面体的旋转角速度：
$$\begin{cases}
\omega_x = \frac{1}{2}(\frac{\partial u_z}{\partial y} - \frac{\partial u_y}{\partial z}) \\
\omega_y = \frac{1}{2}(\frac{\partial u_x}{\partial z} - \frac{\partial u_z}{\partial x}) \\
\omega_z = \frac{1}{2}(\frac{\partial u_y}{\partial x} - \frac{\partial u_x}{\partial y}) \\
\end{cases}$$

综上分析，微分平行六面体最普遍的运动形式均由平移、线应变、角应变、旋转四种基本形式所组成；
当微分六面体无限缩小，即变成质点，故流体质点运动的基本形式也是平移、线应变、角应变、旋转。

</div>

*--- 注意：---*
1. 分析微分平行六面体时，一般假设速度分量沿坐标轴正方向增大。
2. 推导偏转角度时，分母中忽略高阶项（二阶及以上）。
3. 记忆旋转角速度时，可以通过右手定则坐标系结合定义辅助记忆。

---------------------------------------------------------------------------

### Eddy & Potenial Flow

*涡流 和 势流*

<div align="center">

按照流体质点本身有无旋转，将流体运动分为 **有涡流** 和 **无涡流**。
涡是指流体质点绕其自身轴旋转的运动，与通常的旋转运动不同。

<img src="./imgs/6.jpg" width=400 height=180>

对于无涡流，
$$\begin{cases}
\omega_x = \frac{1}{2}(\frac{\partial u_z}{\partial y} - \frac{\partial u_y}{\partial z}) = 0, \Rightarrow \frac{\partial u_z}{\partial y} = \frac{\partial u_y}{\partial z} \\
\omega_y = \frac{1}{2}(\frac{\partial u_x}{\partial z} - \frac{\partial u_z}{\partial x}) = 0, \Rightarrow \frac{\partial u_x}{\partial z} = \frac{\partial u_z}{\partial x} \\
\omega_z = \frac{1}{2}(\frac{\partial u_y}{\partial x} - \frac{\partial u_x}{\partial y}) = 0, \Rightarrow \frac{\partial u_y}{\partial x} = \frac{\partial u_x}{\partial y} \\
\end{cases}$$

**势函数**，对某一标量函数 $ \varphi $ 求梯度，得到矢量 $ \upsilon $，则称 $ \varphi $ 为 $ \upsilon $ 的势函数。
势函数是某些矢量场上关于空间位置处势能的函数，是对场中能量的刻画。
例如重力场，重力势函数 $mg*z$ 在 x,y,z 方向上的偏导就是重力势能在不同方向上的变化速度，即重力。
同理，流速场上可能存在流速势函数，势函数对 x,y,z 的偏导就是流速势在不同方向的变化速度，即流速。

若存在流场势函数$\varphi$，则有
$$\begin{cases}
u_x = \frac{\partial \varphi}{\partial x} \\
u_y = \frac{\partial \varphi}{\partial y} \\
u_z = \frac{\partial \varphi}{\partial z} \\
\end{cases}$$

在 x,y,z 方向继续求导，有
$$\begin{cases}
\frac{\partial^2 \varphi}{\partial x \partial y} = \frac{\partial u_x}{\partial y} = \frac{\partial u_y}{\partial x}  \\
\frac{\partial^2 \varphi}{\partial x \partial z} = \frac{\partial u_x}{\partial z} = \frac{\partial u_z}{\partial x}  \\
\frac{\partial^2 \varphi}{\partial z \partial y} = \frac{\partial u_z}{\partial y} = \frac{\partial u_y}{\partial z}  \\
\end{cases}$$

由此可知，若流场中的流体是无涡流，则必然存在流速势函数。所以，无涡流又称 **势流**，继而
$$\begin{gather*}
\frac{\partial \varphi}{\partial x}\mathrm{d}x + \frac{\partial \varphi}{\partial y}\mathrm{d}y + \frac{\partial \varphi}{\partial z}\mathrm{d}z = u_x\mathrm{d}x + u_y\mathrm{d}y + u_z\mathrm{d}z \\
\mathrm{d}\varphi = u_x\mathrm{d}x + u_y\mathrm{d}y + u_z\mathrm{d}z \end{gather*}$$


<img src="./imgs/7.jpg" width=440 height=240>

有涡流可用旋转角速度的矢量来表征，即 **涡流场**；类似流速场，可以引入 涡线 和 涡束等概念。
**涡线**，某一瞬时涡量场中一条几何曲线，线上各质点在同一瞬时的旋转角速度都与该曲线相切。
$$ \frac{\mathrm{d}x}{\omega_x} = \frac{\mathrm{d}y}{\omega_y} = \frac{\mathrm{d}z}{\omega_z} $$

类似于流线，涡线本身不会相交；恒定流下，涡线形状保持不变。

**涡束**，类似于流束，任意取一微小面积，通过该面积上的各点作出一束涡线空间，即微小涡束。
在微小涡束断面上各点的旋转角速度可以认为是相等的。
**涡旋通量**，类似于流量，是微小涡束的横断面的面积与其上的旋转角速度之积，又称 **涡旋强度**。

<img src="./imgs/8.jpg" width=440 height=260>

在流场中取任意的封闭周线，将流速矢量沿周线的积分，称为 **速度环量** ，可以表示涡旋强度。
在上图中，封闭周线 C 上取微小弧段 Δs，弧段上流速 u 与左手定则绕行下的切线的夹角为 α，
$$ \Gamma = lim \sum {u\cos(\alpha)\Delta s} = \oint_{C} u\cos(\alpha)\mathrm{d}s = \oint_{C} u\cos(u, \mathrm{d}s)\mathrm{d}s $$

$\Gamma$ 即为沿封闭周线C 的速度环量；如果周线上切向速度与周线绕行方向相同，则速度环量为正。
依解析几何，两直线夹角的余弦等于两直线方向余弦的对应乘机之和：
$$\begin{align*}
cos(\alpha) &= \cos(u, \mathrm{d}s) \\
&= \cos(u,x)\cos(\mathrm{d}s,x) + \cos(u,y)\cos(\mathrm{d}s,y) + \cos(u,z)\cos(\mathrm{d}s,z) \\
&= \frac{u_x}{u}\frac{\mathrm{d}x}{\mathrm{d}s} + \frac{u_y}{u}\frac{\mathrm{d}y}{\mathrm{d}s} + \frac{u_z}{u}\frac{\mathrm{d}z}{\mathrm{d}s}\\
\end{align*}$$

$$\begin{align*}
&\Rightarrow u\cos(\alpha)\mathrm{d}s = u_x\mathrm{d}x + u_y\mathrm{d}y + u_z\mathrm{d}z \\
&\Rightarrow \Gamma = \oint_{c} (u_x\mathrm{d}x + u_y\mathrm{d}y + u_z\mathrm{d}z)
\end{align*}$$

如果流体运动是无涡的，则必然存在流速势函数，则有：
$$ \Gamma = \oint_{c} \mathrm{d}\varphi = [\varphi]_{A}^{A} = 0 $$

由此，在无涡流场中的任意封闭周线的速度环量都是 0。

</div>

*--- 注意：---*
1. 环线积分中绕行方向遵循左手定则，即绕行过程中保持左手位于环内。

2. 曲线积分中，直角坐标系与极坐标系的转换（注意积分区间的转换）： 

$$\begin{align*}
\int_{L} O(x,y,z)\mathrm{d}x + P(x,y,z)\mathrm{d}y + Q(x,y,z)\mathrm{d}z &= \int_{L} O\cos\alpha \mathrm{d}s + P\cos\beta\mathrm{d}s + Q\cos\gamma\mathrm{d}s \\
&= \int_{L} (O\cos\alpha + P\cos\beta + Q\cos\gamma) \mathrm{d}s \\
\mathrm{d}s = \mathrm{d}\theta * r
\end{align*}$$

---------------------------------------------------------------------------

### The Continuity Equation of Fluid Motion

*流体运动的连续性方程*

<div align="center">

因流体是连续介质，若在流场中任意选定一个封闭空间，在某一给定时间段中流入封闭空间的流体质量
与流出的流体质量之差应该等于该封闭空间内因密度变化引起的流体质量变化。
如果流体是不可压缩的均质流体，则封闭空间流进与流出的流体质量应该相等。
以上质量守恒关系以数学分析表达成微分方程，即称为**流体运动的连续性方程**。

<img src="./imgs/9.jpg" width=440 height=220>

流场中一微分平行六面体，边长为 dx, dy, dz，形心A(x,y,z) 速度 u(ux,uy,uz)、密度 ρ。经微小时段 dt，

自左面流入的流体质量：$q_{i} = (\rho - \frac{\partial \rho}{\partial x}\frac{\mathrm{d}x}{2})(u_x - \frac{\partial u_x}{\partial x}\frac{\mathrm{d}x}{2})\mathrm{d}y\mathrm{d}z\mathrm{d}t$

自右面流出的流体质量：$q_{o} = (\rho + \frac{\partial \rho}{\partial x}\frac{\mathrm{d}x}{2})(u_x + \frac{\partial u_x}{\partial x}\frac{\mathrm{d}x}{2})\mathrm{d}y\mathrm{d}z\mathrm{d}t$

dt 时段内，x 方向流入与流出六面体的质量差：
$$\Delta m_x = q_o - q_i = - (u_x \frac{\partial \rho}{\partial x} + \rho \frac{\partial u_x}{\partial x})\mathrm{d}x\mathrm{d}y\mathrm{d}z\mathrm{d}t = - \frac{\partial \rho u_x}{\partial x}\mathrm{d}x\mathrm{d}y\mathrm{d}z\mathrm{d}t$$

同理，在 y,z 方向流入与流出六面体的质量差：
$$\begin{gather*}
\Delta m_y = - \frac{\partial \rho u_y}{\partial y}\mathrm{d}x\mathrm{d}y\mathrm{d}z\mathrm{d}t \\ 
\Delta m_z = - \frac{\partial \rho u_z}{\partial z}\mathrm{d}x\mathrm{d}y\mathrm{d}z\mathrm{d}t
\end{gather*}$$

dt 时段内，六面体内流体密度由 $\rho$ 变为 $\rho + \frac{\partial \rho}{\partial t}$，引起的质量变化
$$\Delta m = \frac{\partial \rho}{\partial t}\mathrm{d}x\mathrm{d}y\mathrm{d}z\mathrm{d}t $$

dt 时段内，微分平行六面体内流体的质量守恒（介质连续）原理：
$$\begin{gather*}
\Delta m = \Delta m_x + \Delta m_y + \Delta m_z \\
\frac{\partial \rho}{\partial t}\mathrm{d}x\mathrm{d}y\mathrm{d}z\mathrm{d}t = - [\frac{\partial \rho u_x}{\partial x} + \frac{\partial \rho u_y}{\partial y} + \frac{\partial \rho u_z}{\partial z}]\mathrm{d}x\mathrm{d}y\mathrm{d}z\mathrm{d}t \\
\end{gather*}$$

除以 $\mathrm{d}x\mathrm{d}y\mathrm{d}z\mathrm{d}t$ 之后得到， **可压缩流体非恒定流的连续性方程**：
$$\frac{\partial \rho}{\partial t} + [\frac{\partial \rho u_x}{\partial x} + \frac{\partial \rho u_y}{\partial y} + \frac{\partial \rho u_z}{\partial z}] = 0$$

对不可压缩流体，ρ 为常数、与空间时间无关，连续性方程简化：
$$\frac{\partial u_x}{\partial x} + \frac{\partial u_y}{\partial y} + \frac{\partial u_z}{\partial z} = 0 \,\, \mathrm{Or} \,\, \mathrm{div}\, \textbf{u} = 0$$

由于 $\frac{\partial u_x}{\partial x}, \frac{\partial u_y}{\partial y},\frac{\partial u_z}{\partial z}$ 表示微分六面体沿 x，y，z 方向的线变形速率，故微分平行六面体的体积变化：
$$(\frac{\partial u_x}{\partial x} + \frac{\partial u_y}{\partial y} + \frac{\partial u_z}{\partial z} )\mathrm{d}x\mathrm{d}y\mathrm{d}z\mathrm{d}t$$

对不可压缩流体，微分平行六面体经平移和线变形体积大小不变，角变形和旋转亦不改变体积大小。
$$\begin{align*}
\iiint_{V} \mathrm{div}\, \textbf{u} \,\mathrm{d}V &= \iiint_{V} (\frac{\partial u_x}{\partial x} + \frac{\partial u_y}{\partial y} + \frac{\partial u_z}{\partial z} )\mathrm{d}x\mathrm{d}y\mathrm{d}z\mathrm{d}t \\
&= \iint_{S} u_n \mathrm{d}s \\
&= 0 \\
\end{align*}$$

S是体积V 的封闭表面积；un是S 上各点处的流速在该处外法线方向的投影；$\iint_{S} u_n \mathrm{d}s$ 即速度通量。

分析恒定流下的流管，由于流管侧表面上 un = 0，故
$$\iint_{S} u_n \mathrm{d}s = - \iint_{A_i} u_i\mathrm{d}A_i + \iint_{A_o} u_o\mathrm{d}A_o = 0 $$

Ai,ui 是流管流入横断面的面积、流速； Ao,uo 是流管流出横断面的面积、流速。
$$\begin{gather*}
\iint_{A_i} u_i\mathrm{d}A_i = \iint_{A_o} u_o\mathrm{d}A_o \\
Q_i = Q_o
\end{gather*}$$

恒定流时流管一端有流量流入，对不可压缩流体，另一端必然有相等的流量流出。

</div>

*--- 注意：---*
1. 分析微分六面体内质量变化时，认为六面体空间是固定的、不变的。

2. 计算微分六面体中面上流量时，以形心流速作面上平均流速；或者说，求速度通量时ux在面上线性分布。

3. $\mathrm{div}\, \textbf{u}$ 称为速度散度，结果为标量。

4. 通过高斯定理，可以将体积积分转换为曲面积分。

---------------------------------------------------------------------------







