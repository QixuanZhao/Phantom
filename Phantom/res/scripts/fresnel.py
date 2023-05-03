import numpy as np
import matplotlib.pyplot as plt

N = 512

def RI(R0):
    R0R = np.sqrt(R0)
    return (1 + R0R) / (1 - R0R)

def fres (cosine, R_IT, K) :
    # /// RGB : 700, 546.1, 435.8 (nm)
    cosineI = cosine
    sineI = np.sqrt(1.0 - cosineI * cosineI)
    sineT = R_IT * sineI
    determinator = 1.0 - sineT * sineT
    determinator[determinator < 0] = 0
    cosineT = np.sqrt(determinator)
    
    RT = 1.0 / R_IT

    A = cosineI - RT * cosineT
    B = K * cosineT
    C = cosineI + RT * cosineT
    D = -B

    CD = C * C + D * D
    R = (A * C + B * D) / CD
    I = (B * C - A * D) / CD

    Rs = R * R + I * I


    A = cosineT - RT * cosineI
    B = K * cosineI
    C = cosineT + RT * cosineI
    D = -B

    CD = C * C + D * D
    R = (A * C + B * D) / CD
    I = (B * C - A * D) / CD

    Rp = R * R + I * I

    return (Rs + Rp) / 2.0

    
def Fresnel(theta, eta_i, eta_t, kt):
    theta_i = theta
    cosine_i = np.cos(theta)
    sine_i = np.sin(theta)
    sine_t = eta_i * sine_i / eta_t
    cosine_t = np.sqrt(1 - sine_t ** 2)

    left_s = eta_i * cosine_i
    right_s = (eta_t - kt * 1j) * cosine_t

    Rs = np.abs((left_s - right_s) / (left_s + right_s)) ** 2

    left_p = eta_i * cosine_t
    right_p = (eta_t - kt * 1j) * cosine_i

    Rp = np.abs((left_p - right_p) / (left_p + right_p)) ** 2

    return (Rs + Rp) / 2

X = np.pi / 2 * np.array(list(range(N))) / (N - 1)
ETA_I = np.ones(N)
ETA_T = np.zeros((N, N)) + (200 * np.array(list(range(1, N + 1))) / (N + 1))
KT = np.zeros((N, N)) + (200 * np.array(list(range(N))) / (N))


fig, ((ax1, ax2, ax3), (ax4, ax5, ax6)) = plt.subplots(2, 3)

i_kt = 0
for i in range(5, int(N/2), int(N/50)) :
    ax1.plot(X * 180 / np.pi, Fresnel(X, ETA_I, ETA_T[:,i], KT[:,i_kt]), alpha=(0.3 + 0.7 * (1 - i / int(N/2))), color='b')
for i in range(5, int(N/2), int(N/50)) :
    ax1.plot(X * 180 / np.pi, fres(np.cos(X), 1.0 / ETA_T[:,i], KT[:,i_kt]), alpha=(0.3 + 0.7 * (1 - i / int(N/2))), color='g')
ax1.set_xlabel('Incident Angle')
ax1.set_ylabel(f'Reflectance {KT[0,i_kt]}')
ax1.set_xlim(0, 90)
# ax1.set_ylim(0.4, 1)

i_kt = 5
for i in range(5, int(N/2), int(N/50)) :
    ax2.plot(X * 180 / np.pi, Fresnel(X, ETA_I, ETA_T[:,i], KT[:,i_kt]), alpha=(0.3 + 0.7 * (1 - i / int(N/2))), color='g')
for i in range(5, int(N/2), int(N/50)) :
    ax2.plot(X * 180 / np.pi, fres(np.cos(X), 1.0 / ETA_T[:,i], KT[:,i_kt]), alpha=(0.3 + 0.7 * (1 - i / int(N/2))), color='r')
ax2.set_xlabel('Incident Angle')
ax2.set_ylabel(f'Reflectance {KT[0,i_kt]}')
ax2.set_xlim(0, 90)
# ax2.set_ylim(0.4, 1)

i_kt = 10
for i in range(5, int(N/2), int(N/50)) :
    ax3.plot(X * 180 / np.pi, Fresnel(X, ETA_I, ETA_T[:,i], KT[:,i_kt]), alpha=(0.3 + 0.7 * (1 - i / int(N/2))), color='r')
for i in range(5, int(N/2), int(N/50)) :
    ax3.plot(X * 180 / np.pi, fres(np.cos(X), 1.0 / ETA_T[:,i], KT[:,i_kt]), alpha=(0.3 + 0.7 * (1 - i / int(N/2))), color='c')
ax3.set_xlabel('Incident Angle')
ax3.set_ylabel(f'Reflectance {KT[0,i_kt]}')
ax3.set_xlim(0, 90)
# ax3.set_ylim(0.4, 1)

i_kt = 20
for i in range(5, int(N/2), int(N/50)) :
    ax4.plot(X * 180 / np.pi, Fresnel(X, ETA_I, ETA_T[:,i], KT[:,i_kt]), alpha=(0.3 + 0.7 * (1 - i / int(N/2))), color='c')
for i in range(5, int(N/2), int(N/50)) :
    ax4.plot(X * 180 / np.pi, fres(np.cos(X), 1.0 / ETA_T[:,i], KT[:,i_kt]), alpha=(0.3 + 0.7 * (1 - i / int(N/2))), color='k')
ax4.set_xlabel('Incident Angle')
ax4.set_ylabel(f'Reflectance {KT[0,i_kt]}')
ax4.set_xlim(0, 90)
# ax4.set_ylim(0.4, 1)

i_kt = 30
for i in range(5, int(N/2), int(N/50)) :
    ax5.plot(X * 180 / np.pi, Fresnel(X, ETA_I, ETA_T[:,i], KT[:,i_kt]), alpha=(0.3 + 0.7 * (1 - i / int(N/2))), color='k')
for i in range(5, int(N/2), int(N/50)) :
    ax5.plot(X * 180 / np.pi, fres(np.cos(X), 1.0 / ETA_T[:,i], KT[:,i_kt]), alpha=(0.3 + 0.7 * (1 - i / int(N/2))), color='#008000')
ax5.set_xlabel('Incident Angle')
ax5.set_ylabel(f'Reflectance {KT[0,i_kt]}')
ax5.set_xlim(0, 90)
# ax5.set_ylim(0.4, 1)

i_kt = 40
for i in range(5, int(N/2), int(N/50)) :
    ax6.plot(X * 180 / np.pi, Fresnel(X, ETA_I, ETA_T[:,i], KT[:,i_kt]), alpha=(0.3 + 0.7 * (1 - i / int(N/2))), color='#008000')
for i in range(5, int(N/2), int(N/50)) :
    ax6.plot(X * 180 / np.pi, fres(np.cos(X), 1.0 / ETA_T[:,i], KT[:,i_kt]), alpha=(0.3 + 0.7 * (1 - i / int(N/2))), color='b')
ax6.set_xlabel('Incident Angle')
ax6.set_ylabel(f'Reflectance {KT[0,i_kt]}')
ax6.set_xlim(0, 90)
# ax6.set_ylim(0.4, 1)

plt.show()
