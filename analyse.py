import numpy as np
import matplotlib.pyplot as plt

recs, tims, ress, secs, inds = [], [], [], [], []
with open('data.txt','r') as f:
	lines = f.read().split('\n')
	for line in lines:
		if len(line)>4:
			rec, tim, res, sec, ind = line.split('\t')
			recs.append(int(rec))
			# ~ tims.append(np.log(int(tim)+1))
			tims.append(int(tim))
			if res=="'":
				ress.append(1)
			else:
				ress.append(0)
			secs.append(int(sec))
			inds.append(ind)

recs = np.array(recs)
tims = np.array(tims)
ress = np.array(ress)
secs = np.array(secs)
inds = np.array(inds)
[print(ress[inds==ii], ii) for ii in inds]
plt.loglog(recs[ress==1]+1, tims[ress==1]+1,'o')
plt.loglog(recs[ress==0]+1, tims[ress==0]+1,'x')
plt.show()

pltis = [(1,70), (70,140), (140,5000), (5000,20000), (20000,70000), (70000,150000), (150000,350000), (350000,1000000), (1e6, 1e9)]
fig, axs = plt.subplots(3,3)
num=0
for i in range(3):
	for j in range(3):
		# ~ plti = (10**(num*0.75), 10**(num*0.75+0.75))
		plti = pltis[num]
		print(num)
		print(plti)
		ratscorr = tims[(ress==1)*(recs>plti[0])*(recs<plti[1])]/recs[(ress==1)*(recs>plti[0])*(recs<plti[1])]
		ratsinco = tims[(ress==0)*(recs>plti[0])*(recs<plti[1])]/recs[(ress==0)*(recs>plti[0])*(recs<plti[1])]
		ncorr, bins, _ = axs[i,j].hist(np.log10(ratscorr[(1<ratscorr)]), bins=np.linspace(0,4,20))
		ninco, _, _ = axs[i,j].hist(np.log10(ratsinco[(1<ratsinco)]), bins=np.linspace(0,4,20))
		ax2 = axs[i,j].twinx()
		plt.ylim(0,100)
		plt.plot(bins[:-1]+(bins[1]-bins[0])*0.5,100*ninco/(ncorr+ninco), 'g')
		num = num + 1
plt.show()

ress = ress[(0<recs)*(recs<150)]
tims = tims[(0<recs)*(recs<150)]
ress = ress[tims<5000]
tims = tims[tims<5000]
plt.hist(np.log10(tims[ress==1]+1))
n, bins, patches = plt.hist(np.log10(tims[ress==0]+1))
print(n)
print(bins)
print(patches)
plt.show()
