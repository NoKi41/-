import GCC_PHAT as gcc
import numpy as np
import matplotlib.pyplot as plt

# 设置麦克风阵列，每个麦克风的二维坐标，单位为米
M = 8

MICS = np.array([
    [-0.25, 0],
    [-0.5, 0],
    [-0.75, 0],
    [-1, 0],
    [0, 0],
    [1, 0],
    [1.5, 0],
    [2, 0]])

# 地图分辨率
step = 0.01

# 设置声源坐标
SOURCE = np.array([-4, 3.3])

# 设定原始信号长度
LEN = 256
Signal = np.random.randint(low=-100,high=100,size=LEN)

# 假定信号传播速度为340m每秒，采样频率为4400Hz
SPEED = 340
SAMPLING_RATE = 4400

# 定义每个麦克风接收信号的矩阵，第一个索引为麦克风编号
Signal_MIC = np.zeros((M, LEN))

# 根据几何关系，推算各个麦克风处接收到的信号，不考虑传播中的损耗，只考虑传播延迟
for i in range(M):

    # 先计算从信号源传播过来的时间
    delta_t = np.linalg.norm(SOURCE - MICS[i]) / SPEED

    # 由时间，推算需要延迟的码元数
    delay = int(delta_t * SAMPLING_RATE)

    # if delay > LEN:
    #     print('Error')

    # 在原始信号基础上，进行延迟
    Signal_MIC[i] = np.pad(Signal, (delay, 0), 'constant')[:LEN]
    
    # print(len(Signal_MIC[i]))
    # plt.plot(Signal_MIC[i])

# plt.show()

# 初始化互相关函数矩阵
R = np.zeros((M, M, LEN))

# 计算每一对麦克风之间的接收信号的gcc_phat
for i in range(M):
    for j in range(i+1, M):
        R[i, j, :], _ = gcc.gcc_phat(Signal_MIC[i, :], Signal_MIC[j, :])
        
        # plt.plot(R[i, j, :])

# plt.show()

# 设定一个空间范围，即声源可能在的空间区域
x_range = np.arange(-5, 5, step)
y_range = np.arange(-5, 5, step)

# 初始化地图矩阵，用来存放响应值
map_matrix = np.zeros((len(x_range), len(y_range)))

# 遍历空间范围内的每一个站点
for ix, x in enumerate(x_range):
    for iy, y in enumerate(y_range):
    
        # 用以累计响应值
        sum = 0

        # 遍历每一对麦克风组合
        for p in range(0,M):
            for q in range(p+1, M):
                 
                # 计算站点位置对麦克风对p,q的时间差
                delta_t = abs((np.linalg.norm([y, x] - MICS[p]) - np.linalg.norm([y, x] - MICS[q])) / SPEED) # TODO:norm[x,y] -> norm[y,x]
                
                # 延迟时间对应的延迟码元数
                delay = int(delta_t * SAMPLING_RATE)

                # if delay < 0: # 把delay的abs打掉，解决对称问题
                #     delay = delay + LEN

                # if np.linalg.norm([x, y] - MICS[p]) > np.linalg.norm([x, y] - MICS[q]):
                #     delay = 255 - delay

                delay2 = 255 - delay

                # if delay >= LEN:
                    # print("Error")

                # 累加响应值
                # sum += R[p, q, delay]
                sum = sum + R[p, q, delay] + R[p, q, delay2]
        
        # 把响应值存在地图矩阵的对应位置上
        map_matrix[-iy, ix] = sum # TODO:[ix,iy] -> norm[-iy,ix]


# 可视化结果
plt.imshow(map_matrix, extent=[x_range.min(), x_range.max(), y_range.min(), y_range.max()])
plt.colorbar()

# 标记真实声源位置
plt.scatter(SOURCE[1], SOURCE[0], c='red', marker='x', label='Source') # TODO: SOURCE[0], SOURCE[1] -> SOURCE[1], SOURCE[0]

# 标记麦克风位置
for mic in MICS:
    plt.scatter(mic[1], mic[0], c='white', marker='o', label='Mic' if 'Mic' not in plt.gca().get_legend_handles_labels()[1] else "") # TODO: mic[0], mic[1] -> mic[1], mic[0]

plt.title('Map')
plt.xlabel('Y') # TODO: X -> Y
plt.ylabel('X') # TODO: Y -> X
plt.legend()
plt.show()