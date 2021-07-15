<p align="center">
  <a href="https://github.com/OurForce2020/OpenOasis"><img src="../Resources/logo/logo.png" alt=""></a>
</p>

---------------------------------------------------------------------------

## Navier-Stokes Equations

*流体力学·NS方程组*  

<div align="center">

**流场理论**，将流体运动看作是充满一定空间而由无数流体质点组成的连续介质运动。其中，  
运动流体所占据的空间，叫做 **流场**。  

不同时刻，流场中不同位置的流体质点有各自的流速、加速度、压强、密度，研究流体运动  
规律就是求解流场中这些运动要素的变化情况。  

分析方法是在流场中任意取出一个 **微分平行六面体** 来研究，应用机械运动的一般原理，  
求出表达流体运动规律的微分方程。  
在该方法中，将流体运动看作是三维流动，各项运动要素均是空间坐标 (x, y, z) 的函数。  

*质点无限多，要如何描述整个流体的运动规律？*  

**欧拉法**，通过观察不同流体质点通过**固定**的空间点的运动情况来了解整个流场的流动情况，  
即着眼于研究各种运动要素的 **分布场**。  
采用欧拉法，可把流场中运动要素 f 表示为空间和时间的函数：$ f = f(x, y, z, t) $。  

采用欧拉法，将目光聚焦在空间内水流流动情况，分析对象是空间点处运动要素变化情况，    
而不去追究具体的流体质点的运动轨迹。    

</div>

---------------------------------------------------------------------------

### Velocity、Acceleration、Density & Pressure

*流速、加速度、密度 和 压强*

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

更一般的情况下，流体质点的 **加速度 $a$** 在个方向的投影  

$$\begin{cases}
a_x = \frac{\mathrm{d}u_x}{\mathrm{d}t} = \frac{\partial u_x}{\partial t} + \frac{\partial u_x}{\partial x}\frac{\mathrm{d}x}{\mathrm{d}t} + \frac{\partial u_x}{\partial y}\frac{\mathrm{d}y}{\mathrm{d}t} + \frac{\partial u_x}{\partial z}\frac{\mathrm{d}z}{\mathrm{d}t}\\
  \\
a_y = \frac{\mathrm{d}u_y}{\mathrm{d}t} = \frac{\partial u_y}{\partial t} + \frac{\partial u_y}{\partial x}\frac{\mathrm{d}x}{\mathrm{d}t} + \frac{\partial u_y}{\partial y}\frac{\mathrm{d}y}{\mathrm{d}t} + \frac{\partial u_y}{\partial z}\frac{\mathrm{d}z}{\mathrm{d}t}\\
  \\
a_z = \frac{\mathrm{d}u_z}{\mathrm{d}t} = \frac{\partial u_z}{\partial t} + \frac{\partial u_z}{\partial x}\frac{\mathrm{d}x}{\mathrm{d}t} + \frac{\partial u_z}{\partial y}\frac{\mathrm{d}y}{\mathrm{d}t} + \frac{\partial u_z}{\partial z}\frac{\mathrm{d}z}{\mathrm{d}t}\\
\end{cases}$$

因为存在 $ u_x = \frac{\mathrm{d}x}{\mathrm{d}t}, u_y = \frac{\mathrm{d}y}{\mathrm{d}t}, u_z = \frac{\mathrm{d}z}{\mathrm{d}t} $，代入上式得到   

流体 **速度** 在流速场内的变化率为：
$$\frac{\mathrm{d}u}{\mathrm{d}t} = \frac{\partial u}{\partial t} + u_x\frac{\partial u}{\partial x} + u_y\frac{\partial u}{\partial y} + u_z\frac{\partial u}{\partial z}$$

同样的推理过程，可以得到流体 **密度** 在流场内的变化率（密度作为标量，无方向差别）：  

$$\frac{\mathrm{d}\rho}{\mathrm{d}t} = \frac{\partial \rho}{\partial t} + u_x\frac{\partial \rho}{\partial x} + u_y\frac{\partial \rho}{\partial y} + u_z\frac{\partial \rho}{\partial z}$$

同样的推理过程，可以得到流体 **压强** 在流场内的变化率（压强各向异性，有方向差别）：  

$$\frac{\mathrm{d}p}{\mathrm{d}t} = \frac{\partial p}{\partial t} + u_x\frac{\partial p}{\partial x} + u_y\frac{\partial p}{\partial y} + u_z\frac{\partial p}{\partial z}$$

</div>

*--- 注意：---*
1. 在推导过程中忽略了高阶微量（二阶及以上）。  

2. 特别的，对于恒定流（流场任意点处运动要素不随时间变化）， 

   必然满足：$\begin{cases}
\frac{\partial u_x}{\partial t} = \frac{\partial u_y}{\partial t} = \frac{\partial u_z}{\partial t} = 0 \\
\frac{\partial \rho}{\partial t} = 0 \\
\frac{\partial p}{\partial t} = 0 \\
\end{cases}$, 故运动要素仅为空间的函数：$\begin{cases}
u_x = f_x(x, y, z) \\
u_y = f_y(x, y, z) \\
u_z = f_z(x, y, z) \\
\end{cases}$

3. 特别的，对于均匀流（流场任意点处运动要素相同），  
必然满足：$\begin{cases}
\frac{\partial u_x}{\partial x} = \frac{\partial u_x}{\partial y} = \frac{\partial u_x}{\partial z} = 0 \\
... \\
\frac{\partial \rho}{\partial x} = \frac{\partial \rho}{\partial y} = \frac{\partial \rho}{\partial z} = 0 \\
\frac{\partial p}{\partial x} = \frac{\partial p}{\partial y} = \frac{\partial p}{\partial z} = 0 \\
\end{cases}$, 故运动要素仅为时间的函数：$\begin{cases}
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

### The Basic Performance of Particle Motion

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
  \\
点Q, (u_x + \frac{\partial u_x}{\partial x}\mathrm{d}x, u_y + \frac{\partial u_y}{\partial x}\mathrm{d}x, u_z + \frac{\partial u_z}{\partial x}\mathrm{d}x) \\
  \\
点S, (u_x + \frac{\partial u_x}{\partial z}\mathrm{d}z, u_y + \frac{\partial u_y}{\partial z}\mathrm{d}z, u_z + \frac{\partial u_z}{\partial z}\mathrm{d}z) \\
  \\
点R, (u_x + \frac{\partial u_x}{\partial x}\mathrm{d}x + \frac{\partial u_x}{\partial z}\mathrm{d}z, u_y + \frac{\partial u_y}{\partial x}\mathrm{d}x + \frac{\partial u_y}{\partial z}\mathrm{d}z, u_z + \frac{\partial u_z}{\partial x}\mathrm{d}x + \frac{\partial u_z}{\partial z}\mathrm{d}z) \\
\end{cases}$$

**位置平移**，分析面 PQRS 各角点速度，ux, uz是共有的速度组成部分；暂时不考虑角点 Q,R,S 分速度  
与角点P 的相差部分，则面PQRS 将沿 x 轴移动 uxdt、沿 y 轴移动 uydt，从而到达面P'Q1R1S1 位置。  
在此过程中，面 PQRS 位置发生移动，但是其形状大小均未发生变化。  
由此推论，ux, uy, uz 是整个微分平行六面体在 x, y, z 方向的**位移速度**。  

**线变形**，分析面 PQRS 各角点速度，点R 对 S、点Q 对 P在 x 方向均有相同的速度差值 $ \frac{\partial u_x}{\partial x}\mathrm{d}x $，  
所以，经过时间 dt 之后，边线 SR、PQ 在 x 方向的变形均为 $ \frac{\partial u_x}{\partial x}\mathrm{d}x\mathrm{d}t $；同理，边 PS、QR 在 z 方向  
的变形均为 $ \frac{\partial u_z}{\partial z}\mathrm{d}z\mathrm{d}t $。因此，面 PQRS经过时间 dt、经过平移和边线变形之后，变成矩形 P'Q2R2S2.  
在各坐标轴方向每秒边线单位长度的变形称为 **线变形速率**，又称 **线应变**。  
根据定义，微分平行六面体在各坐标轴方向的线变形速率 ( 线应变) 即为：  
$$\begin{cases}
x 方向,  \frac{\frac{\partial u_x}{\partial x}\mathrm{d}x\mathrm{d}t}{\mathrm{d}x\mathrm{d}t} = \frac{\partial u_x}{\partial x} \\
  \\
y 方向,  \frac{\frac{\partial u_y}{\partial y}\mathrm{d}y\mathrm{d}t}{\mathrm{d}y\mathrm{d}t} = \frac{\partial u_y}{\partial y} \\
  \\
z 方向,  \frac{\frac{\partial u_z}{\partial z}\mathrm{d}z\mathrm{d}t}{\mathrm{d}z\mathrm{d}t} = \frac{\partial u_z}{\partial z} \\
\end{cases}$$

现在，分析矩形平面各角点与边线垂直方向上的分速度差异对运动的影响:    

边线PQ、沿z 方向上，角点Q 的分速度与P 相差 $ \frac{\partial u_z}{\partial x}\mathrm{d}x $，经时间dt 后角点Q 将比P 多移动 $ \frac{\partial u_z}{\partial x}\mathrm{d}x\mathrm{d}t $；  
边线PS、沿x 方向上，角点S 的分速度与P 相差 $ \frac{\partial u_x}{\partial z}\mathrm{d}z $ ，经时间dt 后角点S 将比P 多移动 $ \frac{\partial u_x}{\partial z}\mathrm{d}z\mathrm{d}t $ ；  
前者使边线PQ 逆时针偏转角度 $ \mathrm{d}\beta $，后者使边线PS 顺时针偏转角度 $ \mathrm{d}\alpha $；  
最后，经过平移、线变形、边线偏转，微分矩形平面PQRS 转变为平行四边形 P'Q'R'S'，由图分析知：  
$$\begin{cases}
\mathrm{d}\alpha \approx \tan(\alpha) = \frac{\frac{\partial u_x}{\partial z}\mathrm{d}z\mathrm{d}t}{\mathrm{d}z + \frac{\partial u_z}{\partial z}\mathrm{d}z\mathrm{d}t}, \Rightarrow \mathrm{d}\alpha = \frac{\partial u_x}{\partial z}\mathrm{d}t \\
  \\
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
  \\
\theta_y = \frac{1}{2}(\frac{\partial u_x}{\partial z} + \frac{\partial u_z}{\partial x}) \\
  \\
\theta_z = \frac{1}{2}(\frac{\partial u_x}{\partial y} + \frac{\partial u_y}{\partial x}) \\
\end{cases}$$

**旋转**，平行四边形P'Q3R3S3 绕通过P' 点的 y方向的轴顺时针旋转角度 $ \gamma $ 变成P'Q'R'S'。  
( 旋转是由于边线偏转角度 $ \mathrm{d}\alpha, \mathrm{d}\beta $ 不等所产生的。P'Q3R3S3 的等分角线P'M 将旋转 $ \gamma $ 到达P'M'。)  
绕各坐标轴方向每秒平面的纯旋转角，称为 **旋转角速度**。  
$$ y 方向，\omega_y = \frac{\gamma}{\mathrm{d}t} = \frac{1}{2}(\frac{\mathrm{d}\alpha - \mathrm{d}\beta}{2}) = \frac{1}{2}(\frac{\partial u_x}{\partial z} - \frac{\partial u_z}{\partial x}) $$

