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

stored_avs = {}

# paths_explored = { p : [] for p in ps}

n = 15

modes = ["","unfair"]
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
            time = f.readline().split()[-2]
            assert best_costs[n] == cost and best_paths[n] == path
            times[p].append(float(time))
            f.readline()
            f.readline()

    # print times

    for k in times:
        times[k] = sorted(times[k])[:10]

    avs = { p : sum(times[p])/len(times[p]) for p in ps}
    mins = { p : min(times[p]) for p in ps }

    for p in ps:
        time = times[p]
        av = avs[p]
        print "{1} & {2:.5f} & {3:.5f} & {4:.5f}\\\\".format(n,p,av,min(time),max(time))
        # " ".join(map(str,time))

    for k in sorted(times):
        print k,len(times[k])

    ideals = map(lambda x: avs[ps[0]]/x,ps)


    fig = pyplot.figure()
    ax = fig.add_subplot(111)
    ax.plot(ps,ideals, 'g.-', label = 'Ideal Case')
    ax.plot(ps,[avs[p] for p in ps], 'r.-', label = 'Average Case')
    ax.plot(ps,[mins[p] for p in ps], 'b.-', label = 'Best Case')
    pyplot.xlabel('Processors')
    pyplot.ylabel('Time (s)')
    pyplot.title('Running Times for C = '+str(n))
    pyplot.yscale('log')
    pyplot.legend(loc=1)
    pyplot.savefig(str(n)+'_'+mode+'.png')
        # pyplot.show()
    stored_avs[mode] = avs

# SpeedUp = { p : avs[1]/avs[p] for p in ps }
# Efficiency = { p : SpeedUp[p]/p for p in ps }
# for C in ns:
fig = pyplot.figure()
ax = fig.add_subplot(111)
ax.plot(ps,ps, 'g.-',label='Ideal')
avs = stored_avs[""]
ax.plot(ps,[avs[1]/avs[p] for p in ps], 'r.-',label='Balanced Load')
avs = stored_avs["unfair"]
ax.plot(ps,[avs[1]/avs[p] for p in ps], 'b.-',label='Unbalanced Load')
pyplot.xlabel('Processors')
pyplot.ylabel('SpeedUp')
pyplot.title('Comparison of SpeedUp')
# pyplot.legend(['Ideal SpeedUp','n = '+str(n)],loc=2)
handles, labels = ax.get_legend_handles_labels()
lgd = ax.legend(handles, labels, loc='upper center', bbox_to_anchor=(0.5,-0.1))
# ax.grid('on')
fig.savefig('SpeedUp.png', bbox_extra_artists=(lgd,), bbox_inches='tight')
# pyplot.savefig('SpeedUp.png')
# pyplot.show()

fig = pyplot.figure()
ax = fig.add_subplot(111)
ax.plot(ps,[1]*len(ps), 'g.-',label="Ideal")
avs = stored_avs[""]
ax.plot(ps,[avs[1]/avs[p]/p for p in ps], 'r.-',label='Balanced Load')
avs = stored_avs["unfair"]
ax.plot(ps,[avs[1]/avs[p]/p for p in ps], 'b.-',label='Unbalanced Load')
pyplot.xlabel('Processors')
pyplot.ylabel('Efficiency')
axes = pyplot.gca()
# axes.set_xlim([1,35])
axes.set_ylim([0,1.5])
pyplot.title('Comparison of Efficiencies')
handles, labels = ax.get_legend_handles_labels()
lgd = ax.legend(handles, labels, loc='upper center', bbox_to_anchor=(0.5,-0.1))
# ax.grid('on')
fig.savefig('Efficiency.png', bbox_extra_artists=(lgd,), bbox_inches='tight')
# pyplot.savefig('Efficiency.png')
# pyplot.show()
