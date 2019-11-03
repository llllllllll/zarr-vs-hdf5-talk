import numcodecs
from numcodecs.abc import Codec
from numcodecs.compat import ensure_ndarray
import numpy as np


class DeltaOfDelta(Codec):
    codec_id = 'delta_of_delta'

    dtype = np.dtype('i8')
    astype = np.dtype('i2')

    def encode(self, buf):
        arr = ensure_ndarray(buf).reshape(-1, order='A')
        if arr.dtype != self.dtype:
            raise ValueError(
                f'cannot use DOD encoder with array of type {arr.dtype}',
            )

        out = np.empty(len(arr) - 2 + 12, dtype=self.astype)
        header = out[:12].view(self.dtype)
        header[0] = len(arr)
        if len(arr) == 0:
            return out
        header[1] = arr[0]
        if len(arr) == 1:
            return out
        header[2] = arr[1] - arr[0]
        delta = np.diff(arr)
        out[12:] = np.diff(delta)
        return out

    def decode(self, buf, out=None):
        enc = ensure_ndarray(buf).view(self.astype)
        header = enc[:12].view(self.dtype)

        size = header[0]
        if out is None:
            out = np.empty(size, dtype=self.dtype)
        else:
            out = out.view(self.dtype)
            if len(out) != size:
                raise ValueError('out size does not match data size')

        if size == 0:
            return out
        out[0] = header[1]
        if size == 1:
            return out
        out[1] = header[2]
        out[2:] = enc[12:]

        np.cumsum(out[1:], out=out[1:])
        return np.cumsum(out, out=out)


numcodecs.register_codec(DeltaOfDelta)