由此推论，微分六面体各方向的旋转角速度：  
$$\begin{cases}
\omega_x = \frac{1}{2}(\frac{\partial u_z}{\partial y} - \frac{\partial u_y}{\partial z}) \\
  \\
\omega_y = \frac{1}{2}(\frac{\partial u_x}{\partial z} - \frac{\partial u_z}{\partial x}) \\
  \\
\omega_z = \frac{1}{2}(\frac{\partial u_y}{\partial x} - \frac{\partial u_x}{\partial y}) \\
\end{cases}$$

综上分析，微分平行六面体最普遍的运动形式均由平移、线应变、角应变、旋转四种基本形式所组成；  
当微分六面体**无限缩小**，即变成质点，故流体质点运动的基本形式也是平移、线应变、角应变、旋转。  

</div>

*--- 注意：---*
1. 分析微分平行六面体时，一般假设速度分量沿坐标轴正方向增大。
2. 推导偏转角度时，分母中忽略高阶项（二阶及以上）。
3. 记忆旋转角速度时，可以通过右手定则坐标系结合定义辅助记忆。

---------------------------------------------------------------------------

### Vortex Flow & Potential Flow

*涡流 和 势流*

<div align="center">

按照流体质点本身有无旋转，将流体运动分为 **有涡流** 和 **无涡流**。  
涡是指流体质点绕其自身轴旋转的运动，与通常的旋转运动不同。  

<img src="./imgs/6.jpg" width=400 height=180>

对于无涡流，  
$$\begin{cases}
\omega_x = \frac{1}{2}(\frac{\partial u_z}{\partial y} - \frac{\partial u_y}{\partial z}) = 0, \Rightarrow \frac{\partial u_z}{\partial y} = \frac{\partial u_y}{\partial z} \\
  \\
\omega_y = \frac{1}{2}(\frac{\partial u_x}{\partial z} - \frac{\partial u_z}{\partial x}) = 0, \Rightarrow \frac{\partial u_x}{\partial z} = \frac{\partial u_z}{\partial x} \\
  \\
\omega_z = \frac{1}{2}(\frac{\partial u_y}{\partial x} - \frac{\partial u_x}{\partial y}) = 0, \Rightarrow \frac{\partial u_y}{\partial x} = \frac{\partial u_x}{\partial y} \\
\end{cases}$$

**势函数**，对某一标量函数 $ \varphi $ 求梯度，得到矢量 $ \upsilon $，则称 $ \varphi $ 为 $ \upsilon $ 的势函数。  
势函数是某些矢量场上关于空间位置处势能的函数，是对场中能量的刻画。  
例如重力场，重力势函数 $mg*z$ 在 x,y,z 方向上的偏导就是重力势能在不同方向上的变化速度，即重力。  
同理，流速场上可能存在流速势函数，势函数对 x,y,z 的偏导就是流速势在不同方向的变化速度，即流速。  

若存在流场势函数$\varphi$，则有  
$$
\textbf{x 方向}: u_x = \frac{\partial \varphi}{\partial x} , 
\textbf{y 方向}: u_y = \frac{\partial \varphi}{\partial y} , 
\textbf{z 方向}: u_z = \frac{\partial \varphi}{\partial z} 
$$

在 x,y,z 方向继续求导，有  
$$\begin{cases}
\frac{\partial^2 \varphi}{\partial x \partial y} = \frac{\partial u_x}{\partial y} = \frac{\partial u_y}{\partial x}  \\
  \\
\frac{\partial^2 \varphi}{\partial x \partial z} = \frac{\partial u_x}{\partial z} = \frac{\partial u_z}{\partial x}  \\
  \\
\frac{\partial^2 \varphi}{\partial z \partial y} = \frac{\partial u_z}{\partial y} = \frac{\partial u_y}{\partial z}  \\
\end{cases}$$

由此可知，若流场中的流体是无涡流，则必然存在流速势函数。所以，无涡流又称 **势流**，继而  
$$\begin{gathered}
\frac{\partial \varphi}{\partial x}\mathrm{d}x + \frac{\partial \varphi}{\partial y}\mathrm{d}y + \frac{\partial \varphi}{\partial z}\mathrm{d}z = u_x\mathrm{d}x + u_y\mathrm{d}y + u_z\mathrm{d}z \\
\mathrm{d}\varphi = u_x\mathrm{d}x + u_y\mathrm{d}y + u_z\mathrm{d}z \end{gathered}$$


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
$$\begin{aligned}
cos(\alpha) &= \cos(u, \mathrm{d}s) \\
&= \cos(u,x)\cos(\mathrm{d}s,x) + \cos(u,y)\cos(\mathrm{d}s,y) + \cos(u,z)\cos(\mathrm{d}s,z) \\
&= \frac{u_x}{u}\frac{\mathrm{d}x}{\mathrm{d}s} + \frac{u_y}{u}\frac{\mathrm{d}y}{\mathrm{d}s} + \frac{u_z}{u}\frac{\mathrm{d}z}{\mathrm{d}s}\\
\end{aligned}$$

$$\begin{aligned}
&\Rightarrow u\cos(\alpha)\mathrm{d}s = u_x\mathrm{d}x + u_y\mathrm{d}y + u_z\mathrm{d}z \\
&\Rightarrow \Gamma = \oint_{c} (u_x\mathrm{d}x + u_y\mathrm{d}y + u_z\mathrm{d}z)
\end{aligned}$$

如果流体运动是无涡的，则必然存在流速势函数，则有：  
$$ \Gamma = \oint_{c} \mathrm{d}\varphi = [\varphi]_{A}^{A} = 0 $$

由此，在无涡流场中的任意封闭周线的速度环量都是 0。  

</div>

*--- 注意：---*
1. 环线积分中绕行方向遵循左手定则，即绕行过程中保持左手位于环内。

2. 曲线积分中，直角坐标系与极坐标系的转换（注意积分区间的转换）： 

$$\begin{aligned}
\int_{L} O(x,y,z)\mathrm{d}x + P(x,y,z)\mathrm{d}y + Q(x,y,z)\mathrm{d}z &= \int_{L} O\cos\alpha \mathrm{d}s + P\cos\beta\mathrm{d}s + Q\cos\gamma\mathrm{d}s \\
&= \int_{L} (O\cos\alpha + P\cos\beta + Q\cos\gamma) \mathrm{d}s \\
\mathrm{d}s = \mathrm{d}\theta * r
\end{aligned}$$

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
$$\begin{gathered}
\Delta m_y = - \frac{\partial \rho u_y}{\partial y}\mathrm{d}x\mathrm{d}y\mathrm{d}z\mathrm{d}t \\ 
\Delta m_z = - \frac{\partial \rho u_z}{\partial z}\mathrm{d}x\mathrm{d}y\mathrm{d}z\mathrm{d}t
\end{gathered}$$

dt 时段内，六面体内流体密度由 $\rho$ 变为 $\rho + \frac{\partial \rho}{\partial t}$，引起的质量变化  
$$\Delta m = \frac{\partial \rho}{\partial t}\mathrm{d}x\mathrm{d}y\mathrm{d}z\mathrm{d}t $$

dt 时段内，微分平行六面体内流体的质量守恒（介质连续）原理：  
$$\begin{gathered}
\Delta m = \Delta m_x + \Delta m_y + \Delta m_z \\
\frac{\partial \rho}{\partial t}\mathrm{d}x\mathrm{d}y\mathrm{d}z\mathrm{d}t = - [\frac{\partial \rho u_x}{\partial x} + \frac{\partial \rho u_y}{\partial y} + \frac{\partial \rho u_z}{\partial z}]\mathrm{d}x\mathrm{d}y\mathrm{d}z\mathrm{d}t \\
\end{gathered}$$

除以 $\mathrm{d}x\mathrm{d}y\mathrm{d}z\mathrm{d}t$ 之后得到， **可压缩流体非恒定流的连续性方程**：  
$$\frac{\partial \rho}{\partial t} + [\frac{\partial \rho u_x}{\partial x} + \frac{\partial \rho u_y}{\partial y} + \frac{\partial \rho u_z}{\partial z}] = 0$$

*------------------ * ------------------*

对不可压缩流体，ρ 为常数、与空间时间无关，连续性方程简化：  
$$\frac{\partial u_x}{\partial x} + \frac{\partial u_y}{\partial y} + \frac{\partial u_z}{\partial z} = 0    \mathrm{Or}    \nabla \cdot \textbf{u} = 0$$

由于 $\frac{\partial u_x}{\partial x}, \frac{\partial u_y}{\partial y},\frac{\partial u_z}{\partial z}$ 表示微分六面体沿 x，y，z 方向的线变形速率，故微分平行六面体的体积变化：  
$$(\frac{\partial u_x}{\partial x} + \frac{\partial u_y}{\partial y} + \frac{\partial u_z}{\partial z} )\mathrm{d}x\mathrm{d}y\mathrm{d}z\mathrm{d}t$$

对不可压缩流体，微分平行六面体经平移和线变形体积大小不变，角变形和旋转亦不改变体积大小。  
$$\begin{aligned}
\iiint_{V} \mathrm{div}  \textbf{u}  \mathrm{d}V &= \iiint_{V} (\frac{\partial u_x}{\partial x} + \frac{\partial u_y}{\partial y} + \frac{\partial u_z}{\partial z} )\mathrm{d}x\mathrm{d}y\mathrm{d}z\mathrm{d}t \\
&= \iint_{S} u_n \mathrm{d}s \\
&= 0 \\
\end{aligned}$$

S是体积V 的封闭表面积；un是S 上各点处的流速在该处外法线方向的投影；$\iint_{S} u_n \mathrm{d}s$ 即速度通量。  

*------------------ * ------------------*

分析恒定流下的流管，由于流管侧表面上 un = 0，故  
$$\iint_{S} u_n \mathrm{d}s = - \iint_{A_i} u_i\mathrm{d}A_i + \iint_{A_o} u_o\mathrm{d}A_o = 0 $$

Ai,ui 是流管流入横断面的面积、流速； Ao,uo 是流管流出横断面的面积、流速。  
$$\begin{gathered}
\iint_{A_i} u_i\mathrm{d}A_i = \iint_{A_o} u_o\mathrm{d}A_o \\
Q_i = Q_o
\end{gathered}$$

恒定流时流管一端有流量流入，对不可压缩流体，另一端必然有相等的流量流出。  

</div>

*--- 注意：---*
1. 分析微分六面体内质量变化时，认为六面体空间是固定的、不变的。

2. 计算微分六面体中面上流量时，以形心流速作面上平均流速；或者说，求速度通量时ux在面上线性分布。

3. $\mathrm{div}  \textbf{u}$ 称为速度散度，结果为标量。

4. 通过高斯定理，可以将体积积分转换为曲面积分。

---------------------------------------------------------------------------

### The Motion Equation of Ideal Fluid

*理想流体的运动方程*

<div align="center">

