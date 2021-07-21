<p align="center">
  <a href="https://github.com/OurForce2020/OpenOasis"><img src="../Resources/Logo/logo.png" alt=""></a>
</p>

---------------------------------------------------------------------------

## Saint--Venant Equations

*水动力学·圣维南方程组*  

<div align="center">

**流束理论**，将微小流束看成是水流总流的一个微元体，水流总流就是由无数微小流束组成。  
如此，只要找出微小流束的运动规律，然后对整个水流积分，即得到水流总流的运动规律。  

**微小流束**，充满以流管为边界的一束水流。

在对总流的积分过程中，以断面平均流速替代断面上各点的流速，其误差用动能修正系数、  
动量修正系数等加以修正。因此，  
在流束理论中实际将水流运动视为一元流动，只考虑沿流束轴线方向的运动，而忽略了与，
轴线垂直方向的横向运动，局限。

*面对无数质点构成的连续介质，如何描述整个水流的运动规律？*

**拉格朗日法**，以研究个别水流质点运动为基础，通过对每个质点运动规律的研究获得整个水流的运动规律。  
水流质点，是具有无限小体积的水流质量，而不是水流分子或空间点；是相对于总流、小得类似于一个点。

</div>

---------------------------------------------------------------------------

### Unsteady Flow in Open Channel

*明渠非恒定流*

<div align="center">

+ 明渠非恒定流必是非均匀流，水力要素是时间 t 和流程 s 的函数， f = f(s,t) 。
+ 明渠非恒定流也是波动现象。
+ 明渠非恒定流所及区域内各过水断面水位流量关系一般不是单一稳定的关系。

风波或地震波产生的波动，使水体质点基本上循着一定轨迹（圆或椭圆）往复运动，几乎没有流量传递，  
只是同方向各处质点运动彼此相差一个相位而形成的水面波形的推进，称为 **振动波** 或 **推进波** 。

对于明渠非恒定流的波动，其是由于在非恒定流明渠的某一位置流量和水位发生改变而形成的，  
以重力、通过水流质点的位移形成波的传播，在波所及区域内引起当地流量和水位的变化，称为 **变位波**。

<img src="./Imgs/19.jpg" width=400 height=200>

明渠非恒定流的波由两部分组成：**波峰** 和 **波体**。波峰推进速度 Vw 称为 **波速**，v 为过水断面平均流速。

从明渠非恒定流的水力要素随时间变化的急剧程度划分，  
+ **连续波**，又称 **非恒定渐变流**，要素变化平缓，波高相对波长很小，水流瞬时流线近乎成平行直线；
+ **间断波**，又称 **非恒定急变流**，要素变化剧烈，波高较大，波峰处水面很陡、要素不再是连续函数；

<img src="./Imgs/20.jpg" width=350 height=200>
<img src="./Imgs/21.jpg" width=350 height=200>

**涨水波**，当波到达后引起明渠水位抬高；**落水波**，当波到达后引起明渠水位下降。  
**顺波**，波的传播方向与水流方向一致；反之，则为 **逆波**。  

常见情景是，明渠上水闸快速启闭产生非恒定急变流（不连续波）。若快速开启水闸，下游流量增加，  
致下游水位迅速上涨，形成涨水顺波向下游传播；上游因泄流增加，水位下降，有落水逆波向上传播。  
同理，当明渠上水闸快速关闭时，上游将产生涨水逆波向上游传播，下游将产生落水顺波向下游传播。

<img src="./Imgs/22.jpg" width=700 height=200>

在稳定的没有冲淤变化的明渠内，当水流为恒定流时，因水面坡度为定值，故水位与流量呈单值关系。  
对于明渠非恒定流，由于  
在涨水过程中，同一水位下非恒定流水面坡度更大，因而其流量亦大；  
在落水过程中，同一水位下非恒定流水面坡度更小，因而其流量亦小。  

故而同一水位下水面坡度具有多值关系，使流量相应地具有多值关系，  
形成 **水位流量绳套关系曲线**。  

