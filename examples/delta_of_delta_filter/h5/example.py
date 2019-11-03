import ctypes
import os.path

import numpy as np
import h5py


dll = ctypes.CDLL(os.path.join(os.path.dirname(__file__) + 'filter.so'))
assert dll.dod_filter_register() == 0, 'failed to register dod filter'
filter_id = ctypes.c_int.in_dll(dll, 'dod_filter_id').value

f = h5py.File('a.h5', 'w')
dset = f.create_dataset(
    'dset',
    compression=filter_id,
    dtype='i8',
    shape=(100,),
)

dset[:] = np.arange(100)
dset.flush()
assert (dset[:] == np.arange(100)).all()