假设流体是不可压缩、无粘性的，则称为 **理想流体**。基于这种假设，理想流体的动水压强具有以下特性：
1. 动水压强总是沿着作用面的内法线方向。因为 “无粘假设”，故流体内无切应力，故作用面上仅压应力。
2. 在理想流体中，任何点的动水压强在各个方向上的大小均相等（与静水压强相同）。

<img src="./imgs/12.jpg" width=440 height=240>

在理想流体流场中任意一点A(x, y, z) 处动水压强为 p，速度 ux, uy, uz。以A 为中心取一微分平行六面体，  
其边长为 dx, dy, dz，分别平行于 x, y, z 轴。同时，  
作用于六面体的只有表面力（动水压力）和质量力，假设单位质量的质量力在各轴方向的投影为 fx, fy, fz。  

作用在六面体 x 轴方向的表面上的平均动水压强，  
$$\textbf{左边}, p - \frac{\partial p}{\partial x} \frac{\mathrm{d}x}{2}  ;  \textbf{右边}, p + \frac{\partial p}{\partial x} \frac{\mathrm{d}x}{2}$$

根据力学原理，则有  
$$\begin{gathered}
f_x\rho\mathrm{d}x\mathrm{d}y\mathrm{d}z + (p - \frac{\partial p}{\partial x})\mathrm{d}y\mathrm{d}z - (p + \frac{\partial p}{\partial x})\mathrm{d}y\mathrm{d}z = \rho\mathrm{d}x\mathrm{d}y\mathrm{d}z\frac{\mathrm{d}u_x}{\mathrm{d}t} \\
f_x - \frac{1}{\rho}\frac{\partial p}{\partial x} = \frac{\mathrm{d}u_x}{\mathrm{d}t} \\
\end{gathered}$$

由此推论，六面体在各方向均有类似关系，得到 **理想流体的运动方程**，又称 **欧拉方程**：  
$$\begin{cases}
f_x - \frac{1}{\rho}\frac{\partial p}{\partial x} = \frac{\mathrm{d}u_x}{\mathrm{d}t} \\
  \\
f_y - \frac{1}{\rho}\frac{\partial p}{\partial y} = \frac{\mathrm{d}u_y}{\mathrm{d}t} \\
  \\
f_z - \frac{1}{\rho}\frac{\partial p}{\partial z} = \frac{\mathrm{d}u_z}{\mathrm{d}t} \\
\end{cases}$$

方程适用不可压缩流体或可压缩气体。对于前者，密度 ρ 是常数；对于后者，ρ 为变量。

*------------------ * ------------------*

对于静止流体，ux = uy = uz，由此得到 **静水力学欧拉平衡方程**：
$$\begin{cases}
f_x - \frac{1}{\rho}\frac{\partial p}{\partial x} = 0 \\
f_y - \frac{1}{\rho}\frac{\partial p}{\partial y} = 0 \\
f_z - \frac{1}{\rho}\frac{\partial p}{\partial z} = 0 \\
\end{cases}$$

因为在流场中，加速度由时变加速度和位变加速两部分组成，即：   
$$a_i = \frac{\mathrm{d}u_i}{\mathrm{d}t} = \frac{\partial u_i}{\partial t} + \frac{\partial u_i}{\partial x}\frac{\mathrm{d}x}{\mathrm{d}t} + \frac{\partial u_i}{\partial y}\frac{\mathrm{d}y}{\mathrm{d}t} + \frac{\partial u_i}{\partial z}\frac{\mathrm{d}z}{\mathrm{d}t} $$

所以，欧拉方程简化：  
$$\begin{cases}
f_x - \frac{1}{\rho}\frac{\partial p}{\partial x} = \frac{\partial u_x}{\partial t} + u_x\frac{\partial u_x}{\partial x} + u_y\frac{\partial u_x}{\partial y} + u_z\frac{\partial u_x}{\partial z} \\
  \\
f_y - \frac{1}{\rho}\frac{\partial p}{\partial y} = \frac{\partial u_y}{\partial t} + u_x\frac{\partial u_y}{\partial x} + u_y\frac{\partial u_y}{\partial y} + u_z\frac{\partial u_y}{\partial z} \\
  \\
f_z - \frac{1}{\rho}\frac{\partial p}{\partial z} = \frac{\partial u_z}{\partial t} + u_x\frac{\partial u_z}{\partial x} + u_y\frac{\partial u_z}{\partial y} + u_z\frac{\partial u_z}{\partial z} \\
\end{cases}$$

对于不可压缩理想流体，ρ 已知，单位质量力 fx, fy, fz 已知，未知数仅为 p, ux, uy, uz。如果联解  
欧拉方程和连续性方程，理论上，理想流体中任意点、任意时刻的流速和动水压强是可以求出的。  

*------------------ * ------------------*

欧拉方程引入旋转角速度，得到  
$$\begin{gathered}
\because u = \sqrt{{u_x}^2 + {u_y}^2 + {u_z}^2} \\
  \\
\therefore \frac{\partial}{\partial x}(\frac{u^2}{2}) = \frac{\partial}{\partial x}(\frac{{u_x}^2 + {u_y}^2 + {u_z}^2}{2}) = u_x\frac{\partial u_x}{\partial x} + u_y\frac{\partial u_y}{\partial x} + u_z\frac{\partial u_z}{\partial x} \\
  \\
\Rightarrow u_x\frac{\partial u_x}{\partial x} = \frac{\partial}{\partial x}(\frac{u^2}{2}) - u_y\frac{\partial u_y}{\partial x} - u_z\frac{\partial u_z}{\partial x}
\end{gathered}$$

将以上变形代入欧拉方程，得到  
$$\begin{aligned}
f_x - \frac{1}{\rho}\frac{\partial p}{\partial x} - \frac{\partial u_x}{\partial t} &= \frac{\partial}{\partial x}(\frac{u^2}{2}) - u_y\frac{\partial u_y}{\partial x} - u_z\frac{\partial u_z}{\partial x} + u_y\frac{\partial u_x}{\partial y} + u_z\frac{\partial u_x}{\partial z} \\
&= \frac{\partial}{\partial x}(\frac{u^2}{2}) - u_y(\frac{\partial u_y}{\partial x} - \frac{\partial u_x}{\partial y}) + u_z(\frac{\partial u_x}{\partial z} - \frac{\partial u_z}{\partial x}) \\
&= \frac{\partial}{\partial x}(\frac{u^2}{2}) - u_y\cdot2\omega_z + u_z\cdot2\omega_y
\end{aligned}$$

整理后可得到，**葛罗米柯方程组**
$$\begin{cases}
f_x - \frac{1}{\rho}\frac{\partial p}{\partial x} - \frac{\partial u_x}{\partial t} - \frac{\partial}{\partial x}(\frac{u^2}{2}) = 2(u_z\omega_y - u_y\omega_z) \\
  \\
f_y - \frac{1}{\rho}\frac{\partial p}{\partial y} - \frac{\partial u_y}{\partial t} - \frac{\partial}{\partial y}(\frac{u^2}{2}) = 2(u_x\omega_z - u_z\omega_x) \\
  \\
f_z - \frac{1}{\rho}\frac{\partial p}{\partial z} - \frac{\partial u_z}{\partial t} - \frac{\partial}{\partial z}(\frac{u^2}{2}) = 2(u_y\omega_x - u_x\omega_y) \\
\end{cases}$$

如果作用于流体上的质量力 fx, fy, fz 是有势的，则必然存在 **力函数 $U(x, y, z, t)$**，并且有：  
$$f_x = \frac{\partial U}{\partial x},  f_y = \frac{\partial U}{\partial y},  f_z = \frac{\partial U}{\partial z}$$

对于理想流体，因流体不可压缩，ρ 为常数，得到质量力有势的情况下的葛罗米柯方程组  
$$\begin{cases}
\frac{\partial}{\partial x}(U - \frac{p}{\rho} - \frac{u^2}{2}) - \frac{\partial u_x}{\partial t} = 2(u_z\omega_y - u_y\omega_z) \\
  \\
\frac{\partial}{\partial y}(U - \frac{p}{\rho} - \frac{u^2}{2}) - \frac{\partial u_y}{\partial t} = 2(u_x\omega_z - u_z\omega_x) \\
  \\
\frac{\partial}{\partial z}(U - \frac{p}{\rho} - \frac{u^2}{2}) - \frac{\partial u_z}{\partial t} = 2(u_y\omega_x - u_x\omega_y) \\
\end{cases}$$

方程适用理想流体恒定流和非恒定流、有涡流和无涡流；对无涡流，$\omega_x = \omega_y = \omega_z = 0$。

</div>

---------------------------------------------------------------------------

### The Energy Equation of Ideal Fluid

*理想流体的能量方程*

<div align="center">

**热力学第一定律**，一个静止的热力学系统储能的增加等于外力对系统所做的功与外界传递给系统的热量之和。  

+ 外力做功：表面力做功 + 质量力做功；
+ 外界传热：外部传导热 + 外部辐射热；

一个流体微团可以看作一个热力学系统，同时流体质点总在流动中，  
假设系统偏离平衡态不远，则系统总能量变化率（内能和动能）等于外力的做功功率和外界的传热功率之和。  

对于流体微团，系统单位能量$e_s$ 包括  
+ 系统内能 $e$，由于流体分子热运动而具有的能量；
+ 系统动能 $\frac{v^2}{2}$；
+ 系统势能 $gz$；

将热力学第一定律应用于流体流动，把以上热力学关系以数学表达成微分方程，称为 **流体运动的能量方程**。  
理想流体由于无粘性、不可压缩，所以没有内能损耗，其能量方程反应的是外部做功与流体机械能的关系 。  

<img src="./imgs/12.jpg" width=440 height=240>

在理想流体流场中任意一点A(x, y, z) 处动水压强为 p，速度 ux, uy, uz。以A 为中心取一微分平行六面体，  
其边长为 dx, dy, dz，分别平行于 x, y, z 轴。同时，  
作用于六面体的只有表面力（动水压力）和质量力，假设单位质量的质量力在各轴方向的投影为 fx, fy, fz。  

在 x 方向，平移速度为 ux，外力做功 Wx：  
$$\begin{aligned}
W_x &= (f_x\rho\mathrm{d}x\mathrm{d}y\mathrm{d}z + (p - \frac{\partial p}{\partial x})\mathrm{d}y\mathrm{d}z - (p + \frac{\partial p}{\partial x})\mathrm{d}y\mathrm{d}z)\cdot u_x\mathrm{d}t \\
&= (f_x - \frac{\partial p}{\partial x})\mathrm{d}x\mathrm{d}y\mathrm{d}z\cdot u_x\mathrm{d}t \\
\end{aligned}$$

由此推论，六面体各方向上所受外力做的功：  
$$\begin{cases}
W_x = (f_x - \frac{\partial p}{\partial x})\mathrm{d}x\mathrm{d}y\mathrm{d}z\cdot u_x\mathrm{d}t \\
  \\
W_y = (f_y - \frac{\partial p}{\partial y})\mathrm{d}x\mathrm{d}y\mathrm{d}z\cdot u_y\mathrm{d}t \\
  \\
W_z = (f_z - \frac{\partial p}{\partial z})\mathrm{d}x\mathrm{d}y\mathrm{d}z\cdot u_z\mathrm{d}t \\
\end{cases}$$