同时，在明渠非恒定流时，过水断面上的水面坡度、流速、流量、水位的最大值并不在同一时刻出现。  
涨水过程中，由于洪水波的传递，水面坡度快递增加从而首先出现最大值，而后依次再出现最大流速、  
最大流量、最高水位。落水过程中，首先出现最小流量，然后出现最低水位。

</div>

*--- 注意：---* 
1. 压力管道中非恒定流波的传播是靠压力差的作用，称为压力传播；  
明渠中非恒定流波的传播是靠重力作用，又称重力传播。

2. 对于不连续波，波峰处水面很陡、水力要素不连续，但波体部分水面仍较为平缓，可近似看作渐变流。

---------------------------------------------------------------------------

### The Continuity Equation of Unsteady Flow

*非恒定流连续性方程*

<div align="center">

<img src="./Imgs/23.jpg" width=600 height=240>

在非恒定水流中取出长度为 ds 的微分段作为控制体，两端断面为 n-n、m-m。  
设 n-n 断面的面积为 A，流速为 v，水流密度为 ρ。  

在 dt 时段内，流段上  
通过 n-n 断面流入的水体质量为 $\rho v A \mathrm{d}t$，通过 m-m 断面流出的水体质量 $\rho v A \mathrm{d}t + \frac{\partial}{\partial s}(\rho v A \mathrm{d}t) \mathrm{d}s$；  
$$\Rightarrow \rho v A \mathrm{d}t - [\rho v A \mathrm{d}t + \frac{\partial}{\partial s}(\rho v A \mathrm{d}t) \mathrm{d}s] = \frac{\partial}{\partial t}(\rho A \mathrm{d}s) \mathrm{d}t$$

由此，**非恒定流连续方程** 的普遍形式：  
$$\frac{\partial}{\partial t}(\rho A) + \frac{\partial}{\partial s}(\rho v A) = 0$$

方程适用于压力管道非恒定流、弹性管壁压力管道非恒定流以及明渠非恒定流；适用可压缩 **水击**水流。

对于明渠不可压缩的非恒定流，则有：  
$$\frac{\partial A}{\partial t} + \frac{\partial v A}{\partial s} = \frac{\partial A}{\partial t} + \frac{\partial Q}{\partial s} = \frac{\partial A}{\partial t} + v \frac{\partial A}{\partial s} + A \frac{\partial v}{\partial s}=0 $$

该方程表明$ \frac{\partial Q}{\partial s} < 0 \rightarrow \frac{\partial A}{\partial t} > 0$，流入多于流出，区间内水位将上升产生涨水波；反之，产生落水波。


对于不考虑管壁弹性的管道非恒定流：
$$v A = f(t)$$

该方程表明管道非恒定流的流量只随时间变化，在同一时刻下流量沿程不变（如调压系统液面震荡）。


</div>

*--- 注意：---*   
1. 压力管道中非恒定流波的传播是依靠压力差的作用，故称压力传播；  
明渠中非恒定流波的传播是依靠重力的作用，故称重力传播。

2. 对于非连续波，在波峰处水力要素不再连续，但在波体部分水面仍较为平缓，可以近似看作渐变流 。

---------------------------------------------------------------------------

### The Motion Equation of Unsteady Flow

*非恒定流运动方程*

<div align="center">

<img src="./Imgs/24.jpg" width=600 height=240>

在非恒定流中取长度为 ds 的微小流束，s 轴取于恒定流时水流方向一致，轴线与水平线夹角为 θ。  
n-n 断面上密度为 $\rho$，过水面积为 $A$，湿周为 $\chi$，压强为 $p$;  

m-m 断面上密度为 $\rho + \frac{\partial \rho}{\partial s}\mathrm{d}s$，过水面积为 $A + \frac{\partial A}{\partial s}\mathrm{d}s$，湿周为 $\chi + \frac{\partial \chi}{\partial s}\mathrm{d}s$，压强为 $p + \frac{\partial p}{\partial s}\mathrm{d}s$。

