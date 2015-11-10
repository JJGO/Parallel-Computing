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

modes = ["BFS","DFS"]
for mode in modes:
    times = { p : [] for p in ps}

    # changes = { p : [] for p in ps}
    # paths = { p : [] for p in ps}
    # pushes = { p : [] for p in ps}
    print mode
    with open("Results_"+mode+".txt",'r') as f:
        # for i in range(9*8):
        #     for j in range(8):
        #         f.readline()
        while(f.readline()):
            n = int(f.readline()[:2])
            p = int(f.readline().split()[0])
            path,cost = f.readline().split(':')
            path = map(int,path.split(',')[:-1])
            cost = float(cost[:-1])
            # f.readline()
            # c,np,pu = [],[],[]
            # for i in range(p):
            #     values = map(int,f.readline().split())
            #     c.append(values[1])
            #     np.append(values[2])
            #     pu.append(values[3])
            # changes[p].append(c)
            # paths[p].append(np)
            # pushes[p].append(pu)
            time = f.readline().split()[-2]
            assert best_costs[n] == cost and best_paths[n] == path
            times[p].append(float(time))
            f.readline()
            f.readline()

    # print times

    for k in times:
        times[k] = sorted(times[k])[:25]

    avs = { p : sum(times[p])/len(times[p]) for p in ps}
    mins = { p : min(times[p]) for p in ps }

    for p in ps:
        time = times[p]
        av = avs[p]
        print "{0} & {1} - {2:.5f} {3:.5f} {4:.5f}".format(n,p,av,min(time),max(time))
        # " ".join(map(str,time))

    # paths_per_thread = {}
    # total_paths = {}
    # for proc in ps:
    #     x = [sum(p) for p in paths[proc]]
    #     total_paths[proc] = sum(x)/len(x)
    #     y = [sum(p)*1.0/len(p) for p in paths[proc]]
    #     paths_per_thread[proc] = sum(y)/len(y)
    #     print "{0} - {1:09.1f} {2:09.1f} {3:7d}".format(proc, paths_per_thread[proc], total_paths[proc]*1.0/proc, total_paths[proc])

    # for k in sorted(times):
    #     print k,len(times[k])

    ideals = map(lambda x: avs[ps[0]]/x,ps)


    fig = pyplot.figure()
    ppl.plot(ps,ideals, 'go-')
    ppl.plot(ps,[avs[p] for p in ps], 'ro-')
    ppl.plot(ps,[mins[p] for p in ps], 'bo-')
    pyplot.xlabel('Processors')
    pyplot.ylabel('Time (s)')
    pyplot.title('Running Times for n = '+str(n))
    pyplot.legend(['Ideal Case','Average Case','Best Case'])
    pyplot.yscale('log')
    pyplot.savefig(str(n)+'_'+mode+'.png')
        # pyplot.show()

    SpeedUp = { p : avs[1]/avs[p] for p in ps }
    Efficiency = { p : SpeedUp[p]/p for p in ps }
    # for n in ns:
    fig = pyplot.figure()
    ppl.plot(ps,ps, 'go-')
    ppl.plot(ps,[SpeedUp[p] for p in ps], 'ro-')
    pyplot.xlabel('Processors')
    pyplot.ylabel('SpeedUp')
    pyplot.title('Comparison of SpeedUp')
    pyplot.legend(['Ideal SpeedUp','n = '+str(n)],loc=2)
    pyplot.savefig('SpeedUp_'+mode+'.png')
    # pyplot.show()

    fig = pyplot.figure()
    ppl.plot(ps,[1]*len(ps), 'go-')
    ppl.plot(ps,[Efficiency[p] for p in ps], 'ro-')
    pyplot.xlabel('Processors')
    pyplot.ylabel('Efficiency')
    axes = pyplot.gca()
    # axes.set_xlim([1,35])
    axes.set_ylim([0,1.5])
    pyplot.title('Comparison of Efficiencies')
    pyplot.legend(['Ideal Efficiency','n = '+str(n)],loc=3)
    pyplot.savefig('Efficiency_'+mode+'.png')
    # pyplot.show()