理想流体中不考虑热量（内能），则根据热力学第一定律：  
$$\rho\mathrm{d}x\mathrm{d}y\mathrm{d}z\cdot\frac{\mathrm{d} e_s}{\mathrm{d} t} = \frac{\mathrm{d}(W_x + W_y + W_z)}{\mathrm{d}t}$$

理想流体中，动水压强 p 仅在 z 方向变化，故 $\frac{\partial p}{\partial x} = \frac{\partial p}{\partial y} = 0 $；同时，$e_s$中已包含重力项，故 fz=0；  
$$\rho\frac{\mathrm{d} e_s}{\mathrm{d} t} = - \frac{\partial p}{\partial z}u_z = - \frac{\mathrm{d} p}{\mathrm{d} z}u_z     \Rightarrow     \rho\mathrm{d}(\frac{v^2}{2} + gz) = - \frac{\mathrm{d} p}{\mathrm{d} z}u_z\mathrm{d} t = -\mathrm{d} p
$$

两边积分，得到 **理想流体恒定流的绝对运动的能量方程**，又称 **伯努利方程**：  
$$z + \frac{p}{\rho g} + \frac{v^2}{2g} = \textbf{const}$$

方程适用流体的固体边界对地球没有相对运动、质量力只有重力的理想流体。

*------------------ * ------------------*

如果从葛罗米柯方程出发，采用新思路，推导 **恒定流** 理想流体的能量方程：

恒定流时，$\frac{\partial u_x}{\partial t} = \frac{\partial u_y}{\partial t} = \frac{\partial u_z}{\partial t} = 0$，方程组简化为  
$$\begin{cases}
\frac{\partial}{\partial x}(U - \frac{p}{\rho} - \frac{u^2}{2}) = 2(u_z\omega_y - u_y\omega_z) \\
  \\
\frac{\partial}{\partial y}(U - \frac{p}{\rho} - \frac{u^2}{2})  = 2(u_x\omega_z - u_z\omega_x) \\
  \\
\frac{\partial}{\partial z}(U - \frac{p}{\rho} - \frac{u^2}{2})  = 2(u_y\omega_x - u_x\omega_y) \\
\end{cases}$$

以坐标微小增量 dx, dy, dz分别乘以上式，相加得到  
$$\begin{gathered}
\frac{\partial}{\partial x}(U - \frac{p}{\rho} - \frac{u^2}{2})\mathrm{d} x + \frac{\partial}{\partial y}(U - \frac{p}{\rho} - \frac{u^2}{2})\mathrm{d} y + \frac{\partial}{\partial z}(U - \frac{p}{\rho} - \frac{u^2}{2})\mathrm{d} z \\
  \\
= 2 [(u_z\omega_y - u_y\omega_z)\mathrm{d} x + (u_x\omega_z - u_z\omega_x)\mathrm{d} y + (u_y\omega_x - u_x\omega_y)\mathrm{d} z]
\end{gathered}$$

因为恒定流时，各运动要素与时间无关，故左边实际为对空间坐标的全微分：
$$
\mathrm{d}(U - \frac{p}{\rho} - \frac{u^2}{2}) = 2\left 
|\begin{array}{}
\mathrm{d}x &\mathrm{d}y  &\mathrm{d}z  \\
\omega_x &\omega_y &\omega_z  \\
u_x &u_y &u_z \\
\end{array}
\right|
$$

如果右端矩阵为0，则方程一定可以积分，此时：  
$$ U - \frac{p}{\rho} - \frac{u^2}{2} = \textbf{const} $$

若质量力只有重力：  
$$\mathrm{d}U = f_x\mathrm{d}x + f_y\mathrm{d}y + f_z\mathrm{d}z = -g\mathrm{d}z$$

对上式两端积分，代入整理后得到 **伯努利方程**：  
$$z + \frac{p}{\rho g} + \frac{v^2}{2g} = \textbf{const}$$

分析方程推导采用的假设，对于条件 $\left 
|\begin{array}{}
\mathrm{d}x &\mathrm{d}y  &\mathrm{d}z  \\
\omega_x &\omega_y &\omega_z  \\
u_x &u_y &u_z \\
\end{array}
\right| = 0$ 成立的场景：  
1. $\omega_x = \omega_y = \omega_z = 0$，即无涡流流体（有势流）适用；
2. $u_x = u_y = u_z = 0$，即静止态流体适用；
3. $\frac{\mathrm{d}x}{u_x} = \frac{\mathrm{d}y}{u_y} = \frac{\mathrm{d}z}{u_z}$，即流线方程，即方程适用理想流体恒定流速场的同一根流线各点；
4. $\frac{\mathrm{d}x}{\omega_x} = \frac{\mathrm{d}y}{\omega_y} = \frac{\mathrm{d}z}{\omega_z}$，即涡线方程，即方程适用理想流体恒定涡流的同一根涡线上各点；
5. $\frac{u_x}{\omega_x} = \frac{u_y}{\omega_y} = \frac{u_z}{\omega_z}$，即恒定流中以流线和涡线相重合为特征的螺旋流，即方程适用于螺旋流。

*------------------ * ------------------*

如果从葛罗米柯方程出发，推导 **非恒定流** 理想流体的能量方程：  
非恒定无涡流时，存在以下约束：  
$$\begin{cases}
\omega_x = \omega_y = \omega_z = 0 \\
u_x = \frac{\partial \varphi}{\partial x}, u_y = \frac{\partial \varphi}{\partial y}, u_z = \frac{\partial \varphi}{\partial z}  \\
\end{cases}$$

于是，则有  
$$\frac{\partial u_x}{\partial t} = \frac{\partial}{\partial t}(\frac{\partial \varphi}{\partial x}),   \frac{\partial u_y}{\partial t} = \frac{\partial}{\partial t}(\frac{\partial \varphi}{\partial y}),   \frac{\partial u_z}{\partial t} = \frac{\partial}{\partial t}(\frac{\partial \varphi}{\partial z}) $$

进而，则有  
$$\frac{\partial u_x}{\partial t} = \frac{\partial}{\partial x}(\frac{\partial \varphi}{\partial t}),   \frac{\partial u_y}{\partial t} = \frac{\partial}{\partial y}(\frac{\partial \varphi}{\partial t}),   \frac{\partial u_z}{\partial t} = \frac{\partial}{\partial z}(\frac{\partial \varphi}{\partial t})$$

所以，则有  
$$\begin{cases}
\frac{\partial}{\partial x}(U - \frac{p}{\rho} - \frac{u^2}{2} -\frac{\partial \varphi}{\partial t}) = 0 \\
  \\
\frac{\partial}{\partial y}(U - \frac{p}{\rho} - \frac{u^2}{2} -\frac{\partial \varphi}{\partial t}) = 0 \\
  \\
\frac{\partial}{\partial z}(U - \frac{p}{\rho} - \frac{u^2}{2} -\frac{\partial \varphi}{\partial t}) = 0 \\
\end{cases}$$

以坐标的微分增量 dx, dy, dz分别乘以上式，相加得到   
$$\mathrm{d}(U - \frac{p}{\rho} - \frac{u^2}{2} - \frac{\partial \varphi}{\partial t}) = 0$$

积分，得到  
$$U - \frac{p}{\rho} - \frac{u^2}{2} - \frac{\partial \varphi}{\partial t} = C(t)$$

方程说明，在理想流体的势流场内，在某一指定时刻 t，任何位置处的 $(U - \frac{p}{\rho} - \frac{u^2}{2} - \frac{\partial \varphi}{\partial t})$ 均相同，  
均等于某一常数 C，但在不同时刻有不同常数，所以积分常数 C 是时间的函数。

