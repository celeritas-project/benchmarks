import os
import numpy as np
import pandas as pd
from roofline import roofline

datadir='.'
files=[x for x in os.listdir(datadir) if x.endswith('.csv') and x.startswith('output')]
files.sort()
files=[os.path.join(datadir,file) for file in files]
dfs={}
for file in files:
    tag, ext = os.path.splitext(os.path.basename(file))
    dfs[tag]=pd.DataFrame()
    with open(file,'r') as f:
        cnt=0
        while True:
            ln=f.readline()
            if not ln:
                break
            cnt+=1
            if 'Host Name' in ln:
                break
        df = pd.read_csv(file, skiprows=cnt-1, thousands=',')
        dft=df.groupby(['Kernel Name','Metric Name']).sum()
        dfmetric=pd.pivot_table(dft, index='Kernel Name', columns='Metric Name', values='Metric Value')
        dfmetric['Count']=df.groupby(['Kernel Name']).count()['ID'].div(dfmetric.shape[1])

        dfmetric['Time']=dfmetric['sm__cycles_elapsed.avg'] \
                        / (dfmetric['sm__cycles_elapsed.avg.per_second'] /dfmetric['Count'] )

        dfmetric['CC FLOPs']= 2 * dfmetric['sm__sass_thread_inst_executed_op_dfma_pred_on.sum'] \
                            + dfmetric['sm__sass_thread_inst_executed_op_dmul_pred_on.sum'] \
                            + dfmetric['sm__sass_thread_inst_executed_op_dadd_pred_on.sum'] \
                            + 2 * dfmetric['sm__sass_thread_inst_executed_op_ffma_pred_on.sum'] \
                            + dfmetric['sm__sass_thread_inst_executed_op_fmul_pred_on.sum'] \
                            + dfmetric['sm__sass_thread_inst_executed_op_fadd_pred_on.sum'] \
                            + 2 * dfmetric['sm__sass_thread_inst_executed_op_hfma_pred_on.sum'] \
                            + dfmetric['sm__sass_thread_inst_executed_op_hmul_pred_on.sum'] \
                            + dfmetric['sm__sass_thread_inst_executed_op_hadd_pred_on.sum']

        dfmetric['all FLOPs']= dfmetric['CC FLOPs']

        dfmetric['AI HBM'] = dfmetric['all FLOPs'].div(dfmetric['dram__bytes.sum'])
        dfmetric['AI L2'] = dfmetric['all FLOPs'].div(dfmetric['lts__t_bytes.sum'])
        dfmetric['AI L1'] = dfmetric['all FLOPs'].div(dfmetric['l1tex__t_bytes.sum'])

        dfmetric['GFLOP/s'] = dfmetric['all FLOPs']/ dfmetric['Time'] /1024/1024/1024
        dfs[tag]=dfmetric

short_labels = ['iterate_kn']

tags=dfs.keys()

labels = []
ail1 = []
ail2 = []
aihbm = []
flops = []
for tag in tags:

    with open('{}.md'.format(tag), 'w') as f:
        f.write(dfs[tag].to_markdown())


    dfm=dfs[tag]
    LABELS = dfm.index.tolist()
    AIL1   = dfm['AI L1'].tolist()
    AIL2   = dfm['AI L2'].tolist()
    AIHBM  = dfm['AI HBM'].tolist()
    FLOPS  = dfm['GFLOP/s'].tolist()

    new_labels = []
    for label in short_labels:
        if label in LABELS[0]:
            new_labels.append(label)
            break

    print(tag, FLOPS, AIHBM, AIL2, AIL1, new_labels)
    roofline(tag, FLOPS, AIHBM, AIL2, AIL1, new_labels, 'all')

    lbl = '{}/{}'.format(new_labels[0], '-'.join(tag.split('-')[-2:]))

    labels.append(lbl)
    ail1.extend(AIL1)
    ail2.extend(AIL2)
    aihbm.extend(AIHBM)
    flops.extend(FLOPS)

print('output-all', flops, aihbm, ail2, ail1, labels)
roofline('output-all', flops, aihbm, ail2, ail1, labels, 'all')
