#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Copyright 2020 UT-Battelle, LLC and other Celeritas Developers.
# See the top-level COPYRIGHT file for details.
# SPDX-License-Identifier: (Apache-2.0 OR MIT)
"""
"""
import json
from pprint import pprint
import subprocess
from os import environ


class Run:
    _block_size = 256

    def __init__(self, exe):
        self._exe = '/home/9te/celeritas/_build/app/{}'.format(exe)

        self._host = False
        if 'host' in exe:
            self._host = True

    def run(self, inp, case, verbose=False):
        print("Input:")
        pprint(inp)

        print("Running", self._exe)
        with subprocess.Popen([self._exe, '-'],
                              stdin=subprocess.PIPE,
                              stdout=subprocess.PIPE) as proc:
            (output, _) = proc.communicate(input=json.dumps(inp).encode())

        print("Received {} bytes of data".format(len(output)))
        full = json.loads(output.decode())
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
                    'size': 1024,
                    'front': -1,
                    'delta': 0.1,
                }
            }
        }
        return inp

    @property
    def block_size(self):
        return self._block_size


def main():
    runner = Run('demo-interactor')

    for grid_size in [32, 64, 128, 256, 512]:
        inp = runner.default_inp()
        inp['grid_params']['grid_size'] = grid_size
        case = 'track_const-{}'.format(grid_size)
        runner.run(inp, case)

    for grid_size in [250, 500, 1000, 2000, 4000, 8000, 16000]:
        inp = runner.default_inp()
        inp['grid_params']['grid_size'] = grid_size
        inp['run']['num_tracks'] = grid_size * runner.block_size
        case = 'track-{}'.format(grid_size)
        runner.run(inp, case)

    host_runner = Run('host-demo-interactor')

    for grid_size in [250, 500, 1000, 2000, 4000, 8000, 16000]:
        inp = host_runner.default_inp()
        inp['run']['num_tracks'] = grid_size * runner.block_size
        case = 'host-track-{}'.format(grid_size)
        host_runner.run(inp, case)


if __name__ == "__main__":
    try:
        main()
    except RuntimeError as err:
        print(err)
