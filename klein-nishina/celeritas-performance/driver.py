#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Copyright 2020 UT-Battelle, LLC and other Celeritas Developers.
# See the top-level COPYRIGHT file for details.
# SPDX-License-Identifier: (Apache-2.0 OR MIT)
"""
"""
import sys
import json
from pprint import pprint
import subprocess
from os import environ, pathsep


class Run:
    _block_size = 256

    def __init__(self, exe):

        time_m = 'sm__cycles_elapsed.avg,sm__cycles_elapsed.avg.per_second'
        dp_m = \
            'sm__sass_thread_inst_executed_op_dadd_pred_on.sum,' +\
            'sm__sass_thread_inst_executed_op_dfma_pred_on.sum,' +\
            'sm__sass_thread_inst_executed_op_dmul_pred_on.sum'
        sp_m = \
            'sm__sass_thread_inst_executed_op_fadd_pred_on.sum,' +\
            'sm__sass_thread_inst_executed_op_ffma_pred_on.sum,' +\
            'sm__sass_thread_inst_executed_op_fmul_pred_on.sum'
        hp_m = \
            'sm__sass_thread_inst_executed_op_hadd_pred_on.sum,' +\
            'sm__sass_thread_inst_executed_op_hfma_pred_on.sum,' +\
            'sm__sass_thread_inst_executed_op_hmul_pred_on.sum'
        bytes_m = \
            'dram__bytes.sum,' +\
            'lts__t_bytes.sum,' +\
            'l1tex__t_bytes.sum'
        metrics = '{},{},{},{},{}'.format(time_m, dp_m, sp_m, hp_m, bytes_m)
        ncu = 'ncu -k iterate_kn --metrics {} --csv'.format(metrics)
        exe = '/home/9te/celeritas/_build/app/{}'.format(exe)
        args = '{} {}'.format(ncu, exe)
        self._args = args.split()
        self._args.append('-')

        self._host = False
        if 'host' in exe:
            self._host = True

    def run(self, inp, case, verbose=False):
        print("Input:")
        pprint(inp)

        print("Running", self._args)
        with subprocess.Popen(self._args,
                              stdin=subprocess.PIPE,
                              stdout=subprocess.PIPE) as proc:
            (output, _) = proc.communicate(input=json.dumps(inp).encode())

        print("Received {} bytes of data".format(len(output)))

        lines = output.decode().splitlines()
        cnt = 0
        for line in lines:
            cnt += 1
            if 'Host Name' in line:
                break

        outf = 'output-{}'.format(case)
        with open('{}.csv'.format(outf), 'w') as out:
            for line in lines[cnt-1:]:
                out.write(line)
                out.write('\n')

        full = json.loads(lines[0])
        result = full['result']

        if verbose:
            pprint(result)

        if not self._host:
            num_tracks = result['alive'][0]
            num_iters = len(result['edep'])
            num_track_steps = sum(result['alive'])
            print("Number of steps:", num_iters,
                  "(average", num_track_steps / num_tracks, " per track)")
            print("Fraction of time in kernel:", sum(result['time']) /
                  result['total_time'])
            print()

        with open('{}.json'.format(case), 'w') as out:
            json.dump(full, out)

    def default_inp(self):
        num_tracks = 3907
        max_steps = 256
        energy = 100  # MeV
        inp = {
            'grid_params': {
                'block_size': self.block_size,
                'grid_size': 64,
            },
            'run': {
                'seed': 12345,
                'energy': energy,
                'num_tracks': num_tracks * self.block_size,
                'max_steps': max_steps,
                'tally_grid': {
                    'size': 2048,
                    'front': 0,
                    'delta': 1,
                }
            }
        }
        return inp

    @property
    def block_size(self):
        return self._block_size


def main():
    runner = Run('demo-interactor')
    grid_size = 256

    for multiple in [1, 2, 4]:
        inp = runner.default_inp()
        nump = 4 * grid_size * runner.block_size
        inp['grid_params']['grid_size'] = nump//runner.block_size//multiple
        inp['run']['num_tracks'] = nump
        case = 'track-npt-{}'.format(multiple)
        runner.run(inp, case)


if __name__ == "__main__":

    environ['PATH'] = '/opt/nsight-compute-2020.2' + pathsep + environ['PATH']

    try:
        main()
    except RuntimeError as err:
        print(err)
