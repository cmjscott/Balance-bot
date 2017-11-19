from math import sqrt, cos, sin, acos, atan2, degrees
import numpy as np


class Robot:

	def __init__(self, L0, L1, L2, L3):
		self.links = (L0, L1, L2, L3)
		self.joints = (0, 0, 0)
	
	def solve(self, x, z, theta):
		L0, L1, L2, L3 = self.links
		x0 = L0/2 + x - L3/2 * cos(theta)
		x1 = L0/2 - x - L3/2 * cos(theta)
		z0 = z - L3/2 * sin(theta)
		z1 = z + L3/2 * sin(theta)
		mag0 = sqrt(x0**2 + z0**2)
		mag1 = sqrt(x1**2 + z1**2)
		arg0 = atan2(z0, x0)
		arg1 = atan2(z1, x1)
		theta0 = acos((mag0**2 + L1**2 - L2**2) / (2 * L1 * mag0)) + arg0
		theta1 = acos((mag1**2 + L1**2 - L2**2) / (2 * L1 * mag1)) + arg1
		theta2 = acos((mag1**2 + L2**2 - L1**2) / (2 * L2 * mag1)) - arg1
		self.joints = (theta0, theta1, theta2)
		return self.joints

	def getjoints(self):
		L0, L1, L2, L3 = self.links
		theta0, theta1, theta2 = self.joints
		A = np.array([-L0/2, 0])
		B = A + L1*np.array([cos(theta0), sin(theta0)])
		F = np.array([+L0/2, 0])
		E = F - L1*np.array([cos(theta1), -sin(theta1)])
		D = E - L2*np.array([cos(theta2), sin(theta2)])
		d = np.linalg.norm(B - D)
		a = (L3**2 - L2**2 + d**2) / (2*d)
		h = sqrt(L3**2 - a**2)
		C = D + np.matrix([[a, -h], [h, a]]).dot(B - D) / d
		C = np.squeeze(np.asarray(C))
		return [A, B, C, D, E, F]

if __name__ == '__main__':
	from matplotlib import pyplot as plt
	r = Robot(427, 140, 100, 289)
	r.solve(0, 50, 0)

	x, y = zip(*r.getjoints())
	plt.plot(x, y)
	plt.axes().set_aspect('equal', 'datalim')
	plt.show()
