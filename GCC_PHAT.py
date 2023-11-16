import numpy as np

# 进行GCC_PHAT操作
def gcc_phat(s1, s2):

    f1 = np.fft.fft(s1)
    f2 = np.fft.fft(s2)

    G = np.conj(f1) * f2

    # 相位变换系数矩阵，即最功率谱中每一项就abs
    w = np.abs(G)

    G = G / w

    R = np.fft.ifft(G)

    # 找到R的最大值对应的索引编号
    max_idx = np.argmax(np.abs(R))

    return np.abs(R), max_idx # 输出相位变化后的互相关函数，和峰值索引值


# # 测试样例

# a1 = np.random.randint(size=1000, low=0, high=100)

# # a2的前23位是0，后面是a1，长度也为1000
# delay = 98
# a2 = np.zeros(1000)
# a2[delay:1000] = a1[0:(1000-delay)]

# res1,idx1 = gcc_phat(a1, a2)
# print((res1[idx1-1]), (res1[idx1]), (res1[idx1+1]))
# res2,idx2 = gcc_phat(a2, a1)

# import matplotlib.pyplot as plt
# plt.plot((res1))
# plt.plot((res2))
# plt.show()

# plt.plot((res1)+(res2))
# # plt.plot((res1)*(res1))
# plt.plot((res2)*(res2))
# plt.show()

# print(idx1,idx2)