#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Copyright 2021 UT-Battelle, LLC and other Celeritas Developers.
# See the top-level COPYRIGHT file for details.
# SPDX-License-Identifier: (Apache-2.0 OR MIT)
"""
Remotely launch interactor demo over SSH.
"""

from math import ceil
import json
from pathlib import PurePosixPath as Path
import subprocess

try:
    from tqdm import tqdm
except ImportError:
    def tqdm(iterable, *args, **kwargs):
        yield from iterable

class RemoteCommand:
    def __init__(self, remote):
        self.remote = remote

    def __call__(self, args, stdin=None):
        if stdin is not None:
            stdin = stdin.encode()
        cmd = ["ssh", self.remote] + args
        return subprocess.check_output(cmd, input=stdin).decode()


class Runner:
    block_size = 256

    def __init__(self, remote, build_dir, run_args=None):
        if run_args is None:
            run_args = {
                'seed': 12345,
                'energy': 100.0,  # MeV
                'max_steps': 128,
                'tally_grid': {
                    'size': 2048,
                    'front': 0,
                    'delta': 1,
                }
            }
        self.call_ssh = RemoteCommand(remote)
        self.exe = Path(build_dir) / "app" / "demo-interactor"
        self.run_args = run_args

    def __call__(self, num_tracks):
        inp = {
            'grid_params': {
                'block_size': self.block_size,
                'grid_size': int(ceil(num_tracks / self.block_size)),
                'sync': False,
            },
            'run': self.run_args.copy()
        }
        inp['run']['num_tracks'] = num_tracks

        try:
            result = self.call_ssh([self.exe, "-"], stdin=json.dumps(inp))
        except subprocess.CalledProcessError as e:
            raise RuntimeError(f"command failed with n={num_tracks}") from e
        return json.loads(result)


def main():
    from itertools import count
    from pathlib import Path

    remote = "wildstyle"
    run = Runner(remote, "~/.local/src/celeritas/build-opt")
    run.block_size = 128

    time = []
    tracks = []
    steps = []
    for log2 in tqdm(range(27)):
        try:
            result = run(2**log2)
        except RuntimeError as e:
            print(f"Failed with 2^{log2} tracks: {e!s}")
            break
        else:
            time.append(result['result']['total_time'])
            tracks.append(result['run']['num_tracks'])
            steps.append(sum(result['result']['alive']))

    kernel_stats = {k.pop('name'): k for k in result['runtime']['kernels']}
    result['runtime']['kernels'] = kernel_stats

    version = result['runtime']['version'].partition('+')[2]
    outdir = Path(version + '-reg80')
    outdir.mkdir(exist_ok=True)

    with open(outdir / 'results.json', 'w') as f:
        json.dump({
            'num_tracks': tracks,
            'time': time,
            'steps': steps,
            'final_result': result,
        }, f, indent=0)
    print(f"Saved to {outdir}")

if __name__ == '__main__':
    main()