当质量力仅重力，$U = -gz + C_o$，得到 **理想流体非恒定无涡流的能量方程**， 又称 **拉格朗日方程**：  
$$z + \frac{p}{\rho g} + \frac{u^2}{2g} + \frac{1}{g}\frac{\partial \varphi}{\partial t} = C_{}{'}(t)$$

*------------------ * ------------------*

如果从葛罗米柯方程出发，推导 **非恒定涡流** 理想流体的能量方程：  
只有当理想流体非恒定流流场中各空间点的流速方向不随时间而变化时，方程才能积分。此时，  
流线与迹线重合。此时，  
以 dx, dy, dz 表示同一流线上坐标的微分增量。  

以流线坐标的微分增量 dx, dy, dz 分别乘以葛罗米柯方程组，然后相加得到：  
$$\begin{gathered}
\frac{\partial}{\partial x}(U - \frac{p}{\rho} - \frac{u^2}{2})\mathrm{d}x + \frac{\partial}{\partial y}(U - \frac{p}{\rho} - \frac{u^2}{2})\mathrm{d}y + \frac{\partial}{\partial z}(U - \frac{p}{\rho} - \frac{u^2}{2})\mathrm{d}z \\
  \\
= \frac{\partial u_x}{\partial t}\mathrm{d}x + \frac{\partial u_y}{\partial t}\mathrm{d}y + \frac{\partial u_z}{\partial t}\mathrm{d}z
\end{gathered}$$

因为 $(U - \frac{p}{\rho} - \frac{u^2}{2})$ 是空间时间的函数，若用于同一流线，则空间位置可以用沿流线长度表示：  
$$(U - \frac{p}{\rho} - \frac{u^2}{2}) = f_1(x, y, z, t) = f_2(s, t)$$

所以，则有  
$$\begin{aligned}
\mathrm{d}(U - \frac{p}{\rho} - \frac{u^2}{2}) = \frac{\partial}{\partial s}(U - \frac{p}{\rho} - \frac{u^2}{2})\mathrm{d}s + \frac{\partial}{\partial t}(U - \frac{p}{\rho} - \frac{u^2}{2})\mathrm{d}t \\
= \frac{\partial}{\partial x}(U - \frac{p}{\rho} - \frac{u^2}{2})\mathrm{d}x + \frac{\partial}{\partial y}(U - \frac{p}{\rho} - \frac{u^2}{2})\mathrm{d}y + \frac{\partial}{\partial z}(U - \frac{p}{\rho} - \frac{u^2}{2})\mathrm{d}z + \frac{\partial}{\partial t}(U - \frac{p}{\rho} - \frac{u^2}{2})\mathrm{d}t
\end{aligned}$$

由此，可得  
$$ \frac{\partial}{\partial x}(U - \frac{p}{\rho} - \frac{u^2}{2})\mathrm{d}x + \frac{\partial}{\partial y}(U - \frac{p}{\rho} - \frac{u^2}{2})\mathrm{d}y + \frac{\partial}{\partial z}(U - \frac{p}{\rho} - \frac{u^2}{2})\mathrm{d}z = \frac{\partial}{\partial s}(U - \frac{p}{\rho} - \frac{u^2}{2})\mathrm{d}s$$

代入，得到  
$$\frac{\partial}{\partial s}(U - \frac{p}{\rho} - \frac{u^2}{2})\mathrm{d}s = \frac{\partial u_x}{\partial t}\mathrm{d}x + \frac{\partial u_y}{\partial t}\mathrm{d}y + \frac{\partial u_z}{\partial t}\mathrm{d}z$$

根据流线方程，可得 $\mathrm{d}x = u_x\frac{\mathrm{d}s}{u}，\mathrm{d}y = u_y\frac{\mathrm{d}s}{u}，\mathrm{d}z = u_z\frac{\mathrm{d}s}{u}$，代入上式可以得到：  
$$\frac{\partial}{\partial s}(U - \frac{p}{\rho} - \frac{u^2}{2})\mathrm{d}s = \frac{1}{u}(u_x\frac{\partial u_x}{\partial t} + u_y\frac{\partial u_y}{\partial t} + u_z\frac{\partial u_z}{\partial t})\mathrm{d}s$$

同时因为又有 $u^2 = u_x^2 + u_y^2 + u_z^2$，所以存在 $u\frac{\partial u}{\partial t} = (u_x\frac{\partial u_x}{\partial t} + u_y\frac{\partial u_y}{\partial t} + u_z\frac{\partial u_z}{\partial t})$  
$$\frac{\partial}{\partial s}(U - \frac{p}{\rho} - \frac{u^2}{2})\mathrm{d}s = \frac{\partial u}{\partial t}\mathrm{d}s$$

当质量力仅为重力时，即 $U = -gz + C_o$，积分可得到 **理想流体涡流的能量方程**：  
$$z + \frac{p}{\rho g} + \frac{u^2}{2g} + \frac{1}{g}\int \frac{\partial u}{\partial t}\mathrm{d}s = C $$

方程适用于流线与迹线相互重合的理想流体非恒定涡流，而且只能用于同一根流线上。

</div>

*--- 注意：---*
1. 螺旋流，指流体质点既沿着流线方向运动，同时在运动过程中绕流线旋转。

2. 离心力 $f = m\frac{v^2}{r} = m{\omega^2}r$。

3. 理想流体非恒定、涡方向不变、流线迹线重合的流动，比如刚性管道中的理想流体非恒定流。

---------------------------------------------------------------------------

### The Motion Equation of Real Fluid

*实际流体的运动方程*

<div align="center">

实际流体中由于存在粘性，所以有相对运动的各层流体之间均将产生切应力。因此，  
分析流体质点受力时，表面力将不再沿内法线方向。

在运动流体中任意划出一平面 z，作用在面上任意点 A的表面力 Pz将是倾斜方向的，  
Pz 在 x, y, z 三个方向均有分量：  
+ $p_{zz}$，一个与 z 平面成法向（垂直）的正应力，又称为 动水压强；
+ $\tau_{zx}$，一个与 z 平面成切向（平行）、沿 x 方向的切应力；
+ $\tau_{zy}$，一个与 z 平面成切向（平行）、沿 y 方向的切应力；

一般的，约定以 $p$ 表示正应力，$\tau$ 表示切应力；下加两个脚号，约定  
+ 第一个脚号表示应力作用的面与哪个轴垂直；
+ 第二个脚号表示应力作用方向与哪个轴平行。

<img src="./imgs/17.jpg" width=560 height=240>

在实际流体中分离一个微分平行六面体，密度 ρ，各边长 dx, dy, dz；假设，速度沿各轴的正方向增大。  
在 x 方向，动力平衡方程：    
$$\begin{aligned}
f_x\rho\mathrm{d}x\mathrm{d}y\mathrm{d}z &+ p_{xx}\mathrm{d}y\mathrm{d}z - (p_{xx} + \frac{\partial p_{xx}}{\partial x}\mathrm{d}x)\mathrm{d}y\mathrm{d}z  \\
&- \tau_{zx}\mathrm{d}x\mathrm{d}y + (\tau_{zx} + \frac{\partial \tau_{zx}}{\partial z}\mathrm{d}z)\mathrm{d}x\mathrm{d}y \\
&- \tau_{yx}\mathrm{d}x\mathrm{d}z + (\tau_{yx} + \frac{\partial \tau_{yx}}{\partial y}\mathrm{d}y)\mathrm{d}x\mathrm{d}z \\
&= \rho\mathrm{d}x\mathrm{d}y\mathrm{d}z\frac{\mathrm{d} u_x}{\mathrm{d}t}
\end{aligned}$$

整理，得到
$$f_x - \frac{1}{\rho}\frac{\partial p_{xx}}{\partial x} + \frac{1}{\rho}(\frac{\partial \tau_{zx}}{\partial z} + \frac{\partial \tau_{yx}}{\partial y}) = \frac{\mathrm{d} u_x}{\mathrm{d}t}$$

由此类推，即 **实际流体运动的应力微分方程**：  
$$\begin{cases}
\frac{\mathrm{d} u_x}{\mathrm{d}t} = f_x - \frac{1}{\rho}\frac{\partial p_{xx}}{\partial x} + \frac{1}{\rho}(\frac{\partial \tau_{zx}}{\partial z} + \frac{\partial \tau_{yx}}{\partial y})\\
  \\
\frac{\mathrm{d} u_y}{\mathrm{d}t} = f_y - \frac{1}{\rho}\frac{\partial p_{yy}}{\partial y} + \frac{1}{\rho}(\frac{\partial \tau_{zy}}{\partial z} + \frac{\partial \tau_{xy}}{\partial x}) \\
  \\
\frac{\mathrm{d} u_z}{\mathrm{d}t} = f_z - \frac{1}{\rho}\frac{\partial p_{zz}}{\partial z} + \frac{1}{\rho}(\frac{\partial \tau_{xz}}{\partial x} + \frac{\partial \tau_{yz}}{\partial y}) \\
\end{cases}$$

方程同时适用于牛顿流体和非牛顿流体，同时适用于层流运动和湍流运动。

*------------------ * ------------------*

层流运动时牛顿流体， 存在 **牛顿内摩擦定律**：  
$$\tau = \eta\frac{\mathrm{d}u}{\mathrm{d}s} = \eta\frac{\mathrm{d}\theta}{\mathrm{d}t},   \eta \text{ 为动力粘性系数}$$

方程表明层流运动时实际流体中角应变与流速梯度相等；粘性切应力与流速梯度成正比，与压力无关。  

根据 **达朗贝尔原理**，可得到 **切应力互等定理**：  
$$\tau_{xz} = \tau_{zx},   \tau_{xy} = \tau_{yx},   \tau_{yz} = \tau_{zy}$$

进而，得到  
$$\begin{cases}
\tau_{xz} = \tau_{zx} = \eta \theta_y = \eta (\frac{\partial u_x}{\partial z} + \frac{\partial u_z}{\partial x})  \\
  \\
\tau_{xy} = \tau_{yx} = \eta \theta_z = \eta (\frac{\partial u_x}{\partial y} + \frac{\partial u_y}{\partial x})  \\
  \\
\tau_{yz} = \tau_{zy} = \eta \theta_x = \eta (\frac{\partial u_y}{\partial z} + \frac{\partial u_z}{\partial y})  \\
\end{cases}$$

由于实际流体中动水压强各向不相等，故采用任意三个正交方向作平均值   
$$p = \frac{p_{xx} + p_{yy} + p_{zz}}{3}$$

分析，得到  
$$\begin{cases}
p_{xx} = p - 2\eta\frac{\partial u_x}{\partial x} = p + p_{xx}^{'} \\
  \\
p_{yy} = p - 2\eta\frac{\partial u_y}{\partial y} = p + p_{yy}^{'} \\
  \\
p_{zz} = p - 2\eta\frac{\partial u_z}{\partial z} = p + p_{zz}^{'} \\
\end{cases}$$

方程表明实际流体的动水压强可以看作由两部分所组成，动水平均压强 + 粘滞性产生的附加正应力。  
附加正应力的产生是沿流线的速度梯度所导致，加速时同方向质点间存在分离趋势、产生拉伸变形，  
附加正应力为拉应力，所以质点动水压强减小；反之，附加正应力为压应力，故质点动水压强增大。

综合以上的 切应力与角应变方程 和 正应力与线应变方程，构成 **层流运动下牛顿流体的本构方程组**。  
代入实际流体应力方程，在 x 方向得到  
$$\begin{aligned}
\frac{\mathrm{d} u_x}{\mathrm{d}t} &= \frac{\partial u_x}{\partial t} + u_x\frac{\partial u_x}{\partial x} + u_y\frac{\partial u_x}{\partial y} + u_z\frac{\partial u_x}{\partial z} = f_x - \frac{1}{\rho}\frac{\partial p_{xx}}{\partial x} + \frac{1}{\rho}(\frac{\partial \tau_{zx}}{\partial z} + \frac{\partial \tau_{yx}}{\partial y}) \\
  \\
&= f_x - \frac{1}{\rho}(\frac{\partial p}{\partial x} - 2\eta\frac{\partial^2 u_x}{\partial x^2}) + \frac{\eta}{\rho}(\frac{\partial^2 u_x}{\partial z^2} + \frac{\partial^2 u_z}{\partial x \partial z} + \frac{\partial^2 u_y}{\partial x \partial y} + \frac{\partial^2 u_x}{\partial y^2})  \\
  \\
&= f_x - \frac{1}{\rho}\frac{\partial p}{\partial x} + \frac{\eta}{\rho}(\frac{\partial^2 u_x}{\partial x^2} + \frac{\partial^2 u_x}{\partial y^2} + \frac{\partial^2 u_x}{\partial z^2}) + \frac{\eta}{\rho}\frac{\partial}{\partial x}(\frac{\partial u_x}{\partial x} + \frac{\partial u_y}{\partial y} + \frac{\partial u_z}{\partial z})\\
\end{aligned}$$

采用拉普拉斯算式和散度算式；运动粘性系数 $\nu = \frac{\eta}{\rho}$；进而得到**可压缩粘性牛顿流体层流运动方程**：  
$$\begin{cases}
\frac{\mathrm{d} u_x}{\mathrm{d}t} = f_x - \frac{1}{\rho}\frac{\partial p}{\partial x} + \nu \nabla^2 u_x + \nu \frac{ \partial}{\partial x}( \nabla \cdot u)  \\
  \\
\frac{\mathrm{d} u_y}{\mathrm{d}t} = f_y - \frac{1}{\rho}\frac{\partial p}{\partial y} + \nu \nabla^2 u_y + \nu \frac{ \partial}{\partial y}( \nabla \cdot u)  \\
  \\
\frac{\mathrm{d} u_z}{\mathrm{d}t} = f_z - \frac{1}{\rho}\frac{\partial p}{\partial z} + \nu \nabla^2 u_z + \nu \frac{ \partial}{\partial z}( \nabla \cdot u)  \\
\end{cases}$$

对于不可压缩流体，连续性方程简化为 $\nabla \cdot u = 0$，从而得到  **不可压缩粘性牛顿流体层流运动方程**，  
又称为 **纳维 - 斯托克斯方程**：  
$$\begin{cases}
\frac{\mathrm{d} u_x}{\mathrm{d}t} = f_x - \frac{1}{\rho}\frac{\partial p}{\partial x} + \nu \nabla^2 u_x  \\
  \\
\frac{\mathrm{d} u_y}{\mathrm{d}t} = f_y - \frac{1}{\rho}\frac{\partial p}{\partial y} + \nu \nabla^2 u_y  \\
  \\
\frac{\mathrm{d} u_z}{\mathrm{d}t} = f_z - \frac{1}{\rho}\frac{\partial p}{\partial z} + \nu \nabla^2 u_z  \\
\end{cases}$$

方程适用于不可压缩牛顿流体的层流运动；一般认为**非稳态**的 N-S 方程依然适用于湍流的瞬时运动。  
N-S 方程与连续性方程组成的微分方程组，未知参数 ux,uy,uz, p，若 $\rho,\eta$ 不变，此时方程系统封闭。

又对于 纳维 - 斯托克斯方程：  
$$
\begin{pmatrix}
\frac{\mathrm{d} u_x}{\mathrm{d}t} \\ 
  \\
\frac{\mathrm{d} u_y}{\mathrm{d}t} \\
  \\
\frac{\mathrm{d} u_z}{\mathrm{d}t} \\
\end{pmatrix} = \begin{pmatrix}
\frac{\partial u_x}{\partial t} + u_x\frac{\partial u_x}{\partial x} + u_y\frac{\partial u_x}{\partial y} + u_z\frac{\partial u_x}{\partial z}  \\
  \\
\frac{\partial u_y}{\partial t} + u_x\frac{\partial u_y}{\partial x} + u_y\frac{\partial u_y}{\partial y} + u_z\frac{\partial u_y}{\partial z}  \\
  \\
\frac{\partial u_z}{\partial t} + u_x\frac{\partial u_z}{\partial x} + u_y\frac{\partial u_z}{\partial y} + u_z\frac{\partial u_z}{\partial z}  \\
\end{pmatrix} = \begin{pmatrix}
f_x \\
  \\
f_y \\
  \\
f_z \\
\end{pmatrix} - \frac{1}{\rho}
\begin{pmatrix}
\frac{\partial p}{\partial x}  \\
  \\
\frac{\partial p}{\partial y}  \\
  \\
\frac{\partial p}{\partial z}  \\
\end{pmatrix} + \frac{\eta}{\rho}
\begin{pmatrix}
\frac{\partial^2 u_x}{\partial x^2} + \frac{\partial^2 u_x}{\partial y^2} + \frac{\partial^2 u_x}{\partial z^2}  \\
  \\
\frac{\partial^2 u_y}{\partial x^2} + \frac{\partial^2 u_y}{\partial y^2} + \frac{\partial^2 u_y}{\partial z^2}  \\
  \\
\frac{\partial^2 u_z}{\partial x^2} + \frac{\partial^2 u_z}{\partial y^2} + \frac{\partial^2 u_z}{\partial z^2}  \\
\end{pmatrix}
$$

$$
\Rightarrow \rho [\frac{\partial \textbf{u}}{\partial t} + (\nabla \cdot \textbf{u}) \textbf{u}] = \rho \textbf{f} - \nabla p + \eta \nabla^2 \textbf{u}
$$

其中，方程的左端为流体单元的**动量变化率（惯性项）**，方程右端为作用在流体单元上的**各种作用力**：  
$$\begin{cases}
\frac{\partial \textbf{u}}{\partial t}，\text{时间项，表示方程可以描述非稳态流场}； \\
  \\
(\nabla \cdot \textbf{u})  \textbf{u},\text{对流项，表征流场运动要素在空间的传递，加大速度梯度使流场更不光滑}； \\
  \\
\rho \textbf{f}，\text{源项（质量力），每个时刻外部环境施加的作用引起的增长率}；  \\
  \\
\nabla p,\text{压力项，使速度场满足不可压缩条件，使速度梯度增加从而使流场更加不光滑}；  \\
  \\
\eta \nabla^2 \textbf{u},\text{粘性项、扩散项（粘性力），来自分子间的动量交换，使得速度梯度变光滑}；
\end{cases}$$

*------------------ * ------------------*

对于不可压缩流体，$\nabla \cdot u = 0$，乘以 $\rho u_x$，得到  
$$\rho u_x \frac{\partial u_x}{\partial x} + \rho u_x \frac{\partial u_y}{\partial y}  + \rho u_x \frac{\partial u_z}{\partial z} = 0$$

代入实际流体应力方程，在 x 方向得到  
$$\begin{aligned}
\rho f_x - \frac{\partial p_{xx}}{\partial x} + \frac{\partial \tau_{yx}}{\partial y} + \frac{\partial \tau_{zx}}{\partial z}  &= \rho \frac{\partial u_x}{\partial t} + 2\rho u_x\frac{\partial u_x}{\partial x} + \rho (u_y\frac{\partial u_x}{\partial y} + u_x\frac{\partial u_y}{\partial y}) + \rho (u_z\frac{\partial u_x}{\partial z} + \frac{\partial u_z}{\partial z})  \\
&= \rho \frac{\partial u_x}{\partial t} + \rho \frac{\partial u_{x}^{2}}{\partial x} + \rho \frac{\partial u_x u_y}{\partial y} + \rho \frac{\partial u_x u_z}{\partial z}
\end{aligned}$$

整理，得到  
$$\rho \frac{\partial u_x}{\partial t} = \rho f_x - \frac{\partial}{\partial x}(p_{xx} + \rho u_{x}^{2}) + \frac{\partial}{\partial y}(\tau_{yx} - \rho u_x u_y) + \frac{\partial}{\partial z}(\tau_{zx} - \rho u_x u_z)$$

类推，得到   
$$\begin{cases}
\rho \frac{\partial u_x}{\partial t} = \rho f_x - \frac{\partial}{\partial x}(p_{xx} + \rho u_{x}^{2}) + \frac{\partial}{\partial y}(\tau_{yx} - \rho u_x u_y) + \frac{\partial}{\partial z}(\tau_{zx} - \rho u_x u_z) \\
  \\
\rho \frac{\partial u_y}{\partial t} = \rho f_y - \frac{\partial}{\partial y}(p_{yy} + \rho u_{y}^{2}) + \frac{\partial}{\partial x}(\tau_{xy} - \rho u_x u_y) + \frac{\partial}{\partial z}(\tau_{zy} - \rho u_y u_z)  \\
  \\
\rho \frac{\partial u_z}{\partial t} = \rho f_z - \frac{\partial}{\partial z}(p_{zz} + \rho u_{z}^{2}) + \frac{\partial}{\partial x}(\tau_{xz} - \rho u_x u_z) + \frac{\partial}{\partial y}(\tau_{yz} - \rho u_y u_z) \\
\end{cases}$$ 

上式是实际流体运动的应力微分方程的一种变形，同样适用于层流和湍流、牛顿流体和非牛顿流体。  
但是对于湍流，由于瞬时运动要素存在脉动现象，因此在研究中常常采取  **时均方法** 转换应力方程 。  

例如，对于流速$u_x = \overline{u_x} + u_{x}^{'}$，即  
$u_x$流速可以看作是由时均流速 $\overline{u_x}$ 和脉动流速 $u_{x}^{'}$ 构成，$\overline{u_x}$可视为消除脉动后线性变化的流速 $u_x$。  

类似，重构速度和应力，即可以得到   

速度： $\begin{cases}
u_x = \overline{u_x} + u_{x}^{'} \\
u_y = \overline{u_y} + u_{y}^{'} \\
u_z = \overline{u_z} + u_{z}^{'} \\
\end{cases}$， 表面应力：$\begin{cases}
p_{xx} = \overline{p_{xx}} + p_{xx}^{'}  \\
\tau_{yx} = \overline{\tau_{yx}} + \tau_{yx}^{'}  \\
\tau_{zx} = \overline{\tau_{zx}} + \tau_{zx}^{'}  \\
\end{cases}$， 惯性应力： $\begin{cases}
f_x = \overline{f_x} + f_{x}^{'}  \\
f_y = \overline{f_y} + f_{y}^{'}  \\
f_z = \overline{f_z} + f_{z}^{'}  \\
\end{cases}$  

对连续性方程时间平均，即可以得到：
$$\nabla\cdot\overline{u} = 0$$

对应力方程时间平均，x 方向可得到：  
$$\begin{aligned}
\rho \frac{1}{T}\int_{0}^{T} \frac{\partial u_x}{\partial t}  \mathrm{d}t = \\
\frac{1}{T}[\rho \int_{0}^{T} f_x \mathrm{d}t - \int_{0}^{T} \frac{\partial}{\partial x}(p_{xx} + \rho u_{x}^{2}) \mathrm{d}t + \int_{0}^{T} \frac{\partial}{\partial y}(\tau_{yx} - \rho u_x u_y) \mathrm{d}t + \int_{0}^{T} \frac{\partial}{\partial z}(\tau_{zx} - \rho u_x u_z) \mathrm{d}t] 
\end{aligned}$$

其中，一阶脉动变量项因积分消失：  
$$\begin{cases}
\frac{1}{T}\int_{0}^{T} \frac{\partial u_x}{\partial t}  \mathrm{d}t = \frac{1}{T}\int_{0}^{T} \frac{\partial \overline{u_x}}{\partial t}  \mathrm{d}t + \frac{1}{T}\int_{0}^{T} \frac{\partial u_{x}^{'}}{\partial t}  \mathrm{d}t = \frac{\partial \overline{u_x}}{\partial t} + 0 = \frac{\partial \overline{u_x}}{\partial t} \\
  \\
\frac{1}{T}\int_{0}^{T}  f_x   \mathrm{d}t = \frac{1}{T}\int_{0}^{T} \overline{f_x}  \mathrm{d}t + \frac{1}{T}\int_{0}^{T} f_{x}^{'}  \mathrm{d}t = \overline{f_x} 
\end{cases}$$

对高阶脉动项，一次时均得到均值：  
$$\begin{aligned}
\frac{1}{T}\int_{0}^{T} \frac{\partial}{\partial x}(p_{xx} + u_{x}^{2})  \mathrm{d}t &= \frac{1}{T}\int_{0}^{T} \frac{\partial}{\partial x}(\overline{p_{xx}} + p_{xx}^{'}) \mathrm{d}t + \frac{1}{T}\int_{0}^{T} \frac{\partial}{\partial x}(\overline{u_x} + u_{x}^{'})^2 \mathrm{d} t  \\
&= \frac{\partial \overline{p_{xx}}}{\partial x} + \frac{\rho}{T}\int_{0}^{T} \frac{\partial}{\partial x}(\overline{u_x}^2 + 2\overline{u_x}u_{x}^{'} + u_{x}^{'2})  \mathrm{d}t \\
&= \frac{\partial }{\partial x} (\overline{p_{xx}} + \rho  \overline{u_x}^2 + \rho  \overline{u_{x}^{'2}})
\end{aligned}$$

由此，得到  
$$\begin{aligned}
\rho \frac{\partial \overline{u_x}}{\partial t} = \rho \overline{f_x} &- \frac{\partial }{\partial x} (\overline{p_{xx}} + \rho  \overline{u_x}^2 + \rho  \overline{u_{x}^{'2}}) + \frac{\partial}{\partial y}(\overline{\tau_{yx}} - \rho  \overline{u_x}  \overline{u_y} - \rho  \overline{u_{x}^{'} u_{y}^{'}}) \\
&+ \frac{\partial}{\partial z}(\overline{\tau_{zx}} - \rho  \overline{u_x} \overline{u_z} - \rho  \overline{u_{x}^{'} u_{z}^{'}})
\end{aligned}$$

进而，得到 **湍流时均微分方程**，又称 **雷诺时均方程（RANS）**：  
$$\begin{cases}
\frac{\partial \overline{u_x}}{\partial t} = \overline{f_x} - \frac{\partial }{\partial x} (\overline{p_{xx}} + \frac{1}{\rho}  \overline{u_x}^2 + \frac{1}{\rho}  \overline{u_{x}^{'2}}) + \frac{\partial}{\partial y}(\overline{\tau_{yx}} - \frac{1}{\rho}  \overline{u_x}  \overline{u_y} - \frac{1}{\rho}  \overline{u_{x}^{'} u_{y}^{'}}) + \frac{\partial}{\partial z}(\overline{\tau_{zx}} - \frac{1}{\rho}  \overline{u_x} \overline{u_z} - \frac{1}{\rho}  \overline{u_{x}^{'} u_{z}^{'}})  \\
  \\
\frac{\partial \overline{u_y}}{\partial t} = \overline{f_y} - \frac{\partial }{\partial y} (\overline{p_{yy}} + \frac{1}{\rho}  \overline{u_y}^2 + \frac{1}{\rho}  \overline{u_{y}^{'2}}) + \frac{\partial}{\partial x}(\overline{\tau_{xy}} - \frac{1}{\rho}  \overline{u_x}  \overline{u_y} - \frac{1}{\rho}  \overline{u_{x}^{'} u_{y}^{'}}) + \frac{\partial}{\partial z}(\overline{\tau_{zy}} - \frac{1}{\rho}  \overline{u_y} \overline{u_z} - \frac{1}{\rho}  \overline{u_{y}^{'} u_{z}^{'}})  \\
  \\
\frac{\partial \overline{u_z}}{\partial t} = \overline{f_z} - \frac{\partial }{\partial z} (\overline{p_{zz}} + \frac{1}{\rho}  \overline{u_z}^2 + \frac{1}{\rho}  \overline{u_{z}^{'2}}) + \frac{\partial}{\partial x}(\overline{\tau_{xz}} - \frac{1}{\rho}  \overline{u_x}  \overline{u_z} - \frac{1}{\rho}  \overline{u_{x}^{'} u_{z}^{'}}) + \frac{\partial}{\partial y}(\overline{\tau_{yz}} - \frac{1}{\rho}  \overline{u_y} \overline{u_z} - \frac{1}{\rho}  \overline{u_{y}^{'} u_{z}^{'}})  \\
\end{cases}$$

方程表明，湍流时的时均应力可以视为由两部份构成：  
+ 时均流速 $\overline{u}$ 所构成的层流运动所产生的应力；
+ 脉动流速 $u^{'}$产生的附加应力，又称 **雷诺应力**。

由于湍流瞬时运动要素用时均及脉动运动要素代替，使得方程组未知量增加6个附加正应力，但方程未增。  
通过引入新的方程，使方程组封闭，进而配以适当的数值计算方法，如此便可以构成一个 **湍流数学模型** 。

*------------------ * ------------------*

对于实际流体运动，由于粘性的存在，接触固体表面会形成一层流速分布不均匀的减速薄层，即 **边界层**。  
由此，粘性液流分成两个区域：  
1. 在边界层外的主流，流速梯度为0，无内摩擦力发生，可以视为理想流体运动，符合势流运动规律；
2. 在边界层内的粘性流动，流速自0 增至主流速U0，流速梯度很大，内摩擦力显著，采用 N-S 方程。

在边界层内，与粘性力相比，质量力可以忽略，得到：  
$$\rho [\frac{\partial \textbf{u}}{\partial t} + (\nabla \cdot \textbf{u}) \textbf{u}] = - \nabla p + \eta \nabla^2 \textbf{u}$$

进一步，考虑恒定二维流动：  
$$\begin{cases}
u_x\frac{\partial u_x}{\partial x} + u_y\frac{\partial u_x}{\partial y} = - \frac{1}{\rho}\frac{\partial p}{\partial x} + \nu (\frac{\partial^2 u_x}{\partial x^2} + \frac{\partial^2 u_x}{\partial y^2})  \\
  \\
u_x\frac{\partial u_y}{\partial x} + u_y\frac{\partial u_y}{\partial y} = - \frac{1}{\rho}\frac{\partial p}{\partial x} + \nu (\frac{\partial^2 u_y}{\partial x^2} + \frac{\partial^2 u_y}{\partial y^2})  \\
  \\
\frac{\partial u_x}{\partial x} + \frac{\partial u_y}{\partial y} = 0 \\
\end{cases}$$

其中，x 为沿固体表面的距离（0 -> l）；y 为边界层厚度（0 -> δ）；ux 为沿固体表面流速（0 -> U0）。   
同时可知，$\frac{\delta}{l}$ 是一个微小量。

利用边界层内液流的运动特性，对方程各项作 **量纲分析** 和 **数量级分析**：  
$$y \sim \delta, x \sim l, u_x \sim U_0, \frac{\partial u_x}{\partial y} \sim \frac{U_0}{\delta}, \frac{\partial^2 u_x}{\partial y^2} \sim \frac{U_0}{\delta^2}, \frac{\partial u_x}{\partial x} \sim \frac{U_0}{l}, \frac{\partial^2 u_x}{\partial x^2} \sim \frac{U_0}{l^2}$$

由连续性方程 $\frac{\partial u_x}{\partial x} = - \frac{\partial u_y}{\partial y}$，可知二者具有相同数量级：  
$$\frac{\partial u_y}{\partial y} \sim \frac{U_0}{l}, \frac{\partial^2 u_y}{\partial y^2} \sim \frac{U_0}{l \delta}$$

又因为 $u_y = \int_{0}^{y} \frac{\partial u_y}{\partial y} \mathrm{d}y$，故：  
$$u_y \sim \frac{U_0}{l}\delta, \frac{\partial u_y}{\partial x} \sim \frac{U_0\delta}{l^2}，\frac{\partial^2 u_y}{\partial x^2} \sim \frac{U_0\delta}{l^3}$$

根据上列方程组各项的数量级，进行分析比较可以得出 ：  
1. x 方向，惯性项 $u_x\frac{\partial u_x}{\partial x}, u_y\frac{\partial u_x}{\partial y}$ 为同一数量级，二者相比，无法忽略其中某项；
2. x 方向，粘性项 $\frac{\partial^2 u_x}{\partial x^2}, \frac{\partial^2 u_x}{\partial y^2}$，二者数量级比例 $\frac{U_0}{l^2} : \frac{U_0}{\delta^2} = (\frac{\delta}{l})^2$ 是一个微小值的平方，故前者可以忽略；  
3. y 方向，惯性项 $u_x\frac{\partial u_y}{\partial x}, u_y\frac{\partial u_y}{\partial y}$ 为同一数量级，二者相比，无法忽略其中某项；  
4. y 方向，粘性项 $\frac{\partial^2 u_y}{\partial x^2}, \frac{\partial^2 u_y}{\partial y^2}$，二者数量级比例 $\frac{U_0 \delta}{l^3} : \frac{U_0}{l\delta} = (\frac{\delta}{l})^2$ 是一个微小值的平方，故前者可以忽略；

经此，方程继续简化：  
$$\begin{cases}
u_x\frac{\partial u_x}{\partial x} + u_y\frac{\partial u_x}{\partial y} = - \frac{1}{\rho}\frac{\partial p}{\partial x} + \nu \frac{\partial^2 u_x}{\partial y^2}  \\
  \\
u_x\frac{\partial u_y}{\partial x} + u_y\frac{\partial u_y}{\partial y} = - \frac{1}{\rho}\frac{\partial p}{\partial x} + \nu \frac{\partial^2 u_y}{\partial y^2}  \\
  \\
\frac{\partial u_x}{\partial x} + \frac{\partial u_y}{\partial y} = 0 \\
\end{cases}$$

继续分析，可以发现：
+ y 方向的惯性项相比 x 方向的惯性项，比例 $\frac{\delta}{l}$ 是一个微小量，故忽略前者对方程解的精确度并无影响；
+ y 方向的粘性项相比 x 方向的粘性项，比例 $\frac{\delta}{l}$ 是一个微小量，故忽略前者对方程解的精确度并无影响；

因此方程进一步简化，得到恒定二维流场的 **边界层微分方程**：
$$\begin{cases}
u_x\frac{\partial u_x}{\partial x} + u_y\frac{\partial u_x}{\partial y} = - \frac{1}{\rho}\frac{\partial p}{\partial x} + \nu \frac{\partial^2 u_x}{\partial y^2}  \\
  \\
\frac{\partial p}{\partial x} = 0 \\
  \\
\frac{\partial u_x}{\partial x} + \frac{\partial u_y}{\partial y} = 0 \\
\end{cases}$$

方程基于平面绕流推导，但一般可用于曲率较小的曲面边界层：取固体表面曲线为 x 轴，曲线法向为 y 轴。

</div>

*--- 注意：---*   
1. 速度沿各轴正向增大，则 $\tau_{zx}$ 对作用面起阻滞作用，$\tau_{zx} + \frac{\partial \tau_{zx}}{\partial z}\mathrm{d}z$ 对作用面起推动作用；其他类推。

2. 达朗贝尔原理，在质点运动的任一时刻，主动力F、约束力Fn 与惯性力构成平衡力系：  
$$ F + F_N + (-m \cdot a) = 0 $$

3. 切应力互等定理，相互垂直平面上，切应力成对存在且大小相等，同垂直于平面交线：  
$$\tau_{xz} = \tau_{zx},   \tau_{xy} = \tau_{yx},   \tau_{yz} = \tau_{zy}$$

4. **微分算子**  
$$\nabla = (\frac{\partial}{\partial x}, \frac{\partial}{\partial y}, \frac{\partial}{\partial z})$$

    **梯度算式**  
    + 标量梯度计算获得梯度矢量  
$$\nabla u = (\frac{\partial}{\partial x}, \frac{\partial}{\partial y}, \frac{\partial}{\partial z})u = (\frac{\partial u}{\partial x}, \frac{\partial u}{\partial y}, \frac{\partial u}{\partial z}) = (u_x, u_y, u_z) = \textbf{u} $$ 
    + 矢量梯度计算获得梯度张量  
$$\nabla\textbf{u} = [(\frac{\partial u_x}{\partial x}, \frac{\partial u_x}{\partial y}, \frac{\partial u_x}{\partial z})^T, (\frac{\partial u_y}{\partial x}, \frac{\partial u_y}{\partial y}, \frac{\partial u_y}{\partial z})^T, (\frac{\partial u_z}{\partial x}, \frac{\partial u_z}{\partial y}, \frac{\partial u_z}{\partial z})^T]$$

    **散度算式**
    + 矢量散度计算获得标量  
$$\nabla\cdot \textbf{u} = (\frac{\partial}{\partial x}, \frac{\partial}{\partial y}, \frac{\partial}{\partial z})\cdot(u_x, u_y, u_z) = \frac{\partial u_x}{\partial x} + \frac{\partial u_y}{\partial y} + \frac{\partial u_z}{\partial z}$$
    + 张量散度计算获得矢量
$$\nabla\cdot \textbf{U} = (\frac{\partial U_{xx}}{\partial x} + \frac{\partial U_{xy}}{\partial y} + \frac{\partial U_{xz}}{\partial z}, \frac{\partial U_{yx}}{\partial x} + \frac{\partial U_{yy}}{\partial y} + \frac{\partial U_{yz}}{\partial z}, \frac{\partial U_{zx}}{\partial x} + \frac{\partial U_{zy}}{\partial y} + \frac{\partial U_{zz}}{\partial z})$$

    **拉普拉斯算式**
    + 一般用于标量场计算  
$$\nabla^2 u = \nabla \cdot (\nabla u) = \frac{\partial^2 u}{\partial x^2} + \frac{\partial^2 u}{\partial y^2} + \frac{\partial^2 u}{\partial z^2}$$     

5. N-S 方程可以认为是对 **玻尔兹曼方程** 在近似平衡态下（层流）的一阶近似（欧拉方程是零阶近似）。  

6. 稳态 N-S 方程即恒定流 N-S 方程，但得到的稳态解仍是不稳定的，任意微小扰动会被流场无限放大。


*--- 附录：---*  
1. 关于 **转动** 与 **平动** ：
    + 力（Force），改变物体平动状态；
    + 力矩（Torque），改变物体转动状态；$T = s \times F$，力矩是位移与力的矢量积。
    + 如果一个系统，在研究过程中，不受外力作用，那么这个系统的总动量守恒。
    + 如果一个系统，在研究过程中，不受外力矩的作用，那么这个系统的总角动量守恒。
    + 物体达到静平衡，$\sum F = 0    And    \sum T = 0$ 。

| 运动 | --- | --- | --- | --- | --- |
| :--- | :--- | :--- | :--- | :--- | :--- | 
| 平动 | 位移，$\mathrm{d}s$ | 速度，$v = \frac{\mathrm{d}s}{\mathrm{d}t}$ | 加速度，$a = \frac{\mathrm{d}v}{\mathrm{d}t}$ | 惯性质量，m | 力，$F = ma$
| 转动 | 角位移，$\mathrm{d}\theta$ | 角速度，$\omega = \frac{\mathrm{d}\theta}{\mathrm{d}t}$ | 角加速度，$\alpha = \frac{\mathrm{d}\omega}{\mathrm{d}t}$ | 转动惯量，$I$ | 力矩，$T = I\alpha$|

| 运动 | --- | --- | --- | --- | 
| :--- | :--- | :--- | :--- | :--- |
| 平动 | 动量，$P = mv$ | 动能，$E_k = \frac{1}{2} mv^2$ | 做功，$W = \int_{s1}^{s2}F \mathrm{d}s$ | 静平衡，$\sum F = 0$ |
| 转动 | 角动量，$L = I\alpha$ | 角动能，$E_k = \frac{1}{2} I\omega^2$ |做功，$W = \int_{\theta1}^{\theta2}T \mathrm{d}\theta$ | 静平衡，$\sum T = 0$ |

---------------------------------------------------------------------------

### The Energy Equation of Real Fluid

*实际流体的能量方程*

<div align="center">

基于微分平行六面体对象，关于**控制体**的能量守恒方程的表述，采用**输运方程**的框架，得到  
$$
\textbf{控制体内能量时变率} +（\textbf{控制体输出能量通量} - \textbf{控制体输入能量通量}）= \\
\textbf{外界传热速率} + \textbf{外界做功速率}
$$

<img src="./imgs/17.jpg" width=560 height=240>

在实际流体中取一个微分平行六面体，密度 ρ，各边长 dx, dy, dz；假设，速度、温度沿各轴的正方向增大。

单位时间内，控制体能量时间变化率：  
$$\frac{\partial}{\partial t} (\rho e_s\mathrm{d}x\mathrm{d}y\mathrm{d}z) = \frac{\partial e_s}{\partial t}\rho\mathrm{d}x\mathrm{d}y\mathrm{d}z + e_s\frac{\partial}{\partial t}(\rho\mathrm{d}x\mathrm{d}y\mathrm{d}z) = \frac{\partial e_s}{\partial t}\rho\mathrm{d}x\mathrm{d}y\mathrm{d}z$$

沿 x 方向，控制体的净输出能量通量：  
$$
\Delta e_x = (e_s + \frac{\partial e_s}{\partial x}\mathrm{d}x)(u_x + \frac{\partial u_x}{\partial x}\mathrm{d}x)\rho\mathrm{d}y\mathrm{d}z - e_s u_x \rho \mathrm{d}y\mathrm{d}z = \frac{\partial u_x e_s}{\partial x} \rho \mathrm{d}x\mathrm{d}y\mathrm{d}z
$$

由此类推，控制体的净输出能量通量：  
$$\begin{cases}
\Delta e_x = \frac{\partial u_x e_s}{\partial x} \rho \mathrm{d}x\mathrm{d}y\mathrm{d}z \\
  \\
\Delta e_y = \frac{\partial u_y e_s}{\partial y} \rho \mathrm{d}x\mathrm{d}y\mathrm{d}z \\
  \\
\Delta e_z = \frac{\partial u_z e_s}{\partial z} \rho \mathrm{d}x\mathrm{d}y\mathrm{d}z \\
\end{cases}$$

由此，**控制体内单位质量能量变化率**：  
$$\frac{\mathrm{d} e_s}{\mathrm{d} t} = \rho\frac{\partial e_s}{\partial t} + \Delta e_x + \Delta e_y + \Delta e_z = \rho(\frac{\partial e_s}{\partial t} + \frac{\partial u_x e_s}{\partial x} + \frac{\partial u_y e_s}{\partial y} + \frac{\partial u_z e_s}{\partial z}) $$

单位时间内，**质量力做功速率**：  
$$W_{m,x} = f_x u_x \rho \mathrm{d}x\mathrm{d}y\mathrm{d}z,   W_{m,y} = f_y u_y \rho \mathrm{d}x\mathrm{d}y\mathrm{d}z,   W_{m,z} = f_z u_z \rho \mathrm{d}x\mathrm{d}y\mathrm{d}z$$

在 x 方向，表面力的做功速率：  
$$\begin{cases}
\textbf{x 方向前后切面}，(p_{xx} + \frac{\partial p_{xx}}{\partial x}\mathrm{d}x) \mathrm{d}y\mathrm{d}z (u_x + \frac{\partial u_x}{\partial x}\mathrm{d}x) - p_{xx} u_x \mathrm{d}y\mathrm{d}z; \\
  \\
\textbf{x 方向左右切面}，(\tau_{yx} + \frac{\partial \tau_{yx}}{\partial y}\mathrm{d}y) \mathrm{d}x\mathrm{d}z (u_x + \frac{\partial u_x}{\partial y}\mathrm{d}y) - \tau_{yx} u_x \mathrm{d}x\mathrm{d}z; \\
  \\
\textbf{x 方向上下切面}，(\tau_{zx} + \frac{\partial \tau_{zx}}{\partial z}\mathrm{d}z) \mathrm{d}x\mathrm{d}y (u_x + \frac{\partial u_x}{\partial z}\mathrm{d}z) - \tau_{zx} u_x \mathrm{d}x\mathrm{d}y; \\
\end{cases}$$

$$\Rightarrow (\frac{\partial p_{xx} u_x}{\partial x} + \frac{\partial \tau_{yx} u_x}{\partial y} + \frac{\partial \tau_{zx} u_x}{\partial z}) \mathrm{d}x\mathrm{d}y\mathrm{d}z $$

由此类推得，**表面力做功速率**：  
$$\begin{cases}
W_{s,x} = (\frac{\partial p_{xx} u_x}{\partial x} + \frac{\partial \tau_{yx} u_x}{\partial y} + \frac{\partial \tau_{zx} u_x}{\partial z}) \mathrm{d}x\mathrm{d}y\mathrm{d}z  \\
  \\
W_{s,y} = (\frac{\partial p_{yy} u_y}{\partial y} + \frac{\partial \tau_{xy} u_y}{\partial x} + \frac{\partial \tau_{zy} u_y}{\partial z}) \mathrm{d}x\mathrm{d}y\mathrm{d}z  \\
  \\
W_{s,z} = (\frac{\partial p_{zz} u_z}{\partial z} + \frac{\partial \tau_{xz} u_z}{\partial x} + \frac{\partial \tau_{yz} u_z}{\partial x}) \mathrm{d}x\mathrm{d}y\mathrm{d}z  \\
\end{cases}$$

由此，**控制体单位质量外界做功速率**：  
$$\begin{gathered}
\frac{\mathrm{d}W}{\mathrm{d}t} = \rho f_x u_x + \rho f_y u_y + \rho f_z u_z + \\
\frac{\partial}{\partial x}(p_{xx}u_x + \tau_{yx} u_y + \tau_{zx} u_z) + \frac{\partial}{\partial y}(p_{yy}u_y + \tau_{xy} u_x + \tau_{zy} u_z) + \frac{\partial}{\partial z}(p_{zz} u_z + \tau_{xz} u_x + \tau_{yz} u_y)  \\
\end{gathered}$$

设单位时间内由辐射传入单位质量流体的热量为 q，则单位时间内获得**辐射热**：  
$$q\rho\mathrm{d}x\mathrm{d}y\mathrm{d}z$$

由于导热使从高温区传导到低温区，根据**傅里叶定律**，x 方向上控制体传导热：  
$$J_{T,x}\mathrm{d}y\mathrm{d}z - (J_{T,x} + \frac{\partial J_{T,x}}{\partial x}\mathrm{d}x)\mathrm{d}y\mathrm{d}z = \kappa \frac{\partial^2 T}{\partial x^2}\mathrm{d}x\mathrm{d}y\mathrm{d}z$$

由此类推，单位时间内获得**传导热**：  
$$(\kappa \frac{\partial^2 T}{\partial x^2} + \kappa \frac{\partial^2 T}{\partial y^2} + \kappa \frac{\partial^2 T}{\partial z^2})\mathrm{d}x\mathrm{d}y\mathrm{d}z = (\nabla^2 T) \kappa\mathrm{d}x\mathrm{d}y\mathrm{d}z $$

综合以上分析，根据热力学第一定律，从而得到**不可压缩实际流体的能量方程**：    
$$\frac{\mathrm{d} e_s}{\mathrm{d} t} = f_m \cdot u + \frac{1}{\rho} \nabla (f_s \cdot u) + \frac{1}{\rho} \nabla^2 (\kappa T) + q$$


</div>

*--- 注意：---*  
1. 傅里叶定律：导热现象中，单位时间内通过给定截面的热量，正比于垂直于该截面方向上的温度梯度  
和截面面积，而热量传递的方向则与温度升高的方向相反。  
沿 x 方向的热流密度 $J_{T,x} (W\cdot m^{-2})$，导热系数 $\kappa (W\cdot m^{-1}\cdot K^{-1})$，根据定律  $J_{T,x} = -\kappa \frac{\partial T}{\partial x}$ 。

---------------------------------------------------------------------------

### References

*参考文献*

[1] 吴持恭. 水力学：下册[J]. 高等教育出版社, 2007.  
[2] 李福宝,李勤,等. 流体力学[J]. 冶金工业出版社, 2010.  

---------------------------------------------------------------------------

### Authors

[1] **朗月**，“ 希望这篇文章能够为你提供帮助，如有错误望不吝指正，欢迎交流！:D ”  

---------------------------------------------------------------------------