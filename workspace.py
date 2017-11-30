from matplotlib import pyplot as plt
import numpy as np
from IK import Robot


class Workspace:

	def __init__(self, L0, L1, L2, L3):
		self.robot = Robot(L0, L1, L2, L3)
		self.precision = 1

	def calculate(self, theta):
		L0, L1, L2, L3 = self.robot.links
		W = round(L0 / self.precision)
		H = round((L1 + L2) / self.precision)
		matrix = np.zeros((H, W), dtype='bool')
		for m,n in np.mgrid[:H,:W].reshape(2,-1).T:
			G = self.precision * np.array([n - W / 2, H - m])
			C = G - L3 / 2 * np.array([np.cos(theta), -np.sin(theta)])
			D = G + L3 / 2 * np.array([np.cos(theta), -np.sin(theta)])
			AC = np.linalg.norm(C - np.array([-L0 / 2, 0]))
			FD = np.linalg.norm(D - np.array([+L0 / 2, 0]))
			matrix[m,n] = (AC > L1 - L2 and AC < L1 + L2) and (FD > L1 - L2 and FD < L1 + L2)
		return matrix

	def plot(self, theta_deg):
		L0, L1, L2, L3 = self.robot.links
		theta = np.radians(theta_deg)
		x, z = L1 * 2/3 * np.array([-np.sin(theta), np.cos(theta)])
		self.robot.solve(x, z, -theta)
		matrix = self.calculate(theta)
		joints = self.robot.getjoints()
		joints.insert(3, (x, z))
		plt.imshow(matrix, cmap='Reds', extent=[-L0 /2, L0 / 2, 0, L1 + L2])
		plt.plot(*zip(*joints), 'bo-')
		plt.title('theta = {:.0f}°'.format(theta_deg))

if __name__ == '__main__':
	ws = Workspace(600, 140, 100, 337)
	ws.precision = 1
	plt.figure()
	plt.subplot(321); ws.plot(0)
	plt.subplot(322); ws.plot(-5)
	plt.subplot(323); ws.plot(-10)
	plt.subplot(324); ws.plot(-15)
	plt.subplot(325); ws.plot(-20)
	plt.subplot(326); ws.plot(-25)
	plt.show()

