from matplotlib import pyplot
# from mpltools import style
import prettyplotlib as ppl
# from mpltools import layout

# style.use('ggplot')

# figsize = layout.figaspect(scale=1.2)

ps = [1,4,16,32]
# ps = [1,2,4,8]
# ps = range(1,9)
best_paths = {}
best_paths[13] = [0, 9, 1, 8, 7, 2, 3, 4, 11, 6, 10, 12, 5]
best_paths[15] = [0, 13, 9, 1, 8, 14, 7, 2, 3, 4, 11, 6, 10, 12, 5]
best_costs = {}
best_costs[13] = 778.211287
best_costs[15] = 782.989290



# paths_explored = { p : [] for p in ps}

n = 15


times = { p : [] for p in ps}

changes = { p : [] for p in ps}
paths = { p : [] for p in ps}
pushes = { p : [] for p in ps}

with open("Results_BFS_0.txt",'r') as f:
    while(f.readline()):
        n = int(f.readline()[:2])
        p = int(f.readline().split()[0])
        path,cost = f.readline().split(':')
        path = map(int,path.split(',')[:-1])
        cost = float(cost[:-1])
        f.readline()
        c,np,pu = [],[],[]
        for i in range(p):
            values = map(int,f.readline().split())
            c.append(values[1])
            np.append(values[2])
            pu.append(values[3])
        changes[p].append(c)
        paths[p].append(np)
        pushes[p].append(pu)
        time = f.readline().split()[-2]
        assert best_costs[n] == cost and best_paths[n] == path
        times[p].append(float(time)/3.0)
        f.readline()
        f.readline()

# print times

for k in times:
    times[k] = sorted(times[k])[:20]

avs = { p : sum(times[p])/len(times[p]) for p in ps}
mins = { p : min(times[p]) for p in ps }

for p in ps:
    time = times[p]
    av = avs[p]
    print "{0} & {1} - {2:.5f} {3:.5f} {4:.5f}".format(n,p,av,min(time),max(time))
    # " ".join(map(str,time))

paths_per_thread = {}
total_paths = {}
ideal_paths_per_thread = {}

for proc in ps:
    x = [sum(p) for p in paths[proc]]
    total_paths[proc] = sum(x)/len(x)
    y = [sum(p)*1.0/len(p) for p in paths[proc]]
    paths_per_thread[proc] = sum(y)/len(y)
    z = total_paths[proc]*1.0/proc
    ideal_paths_per_thread[proc] = z
    print "{0} - {1:09.1f} {2:09.1f} {3:7d}".format(proc, paths_per_thread[proc], ideal_paths_per_thread[proc], total_paths[proc])

pushes_per_thread = {}
total_pushes = {}
ideal_pushes_per_thread = {}

for proc in ps:
    x = [sum(p) for p in pushes[proc]]
    total_pushes[proc] = sum(x)/len(x)
    y = [sum(p)*1.0/len(p) for p in pushes[proc]]
    pushes_per_thread[proc] = sum(y)/len(y)
    z = total_pushes[proc]*1.0/proc
    ideal_pushes_per_thread[proc] = z
    print "{0} - {1:05.1f} {2:05.1f} {3:7d}".format(proc, pushes_per_thread[proc], ideal_pushes_per_thread[proc], total_pushes[proc])

changes_per_thread = {}
total_changes = {}
ideal_changes_per_thread = {}

for proc in ps:
    x = [sum(p) for p in changes[proc]]
    total_changes[proc] = sum(x)/len(x)
    y = [sum(p)*1.0/len(p) for p in changes[proc]]
    changes_per_thread[proc] = sum(y)/len(y)
    z = total_changes[proc]*1.0/proc
    ideal_changes_per_thread[proc] = z
    print "{0} - {1:02.1f} {2:02.1f} {3:7d}".format(proc, changes_per_thread[proc], ideal_changes_per_thread[proc], total_changes[proc])


for k in sorted(times):
    print k,len(times[k])

ideals = map(lambda x: avs[ps[0]]/x,ps)


fig = pyplot.figure()
ppl.plot(ps,[ideal_paths_per_thread[p] for p in ps], 'go-')
ppl.plot(ps,[paths_per_thread[p] for p in ps], 'ro-')
ppl.plot(ps,[total_paths[p] for p in ps], 'bo-')
pyplot.xlabel('Processors')
pyplot.ylabel('Nodes explored')
pyplot.title('Load balancing of nodes for C = '+str(n))
pyplot.legend(['Ideal Node Balancing','Nodes explored per thread','Total nodes explored'],loc=5)
# pyplot.yscale('log')
pyplot.savefig('BFS_paths.png')
# pyplot.show()

fig = pyplot.figure()
ppl.plot(ps,[ideal_pushes_per_thread[p] for p in ps], 'go-')
ppl.plot(ps,[pushes_per_thread[p] for p in ps], 'ro-')
ppl.plot(ps,[total_pushes[p] for p in ps], 'bo-')
pyplot.xlabel('Processors')
pyplot.ylabel('Jobs returned to the global queue')
pyplot.title('Load balancing of work for C = '+str(n))
pyplot.legend(['Perfect Load Balancing','Works queued per thread','Total works queued'],loc=2)
# pyplot.yscale('log')
axes = pyplot.gca()
axes.set_xlim([ps[0]-1,ps[-1]+1])
# axes.set_ylim([min(ideal_pushes_per_thread.values())-1e4,max(total_pushes.values())+1e4])
# pyplot.savefig(str(n)+'_'+mode+'.png')
pyplot.savefig('BFS_pushes.png')
# pyplot.show()

fig = pyplot.figure()
ppl.plot(ps,[ideal_changes_per_thread[p] for p in ps], 'go-')
ppl.plot(ps,[changes_per_thread[p] for p in ps], 'ro-')
ppl.plot(ps,[total_changes[p] for p in ps], 'bo-')
pyplot.xlabel('Processors')
pyplot.ylabel('Number of changes on the global bound')
pyplot.title('Bound updates for C = '+str(n))
pyplot.legend(['Ideal changes per thread','Changes per thread','Total bound changes'],loc=1)
axes = pyplot.gca()
axes.set_xlim([ps[0]-1,ps[-1]+1])
axes.set_ylim([min(ideal_changes_per_thread.values())-1,max(total_changes.values())+1])
# pyplot.savefig(str(n)+'_'+mode+'.png')
pyplot.savefig('BFS_changes.png')
# pyplot.show()