应用牛顿第二定律，分析微小流束的受力：  
$$\begin{aligned}
&1. 水压力，平行 s轴上 (p + \frac{\partial p}{\partial s} \frac{\mathrm{d}s}{2})(\frac{\partial A}{\partial s}\mathrm{d}s)；垂直 s轴上 (pA - (p + \frac{\partial p}{\partial s}\mathrm{d}s)(A + \frac{\partial A}{\partial s}\mathrm{d}s))；  \\
  \\
&2. 粘阻力，平行 s轴上 -[\tau (\chi + \frac{\partial \chi}{\partial s} \frac{\mathrm{d}s}{2})\mathrm{d}s] \cos \alpha，\tau 为单位周界面上平均阻力，\cos \alpha一般近似1.0； \\
  \\
&3. 质量力，垂直 s轴上 (\rho + \frac{\partial \rho}{\partial s} \frac{\mathrm{d}s}{2})(A + \frac{\partial A}{\partial s}\frac{\mathrm{d}s}{2}) g \mathrm{d}s \cdot \sin \theta = (\rho + \frac{\partial \rho}{\partial s} \frac{\mathrm{d}s}{2})(A + \frac{\partial A}{\partial s}\frac{\mathrm{d}s}{2}) g \mathrm{d}s \cdot (- \frac{\partial z}{\partial s})
\end{aligned}$$

根据牛顿第二定律，流段流体动量变化率：   
$$(\rho + \frac{\partial \rho}{\partial s} \frac{\mathrm{d}s}{2})(A + \frac{\partial A}{\partial s}\frac{\mathrm{d}s}{2}) g \mathrm{d}s \cdot \frac{\mathrm{d} v}{\mathrm{d} t} \quad And \quad \frac{\mathrm{d} v}{\mathrm{d} t} = \frac{\partial v}{\partial t} + v \frac{\partial v}{\partial s}$$

代入，得到 **微小流束非恒定流的运动方程**：  
$$\frac{\partial}{\partial s}(z + \frac{p}{\rho g} + \frac{v^2}{2g}) = - \frac{\tau \chi}{\rho g A} - \frac{1}{g} \frac{\partial v}{\partial t}$$

设总流为渐变流，对整个总流过水断面积分，略去断面上流速分布不均匀的影响，得到 **非恒定总流的运动方程**：      
$$\frac{\partial}{\partial s}(z + \frac{p}{\rho g} + \frac{v^2}{2g}) = - \frac{\tau_0 \chi_0}{\rho g A} - \frac{1}{g} \frac{\partial v}{\partial t}$$

$z, p, v$ 分别为总流过水断面的平均高程、压强及流速；$A$ 过水面积；$\chi_0$ 为湿周；$\tau_0$ 为断面周界上平均切应力。 

*------------------ * ------------------*

对不可压缩流体，$\rho g$ 为常数，将方程各项乘以 ds，从断面 1-1 至 2-2 积分，进而得到 **非恒定总流的能量方程**：
$$z_1 + \frac{p_1}{\rho g} + \frac{{v_1}^2}{2g} = z_2 + \frac{p_2}{\rho g} + \frac{{v_2}^2}{2g} + \int_{1}^{2} \frac{\tau_0 \chi_0}{\rho g A} \mathrm{d}s + \frac{1}{g} \int_{1}^{2} \frac{\partial v}{\partial t} \mathrm{d}s$$

其中，  
+ $\int_{1}^{2} \frac{\tau_0 \chi_0}{\rho g A} \mathrm{d}s$，代表总流单位质量流体的阻力在断面 1-1 至 2-2 间所作的功，即**内能耗散** $h_f$；  
+ $\int_{1}^{2} \frac{\partial v}{\partial t} \mathrm{d}s$，代表单位质量流体因当地加速度 $\frac{\partial v}{\partial t}$ 而引起的惯性力在断面 1-1 至 2-2 间所作功，即**惯性水头** $h_a$；

