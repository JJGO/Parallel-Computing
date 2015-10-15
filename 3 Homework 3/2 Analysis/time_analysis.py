from matplotlib import pyplot
# from mpltools import style
import prettyplotlib as ppl
# from mpltools import layout

# style.use('ggplot')

# figsize = layout.figaspect(scale=1.2)

ps = [1,4,16,36]
ns = [1000,2000]
sums = {1000 : "4283810.871966", 2000 : "17631642.148465"}
mins = {1000 : "-892.718330", 2000 : "-1800.724437"}
times = { (n,p) : [] for p in ps for n in ns}

with open("result.txt",'r') as f:
    # for i in range(9*8):
    #     for j in range(8):
    #         f.readline()
    while(f.readline()):
        n = int(f.readline()[:4])
        p = int(f.readline().split()[0])
        suma = f.readline().split()[-1]
        mina = f.readline().split()[-1]
        time = f.readline().split()[-2]
        times[(n,p)].append(float(time))
        f.readline()
        f.readline()

for k in times:
    times[k] = sorted(times[k])[:20]

avs = { (n,p) : sum(times[(n,p)])/len(times[(n,p)]) for p in ps for n in ns}
mins = { (n,p) : min(times[(n,p)]) for p in ps for n in ns }
for n in ns:
    for p in ps:
        time = times[(n,p)]
        av = avs[(n,p)]
        print "{0} & {1} - {2:.5f} {3:.5f} {4:.5f}".format(n,p,av,min(time),max(time))
        # " ".join(map(str,time))

print
for p in ps:
    print"{0} - {1:.5f}".format(p,avs[(2000,p)]/avs[(1000,p)])
print
for n in ns:
    for p in ps[1:]:
        ES = 1.0/(1.0/p+100.0/(n*p**.5))
        print"{0} & {1} - {2:.5f} - {3:.5f}".format(n,p,avs[(n,1)]/avs[(n,p)],ES)

# for k in times:
#     print k,len(times[k])

ideals = {n : map(lambda x: avs[(n,ps[0])]/x,ps) for n in ns}


for n in ns:
    fig = pyplot.figure()
    ppl.plot(ps,ideals[n], 'go-')
    ppl.plot(ps,[avs[(n,p)] for p in ps], 'ro-')
    ppl.plot(ps,[mins[(n,p)] for p in ps], 'bo-')
    pyplot.xlabel('Processors')
    pyplot.ylabel('Time (s)')
    pyplot.title('Running Times for n = '+str(n))
    pyplot.legend(['Ideal Case','Average Case','Best Case'])
    pyplot.yscale('log')
    if n == 2000:
        axes = pyplot.gca()
        # axes.set_xlim([1,40])
        axes.set_ylim([0.5,30])
    pyplot.savefig(str(n)+'.png')
    # pyplot.show()

SpeedUp = { (n,p) : mins[n,1]/mins[n,p] for p in ps for n in ns}
Efficiency = { (n,p) : SpeedUp[(n,p)]/p for p in ps for n in ns}
# for n in ns:
fig = pyplot.figure()
ppl.plot(ps,ps, 'go-')
ppl.plot(ps,[SpeedUp[(1000,p)] for p in ps], 'ro-')
ppl.plot(ps,[SpeedUp[(2000,p)] for p in ps], 'bo-')
pyplot.xlabel('Processors')
pyplot.ylabel('SpeedUp')
pyplot.title('Comparison of SpeedUp')
pyplot.legend(['Ideal SpeedUp','n = 1000','n = 2000'],loc=2)
pyplot.savefig('SpeedUp.png')
# pyplot.show()

# for n in ns:
fig = pyplot.figure()
ppl.plot(ps,[1]*len(ps), 'go-')
ppl.plot(ps,[Efficiency[(1000,p)] for p in ps], 'ro-')
ppl.plot(ps,[Efficiency[(2000,p)] for p in ps], 'bo-')
pyplot.xlabel('Processors')
pyplot.ylabel('Efficiency')
axes = pyplot.gca()
axes.set_xlim([1,40])
axes.set_ylim([0,1.1])
pyplot.title('Comparison of Efficiencies')
pyplot.legend(['Ideal Efficiency','n = 1000','n = 2000'],loc=3)
pyplot.savefig('Efficiency.png')
# pyplot.show()
