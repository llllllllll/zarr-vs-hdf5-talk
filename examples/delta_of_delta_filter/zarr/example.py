import numpy as np
import zarr

from filter import DeltaOfDelta


f = zarr.open('a.zarr', 'w')
dset = f.create_dataset(
    'dset',
    compressor=DeltaOfDelta(),
    dtype='i8',
    shape=(100,),
)

dset[:] = np.arange(100)
assert (dset[:] == np.arange(100)).all()