*------------------ * ------------------*

对于明渠渐变流，自由表面压力为 0，得到 **明渠非恒定渐变总流的运动方程**，  
又称 **圣维南方程**：
$$-\frac{\partial z}{\partial s} = \frac{\partial h_f}{\partial s} + \frac{1}{g} \frac{\partial v}{\partial t} + \frac{v}{g}\frac{\partial v}{\partial s}$$

其中，各项含义：  
+ $-\frac{\partial z}{\partial s}$，表示水面坡度 $J$，代表单位重量流体的势能沿程的变化率；
+ $\frac{1}{g} \frac{\partial v}{\partial t}$，表示波动坡度 $J_w$，代表当地加速度作用在单位重量流体上产生的惯性力沿着单位流程所作的功；  
+ $\frac{v}{g}\frac{\partial v}{\partial s}$，表示动能坡度 $J_v$，代表单位重量流体动能沿程的变化率(或迁移加速度的惯性力单位流程做功) ；    
+ $\frac{\partial h_f}{\partial s}$，表示摩阻坡度（又称为水力坡度） $J_f$，代表单位重量流体沿着单位流程克服摩擦阻力所作的功 。

因而，运动方程可以表示为能量关系：  
$$J = J_w + J_v + J_f$$

该方程表明能量守恒关系：明渠非恒定流中势能沿流程的改变，将用于克服阻力做功和转化为加速度而做功。

对于明渠恒定流，$J_w = 0$，可得到 **明渠恒定非均匀渐变流的基本微分方程**：
$$-\frac{\mathrm{d} z}{\mathrm{d} s} = \frac{\mathrm{d} h_f}{\mathrm{d} s} + \frac{v}{g} \frac{\mathrm{d} v}{\mathrm{d} s}$$

对于明渠恒定均匀流，$J_v = 0$，得到 **明渠恒定均匀流公式**：  
$$-\frac{\mathrm{d} z}{\mathrm{d} s} = \frac{\mathrm{d} h_f}{\mathrm{d} s} = J_f \quad Or \quad J_f = \frac{Q^2}{K^2}$$

明渠恒定均匀流是阻力和重力平衡时的运动，动能保持不变，水头损失来自势能的减少；摩阻坡度等于底坡。  
$$流量模数 K = A C \sqrt{R}, \quad 水力半径 R = \frac{A}{\chi}, \quad 谢才系数 C = \frac{1}{n} R^{\frac{1}{6}}, \quad 曼宁糙率n$$

若将摩阻坡度近似按恒定均匀流计算：
$$J_f = \frac{\partial h_f}{\partial s} = \frac{Q^2}{K^2} = \frac{v^2}{C^2 R}$$

进一步的，若明渠底高程为 z0，水深为 h，底坡为 i，则有 z = z0 + h，故有 $\frac{\partial z}{\partial s} = \frac{\partial z_0}{\partial s} + \frac{\partial h}{\partial s} = -i + \frac{\partial h}{\partial s}$。  
从而，可以得到圣维南方程的变形：  
$$i - \frac{\partial h}{\partial s} = \frac{\partial h_f}{\partial s} + \frac{1}{g} \frac{\partial v}{\partial t} + \frac{v}{g}\frac{\partial v}{\partial s}$$


</div>

*--- 注意：---*   
1. 未考虑微分时段 dt 内 $\rho, A, \chi, p$ 的变化，加入考虑、略去高阶项，推导结果不变。

2. 分析受力时，侧面积计算采用平均面积 $A + \frac{\partial A}{\partial s}\frac{\mathrm{d}s}{2}$；同理，对于密度、压力、湿周、阻力也采用平均值。

3. 测压管水头 $z + \frac{p}{\rho g}$，总水头 $z + \frac{p}{\rho g} + \frac{u^2}{2g}$。

---------------------------------------------------------------------------



---------------------------------------------------------------------------

