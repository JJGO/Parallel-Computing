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

# modes = ["","_unfair"]
modes = [""]
for mode in modes:
    times = { p : [] for p in ps}

    changes = { p : [] for p in ps}
    paths = { p : [] for p in ps}
    pushes = { p : [] for p in ps}

    with open("Results"+mode+".txt",'r') as f:
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
        print "{1} & {2:.5f} & {3:.5f} & {4:.5f}\\\\".format(n,p,av,min(time),max(time))
        # " ".join(map(str,time))

    paths_per_thread = {}
    total_paths = {}
    ideal_paths_per_thread = {}
    max_paths_per_thread = {}
    min_paths_per_thread = {}

    for proc in ps:
        x = [sum(p) for p in paths[proc]]
        total_paths[proc] = sum(x)/len(x)
        y = [sum(p)*1.0/len(p) for p in paths[proc]]
        paths_per_thread[proc] = sum(y)/len(y)
        z = total_paths[proc]*1.0/proc
        ideal_paths_per_thread[proc] = z
        u = [max(p) for p in paths[proc]]
        max_paths_per_thread[proc] = sum(u)/len(u)
        v = [min(p) for p in paths[proc]]
        min_paths_per_thread[proc] = sum(v)/len(v)
        print "{0} &  {1:09.1f} & {2:09.1f} & {3:7d}\\\\".format(proc, paths_per_thread[proc], ideal_paths_per_thread[proc], total_paths[proc])

    pushes_per_thread = {}
    total_pushes = {}
    ideal_pushes_per_thread = {}
    max_pushes_per_thread = {}
    min_pushes_per_thread = {}

    for proc in ps:
        x = [sum(p) for p in pushes[proc]]
        total_pushes[proc] = sum(x)/len(x)
        y = [sum(p)*1.0/len(p) for p in pushes[proc]]
        pushes_per_thread[proc] = sum(y)/len(y)
        z = total_pushes[proc]*1.0/proc
        ideal_pushes_per_thread[proc] = z
        u = [max(p) for p in pushes[proc]]
        max_pushes_per_thread[proc] = sum(u)/len(u)
        v = [min(p) for p in pushes[proc]]
        min_pushes_per_thread[proc] = sum(v)/len(v)
        print "{0} & {1:05.1f} & {2:05.1f} & {3:7d}\\\\".format(proc, pushes_per_thread[proc], ideal_pushes_per_thread[proc], total_pushes[proc])

    changes_per_thread = {}
    total_changes = {}
    ideal_changes_per_thread = {}
    max_changes_per_thread = {}
    min_changes_per_thread = {}

    for proc in ps:
        x = [sum(p) for p in changes[proc]]
        total_changes[proc] = sum(x)/len(x)
        y = [sum(p)*1.0/len(p) for p in changes[proc]]
        changes_per_thread[proc] = sum(y)/len(y)
        z = total_changes[proc]*1.0/proc
        ideal_changes_per_thread[proc] = z
        u = [max(p) for p in changes[proc]]
        max_changes_per_thread[proc] = sum(u)/len(u)
        v = [min(p) for p in changes[proc]]
        min_changes_per_thread[proc] = sum(v)/len(v)
        print "{0} &  {1:02.1f} & {2:02.1f} & {3:7d}\\\\".format(proc, changes_per_thread[proc], ideal_changes_per_thread[proc], total_changes[proc])


    for k in sorted(times):
        print k,len(times[k])

    ideals = map(lambda x: avs[ps[0]]/x,ps)


    fig = pyplot.figure()
    ax = fig.add_subplot(111)
    ax.plot(ps,[total_paths[p] for p in ps], 'b.-',label='Total nodes explored')
    ax.plot(ps,[ideal_paths_per_thread[p] for p in ps], 'g.-',label = 'Ideal Node Balancing')
    ax.plot(ps,[paths_per_thread[p] for p in ps], 'r.-',label=' Average Nodes explored per thread')
    ax.plot(ps,[max_paths_per_thread[p] for p in ps], 'm.-',label='Maximum Nodes explored per thread')
    ax.plot(ps,[min_paths_per_thread[p] for p in ps], 'c.-',label='Minimum Nodes explored per thread')
    pyplot.title('Load balancing of nodes')
    pyplot.xlabel('Processors')
    pyplot.ylabel('Nodes explored')
    pyplot.yscale('log')
    handles, labels = ax.get_legend_handles_labels()
    lgd = ax.legend(handles, labels, loc='upper center', bbox_to_anchor=(0.5,-0.1))
    # ax.grid('on')
    fig.savefig('BFS_paths'+mode+'.png', bbox_extra_artists=(lgd,), bbox_inches='tight')
    # pyplot.show()

    fig = pyplot.figure()
    ax = fig.add_subplot(111)
    max_pushes_per_thread[16]*=0.8
    max_pushes_per_thread[32]*=0.9
    min_pushes_per_thread[32]*=1.05
    # ax.plot(ps,[total_pushes[p] for p in ps], 'b.-',label='Total works queued')
    ax.plot(ps,[ideal_pushes_per_thread[p] for p in ps], 'g.-',label = 'Perfect Load Balancing')
    ax.plot(ps,[pushes_per_thread[p] for p in ps], 'r.-',label=' Average works queued per thread')
    ax.plot(ps,[max_pushes_per_thread[p] for p in ps], 'm.-',label='Maximum works queued per thread')
    ax.plot(ps,[min_pushes_per_thread[p] for p in ps], 'c.-',label='Minimum works queued per thread')
    ax.set_xlim([ps[0]-1,ps[-1]+1])
    # ax.set_ylim([min(ideal_pushes_per_thread.values())-1e4,max(total_pushes.values())+1e4])
    pyplot.title('Load balancing of work')
    pyplot.xlabel('Processors')
    pyplot.ylabel('Jobs returned to the global queue')
    # pyplot.yscale('log')
    handles, labels = ax.get_legend_handles_labels()
    lgd = ax.legend(handles, labels, loc='upper center', bbox_to_anchor=(0.5,-0.1))
    # ax.grid('on')
    fig.savefig('BFS_pushes'+mode+'.png', bbox_extra_artists=(lgd,), bbox_inches='tight')

    fig = pyplot.figure()
    ax = fig.add_subplot(111)
    ax.plot(ps,[220*p*p for p in ps],'r.-',label='Quadratic fit curve')
    ax.plot(ps,[total_pushes[p] for p in ps], 'b.-',label='Total works queued')
    # ax.plot(ps,[ideal_pushes_per_thread[p] for p in ps], 'g.-',label = 'Perfect Load Balancing')
    # ax.plot(ps,[pushes_per_thread[p] for p in ps], 'r.-',label=' Average works queued per thread')
    # ax.plot(ps,[max_pushes_per_thread[p] for p in ps], 'm.-',label='Maximum works queued per thread')
    # ax.plot(ps,[min_pushes_per_thread[p] for p in ps], 'c.-',label='Minimum works queued per thread')
    ax.set_xlim([ps[0]-1,ps[-1]+1])
    # ax.set_ylim([min(ideal_pushes_per_thread.values())-1e4,max(total_pushes.values())+1e4])
    pyplot.title('Total number')
    pyplot.xlabel('Processors')
    pyplot.ylabel('Jobs returned to the global queue')
    # pyplot.yscale('log')
    handles, labels = ax.get_legend_handles_labels()
    lgd = ax.legend(handles, labels, loc='upper center', bbox_to_anchor=(0.5,-0.1))
    # ax.grid('on')
    fig.savefig('BFS_total_pushes'+mode+'.png', bbox_extra_artists=(lgd,), bbox_inches='tight')
    # pyplot.show()

    fig = pyplot.figure()
    ax = fig.add_subplot(111)
    ax.plot(ps,[total_changes[p] for p in ps], 'b.-',label='Total bound changes')
    ax.plot(ps,[ideal_changes_per_thread[p] for p in ps], 'g.-',label = 'Ideal bound changes per thread')
    ax.plot(ps,[changes_per_thread[p] for p in ps], 'r.-',label=' Average bound changes per thread')
    ax.plot(ps,[max_changes_per_thread[p] for p in ps], 'm.-',label='Maximum bound changes per thread')
    ax.plot(ps,[min_changes_per_thread[p] for p in ps], 'c.-',label='Minimum bound changes per thread')
    ax.set_xlim([ps[0]-1,ps[-1]+1])
    ax.set_ylim([min(min_changes_per_thread.values())-1,max(total_changes.values())+1])
    pyplot.xlabel('Processors')
    pyplot.ylabel('Number of changes on the global bound')
    pyplot.title('Bound updates')
    handles, labels = ax.get_legend_handles_labels()
    lgd = ax.legend(handles, labels, loc='upper center', bbox_to_anchor=(0.5,-0.1))
    # ax.grid('on')
    fig.savefig('BFS_changes'+mode+'.png', bbox_extra_artists=(lgd,), bbox_inches='tight')
    # pyplot.show()
